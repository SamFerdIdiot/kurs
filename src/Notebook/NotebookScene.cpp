#include "Notebook/NotebookScene.h"
#include "UIConstants.h"
#include "EventHelper.h"
#include <iostream>
#include <sstream>

// Helper function to convert UTF-8 string to SFML string
static sf::String utf8(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

NotebookScene::NotebookScene(PlayerState* playerState, const std::string& initialEntryId)
    : m_playerState(playerState),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_currentEntryId(initialEntryId),
      m_textRevealTimer(0.0f),
      m_charactersPerSecond(50.0f),  // 50 символов в секунду
      m_textFullyRevealed(false),
      m_canSkipText(true),
      m_selectedChoiceIndex(0),
      m_fontLoaded(false) {

    // Загрузить шрифт
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
        std::cout << "[NotebookScene] Font loaded successfully" << std::endl;
    } else {
        std::cerr << "[NotebookScene] Failed to load font" << std::endl;
    }

    // Загрузить тестовую запись
    loadTestEntry();

    std::cout << "[NotebookScene] Initialized with entry: " << m_currentEntryId << std::endl;
}

void NotebookScene::handleInput(const sf::Event& event) {
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

        // Пробел или Enter - пропустить анимацию текста
        if ((keyPressed->code == sf::Keyboard::Key::Space ||
             keyPressed->code == sf::Keyboard::Key::Enter) &&
            !m_textFullyRevealed && m_canSkipText) {
            skipTextAnimation();
            return;
        }

        // Если текст полностью показан, обрабатываем навигацию по выборам
        if (m_textFullyRevealed && !m_choices.empty()) {
            // Навигация стрелками
            if (keyPressed->code == sf::Keyboard::Key::Up) {
                moveSelectionUp();
            }
            else if (keyPressed->code == sf::Keyboard::Key::Down) {
                moveSelectionDown();
            }

            // Подтверждение выбора
            else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                handleChoice(m_selectedChoiceIndex);
            }

            // Выбор цифрами 1-9
            else if (keyPressed->code >= sf::Keyboard::Key::Num1 &&
                     keyPressed->code <= sf::Keyboard::Key::Num9) {
                int choiceIndex = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num1);
                if (choiceIndex < static_cast<int>(m_choices.size())) {
                    m_selectedChoiceIndex = choiceIndex;
                    handleChoice(choiceIndex);
                }
            }
        }
    }
}

void NotebookScene::update(float deltaTime) {
    // Обновить анимацию печати текста
    if (!m_textFullyRevealed) {
        updateTextReveal(deltaTime);
    }
}

void NotebookScene::render(sf::RenderWindow& window) {
    // Очистить экран (фон как старая бумага)
    window.clear(sf::Color(230, 220, 210));

    if (!m_fontLoaded) {
        return;
    }

    // Отрендерить HUD сверху
    renderHUD(window);

    // Отрендерить текст записи
    renderText(window);

    // Отрендерить варианты выбора (если текст полностью показан)
    if (m_textFullyRevealed) {
        renderChoices(window);
    }
}

SceneType NotebookScene::getNextScene() const {
    return m_nextScene;
}

bool NotebookScene::isFinished() const {
    return m_isFinished;
}

// === УПРАВЛЕНИЕ ЗАПИСЯМИ ===

void NotebookScene::showEntry(const std::string& entryId) {
    m_currentEntryId = entryId;
    m_revealedText.clear();
    m_textRevealTimer = 0.0f;
    m_textFullyRevealed = false;
    m_selectedChoiceIndex = 0;

    // TODO: В будущем загружать из JSON
    // Пока используем хардкод для разных entry ID
    if (entryId == "tutorial_start") {
        loadTestEntry();
    } else if (entryId == "day1_departure") {
        loadDayOneDeparture();
    } else {
        // Неизвестный entry, загружаем дефолтный
        loadTestEntry();
    }

    std::cout << "[NotebookScene] Showing entry: " << entryId << std::endl;
}

void NotebookScene::skipTextAnimation() {
    m_revealedText = m_fullText;
    m_textFullyRevealed = true;
    std::cout << "[NotebookScene] Text animation skipped" << std::endl;
}

// === ПРИВАТНЫЕ МЕТОДЫ ===

void NotebookScene::updateTextReveal(float deltaTime) {
    if (m_textFullyRevealed) {
        return;
    }

    m_textRevealTimer += deltaTime;

    // Вычислить сколько символов должно быть показано
    int charactersToReveal = static_cast<int>(m_textRevealTimer * m_charactersPerSecond);

    // Ограничить до размера полного текста
    charactersToReveal = std::min(charactersToReveal, static_cast<int>(m_fullText.size()));

    // Обновить показанный текст
    m_revealedText = m_fullText.substr(0, charactersToReveal);

    // Проверить, весь ли текст показан
    if (m_revealedText.size() >= m_fullText.size()) {
        m_textFullyRevealed = true;
        std::cout << "[NotebookScene] Text fully revealed" << std::endl;
    }
}

