#include "Journal/JournalScene.h"
#include "UIConstants.h"
#include "EventHelper.h"
#include <iostream>
#include <sstream>

// Helper function to convert UTF-8 string to SFML string
static sf::String utf8(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

JournalScene::JournalScene(PlayerState* playerState, const std::string& startCityId)
    : m_playerState(playerState),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fontLoaded(false),
      m_selectedChoiceIndex(0) {

    // Загрузить шрифт
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
        std::cout << "[JournalScene] Font loaded successfully" << std::endl;
    } else {
        std::cerr << "[JournalScene] Failed to load font" << std::endl;
    }

    // Показать стартовую записку города
    showCityNote(startCityId);

    std::cout << "[JournalScene] Initialized with start city: " << startCityId << std::endl;
}

void JournalScene::handleInput(const sf::Event& event) {
    // Закрытие окна
    if (EventHelper::isClosed(event)) {
        m_isFinished = true;
        return;
    }

    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        // Escape - выход в главное меню
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            m_isFinished = true;
            m_nextScene = SceneType::MAIN_MENU;
            return;
        }

        // Получить текущие варианты выбора
        if (!m_currentNote) return;

        auto choices = m_currentNote->getChoices();
        if (choices.empty()) return;

        // === НАВИГАЦИЯ СТРЕЛКАМИ ===
        if (keyPressed->code == sf::Keyboard::Key::Up) {
            moveSelectionUp();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Down) {
            moveSelectionDown();
        }

        // === ПОДТВЕРЖДЕНИЕ ВЫБОРА ===
        else if (keyPressed->code == sf::Keyboard::Key::Enter) {
            handleChoice(m_selectedChoiceIndex);
        }

        // === ВЫБОР ЦИФРАМИ 1-9 ===
        else if (keyPressed->code >= sf::Keyboard::Key::Num1 &&
                 keyPressed->code <= sf::Keyboard::Key::Num9) {
            int choiceIndex = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num1);
            if (choiceIndex < static_cast<int>(choices.size())) {
                m_selectedChoiceIndex = choiceIndex;
                handleChoice(choiceIndex);
            }
        }
    }
}

void JournalScene::update(float deltaTime) {
    if (!m_currentNote) {
        return;
    }

    // Обновить текущую записку (для динамических записок типа TravelNote)
    m_currentNote->update(deltaTime);

    // Проверить, завершена ли записка
    if (m_currentNote->isFinished()) {
        handleNoteFinished();
    }
}

void JournalScene::render(sf::RenderWindow& window) {
    // Очистить экран (фон как старая бумага)
    window.clear(sf::Color(230, 220, 210));

    if (!m_fontLoaded) {
        return;
    }

    // Отрендерить минимальный HUD сверху
    renderHUD(window);

    // Отрендерить текущую записку
    if (m_currentNote) {
        m_currentNote->render(window, m_font, m_selectedChoiceIndex);
    }
}

void JournalScene::renderHUD(sf::RenderWindow& window) {
    if (!m_playerState || !m_fontLoaded) {
        return;
    }

    // === МИНИМАЛЬНЫЙ HUD СВЕРХУ ЭКРАНА ===

    // Фон для HUD
    sf::RectangleShape hudBackground;
    hudBackground.setSize(sf::Vector2f(UI::SCREEN_WIDTH, 50.0f));
    hudBackground.setPosition(sf::Vector2f(0.0f, 0.0f));
    hudBackground.setFillColor(sf::Color(40, 35, 30, 200));  // Темный полупрозрачный
    window.draw(hudBackground);

    // Состояние машины и героя
    float xPos = 30.0f;
    float yPos = 15.0f;

    // Топливо
    std::stringstream fuelText;
    fuelText << "ТОПЛИВО: " << static_cast<int>(m_playerState->getFuel()) << "L";
    sf::Text fuel(m_font);
    fuel.setString(utf8(fuelText.str()));
    fuel.setCharacterSize(18);
    fuel.setFillColor(sf::Color(200, 180, 120));
    fuel.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(fuel);

    xPos += 250.0f;

    // Энергия
    std::stringstream energyText;
    energyText << "ЭНЕРГИЯ: " << static_cast<int>(m_playerState->getEnergy()) << "%";
    sf::Text energy(m_font);
    energy.setString(utf8(energyText.str()));
    energy.setCharacterSize(18);
    energy.setFillColor(sf::Color(200, 180, 120));
    energy.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(energy);

    xPos += 250.0f;

    // Деньги
    std::stringstream moneyText;
    moneyText << "ДЕНЬГИ: " << static_cast<int>(m_playerState->getMoney()) << " ₽";
    sf::Text money(m_font);
    money.setString(utf8(moneyText.str()));
    money.setCharacterSize(18);
    money.setFillColor(sf::Color(200, 180, 120));
    money.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(money);
}

