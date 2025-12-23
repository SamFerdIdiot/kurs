# Test CMakeLists.txt for Road Trip Adventure RPG
# This file is included from the main CMakeLists.txt

# Find Google Test
# Try FetchContent first for easier cross-platform setup
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable Google Test discovery
include(GoogleTest)

# Create library with all source files (excluding main.cpp)
# This allows tests to link against the same code
set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Car.cpp
    ${CMAKE_SOURCE_DIR}/src/PlayerState.cpp
    ${CMAKE_SOURCE_DIR}/src/InventorySystem.cpp
    ${CMAKE_SOURCE_DIR}/src/NPC.cpp
    ${CMAKE_SOURCE_DIR}/src/DialogueManager.cpp
    ${CMAKE_SOURCE_DIR}/src/GameTypes.cpp
    ${CMAKE_SOURCE_DIR}/src/ExperienceSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/EventManager.cpp
    ${CMAKE_SOURCE_DIR}/src/EventFactory.cpp
    ${CMAKE_SOURCE_DIR}/src/QuestManager.cpp
    ${CMAKE_SOURCE_DIR}/src/GameStateManager.cpp
    ${CMAKE_SOURCE_DIR}/src/SaveSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/Notebook/NotebookEntry.cpp
)

# Check if SFML is available for tests
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

# Create the test library
add_library(kkurs_lib STATIC ${LIB_SOURCES})
target_include_directories(kkurs_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}  # Add tests directory for test_helpers.h
)

if(SFML_FOUND)
    # Use real SFML
    message(STATUS "Using real SFML for tests")
    target_link_libraries(kkurs_lib PUBLIC sfml-graphics sfml-window sfml-system)
else()
    # Use mock SFML for testing without SFML installation
    message(STATUS "SFML not found. Using mock SFML for tests.")
    message(STATUS "Mock SFML provides stub implementations for testing logic without graphics.")
    target_include_directories(kkurs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks/sfml)
endif()

# Unit Tests
set(UNIT_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_sanity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_npc_dialogue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_quest_manager.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_ui.cpp  # Excluded - requires UI classes
    # ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_phase4_scenes.cpp  # Excluded - requires UI classes
    # ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_phase4_scenes_improved.cpp  # Excluded - requires UI classes
    # ${CMAKE_CURRENT_SOURCE_DIR}/test_AbilityNode.cpp  # Excluded - abilities moved to inventory
    # ${CMAKE_CURRENT_SOURCE_DIR}/test_AbilityTreeSystem.cpp  # Excluded - abilities moved to inventory
    ${CMAKE_CURRENT_SOURCE_DIR}/test_companion_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_day0_complete.cpp  # День 0: полное тестирование
    ${CMAKE_CURRENT_SOURCE_DIR}/test_full_system_integration.cpp  # Полная интеграция систем
    ${CMAKE_CURRENT_SOURCE_DIR}/test_out_of_fuel.cpp  # Тест события "Закончилось топливо"
    ${CMAKE_CURRENT_SOURCE_DIR}/test_conditional_choices.cpp  # Тест условных выборов
    # Add more unit test files here as they are created
    # ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_character.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_car.cpp
)

add_executable(unit_tests ${UNIT_TEST_SOURCES})
target_link_libraries(unit_tests
    PRIVATE
    kkurs_lib
    GTest::gtest_main
)

# Discover tests for CTest
gtest_discover_tests(unit_tests
    PROPERTIES
    LABELS "unit"
)

# Integration Tests (to be added later)
# set(INTEGRATION_TEST_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/integration/test_scene_transitions.cpp
# )
#
# add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})
# target_link_libraries(integration_tests
#     PRIVATE
#     kkurs_lib
#     GTest::gtest_main
# )
# 
# gtest_discover_tests(integration_tests
#     PROPERTIES
#     LABELS "integration"
# )

# Optional: Code coverage
if(CMAKE_BUILD_TYPE MATCHES Debug AND ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(kkurs_lib PRIVATE --coverage)
        target_link_options(kkurs_lib PRIVATE --coverage)
    endif()
endif()
