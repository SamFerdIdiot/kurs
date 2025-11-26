# Road Trip Adventure RPG - Keep Driving Style with Russian 90s Theme
# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(kkurs VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# For Mac OS users: Set SFML path hint
# Usage: cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML
# Or: cmake .. -DSFML_DIR=/usr/local/lib/cmake/SFML
if(APPLE AND NOT DEFINED SFML_DIR)
    message(STATUS "==================================================")
    message(STATUS "Mac OS detected. SFML_DIR not set.")
    message(STATUS "Please specify SFML location with:")
    message(STATUS "  cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
    message(STATUS "Or if installed via brew:")
    message(STATUS "  cmake .. -DSFML_DIR=/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")
    message(STATUS "==================================================")
endif()

# Option to skip building the main executable (useful for CI without SFML)
option(BUILD_EXECUTABLE "Build the main executable" ON)

# Find SFML
if(BUILD_EXECUTABLE)
    find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainMenuScene.cpp
    src/CharacterCreationScene.cpp
    src/CharacterCreationMenu.cpp
    src/SceneManager.cpp
    src/PlayerState.cpp
    src/Character.cpp
    src/Car.cpp
    src/InventorySystem.cpp
    src/GridInventorySystem.cpp
    src/NPC.cpp
    src/DialogueManager.cpp
    src/GameTypes.cpp
    src/ExperienceSystem.cpp
    src/AbilityNode.cpp
    src/AbilityTreeSystem.cpp
    src/AbilityTreeScene.cpp
    src/RestScene.cpp
    src/NodeScene.cpp
    src/MapScene.cpp
    src/EventManager.cpp
    src/CinematicEventScene.cpp
    src/TravelCinematicScene.cpp
    src/QuestManager.cpp
    src/LocationScene.cpp
    src/Structure.cpp
    src/GameStateManager.cpp
    src/SaveSystem.cpp
    src/CharacterScene.cpp
    src/CompanionsScene.cpp
    src/QuestsScene.cpp
    src/InventoryScene.cpp
    src/UI/UIComponent.cpp
    src/UI/HUDPanel.cpp
    src/UI/ResourceBar.cpp
)

# Create executable
if(BUILD_EXECUTABLE)
    add_executable(kkurs ${SOURCES})

    # Link SFML libraries (SFML 3.x)
    target_link_libraries(kkurs SFML::Graphics SFML::Window SFML::System)

    # Copy assets to build directory so they can be found at runtime
    # Copy fonts
    add_custom_command(TARGET kkurs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets/fonts
            ${CMAKE_BINARY_DIR}/assets/fonts
        COMMENT "Copying fonts to build directory"
    )
    
    # Copy font images (Press_Start_2P, Jersey_10)
    add_custom_command(TARGET kkurs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/images/Press_Start_2P
            ${CMAKE_BINARY_DIR}/images/Press_Start_2P
        COMMENT "Copying Press_Start_2P font to build directory"
    )
    
    add_custom_command(TARGET kkurs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/images/Jersey_10
            ${CMAKE_BINARY_DIR}/images/Jersey_10
        COMMENT "Copying Jersey_10 font to build directory"
    )

    # Installation
    install(TARGETS kkurs DESTINATION bin)
endif()

# Enable testing
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    message(STATUS "Tests enabled. Run 'ctest' to execute tests.")
endif()

# Print configuration
message(STATUS "==================================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
if(BUILD_EXECUTABLE)
    message(STATUS "  SFML version: ${SFML_VERSION}")
endif()
message(STATUS "  Build executable: ${BUILD_EXECUTABLE}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "==================================================")
 
