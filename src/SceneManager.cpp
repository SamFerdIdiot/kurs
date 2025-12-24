#include "SceneManager.h"
#include "MainMenuScene.h"






#include "Notebook/NotebookScene.h"
#include "GameStateManager.h"
#include "EventHelper.h"
#include <stdexcept>
#include <iostream>


SceneManager::SceneManager(sf::RenderWindow& window)
    : m_window(window),
      m_currentSceneType(SceneType::MAIN_MENU) {


    m_currentScene = createScene(SceneType::MAIN_MENU);

    if (!m_currentScene) {
        throw std::runtime_error("Failed to create main menu scene");
    }
}


std::unique_ptr<Scene> SceneManager::createScene(SceneType type) {
    switch (type) {
        case SceneType::MAIN_MENU:
            std::cout << "Creating Main Menu Scene" << std::endl;
            return std::make_unique<MainMenuScene>();






















        case SceneType::NOTEBOOK: {
            std::cout << "Creating Notebook Scene" << std::endl;

            auto& playerState = GameStateManager::getInstance().getPlayerState();

            std::string entryId = playerState.getCurrentNotebookEntryId();
            if (entryId.empty()) {
                entryId = "tutorial_start";
            }
            std::cout << "[SceneManager] Loading notebook entry: " << entryId << std::endl;
            return std::make_unique<NotebookScene>(&playerState, entryId);
        }

        case SceneType::EXIT:
            std::cout << "Exit requested" << std::endl;
            return nullptr;


        case SceneType::MOTEL:
            std::cerr << "Motel scene not yet implemented, returning to main menu" << std::endl;
            return std::make_unique<MainMenuScene>();

        default:
            std::cerr << "Scene type not implemented yet, returning to main menu" << std::endl;
            return std::make_unique<MainMenuScene>();
    }
}


void SceneManager::run() {
    sf::Clock clock;

    while (m_window.isOpen() && m_currentScene) {
        float deltaTime = clock.restart().asSeconds();


        auto& playerState = GameStateManager::getInstance().getPlayerState();
        playerState.updatePlayTime(deltaTime);


        while (const std::optional<sf::Event> event = m_window.pollEvent()) {
            if (EventHelper::isClosed(*event)) {
                m_window.close();
                return;
            }


            m_currentScene->handleInput(*event);
        }


        m_currentScene->update(deltaTime);


        if (m_currentScene->isFinished()) {
            SceneType nextSceneType = m_currentScene->getNextScene();


            if (nextSceneType == SceneType::EXIT) {
                m_window.close();
                return;
            }


            m_currentSceneType = nextSceneType;
            m_currentScene = createScene(nextSceneType);

            if (!m_currentScene) {

                m_window.close();
                return;
            }
        }


        m_window.clear();
        m_currentScene->render(m_window);
        m_window.display();
    }
}