SceneType JournalScene::getNextScene() const {
    return m_nextScene;
}

bool JournalScene::isFinished() const {
    return m_isFinished;
}

// === УПРАВЛЕНИЕ ЗАПИСКАМИ ===

void JournalScene::showCityNote(const std::string& cityId) {
    m_currentNote = std::make_unique<CityNote>(cityId, m_playerState);
    m_selectedChoiceIndex = 0;  // Сбросить выбор
    std::cout << "[JournalScene] Showing CityNote: " << cityId << std::endl;
}

// === ПРИВАТНЫЕ МЕТОДЫ ===

void JournalScene::handleChoice(int choiceIndex) {
    if (!m_currentNote) {
        return;
    }

    auto choices = m_currentNote->getChoices();
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(choices.size())) {
        return;
    }

    const auto& choice = choices[choiceIndex];

    // Пропустить недоступные варианты
    if (choice.isDisabled) {
        std::cout << "[JournalScene] Choice disabled: " << choice.disabledReason << std::endl;
        return;
    }

    std::cout << "[JournalScene] Player chose: " << choice.text << std::endl;

    // Выполнить выбор
    m_currentNote->executeChoice(choiceIndex, m_playerState);

    // Показать результат (если есть)
    if (!choice.outcomeText.empty()) {
        std::cout << "[JournalScene] Outcome: " << choice.outcomeText << std::endl;
        // TODO: Показать outcome в отдельном UI (например, всплывающее окно)
    }
}

void JournalScene::moveSelectionUp() {
    if (!m_currentNote) return;

    auto choices = m_currentNote->getChoices();
    if (choices.empty()) return;

    m_selectedChoiceIndex--;
    if (m_selectedChoiceIndex < 0) {
        m_selectedChoiceIndex = static_cast<int>(choices.size()) - 1;
    }

    std::cout << "[JournalScene] Selection moved up to: " << m_selectedChoiceIndex << std::endl;
}

void JournalScene::moveSelectionDown() {
    if (!m_currentNote) return;

    auto choices = m_currentNote->getChoices();
    if (choices.empty()) return;

    m_selectedChoiceIndex++;
    if (m_selectedChoiceIndex >= static_cast<int>(choices.size())) {
        m_selectedChoiceIndex = 0;
    }

    std::cout << "[JournalScene] Selection moved down to: " << m_selectedChoiceIndex << std::endl;
}

void JournalScene::handleNoteFinished() {
    if (!m_currentNote) {
        return;
    }

    std::cout << "[JournalScene] Note finished: " << m_currentNote->getId() << std::endl;

    // Обработать завершение в зависимости от типа записки
    if (m_currentNote->getType() == Note::Type::CITY) {
        CityNote* cityNote = dynamic_cast<CityNote*>(m_currentNote.get());
        if (cityNote) {
            auto action = cityNote->getSelectedAction();

            switch (action) {
                case CityNote::Action::GAS_STATION:
                    // Заправились - показать ту же самую записку города снова
                    std::cout << "[JournalScene] Player refueled, showing city again" << std::endl;
                    showCityNote(cityNote->getId().substr(5));  // Убрать префикс "city_"
                    break;

                case CityNote::Action::MOTEL:
                    // Отдохнули - показать ту же самую записку города снова
                    std::cout << "[JournalScene] Player rested, showing city again" << std::endl;
                    showCityNote(cityNote->getId().substr(5));
                    break;

                case CityNote::Action::WORK:
                    // TODO: Показать WorkNote
                    std::cout << "[JournalScene] TODO: Show WorkNote" << std::endl;
                    // Пока просто возврат к городу
                    showCityNote(cityNote->getId().substr(5));
                    break;

                case CityNote::Action::SHOP:
                    // TODO: Показать ShopNote
                    std::cout << "[JournalScene] TODO: Show ShopNote" << std::endl;
                    // Пока просто возврат к городу
                    showCityNote(cityNote->getId().substr(5));
                    break;

                case CityNote::Action::TRAVEL:
                    // TODO: Показать TravelNote
                    std::cout << "[JournalScene] TODO: Show TravelNote (route selection)" << std::endl;
                    // Пока просто выход в главное меню
                    m_isFinished = true;
                    m_nextScene = SceneType::MAIN_MENU;
                    break;

                default:
                    break;
            }
        }
    }

    // TODO: Обработка других типов записок (TravelNote, EventNote и т.д.)
}