void NotebookScene::renderText(sf::RenderWindow& window) {
    const float PADDING = 50.0f;
    const float LINE_SPACING = 1.5f;

    // === ФОН ЗАПИСКИ (цвет старой бумаги) ===
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(UI::SCREEN_WIDTH * 0.9f, UI::SCREEN_HEIGHT * 0.85f));
    background.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f, UI::SCREEN_HEIGHT * 0.1f));
    background.setFillColor(sf::Color(245, 235, 220));  // Старая бумага
    background.setOutlineColor(sf::Color(100, 90, 80));
    background.setOutlineThickness(2.0f);
    window.draw(background);

    float yPos = UI::SCREEN_HEIGHT * 0.1f + PADDING;

    // === ЗАГОЛОВОК (ID записи) ===
    sf::Text titleText(m_font);
    titleText.setString(utf8("ДНЕВНИК ПУТЕШЕСТВЕННИКА"));
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color(50, 40, 30));  // Темно-коричневый
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    window.draw(titleText);

    yPos += 50.0f;

    // === ТЕКСТ ЗАПИСИ (с анимацией печати) ===
    sf::Text bodyText(m_font);
    bodyText.setString(utf8(m_revealedText));
    bodyText.setCharacterSize(18);
    bodyText.setFillColor(sf::Color(50, 40, 30));
    bodyText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    bodyText.setLineSpacing(LINE_SPACING);

    window.draw(bodyText);

    // === ПОДСКАЗКА ПРОПУСКА (если анимация идет) ===
    if (!m_textFullyRevealed && m_canSkipText) {
        yPos = UI::SCREEN_HEIGHT * 0.9f - 30.0f;
        sf::Text hintText(m_font);
        hintText.setString(utf8("[Нажмите ПРОБЕЛ, чтобы пропустить]"));
        hintText.setCharacterSize(14);
        hintText.setFillColor(sf::Color(120, 110, 100));  // Светло-коричневый
        hintText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.5f - 150.0f, yPos));
        window.draw(hintText);
    }
}

void NotebookScene::renderChoices(sf::RenderWindow& window) {
    if (m_choices.empty()) {
        return;
    }

    const float PADDING = 50.0f;
    float yPos = UI::SCREEN_HEIGHT * 0.65f;

    // === ЗАГОЛОВОК ВЫБОРОВ ===
    sf::Text choicesHeader(m_font);
    choicesHeader.setString(utf8("ЧТО ДЕЛАТЬ?"));
    choicesHeader.setCharacterSize(20);
    choicesHeader.setFillColor(sf::Color(100, 50, 50));  // Красноватый
    choicesHeader.setStyle(sf::Text::Bold);
    choicesHeader.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    window.draw(choicesHeader);

    yPos += 40.0f;

    // === ОТРИСОВКА КАЖДОГО ВЫБОРА ===
    for (size_t i = 0; i < m_choices.size(); i++) {
        const auto& choice = m_choices[i];

        // Подсветка выбранного варианта
        if (static_cast<int>(i) == m_selectedChoiceIndex) {
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(UI::SCREEN_WIDTH * 0.75f, 30.0f));
            highlight.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING - 10.0f, yPos - 5.0f));
            highlight.setFillColor(sf::Color(255, 240, 200, 150));  // Светло-желтая подсветка
            window.draw(highlight);
        }

        // Стрелка для выбранного варианта
        std::string arrow = (static_cast<int>(i) == m_selectedChoiceIndex) ? "▸ " : "  ";

        // Номер варианта
        std::string choiceText = arrow + "[" + std::to_string(i + 1) + "] " + choice.text;

        sf::Text choiceTextSf(m_font);
        choiceTextSf.setString(utf8(choiceText));
        choiceTextSf.setCharacterSize(18);

        // Цвет зависит от доступности
        if (choice.isDisabled) {
            choiceTextSf.setFillColor(sf::Color(150, 140, 130));  // Серый для недоступных
        } else {
            choiceTextSf.setFillColor(sf::Color(50, 40, 30));
        }

        choiceTextSf.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
        window.draw(choiceTextSf);

        // Причина недоступности
        if (choice.isDisabled && !choice.disabledReason.empty()) {
            sf::Text reasonText(m_font);
            reasonText.setString(utf8("  ⚠️ " + choice.disabledReason));
            reasonText.setCharacterSize(14);
            reasonText.setFillColor(sf::Color(180, 50, 50));  // Красный
            reasonText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING + 30.0f, yPos + 22.0f));
            window.draw(reasonText);

            yPos += 50.0f;  // Больше места для двустрочного варианта
        } else {
            yPos += 35.0f;
        }
    }

    // === ПОДСКАЗКА УПРАВЛЕНИЯ ===
    yPos = UI::SCREEN_HEIGHT * 0.9f - 30.0f;
    sf::Text hintText(m_font);
    hintText.setString(utf8("[Используйте 1-9 или стрелки + Enter]"));
    hintText.setCharacterSize(14);
    hintText.setFillColor(sf::Color(120, 110, 100));  // Светло-коричневый
    hintText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.5f - 200.0f, yPos));
    window.draw(hintText);
}

void NotebookScene::renderHUD(sf::RenderWindow& window) {
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

void NotebookScene::handleChoice(int choiceIndex) {
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(m_choices.size())) {
        return;
    }

    const auto& choice = m_choices[choiceIndex];

    // Пропустить недоступные варианты
    if (choice.isDisabled) {
        std::cout << "[NotebookScene] Choice disabled: " << choice.disabledReason << std::endl;
        return;
    }

    std::cout << "[NotebookScene] Player chose: " << choice.text << std::endl;

    // Перейти к следующей записи
    if (!choice.nextEntryId.empty()) {
        showEntry(choice.nextEntryId);
    } else {
        // Если нет следующей записи, вернуться в главное меню
        m_isFinished = true;
        m_nextScene = SceneType::MAIN_MENU;
    }
}

void NotebookScene::moveSelectionUp() {
    if (m_choices.empty()) return;

    m_selectedChoiceIndex--;
    if (m_selectedChoiceIndex < 0) {
        m_selectedChoiceIndex = static_cast<int>(m_choices.size()) - 1;
    }

    std::cout << "[NotebookScene] Selection moved up to: " << m_selectedChoiceIndex << std::endl;
}

void NotebookScene::moveSelectionDown() {
    if (m_choices.empty()) return;

    m_selectedChoiceIndex++;
    if (m_selectedChoiceIndex >= static_cast<int>(m_choices.size())) {
        m_selectedChoiceIndex = 0;
    }

    std::cout << "[NotebookScene] Selection moved down to: " << m_selectedChoiceIndex << std::endl;
}

void NotebookScene::loadTestEntry() {
    // Тестовая запись для прототипа
    m_fullText =
        "Утро. 7 апреля 1995 года.\n\n"
        "Я решил. Еду в Германию.\n"
        "Сергей обещал работу, жилье.\n\n"
        "Путь неблизкий - 2500 км через всю страну.\n"
        "Москва → Смоленск → Минск → Варшава → Берлин.\n\n"
        "Пора.";

    m_revealedText.clear();
    m_textRevealTimer = 0.0f;
    m_textFullyRevealed = false;

    // Добавить тестовые выборы
    m_choices.clear();

    SimpleChoice choice1;
    choice1.text = "Выехать из города";
    choice1.nextEntryId = "day1_departure";
    choice1.isDisabled = false;
    m_choices.push_back(choice1);

    SimpleChoice choice2;
    choice2.text = "Вернуться домой";
    choice2.nextEntryId = "";  // Вернуться в главное меню
    choice2.isDisabled = false;
    m_choices.push_back(choice2);

    SimpleChoice choice3;
    choice3.text = "Заправиться перед дорогой";
    choice3.nextEntryId = "gas_station";
    choice3.isDisabled = true;
    choice3.disabledReason = "Пока не реализовано";
    m_choices.push_back(choice3);
}

void NotebookScene::loadDayOneDeparture() {
    // Запись о выезде из города - продолжение
    m_fullText =
        "Выезжаю из города.\n\n"
        "Мама плакала.\n"
        "Я обещал писать. Отправлять деньги.\n\n"
        "Впереди 2500 км.\n"
        "Первый город - Смоленск.";

    m_revealedText.clear();
    m_textRevealTimer = 0.0f;
    m_textFullyRevealed = false;

    // Выборы для второй страницы
    m_choices.clear();

    SimpleChoice choice1;
    choice1.text = "Продолжить путь";
    choice1.nextEntryId = "";  // Пока вернется в меню
    choice1.isDisabled = false;
    m_choices.push_back(choice1);

    SimpleChoice choice2;
    choice2.text = "Вернуться";
    choice2.nextEntryId = "";
    choice2.isDisabled = false;
    m_choices.push_back(choice2);
}
