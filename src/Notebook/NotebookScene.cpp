#include "Notebook/NotebookScene.h"
#include "UIConstants.h"
#include "EventHelper.h"
#include "GameStateManager.h"
#include "EventFactory.h"  // Для инициализации событий
#include <iostream>
#include <sstream>
#include <cstdlib>  // Для rand()

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
      m_choiceTimer(0.0f),
      m_eventManager(),
      m_resourceEventSystem(playerState, &m_eventManager),  // Инициализация системы ресурсов
      m_eventTriggerChance(0.2f),    // 20% шанс случайного события
      m_pendingEventId(""),
      m_pendingNextEntryId(""),      // ID следующей записи после события
      m_fontLoaded(false) {

    // Загрузить шрифты для разных типов записей
    if (auto fontOpt = FontLoader::load()) {
        m_fontPresent = *fontOpt;
        m_fontPast = *fontOpt;
        m_fontThought = *fontOpt;
        m_fontLoaded = true;
        std::cout << "[NotebookScene] Fonts loaded successfully for all entry types" << std::endl;
    } else {
        std::cerr << "[NotebookScene] Failed to load fonts" << std::endl;
    }

    // Загрузить День 0
    loadDay0_KnockOnDoor();
    loadDay0_Volga();
    loadDay0_FirstTask();

    // Загрузить ветки обучения
    loadDay0_BranchA_MapAndTower();
    loadDay0_BranchA_Geologist();
    loadDay0_BranchA_3();
    loadDay0_BranchA_4();
    loadDay0_BranchA_5();

    loadDay0_BranchB_Engine();
    loadDay0_BranchB_Blacksmith();
    loadDay0_BranchB_3();
    loadDay0_BranchB_4();
    loadDay0_BranchB_5();

    loadDay0_BranchC_Truck();
    loadDay0_BranchC_Teahouse();
    loadDay0_BranchC_3();
    loadDay0_BranchC_4();
    loadDay0_BranchC_5();

    loadDay0_BranchD_Route();
    loadDay0_BranchD_Boys();
    loadDay0_BranchD_3();
    loadDay0_BranchD_4();
    loadDay0_BranchD_5();

    // Загрузить общие записи
    loadDay0_Provocation();  // НОВАЯ ЗАПИСЬ
    loadDay0_GarageFinale();
    loadDay0_TransitionPresent();

    // Загрузить День 1 (начало)
    loadDay1_BorderCrossing();
    loadDay1_CustomsCheck();

    // Загрузить тестовые записи
    loadTestThoughtSystem();

    // Загрузить демо-контент (простой пример)
    loadDemo_Start();
    loadDemo_Road();
    loadDemo_Choice();
    loadDemo_Highway();
    loadDemo_Backroad();
    loadDemo_Finale();

    // Инициализировать систему случайных событий
    EventFactory::initializeAllEvents(m_eventManager);
    std::cout << "[NotebookScene] EventManager initialized with "
              << m_eventManager.getAllEvents().size() << " events" << std::endl;

    // Показать начальную запись
    showEntry(m_currentEntryId);

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

    // Обновить таймер выбора (если есть выборы и текст полностью показан)
    if (m_textFullyRevealed && !m_choices.empty()) {
        m_choiceTimer += deltaTime;
    }

    // Проверка ресурсов убрана из автоматического обновления
    // Теперь события срабатывают только после выборов игрока в handleChoice()
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

    // Сохранить текущую запись в PlayerState для сохранения игры
    if (m_playerState) {
        m_playerState->setCurrentNotebookEntryId(entryId);
    }

    m_revealedText.clear();
    m_textRevealTimer = 0.0f;
    m_textFullyRevealed = false;
    m_selectedChoiceIndex = 0;

    // Сбросить таймер выбора
    m_choiceTimer = 0.0f;

    // Очистить сохраненный следующий entry (если был)
    // Это важно, т.к. мы уже перешли к нужной записи
    m_pendingNextEntryId.clear();

    // Найти запись по ID
    auto it = m_entries.find(entryId);
    if (it != m_entries.end()) {
        m_currentEntry = it->second;
        m_fullText = m_currentEntry.text;
        m_charactersPerSecond = m_currentEntry.printSpeed;
        m_canSkipText = m_currentEntry.canSkip;

        // Получить доступные выборы для игрока
        m_choices = m_currentEntry.getAvailableChoices(m_playerState);

        std::cout << "[NotebookScene] Loaded entry: " << entryId
                  << " (type: " << static_cast<int>(m_currentEntry.type) << ")"
                  << " with " << m_choices.size() << " available choices" << std::endl;
    } else {
        std::cerr << "[NotebookScene] Entry not found: " << entryId << std::endl;
        m_fullText = "Запись не найдена: " + entryId;
        m_choices.clear();
    }
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
    const float LINE_SPACING = 1.0f;  // Уменьшено с 1.5f до 1.0f
    const float MAX_TEXT_WIDTH = UI::SCREEN_WIDTH * 0.8f;  // Максимальная ширина текста

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
    sf::Text titleText(m_fontPresent);  // Всегда используем шрифт PRESENT для заголовка
    titleText.setString(utf8("ДНЕВНИК ПУТЕШЕСТВЕННИКА"));
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color(50, 40, 30));  // Темно-коричневый
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    window.draw(titleText);

    yPos += 50.0f;

    // === ТЕКСТ ЗАПИСИ (с анимацией печати и автопереносом) ===
    // Перенести текст по словам с учетом ширины
    std::string wrappedText = wrapText(m_revealedText, getCurrentFont(), getCurrentFontSize(), MAX_TEXT_WIDTH);

    // Используем шрифт, цвет и размер в зависимости от типа записи
    sf::Text bodyText(getCurrentFont());
    bodyText.setString(utf8(wrappedText));
    bodyText.setCharacterSize(getCurrentFontSize());
    bodyText.setFillColor(getCurrentTextColor());
    bodyText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    bodyText.setLineSpacing(LINE_SPACING);

    window.draw(bodyText);

    // === ПОДСКАЗКА ПРОПУСКА (если анимация идет) ===
    if (!m_textFullyRevealed && m_canSkipText) {
        yPos = UI::SCREEN_HEIGHT * 0.9f - 30.0f;
        sf::Text hintText(m_fontPresent);
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
    sf::Text choicesHeader(m_fontPresent);  // Выборы всегда в настоящем времени
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

        sf::Text choiceTextSf(m_fontPresent);
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
            sf::Text reasonText(m_fontPresent);
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
    sf::Text hintText(m_fontPresent);
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
    sf::Text fuel(m_fontPresent);
    fuel.setString(utf8(fuelText.str()));
    fuel.setCharacterSize(18);
    fuel.setFillColor(sf::Color(200, 180, 120));
    fuel.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(fuel);

    xPos += 250.0f;

    // Энергия
    std::stringstream energyText;
    energyText << "ЭНЕРГИЯ: " << static_cast<int>(m_playerState->getEnergy()) << "%";
    sf::Text energy(m_fontPresent);
    energy.setString(utf8(energyText.str()));
    energy.setCharacterSize(18);
    energy.setFillColor(sf::Color(200, 180, 120));
    energy.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(energy);

    xPos += 250.0f;

    // Деньги
    std::stringstream moneyText;
    moneyText << "ДЕНЬГИ: " << static_cast<int>(m_playerState->getMoney()) << " ₽";
    sf::Text money(m_fontPresent);
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

    // Выполнить действие выбора (если есть)
    if (choice.action) {
        choice.action(m_playerState);
    }

    // Перейти к следующей записи
    if (!choice.nextEntryIds.empty() && !choice.nextEntryIds[0].empty()) {
        // Проверить, есть ли ожидающее событие
        if (!m_pendingEventId.empty()) {
            std::cout << "[NotebookScene] Showing pending event: " << m_pendingEventId << std::endl;
            showEventAsEntry(m_pendingEventId);
            return;
        }

        // ВАЖНО: Сохранить куда нужно перейти ПЕРЕД вызовом события
        std::string nextEntryId = choice.nextEntryIds[0];

        // СНАЧАЛА проверить критические ресурсы (наивысший приоритет)
        std::string resourceEventId = m_resourceEventSystem.checkResourcesAndTriggerEvents();
        if (!resourceEventId.empty()) {
            std::cout << "[NotebookScene] CRITICAL: Resource event triggered: " << resourceEventId << std::endl;
            m_pendingNextEntryId = nextEntryId; // Сохранить следующую запись
            showEventAsEntry(resourceEventId);
            return;
        }

        // Попытаться вызвать случайное событие при переходе
        if (tryTriggerRandomEvent()) {
            std::cout << "[NotebookScene] Random event triggered, saving next entry: " << nextEntryId << std::endl;
            m_pendingNextEntryId = nextEntryId; // Сохранить следующую запись
            showEventAsEntry(m_pendingEventId);
            return;
        }

        // Нормальный переход к следующей записи
        // TODO: В будущем здесь будет обработка вероятностей для ветвления
        // Пока просто берем первую запись
        showEntry(nextEntryId);

        // Автосохранение после каждого перехода
        GameStateManager::getInstance().autoSave();
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

// === ДЕНЬ 0: УРОКИ ДОРОГИ ===

void NotebookScene::loadDay0_KnockOnDoor() {
    // ЗАПИСЬ 1.1: СТУК В ДВЕРЬ
    NotebookEntry entry("day0_knock", EntryType::PAST,
        "ВОСПОМИНАНИЕ: ПОЛГОДА НАЗАД, 6:00\n\n"
        "Стук в дверь разбудил точнее будильника. Три чётких удара, никакой лишней настойчивости. "
        "Я открыл. Он стоял на пороге в рабочей робе, пахнущей соляркой и металлом. "
        "В глазах — не усталость, а конкретное дело.\n\n"
        "«Подъём. Твой диплом — кусок картона. Сегодня будет настоящая аттестация. "
        "Садись в машину. Буду принимать работу.»");

    entry.date = "Полгода назад, 6:00";
    entry.location = "Дом";
    entry.mood = "Внезапное пробуждение";
    entry.printSpeed = 40.0f;  // Медленнее для погружения

    NotebookChoice choice1;
    choice1.text = "[Продолжить]";
    choice1.nextEntryIds = {"day0_volga"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry: " << entry.id << std::endl;
}

void NotebookScene::loadDay0_Volga() {
    // ЗАПИСЬ 1.2: МАШИНА
    NotebookEntry entry("day0_volga", EntryType::PAST,
        "ВОСПОМИНАНИЕ: 6:20\n\n"
        "Его машина была нестарой, но потёртой, с царапинами на порогах. "
        "В салоне — запах бензина, перегара вчерашнего и яблока. "
        "Он швырнул на заднее сиденье кожаную папку.\n\n"
        "«Спишь? Выключай. Сегодня будешь глазами и мозгами.»\n\n"
        "Двигатель взревел с первой попытки.\n\n"
        "«Слушай. Это не просто шум. Это отчёт о состоянии.»");

    entry.date = "Полгода назад, 6:20";
    entry.location = "Волга";
    entry.mood = "Рабочая атмосфера";
    entry.printSpeed = 40.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить]";
    choice1.nextEntryIds = {"day0_first_task"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry: " << entry.id << std::endl;
}

void NotebookScene::loadDay0_FirstTask() {
    // ЗАПИСЬ 2: ПЕРВАЯ ЗАДАЧА - ВЫБОР ПОДХОДА (4 ветки)
    NotebookEntry entry("day0_first_task", EntryType::PAST,
        "ДИАЛОГ: 6:45, ЗА ГОРОДОМ\n\n"
        "Молчали, пока не кончилась асфальтовая лента. "
        "На грунтовке машина дёрнулась, будто споткнулась. Потом ещё раз.\n\n"
        "«Чувствуешь? — спросил он, не глядя. — Твои пять минут. Объясняй.»");

    entry.date = "Полгода назад, 6:45";
    entry.location = "За городом, грунтовка";
    entry.mood = "Экзаменационный";
    entry.printSpeed = 45.0f;

    // ВЕТКА А: Системный анализ (понимание контекста)
    NotebookChoice choiceA;
    choiceA.text = "Спросить, когда началось, при каких условиях";
    choiceA.nextEntryIds = {"day0_branch_a_map"};
    choiceA.action = [](PlayerState* player) {
        player->addTrait("analytical");  // Черта: аналитический
        std::cout << "[Day0] Выбрана ветка А: Системный анализ" << std::endl;
    };
    entry.addChoice(choiceA);

    // ВЕТКА Б: Чувство материала (слушать машину)
    NotebookChoice choiceB;
    choiceB.text = "Закрыть глаза, слушать двигатель и руки";
    choiceB.nextEntryIds = {"day0_branch_b_engine"};
    choiceB.action = [](PlayerState* player) {
        player->addTrait("intuitive");  // Черта: интуитивный
        std::cout << "[Day0] Выбрана ветка Б: Чувство материала" << std::endl;
    };
    entry.addChoice(choiceB);

    // ВЕТКА В: Командная работа (спросить бывалого)
    NotebookChoice choiceC;
    choiceC.text = "Спросить: «Что обычно в таких случаях?»";
    choiceC.nextEntryIds = {"day0_branch_c_truck"};
    choiceC.action = [](PlayerState* player) {
        player->addTrait("social");  // Черта: социальный
        std::cout << "[Day0] Выбрана ветка В: Командная работа" << std::endl;
    };
    entry.addChoice(choiceC);

    // ВЕТКА Г: Ресурсоэффективность (быстро и по делу)
    NotebookChoice choiceD;
    choiceD.text = "Сразу лезть в багажник за инструментом";
    choiceD.nextEntryIds = {"day0_branch_d_route"};
    choiceD.action = [](PlayerState* player) {
        player->addTrait("practical");  // Черта: практичный
        std::cout << "[Day0] Выбрана ветка Г: Ресурсоэффективность" << std::endl;
    };
    entry.addChoice(choiceD);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry: " << entry.id << std::endl;
}

// === ВЕТКА А: СИСТЕМНЫЙ АНАЛИЗ (Понимание контекста) ===

void NotebookScene::loadDay0_BranchA_MapAndTower() {
    // ЗАПИСЬ 3А: КАРТА И ВЫШКИ - Системный подход к навигации
    NotebookEntry entry("day0_branch_a_map", EntryType::PAST,
        "ВЕТКА А: СИСТЕМНЫЙ АНАЛИЗ\n"
        "8:30, Главная дорога\n\n"
        "Мы едем по трассе. Наставник откладывает карту на торпедо.\n\n"
        "— Карта врёт, — говорит он спокойно. — Не потому что плохая. "
        "Потому что старая. Смотри.\n\n"
        "Он показывает в окно:\n\n"
        "— Видишь вышки сотовой связи? Если они идут густо — значит город близко. "
        "Редко стоят — деревня или пустошь. Совсем нет — глушь.\n\n"
        "Я смотрю внимательнее. Действительно — впереди вышки учащаются.\n\n"
        "— А ещё смотри на асфальт, — продолжает он. — Если ровный, свежий — значит "
        "дорога важная, ремонтируют регулярно. Если ямы, заплатки — значит забыли.\n\n"
        "— То есть система дорог сама подсказывает?\n\n"
        "— Именно. Система ВСЕГДА даёт подсказки. Надо просто смотреть правильно.\n\n"
        "Он сворачивает на развилку. Указателя нет.\n\n"
        "— Куда едем? — спрашиваю я.\n\n"
        "— А ты сам определи. Посмотри на дороги.\n\n"
        "Я вижу:\n"
        "• Налево — асфальт свежий, широкий, есть разметка\n"
        "• Направо — узкая дорога, старый асфальт, заплатки\n\n"
        "— Налево, — говорю уверенно. — Там основная дорога.\n\n"
        "— Правильно. Видишь? Не нужна карта. Нужно понимать систему. "
        "Это работает везде: на дороге, на заводе, в жизни.\n\n"
        "Мы едем дальше. Я смотрю на дорогу по-новому.");

    entry.date = "Полгода назад, 8:30";
    entry.location = "Главная дорога";
    entry.mood = "Обучающий";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путь]";
    choice1.nextEntryIds = {"day0_branch_a_geologist"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch A-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchA_Geologist() {
    // ЗАПИСЬ 4А: ГЕОЛОГ - Системное мышление в действии + получение артефакта
    NotebookEntry entry("day0_branch_a_geologist", EntryType::PAST,
        "10:00, Придорожная заправка\n\n"
        "Пока Наставник заправляет машину, к нам подходит мужчина в потрёпанной куртке. "
        "На груди бирка с надписью \"Геолог\".\n\n"
        "— Сколько лет! — кричит он, узнав Наставника.\n\n"
        "Они обнимаются. Наставник поворачивается ко мне:\n\n"
        "— Знакомься. Это Геолог. Проектировал буровые установки в Сибири. "
        "Умнейший инженер.\n\n"
        "Геолог усмехается:\n\n"
        "— Расскажи мне лучше про этого парня. Ты же просто так людей не возишь.\n\n"
        "— Едет на производство в другой город. Хочет учиться, как станки устроены.\n\n"
        "Глаза геолога загораются:\n\n"
        "— Промышленное оборудование? Подожди-ка...\n\n"
        "Он достаёт из рюкзака потрёпанный блокнот в кожаной обложке:\n\n"
        "— Вот смотри. Я всегда вожу это с собой. Здесь я записываю ВСЁ про оборудование, "
        "которое видел за 30 лет:\n"
        "• Технические характеристики\n"
        "• Где используется\n"
        "• Кто производитель\n"
        "• Как можно улучшить\n\n"
        "Он листает страницы — чертежи, схемы, расчёты.\n\n"
        "— Видишь? Система. Когда ты систематизируешь знания — они начинают работать на тебя. "
        "Случайные наблюдения превращаются в опыт.\n\n"
        "Он отрывает несколько страниц:\n\n"
        "— Здесь мои заметки о современных токарных станках. Я видел их на выставке. "
        "Когда доедешь — пригодится. Держи.\n\n"
        "Я беру страницы. Почерк мелкий, но аккуратный. Схемы точные.\n\n"
        "— Спасибо, — говорю я.\n\n"
        "— Не за что. Только обещай: когда вернёшься — запиши ВСЁ, что увидишь. "
        "Не в голове держи. На бумаге. Система знаний сильнее памяти.\n\n"
        "Наставник кивает:\n\n"
        "— Слушай его. Геолог всегда прав.");

    entry.date = "Полгода назад, 10:00";
    entry.location = "Придорожная заправка";
    entry.mood = "Встреча с мудрым человеком";
    entry.printSpeed = 45.0f;

    // Выбор 1: Взять журнал (знания - правильный путь)
    NotebookChoice choiceJournal;
    choiceJournal.text = "[Взять заметки о станках]";
    choiceJournal.nextEntryIds = {"day0_branch_a_3"};  // Теперь ведет к 3-й записи ветки
    choiceJournal.action = [](PlayerState* player) {
        player->addStoryItem("technical_journal");  // Артефакт: Журнал
        player->addPrinciple("understanding_context");  // ПРИНЦИП: Понимание контекста
        player->modifyEnergy(-5.0f);  // Чтение и изучение
        std::cout << "[Day0-A] Получен артефакт: Журнал с пометками" << std::endl;
        std::cout << "[Day0-A] Получен принцип: Системное понимание контекста" << std::endl;
    };
    entry.addChoice(choiceJournal);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch A-2): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchA_3() {
    // ЗАПИСЬ 3А: СИСТЕМНЫЙ АНАЛИЗ - Учимся видеть связи
    NotebookEntry entry("day0_branch_a_3", EntryType::PAST,
        "ВЕТКА А: СИСТЕМНЫЙ АНАЛИЗ\n"
        "11:30, Остановка у придорожного кафе\n\n"
        "Наставник заказывает чай, разворачивает блокнот на столе.\n\n"
        "— Запиши: «Машина — это система. Система — это связи между частями».\n\n"
        "Я записываю.\n\n"
        "— Смотри, — он чертит простую схему. — Двигатель сжигает топливо. "
        "Генератор от двигателя заряжает аккумулятор. Аккумулятор питает стартер. "
        "Стартер заводит двигатель. Видишь круг?\n\n"
        "Киваю.\n\n"
        "— Теперь представь: сел аккумулятор. Что происходит с системой?\n\n"
        "— Стартер не крутит. Двигатель не заводится.\n\n"
        "— Точно. А если генератор сломан?\n\n"
        "— Аккумулятор разряжается. Потом то же самое — машина встанет.\n\n"
        "— Правильно. Теперь главный вопрос: где начинается проблема?\n\n"
        "Молчу. Думаю.\n\n"
        "— Проблема начинается там, где ты не видишь связи. Многие меняют аккумулятор, "
        "потому что он сел. Но если генератор не заряжает — новый аккумулятор тоже сядет. "
        "Понимаешь?\n\n"
        "— Понимаю. Надо смотреть на всю систему, а не на одну деталь.\n\n"
        "— Вот именно. Записывай: «Чини не симптом, а причину. Ищи слабое звено в цепи».");

    entry.date = "Полгода назад, 11:30";
    entry.location = "Придорожное кафе";
    entry.mood = "Системное мышление";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить урок]";
    choice1.nextEntryIds = {"day0_branch_a_4"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch A-3): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchA_4() {
    // ЗАПИСЬ 4А: ПРАКТИКА СИСТЕМНОГО АНАЛИЗА - Диагностика
    NotebookEntry entry("day0_branch_a_4", EntryType::PAST,
        "13:00, Снова в пути\n\n"
        "Едем дальше. Вдруг машина начинает вибрировать на скорости.\n\n"
        "— Чувствуешь? — спрашивает Наставник.\n\n"
        "— Да. Трясёт.\n\n"
        "— Где трясёт? Руль? Кузов? Сиденье?\n\n"
        "Прислушиваюсь.\n\n"
        "— Руль вибрирует. Особенно на 80-90 км/ч.\n\n"
        "— Хорошо. Анализируй систему: что связано с рулём?\n\n"
        "Думаю вслух:\n\n"
        "— Передние колёса. Подвеска. Рулевая рейка.\n\n"
        "— Отлично. Теперь сужай круг: вибрация появляется на скорости. "
        "Что из этого зависит от скорости?\n\n"
        "— Колёса. Они же крутятся быстрее.\n\n"
        "— Молодец. Теперь что может быть с колёсами?\n\n"
        "— Балансировка? Или кривой диск?\n\n"
        "Наставник улыбается:\n\n"
        "— Точно. Видишь, как работает логика? Симптом → система → элементы → проверка. "
        "Вот это и есть диагностика.\n\n"
        "Он сворачивает на станцию. Мастер проверяет балансировку — действительно, "
        "переднее правое колесо разбалансировано.\n\n"
        "Пока ждём, Наставник говорит:\n\n"
        "— Запиши в блокнот: «Системный анализ — это не магия. Это метод. "
        "Разбивай проблему на части. Ищи связи. Проверяй гипотезы». Это работает не только с машинами.");

    entry.date = "Полгода назад, 13:00";
    entry.location = "В пути";
    entry.mood = "Практическая диагностика";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Записать метод]";
    choice1.nextEntryIds = {"day0_branch_a_5"};
    choice1.action = [](PlayerState* player) {
        player->modifyMoney(-15.0f);  // Балансировка колеса
    };
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch A-4): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchA_5() {
    // ЗАПИСЬ 5А: ЗАВЕРШЕНИЕ ВЕТКИ А - Закрепление принципа
    NotebookEntry entry("day0_branch_a_5", EntryType::PAST,
        "15:00, Последняя остановка перед городом\n\n"
        "Останавливаемся на смотровой площадке. Наставник достаёт термос с кофе.\n\n"
        "— Ну что, понял систему?\n\n"
        "Киваю, листаю блокнот. Там уже несколько страниц записей: схемы, заметки, связи.\n\n"
        "— Понял. Машина — это не набор деталей. Это система связей. "
        "Если сломалось что-то — надо смотреть на всю цепочку.\n\n"
        "— Правильно. Но главное не это.\n\n"
        "Я смотрю на него.\n\n"
        "— Главное — этот подход работает везде. В любой технике. В любой работе. "
        "В жизни вообще. Проблема → анализ → связи → решение. Это универсальный метод.\n\n"
        "Он хлопает меня по плечу:\n\n"
        "— Вот ты сегодня научился не просто чинить машину. Ты научился думать системно. "
        "Это дороже любого инструмента.\n\n"
        "Допиваю кофе. Смотрю на машину. Теперь вижу её по-другому — не как груду железа, "
        "а как организм. Где всё связано.\n\n"
        "— Спасибо, — говорю.\n\n"
        "— Не благодари. Пользуйся. И передавай дальше, когда будешь учить кого-то.\n\n"
        "Я закрываю блокнот. Этот блокнот теперь — не просто тетрадка. Это инструмент. "
        "Способ мыслить.");

    entry.date = "Полгода назад, 15:00";
    entry.location = "Смотровая площадка";
    entry.mood = "Понимание системы";
    entry.printSpeed = 45.0f;

    NotebookChoice choiceFinish;
    choiceFinish.text = "[Ехать дальше]";
    choiceFinish.nextEntryIds = {"day0_provocation"};
    entry.addChoice(choiceFinish);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch A-5): " << entry.id << std::endl;
}

// === ВЕТКА Б: СЛУШАТЬ МАТЕРИАЛ (Чувство машины) ===

void NotebookScene::loadDay0_BranchB_Engine() {
    // ЗАПИСЬ 3Б: СЛУШАТЬ ДВИГАТЕЛЬ - Практический подход
    NotebookEntry entry("day0_branch_b_engine", EntryType::PAST,
        "ВЕТКА Б: ЧУВСТВО МАТЕРИАЛА\n"
        "8:30, В дороге\n\n"
        "Наставник сбрасывает скорость и съезжает на обочину.\n\n"
        "— Заглуши.\n\n"
        "Я глушу двигатель. Тишина.\n\n"
        "— Заводи на нейтрали. И не газуй.\n\n"
        "Завожу. Двигатель работает ровно на холостых.\n\n"
        "Наставник слушает секунд десять. Потом:\n\n"
        "— Четвёртый. Свеча.\n\n"
        "— Откуда знаешь?\n\n"
        "— Такт другой. Не сразу услышишь, надо привыкнуть. Включи музыку на радио.\n\n"
        "Включаю радио — обычная попса. Наставник выключает:\n\n"
        "— А теперь послушай мотор. Он тоже играет. Только у него ритм четырёхтактный. "
        "Бум-бум-бум-бум. Когда один цилиндр троит — ритм сбивается. Бум-бум-пауза-бум.\n\n"
        "Я прислушиваюсь. Сначала ничего. Потом действительно слышу: где-то "
        "проскакивает крошечная задержка.\n\n"
        "— Слышу, — говорю.\n\n"
        "— Вот. Это не магия. Это практика. Чем больше моторов послушаешь — "
        "тем быстрее научишься различать.\n\n"
        "Он достаёт свечи. Четвёртая действительно в нагаре — чёрная, как уголь.\n\n"
        "— Смотри, — показывает он. — Переобогащённая смесь. Видишь по цвету? "
        "Нормальная свеча — серо-коричневая. Чёрная — много бензина. Белая — мало.\n\n"
        "Ставит запасную. Заводим. Теперь мотор работает ровно.\n\n"
        "— Запомни: машина всегда подсказывает, что с ней не так. Звук, запах, цвет свечей. "
        "Просто надо внимательно смотреть и слушать. Без датчиков и компьютера.");

    entry.date = "Полгода назад, 8:30";
    entry.location = "Обочина дороги";
    entry.mood = "Практическое обучение";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путь]";
    choice1.nextEntryIds = {"day0_branch_b_blacksmith"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch B-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchB_Blacksmith() {
    // ЗАПИСЬ 4Б: ГАРАЖНЫЙ РЕМОНТ - Работа руками + получение артефакта
    NotebookEntry entry("day0_branch_b_blacksmith", EntryType::PAST,
        "10:00, Гараж на окраине\n\n"
        "Наставник сворачивает в гаражный кооператив. Останавливается у знакомого бокса.\n\n"
        "— Есть кто?\n\n"
        "Из гаража выходит мужик лет шестидесяти, руки по локоть в масле.\n\n"
        "— Здорово! Заходи. Как раз движок собираю.\n\n"
        "Внутри — обычный гараж. На верстаке разобранный двигатель. "
        "Запчасти, тряпки, банки с болтами.\n\n"
        "— Это молодой, — показывает Наставник. — Учу его технике. Покажи ему свои игрушки.\n\n"
        "Механик усмехается и достаёт откуда-то из-под верстака старую деревянную коробку:\n\n"
        "— Смотри. Это мой дед делал. Советские ключи, 50-е годы. Вот потрогай.\n\n"
        "Он даёт мне гаечный ключ. Тяжёлый, металл тёмный от времени и масла.\n\n"
        "— Чувствуешь? Это ещё нормальная сталь была. Сейчас такую не делают — "
        "хрупкая, гнётся. А эта — хоть танк чини.\n\n"
        "Он показывает на детали на верстаке:\n\n"
        "— Вот поршень. Видишь задиры? Перегрев. Масло плохое лили или антифриз кончился. "
        "А вот кольца — стёрлись. Смотри по цвету нагара: чёрное — переливает, белое — бедная смесь.\n\n"
        "Я беру одно кольцо. Оно тонкое, с царапинами.\n\n"
        "— Руками трогать надо, — продолжает Механик. — Не на глаз. Пальцами почувствуешь "
        "любую выработку, любую трещину. Лучше всякого микрометра.\n\n"
        "Он достаёт сигарету, я тянусь за зажигалкой — и случайно задеваю рукой только что "
        "сваренную трубу на верстаке.\n\n"
        "Горячая! Блин!\n\n"
        "Я отдёргиваю руку. Кожа покраснела.\n\n"
        "— Ну вот, — говорит Механик спокойно. — Теперь запомнишь. Сварка остывает минут двадцать. "
        "На глаз не видно, а руку сожжёт. Это урок. Будешь теперь осторожнее.\n\n"
        "Наставник кивает:\n\n"
        "— Руками работать — значит чувствовать материал. Книжки тебе температуру металла "
        "не покажут. Только опыт. Иногда болезненный.");

    entry.date = "Полгода назад, 10:00";
    entry.location = "Гараж";
    entry.mood = "Практический урок";
    entry.printSpeed = 45.0f;

    // Только один вариант — принять урок
    NotebookChoice choiceAccept;
    choiceAccept.text = "[Взять урок на заметку]";
    choiceAccept.nextEntryIds = {"day0_branch_b_3"};
    choiceAccept.action = [](PlayerState* player) {
        player->addStoryItem("burned_finger");  // Артефакт: Ожог от трубы
        player->addPrinciple("listen_material");  // ПРИНЦИП: Слушать материал
        player->modifyEnergy(-3.0f);  // Небольшая боль
        std::cout << "[Day0-B] Получен артефакт: Ожог-напоминание" << std::endl;
        std::cout << "[Day0-B] Получен принцип: Слушать материал (практический опыт)" << std::endl;
    };
    entry.addChoice(choiceAccept);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch B-2): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchB_3() {
    // ЗАПИСЬ 3Б: ЧУВСТВО ВИБРАЦИИ - Диагностика на слух и осязание
    NotebookEntry entry("day0_branch_b_3", EntryType::PAST,
        "ВЕТКА Б: ЧУВСТВО МАТЕРИАЛА\n"
        "12:00, Грунтовая дорога\n\n"
        "Съезжаем с асфальта на грунтовку. Машина начинает подпрыгивать на кочках.\n\n"
        "— Чувствуешь, как трясёт? — спрашивает Наставник.\n\n"
        "— Да. Подвеска жёсткая.\n\n"
        "— Не просто жёсткая. Слушай вибрацию. Вот кочка. Чувствуешь удар?\n\n"
        "Проезжаем ещё одну яму. Чувствую жёсткий удар в руль и спину.\n\n"
        "— Чувствую. Будто пружина не отрабатывает.\n\n"
        "— Правильно. А теперь слушай звук. Металлический лязг или глухой стук?\n\n"
        "Прислушиваюсь. Проезжаем ещё несколько кочек.\n\n"
        "— Лязг. Звенит что-то.\n\n"
        "— Точно. Это амортизатор пробит. Отбойник изношен, шток бьёт по корпусу. "
        "Слышишь металл по металлу?\n\n"
        "Он останавливается, лезет под машину. Показывает мне:\n\n"
        "— Смотри. Вот амортизатор. Видишь — шток весь в подтёках? Масло вытекло. "
        "Теперь он не гасит удары, а просто передаёт их на кузов.\n\n"
        "Трогаю амортизатор — он мокрый от масла.\n\n"
        "— Чувствуешь? Рукой можно определить утечку быстрее, чем глазом. "
        "Особенно если грязь въелась.\n\n"
        "Он вытирает руки:\n\n"
        "— Запомни: машина говорит с тобой через вибрацию, звук, запах. "
        "Твоё тело — лучший диагностический прибор. Надо только научиться слушать.");

    entry.date = "Полгода назад, 12:00";
    entry.location = "Грунтовая дорога";
    entry.mood = "Чувство вибрации";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путь]";
    choice1.nextEntryIds = {"day0_branch_b_4"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch B-3): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchB_4() {
    // ЗАПИСЬ 4Б: ЗАПАХИ И ТЕМПЕРАТУРА - Диагностика через обоняние
    NotebookEntry entry("day0_branch_b_4", EntryType::PAST,
        "13:30, Заправка\n\n"
        "Останавливаемся заправиться. Наставник заливает бензин, а я проверяю масло.\n\n"
        "Открываю капот. Достаю щуп.\n\n"
        "— Понюхай масло, — говорит Наставник.\n\n"
        "— Что?\n\n"
        "— Понюхай. Серьёзно.\n\n"
        "Подношу щуп к носу. Пахнет... маслом. Немного горьковато.\n\n"
        "— Чувствуешь горелый запах?\n\n"
        "— Немного.\n\n"
        "— Это перегрев. Масло подгорает. Не критично, но надо следить. "
        "А теперь открой крышку радиатора. Осторожно, не обожгись.\n\n"
        "Открываю. Из горловины идёт пар.\n\n"
        "— Понюхай.\n\n"
        "Нюхаю. Пахнет... сладковато?\n\n"
        "— Антифриз, — киваю.\n\n"
        "— Правильно. А если бы пахло выхлопом — значит пробита прокладка, газы идут в систему охлаждения. "
        "Видишь? По запаху можно диагностировать кучу проблем.\n\n"
        "Он показывает на тормозные диски:\n\n"
        "— Подойди, дай руку. Не трогай диск, просто поднеси.\n\n"
        "Подношу ладонь к диску. Тепло.\n\n"
        "— Чувствуешь жар? Это нормально после езды. Но если диск раскалён — значит колодки не отходят, "
        "подклинивает суппорт. Или трос ручника натянут.\n\n"
        "Он закрывает капот:\n\n"
        "— Твои чувства — это инструменты. Нос, уши, руки, глаза. Не игнорируй их.");

    entry.date = "Полгода назад, 13:30";
    entry.location = "Заправка";
    entry.mood = "Диагностика по запаху";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Ехать дальше]";
    choice1.nextEntryIds = {"day0_branch_b_5"};
    choice1.action = [](PlayerState* player) {
        player->addFuel(30.0f);  // Заправились
        player->modifyMoney(-25.0f);  // Заплатили за топливо
    };
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch B-4): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchB_5() {
    // ЗАПИСЬ 5Б: ЗАВЕРШЕНИЕ ВЕТКИ Б - Чувство материала
    NotebookEntry entry("day0_branch_b_5", EntryType::PAST,
        "15:30, Конец пути\n\n"
        "Подъезжаем к городу. Наставник сбавляет скорость, останавливается на последней обочине.\n\n"
        "— Ну что, — говорит он. — Чему научился?\n\n"
        "Думаю.\n\n"
        "— Научился слушать машину. Звук двигателя, вибрацию, запахи. "
        "Понял, что не нужны приборы — тело само чувствует проблемы.\n\n"
        "Наставник кивает:\n\n"
        "— Правильно. Это называется — чувство материала. Хороший механик "
        "может диагностировать машину вслепую. Просто слушая и трогая.\n\n"
        "Он хлопает по капоту:\n\n"
        "— Машина — это живой организм. Она дышит, греется, стареет. "
        "И если ты научишься её чувствовать — она никогда тебя не подведёт.\n\n"
        "Я смотрю на свою руку. На пальце ещё виден красный след от ожога о трубу.\n\n"
        "— Этот ожог, — говорит Наставник, — лучший учитель. Теперь ты никогда не забудешь, "
        "что сварка горячая. Так и с машиной — каждый опыт оставляет след. Хороший или плохой.\n\n"
        "Он заводит двигатель. Мотор работает ровно.\n\n"
        "— Слышишь? Вот так должно быть. Ровно, чётко. Запомни этот звук. "
        "Это эталон. Всё остальное — отклонение.\n\n"
        "Еду дальше. Теперь я слышу машину по-другому. Каждый звук, каждая вибрация — "
        "это сигнал. Язык, на котором она говорит.");

    entry.date = "Полгода назад, 15:30";
    entry.location = "Въезд в город";
    entry.mood = "Понимание материала";
    entry.printSpeed = 45.0f;

    NotebookChoice choiceFinish;
    choiceFinish.text = "[Завершить день]";
    choiceFinish.nextEntryIds = {"day0_provocation"};
    entry.addChoice(choiceFinish);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch B-5): " << entry.id << std::endl;
}

// Сохраняем старое имя для обратной совместимости
void NotebookScene::loadDay0_BranchB_Driver() {
    loadDay0_BranchB_Blacksmith();
}

// === ВЕТКА В: СПРОСИ БЫВАЛОГО (Командная работа) ===

void NotebookScene::loadDay0_BranchC_Truck() {
    // ЗАПИСЬ 3В: ПОМОЩЬ ДАЛЬНОБОЙЩИКУ - Обмен опытом
    NotebookEntry entry("day0_branch_c_truck", EntryType::PAST,
        "ВЕТКА В: КОМАНДНАЯ РАБОТА\n"
        "9:15, Обочина дороги\n\n"
        "На обочине стоит грузовик с поднятой кабиной. Водитель лежит под машиной.\n\n"
        "Наставник сбавляет скорость, останавливается.\n\n"
        "— Что случилось? — кричит он.\n\n"
        "— Да стартер сдох, — вылезает дальнобойщик, весь в масле. — Замкнуло где-то. "
        "Уже час ковыряюсь.\n\n"
        "Наставник подходит, смотрит:\n\n"
        "— Проводка. Видишь — изоляция перетёрлась. Вот тут короткое.\n\n"
        "— Блин, точно. А я стартер разбирал...\n\n"
        "Наставник поворачивается ко мне:\n\n"
        "— Помогай. Мы с ним провод заменим, ты инструменты подавай.\n\n"
        "Работаем втроём минут двадцать. Дальнобойщик рассказывает:\n\n"
        "— Я вчера на этой же дороге видел машину стоит. У парня генератор полетел. "
        "Я ему запасной отдал — у меня всегда с собой. Он мне номер оставил, говорит, в городе отдаст.\n\n"
        "— А ты не боишься, что кинет?\n\n"
        "— Да ладно. Мы ж все тут друг друга знаем. Сегодня я ему помог, завтра он мне. "
        "В дороге без этого никак.\n\n"
        "Меняем провод. Заводим — работает.\n\n"
        "— Спасибо, мужики. Выручили.\n\n"
        "Наставник закуривает:\n\n"
        "— Видишь? Одному бы ты час ещё ковырялся. А втроём — двадцать минут. "
        "В дороге, на производстве — везде так. Один в поле не воин.");

    entry.date = "Полгода назад, 9:15";
    entry.location = "Обочина дороги";
    entry.mood = "Взаимопомощь";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путь]";
    choice1.nextEntryIds = {"day0_branch_c_teahouse"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchC_Teahouse() {
    // ЗАПИСЬ 4В: ПРИДОРОЖНОЕ КАФЕ - Обмен знаниями + получение артефакта
    NotebookEntry entry("day0_branch_c_teahouse", EntryType::PAST,
        "10:30, Придорожное кафе \"Огонёк\"\n\n"
        "Дальнобойщик пригласил нас на чай в ближайшее кафе.\n\n"
        "Внутри — обычная столовая. Пластиковые столы, запах котлет.\n\n"
        "За соседним столом сидят ещё двое водителей. Один из них поднимает голову:\n\n"
        "— Ты что тут делаешь?\n\n"
        "— Сто лет не виделись! — Наставник жмёт ему руку.\n\n"
        "Оказывается, они вместе работали на заводе лет десять назад. Садимся за один стол.\n\n"
        "Разговор идёт о работе. Токарь работает теперь на другом предприятии.\n\n"
        "— Слушай, — говорит он. — Помнишь, у нас на третьем цехе была проблема "
        "с креплением? Мы тогда месяц бились.\n\n"
        "— Помню. А что?\n\n"
        "— Так я недавно нашёл решение! Вот смотри, — достаёт из кармана блокнот, рисует схему. — "
        "Надо было просто шайбу дополнительную поставить. Вот тут.\n\n"
        "Наставник смотрит, кивает:\n\n"
        "— Точно. Надо же. Мы тогда не додумались.\n\n"
        "Я слушаю их разговор. Они обмениваются опытом — кто что решил, кто где работает, "
        "какие проблемы были.\n\n"
        "Токарь поворачивается ко мне:\n\n"
        "— А ты кто?\n\n"
        "— Инженер. Еду учиться на производство.\n\n"
        "— О, хорошо. Слушай, у меня знакомые там работают. "
        "Если что — вот телефон, — пишет на салфетке. — Скажешь, от меня. Помогут.\n\n"
        "Он достаёт из бумажника старую фотографию:\n\n"
        "— Вот мы на заводе, 85-й год. Бригада токарей. Хорошее время было.\n\n"
        "На фото — несколько мужиков у станка, все улыбаются.\n\n"
        "Наставник говорит мне тихо:\n\n"
        "— Запомни: опыт не в книгах. Опыт — у людей. Спрашивай, общайся, помогай. "
        "Сегодня ты кому-то помог — завтра тебе помогут.");

    entry.date = "Полгода назад, 10:30";
    entry.location = "Кафе \"Огонёк\"";
    entry.mood = "Братство дороги";
    entry.printSpeed = 45.0f;

    // Один выбор — принять артефакт
    NotebookChoice choicePhoto;
    choicePhoto.text = "[Взять фотографию и телефон]";
    choicePhoto.nextEntryIds = {"day0_branch_c_3"};
    choicePhoto.action = [](PlayerState* player) {
        player->addStoryItem("photo_at_machine");  // Артефакт: Фото бригады
        player->addPrinciple("ask_experienced");  // ПРИНЦИП: Спрашивать у опытных
        std::cout << "[Day0-C] Получен артефакт: Фотография бригады у станка" << std::endl;
        std::cout << "[Day0-C] Получен принцип: Спрашивать у опытных" << std::endl;
    };
    entry.addChoice(choicePhoto);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-2): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchC_3() {
    // ЗАПИСЬ 3В: ОБМЕН ОПЫТОМ - Ценность коммуникации
    NotebookEntry entry("day0_branch_c_3", EntryType::PAST,
        "ВЕТКА В: КОМАНДНАЯ РАБОТА\n"
        "12:00, В пути\n\n"
        "Едем дальше. Наставник достаёт термос с кофе.\n\n"
        "— Видел? — говорит он. — Токарь решил проблему, с которой мы бились месяц. "
        "Просто потому что у него был другой опыт.\n\n"
        "Киваю.\n\n"
        "— Это и есть главный принцип: никто не знает всего. Даже самый опытный мастер.\n\n"
        "Он делает глоток кофе:\n\n"
        "— Смотри. Ты инженер. Ты знаешь теорию, формулы, чертежи. Но токарь знает, "
        "как ведёт себя металл на станке. Сварщик знает, как держать электрод. "
        "Водитель знает, где на трассе опасные участки. Понимаешь?\n\n"
        "— Понимаю. Каждый — эксперт в своей области.\n\n"
        "— Точно. И если ты умеешь спрашивать — ты получаешь доступ к опыту сотен людей. "
        "Вместо того чтобы набивать шишки самому.\n\n"
        "Он показывает на телефон, который дал Токарь:\n\n"
        "— Вот этот телефон может быть ценнее любого учебника. Потому что на другом конце — "
        "человек, который уже прошёл то, что тебе только предстоит.\n\n"
        "— Но как спрашивать правильно? Чтобы не выглядеть тупым?\n\n"
        "Наставник смеётся:\n\n"
        "— Тупым выглядит тот, кто делает вид, что всё знает. Умный человек задаёт вопросы. "
        "Причём конкретные. Не «как это работает», а «почему ты сделал именно так, а не иначе».");

    entry.date = "Полгода назад, 12:00";
    entry.location = "В пути";
    entry.mood = "Обмен знаниями";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить урок]";
    choice1.nextEntryIds = {"day0_branch_c_4"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-3): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchC_4() {
    // ЗАПИСЬ 4В: КОЛЛЕКТИВНЫЙ РАЗУМ - Практика командной работы
    NotebookEntry entry("day0_branch_c_4", EntryType::PAST,
        "14:00, Остановка у мастерской\n\n"
        "Останавливаемся у придорожной мастерской. Наставник хочет купить запчасть.\n\n"
        "Внутри работают трое: мастер, ученик и пожилой механик.\n\n"
        "— Есть прокладка на карбюратор? — спрашивает Наставник.\n\n"
        "— Была. Но кончилась. Можем заказать, дня через три придёт.\n\n"
        "Наставник качает головой:\n\n"
        "— Долго. Может, можно сделать самим?\n\n"
        "Мастер задумывается. Ученик предлагает:\n\n"
        "— Можно вырезать из паронита. У нас есть листовой.\n\n"
        "Пожилой механик качает головой:\n\n"
        "— Паронит слишком жёсткий. Лучше резину. Вон, у меня от велосипедной камеры есть.\n\n"
        "Мастер смотрит на них обоих:\n\n"
        "— А давайте скомбинируем? Паронит как основа, а сверху тонкий слой резины для уплотнения.\n\n"
        "Они берутся за работу. Вырезают прокладку вместе, проверяют, подгоняют. "
        "Через полчаса готово.\n\n"
        "— Вот, — говорит Мастер. — Даже лучше заводской получилась.\n\n"
        "Наставник платит, и мы выходим. Он говорит:\n\n"
        "— Видел? Один предложил паронит, второй — резину, третий — объединил идеи. "
        "Это и есть командная работа. Не один умник, а коллективный разум.\n\n"
        "— Но если бы они не умели слушать друг друга?\n\n"
        "— Тогда бы спорили. Или мастер приказал бы делать по-своему, не слушая молодых. "
        "Но хороший мастер всегда слушает. Даже учеников.");

    entry.date = "Полгода назад, 14:00";
    entry.location = "Придорожная мастерская";
    entry.mood = "Коллективное решение";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Ехать дальше]";
    choice1.nextEntryIds = {"day0_branch_c_5"};
    choice1.action = [](PlayerState* player) {
        player->modifyMoney(-10.0f);  // Заплатили за прокладку
    };
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-4): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchC_5() {
    // ЗАПИСЬ 5В: ЗАВЕРШЕНИЕ ВЕТКИ В - Ценность команды
    NotebookEntry entry("day0_branch_c_5", EntryType::PAST,
        "16:00, Финальная остановка\n\n"
        "Подъезжаем к городу. Наставник останавливается на последней площадке.\n\n"
        "— Ну, — говорит он. — Усвоил?\n\n"
        "Думаю над тем, что видел сегодня.\n\n"
        "— Усвоил. Один человек ограничен своим опытом. Но если умеешь работать с людьми, "
        "спрашивать, слушать — получаешь доступ к опыту многих.\n\n"
        "Наставник кивает:\n\n"
        "— Правильно. Это не слабость — просить помощи. Это сила. Умные люди знают, "
        "когда спросить у того, кто знает лучше.\n\n"
        "Он достаёт ту фотографию, что дал Токарь:\n\n"
        "— Видишь этих людей? Бригада. Они работали вместе. Каждый знал своё дело, "
        "но умели помогать друг другу. Один слабее в чём-то — другой подхватит.\n\n"
        "Я смотрю на фото. Улыбающиеся лица, руки в масле, станок позади.\n\n"
        "— Это не просто работа, — продолжает он. — Это братство. Когда ты знаешь, "
        "что можешь рассчитывать на других. И они — на тебя.\n\n"
        "Он заводит двигатель:\n\n"
        "— Запомни: инженер без команды — это теоретик. А инженер с командой — это сила. "
        "Учись работать с людьми. Это важнее любой технической компетенции.\n\n"
        "Я убираю фотографию в карман. Этот снимок теперь — напоминание. "
        "О том, что мы сильнее вместе.");

    entry.date = "Полгода назад, 16:00";
    entry.location = "Въезд в город";
    entry.mood = "Сила команды";
    entry.printSpeed = 45.0f;

    NotebookChoice choiceFinish;
    choiceFinish.text = "[Завершить день]";
    choiceFinish.nextEntryIds = {"day0_provocation"};
    entry.addChoice(choiceFinish);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-5): " << entry.id << std::endl;
}

// Сохраняем старое имя для обратной совместимости
void NotebookScene::loadDay0_BranchC_Package() {
    loadDay0_BranchC_Teahouse();
}

// === ВЕТКА Г: РЕСУРСОЭФФЕКТИВНОСТЬ (Быстро и по делу) ===

void NotebookScene::loadDay0_BranchD_Route() {
    // ЗАПИСЬ 3Г: РАСЧЁТ МАРШРУТА
    NotebookEntry entry("day0_branch_d_route", EntryType::PAST,
        "ЗАДАНИЕ: 8:30\n\n"
        "Я сразу начал проверять: свечи, топливный фильтр, карбюратор. Самые частые причины.\n\n"
        "Карбюратор оказался забит. Я показал Наставнику.\n\n"
        "«Быстро, — кивнул он. — Но есть проблема. Ты проверил три вещи наугад. "
        "А если бы причина была в четвёртой? Ты потратил бы время впустую.»\n\n"
        "Он почистил карбюратор, продолжая:\n\n"
        "«Принцип Парето: 80% проблем вызваны 20% причин. Ты действовал правильно — "
        "начал с самого вероятного. Но запомни: ресурсоэффективность — это не спешка. "
        "Это умение расставлять приоритеты.»\n\n"
        "---\n\n"
        "«Бензина полбака. Доехать до озера. Проложи маршрут и скажи, как вести, чтобы растянуть.»\n\n"
        "Я предложил прямой путь и постоянную скорость.\n\n"
        "«Прямолинейно. Но ветер боковой, и в конце подъём. Иногда длиннее — выгоднее. "
        "Считай не километры, а литры на конечную точку.»");

    entry.date = "Полгода назад, 8:30";
    entry.location = "Трасса";
    entry.mood = "Расчётливость";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить]";
    choice1.nextEntryIds = {"day0_branch_d_boys"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchD_Boys() {
    // ЗАПИСЬ 4Г: МАЛЬЧИШКИ И МЕТАЛЛ (Выбор артефакта)
    NotebookEntry entry("day0_branch_d_boys", EntryType::PAST,
        "ВСТРЕЧА: 10:00, ОПУШКА\n\n"
        "Старый «машину» разбирали на запчасти пацаны.\n\n"
        "«Дядь, не мешай! Машина мёртвая!»\n\n"
        "«Можно прогнать, — сказал он. — А можно выторговать что-то полезное.»\n\n"
        "Я поторговался. Купил у них исправный стартер и свечи дешевле, "
        "чем они бы сдали в металлолом.\n\n"
        "«На, ещё вот. — Старший мальчишка сунул мне игрушечную машинку, такую же. — За бесплатно.»");

    entry.date = "Полгода назад, 10:00";
    entry.location = "Опушка";
    entry.mood = "Торговля";
    entry.printSpeed = 45.0f;

    // Выбор 1: Взять стартер и свечи (полезное)
    NotebookChoice choiceStarter;
    choiceStarter.text = "Взять стартер и свечи (полезное)";
    choiceStarter.nextEntryIds = {"day0_branch_d_3"};
    choiceStarter.action = [](PlayerState* player) {
        player->addStoryItem("spare_starter");  // Артефакт: Исправный стартер
        player->addPrinciple("resource_efficient");  // ПРИНЦИП: Ресурсоэффективность
        std::cout << "[Day0-D] Получен артефакт: Стартер + Принцип: Ресурсоэффективность" << std::endl;
    };
    entry.addChoice(choiceStarter);

    // Выбор 2: Взять игрушку (сентименты)
    NotebookChoice choiceToy;
    choiceToy.text = "Взять игрушку (бесполезно, но...)";
    choiceToy.nextEntryIds = {"day0_branch_d_3"};
    choiceToy.action = [](PlayerState* player) {
        player->addMoney(25.0f);  // Меньше выгоды
        player->addPrinciple("resource_efficient");  // Принцип все равно получаем
        std::cout << "[Day0-D] Получена игрушка + Принцип: Ресурсоэффективность" << std::endl;
    };
    entry.addChoice(choiceToy);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-2): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchD_3() {
    // ЗАПИСЬ 3Г: ОПТИМИЗАЦИЯ РАСХОДОВ - Принцип минимализма
    NotebookEntry entry("day0_branch_d_3", EntryType::PAST,
        "ВЕТКА Г: РЕСУРСОЭФФЕКТИВНОСТЬ\n"
        "12:30, Заправка\n\n"
        "Останавливаемся заправиться. Наставник проверяет уровень топлива.\n\n"
        "— Сколько заливать? — спрашиваю.\n\n"
        "— Посчитай сам. До дома сто километров. Расход — восемь литров на сотню. "
        "Сколько нужно?\n\n"
        "— Восемь литров.\n\n"
        "— А если встанем в пробке? Или случится что-то?\n\n"
        "Думаю.\n\n"
        "— Десять литров. С запасом.\n\n"
        "— Правильно. Запас нужен. Но не перебарщивай. Видишь тех, кто заправляет под горлышко?\n\n"
        "Киваю. Несколько водителей заливают полные баки.\n\n"
        "— Им кажется, что чем больше бензина — тем лучше. Но больше бензина — больше вес. "
        "Больше вес — больше расход. Видишь логику?\n\n"
        "— Вижу. Переплачиваешь за вес, который сам возишь.\n\n"
        "— Точно. Ресурсоэффективность — это баланс. Не экономить в ущерб себе, "
        "но и не переплачивать за избыток.\n\n"
        "Он заливает ровно десять литров, платит.\n\n"
        "— Запомни: в технике, как и в жизни, минимализм — это сила. "
        "Чем меньше лишнего — тем проще управлять, чинить, двигаться.");

    entry.date = "Полгода назад, 12:30";
    entry.location = "Заправка";
    entry.mood = "Рациональность";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путь]";
    choice1.nextEntryIds = {"day0_branch_d_4"};
    choice1.action = [](PlayerState* player) {
        player->addFuel(20.0f);  // Заправились
        player->modifyMoney(-15.0f);  // Заплатили оптимально
    };
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-3): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchD_4() {
    // ЗАПИСЬ 4Г: ВРЕМЯ — ТОЖЕ РЕСУРС
    NotebookEntry entry("day0_branch_d_4", EntryType::PAST,
        "14:00, В пути\n\n"
        "Едем по трассе. Наставник смотрит на часы.\n\n"
        "— Скоро пробка. Объездную знаешь?\n\n"
        "— Нет. А откуда?\n\n"
        "— Карта. Навигатор. Местные. Нужно знать варианты, а не ехать вслепую.\n\n"
        "Он сворачивает на грунтовку. Дорога хуже, но пустая.\n\n"
        "— Видишь? Здесь на десять минут дольше. Но на трассе стояли бы час. "
        "Время — это тоже ресурс. Иногда важнее топлива.\n\n"
        "Я смотрю на часы. Действительно, мы выигрываем.\n\n"
        "— Принцип простой, — говорит он. — Всегда считай, что важнее: скорость, деньги, время, силы. "
        "И выбирай оптимальное.\n\n"
        "Он показывает на приборную панель:\n\n"
        "— Вот видишь стрелку температуры? Если она ползёт вверх — можно остановиться сейчас "
        "на пять минут, остудить. Или ехать дальше и рисковать перегревом. Что ты выберешь?\n\n"
        "— Остановлюсь. Пять минут — это ничто по сравнению с ремонтом.\n\n"
        "— Молодец. Вот это и есть ресурсоэффективность. Не спешка. Не скупость. А умение считать.\n\n"
        "Он останавливается, глушит двигатель на пять минут. Мы сидим в тишине.\n\n"
        "— Время стоит денег. Деньги стоят времени. Топливо стоит и того, и другого. "
        "Всегда думай, что ты тратишь. И окупается ли это.");

    entry.date = "Полгода назад, 14:00";
    entry.location = "Грунтовка";
    entry.mood = "Расчёт времени";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Ехать дальше после охлаждения]";
    choice1.nextEntryIds = {"day0_branch_d_5"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-4): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchD_5() {
    // ЗАПИСЬ 5Г: ЗАВЕРШЕНИЕ ВЕТКИ Г - Философия эффективности
    NotebookEntry entry("day0_branch_d_5", EntryType::PAST,
        "16:30, Приезд домой\n\n"
        "Подъезжаем к дому. Наставник выключает двигатель, оборачивается.\n\n"
        "— Ну что, понял?\n\n"
        "Киваю.\n\n"
        "— Понял. Ресурсоэффективность — это не жадность. Это умение считать. "
        "Топливо, деньги, время, силы. Всё это ресурсы. И нужно их расходовать с умом.\n\n"
        "— Правильно. Но главное — это не просто экономия. Это осознанность.\n\n"
        "Он достаёт блокнот, показывает записи:\n\n"
        "— Смотри. Я веду учёт. Сколько потратил на топливо, на запчасти, на ремонт. "
        "Это не скупость. Это контроль. Когда знаешь, сколько тратишь — можешь планировать.\n\n"
        "Я беру блокнот. Там аккуратные цифры, даты, суммы.\n\n"
        "— Ресурсоэффективность — это философия. Не трать больше, чем нужно. "
        "Но и не экономь в ущерб себе. Баланс.\n\n"
        "Он хлопает по рулю:\n\n"
        "— Эта машина служит мне тридцать лет. Знаешь почему? Не потому, что я на ней экономлю. "
        "А потому что я вкладываю ровно столько, сколько нужно. Не больше, не меньше.\n\n"
        "Я смотрю на машину. Старая, но ухоженная. Каждая деталь на месте.\n\n"
        "— Запомни, — говорит он. — Ресурсоэффективность — это уважение к тому, что у тебя есть. "
        "Когда ты ценишь время, деньги, силы — ты тратишь их с умом. И это делает тебя сильнее.\n\n"
        "Я выхожу из машины. Теперь я понимаю: эффективность — это не скупость. "
        "Это умение жить осознанно.");

    entry.date = "Полгода назад, 16:30";
    entry.location = "Дом Наставника";
    entry.mood = "Осознанность";
    entry.printSpeed = 45.0f;

    NotebookChoice choiceFinish;
    choiceFinish.text = "[Завершить день]";
    choiceFinish.nextEntryIds = {"day0_provocation"};
    entry.addChoice(choiceFinish);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-5): " << entry.id << std::endl;
}

// Сохраняем старое имя для обратной совместимости
void NotebookScene::loadDay0_BranchD_Choice() {
    loadDay0_BranchD_Boys();
}

// === ОБЩАЯ ЗАПИСЬ: ПРОВОКАЦИЯ (для всех веток) ===

void NotebookScene::loadDay0_Provocation() {
    // ЗАПИСЬ 5: ПРОВОКАЦИЯ
    NotebookEntry entry("day0_provocation", EntryType::PAST,
        "ВОСПОМИНАНИЕ: ПОЛГОДА НАЗАД, 8:15\n\n"
        "— Что, обдумываешь моё предложение? — спросил он, не глядя. — "
        "Не тормози. Я не вечный. Нужен человек, который головой работает, а не ждёт указаний.");

    entry.date = "Полгода назад, 8:15";
    entry.location = "Волга";
    entry.mood = "Напряжённый";
    entry.printSpeed = 45.0f;

    // Выбор 1: Огрызнуться
    NotebookChoice choice1;
    choice1.text = "«Сам ты вечно ворчишь.»";
    choice1.nextEntryIds = {"day0_garage_finale"};
    choice1.action = [](PlayerState* player) {
        player->addTrait("impulsive");  // Черта: импульсивный
        std::cout << "[Day0] Игрок огрызнулся (черта: импульсивный)" << std::endl;
    };
    entry.addChoice(choice1);

    // Выбор 2: Подумать
    NotebookChoice choice2;
    choice2.text = "«Я думаю.»";
    choice2.nextEntryIds = {"day0_garage_finale"};
    choice2.action = [](PlayerState* player) {
        player->addTrait("patient");  // Черта: терпеливый
        std::cout << "[Day0] Игрок подумал (черта: терпеливый)" << std::endl;
    };
    entry.addChoice(choice2);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Provocation): " << entry.id << std::endl;
}

// === ОБЩИЙ ФИНАЛ: ГАРАЖ (для всех веток) ===

void NotebookScene::loadDay0_GarageFinale() {
    // ЗАПИСЬ 6: ПЕРЕДАЧА ИНСТРУМЕНТА (в гараже)
    NotebookEntry entry("day0_garage_finale", EntryType::PAST,
        "ВОСПОМИНАНИЕ: ПОЛГОДА НАЗАД, ВЕЧЕР, ГАРАЖ\n\n"
        "Он подвёл меня к «машинуу».\n\n"
        "— Вот. Твой. Я всё, что мог, поправил. Остальное — на тебе. — "
        "Сунул ключи и потрёпанный конверт. — Документы. Контракт с заводом там, за границей. "
        "Адрес общежития для рабочих. Больше у меня для тебя ничего нет.\n\n"
        "Положил руку на крышу, смотрел куда-то мимо.\n\n"
        "— Механизм ломается, когда используется не по делу. — Он отвёл взгляд. — "
        "Своё дело найдёшь — выживешь. Не найдёшь… Ну, хоть попробуешь. "
        "Пиши, как устроился. Если будет что писать.\n\n"
        "Развернулся и пошёл прочь, не прощаясь.");

    entry.date = "Полгода назад, вечер";
    entry.location = "Гараж";
    entry.mood = "Прощание";
    entry.printSpeed = 35.0f;  // Медленнее для драматизма

    NotebookChoice choice1;
    choice1.text = "[Продолжить в настоящее время]";
    choice1.nextEntryIds = {"day0_transition_present"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Garage Finale): " << entry.id << std::endl;
}

// === ПЕРЕХОД В НАСТОЯЩЕЕ ===

void NotebookScene::loadDay0_TransitionPresent() {
    // ЗАПИСЬ 7: НАСТОЯЩЕЕ ВРЕМЯ. ДОРОГА
    NotebookEntry entry("day0_transition_present", EntryType::PRESENT,
        "Сегодня. Дорога.\n\n"
        "И вот сейчас, на этом длинном пути, "
        "его голос в голове чёток, как никогда. Не наставник из кино. "
        "Просто уставший мужик, который передал мне то, что считал нужным.\n\n"
        "В бардачке — артефакт с того дня. Конверт. Ключи от машины, "
        "которая теперь и дом, и средство передвижения, и единственный актив.\n\n"
        "Он остался там. Я еду дальше. Не за счастьем. За шансом. За работой. "
        "За возможностью не сломаться.\n\n"
        "Впереди — неизвестность. Новый завод в Городе N. "
        "Общежитие для таких же, как я.\n\n"
        "А между прошлым и будущим — только этот гул мотора, дорога и его последний вопрос, "
        "который теперь звучит во мне самом:\n\n"
        "«Ну что, выживешь?»");

    entry.date = "Сегодня";
    entry.location = "В пути";
    entry.mood = "Решимость";
    entry.printSpeed = 40.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить путешествие]";
    choice1.nextEntryIds = {"day1_border_crossing"};  // Переход к Дню 1
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Transition to Present): " << entry.id << std::endl;
}

// === ДЕНЬ 1: ПЕРЕХОД ГРАНИЦЫ ===

void NotebookScene::loadDay1_BorderCrossing() {
    NotebookEntry entry("day1_border_crossing", EntryType::PRESENT,
        "ДЕНЬ 1. ГРАНИЦА\n"
        "Сегодня, утро\n\n"

        "Граница. Очередь из машин тянется на километр.\n\n"

        "Я стою двадцатым. Впереди — грузовики, легковушки, автобусы. "
        "Все едут куда-то. У всех своя история.\n\n"

        "Таможенник проверяет документы долго. Каждую машину.\n\n"

        "Я смотрю на конверт с документами. Контракт с заводом. Виза. Приглашение.\n"
        "Всё есть. Всё должно быть в порядке.\n\n"

        "Но руки всё равно дрожат.\n\n"

        "Это граница. После неё — другая жизнь. Не лучше, не хуже. Просто другая.\n\n"

        "Очередь движется медленно. У меня есть время подумать.\n\n"

        "Что я везу с собой? Машину. Документы. Воспоминания.\n"
        "И уроки того дня, полгода назад, когда Наставник учил меня смотреть на мир по-другому.\n\n"

        "Он сказал: \"Выживешь\".\n\n"

        "Надо бы ответить: \"Да\".\n\n");

    entry.date = "Сегодня, утро";
    entry.location = "Граница";
    entry.mood = "Тревожное ожидание";
    entry.printSpeed = 40.0f;

    NotebookChoice choice1;
    choice1.text = "[Подъехать к таможне]";
    choice1.nextEntryIds = {"day1_customs_check"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 1 entry (Border): " << entry.id << std::endl;
}

void NotebookScene::loadDay1_CustomsCheck() {
    NotebookEntry entry("day1_customs_check", EntryType::PRESENT,
        "ТАМОЖНЯ\n\n"

        "— Документы.\n\n"

        "Протягиваю паспорт, контракт, визу.\n\n"

        "Таможенник листает. Долго. Слишком долго.\n\n"

        "— Едете на завод?\n\n"

        "— Да.\n\n"

        "— Один?\n\n"

        "— Да.\n\n"

        "— Что везёте?\n\n"

        "— Личные вещи. Одежда. Инструменты.\n\n"

        "Он кивает. Ставит штамп. Возвращает документы.\n\n"

        "— Удачи.\n\n"

        "Я еду дальше. Шлагбаум поднимается.\n\n"

        "Всё. Граница пройдена.\n\n"

        "Впереди — дорога. Новая страна. Новая жизнь.\n\n"

        "Наставник говорил: \"Контекст всегда подскажет. Слушай материал. Спрашивай у опытных. Считай ресурсы\".\n\n"

        "Сейчас это звучит как мантра.\n\n"

        "Еду дальше. История продолжается.\n\n"

        "[КОНЕЦ ПРОЛОГА]\n\n"

        "Спасибо за игру! Это был День 0 — вступление к большому путешествию.\n"
        "Продолжение следует...\n\n");

    entry.date = "Сегодня, полдень";
    entry.location = "После границы";
    entry.mood = "Облегчение и новое начало";
    entry.printSpeed = 40.0f;

    NotebookChoice choice1;
    choice1.text = "[Закончить пролог]";
    choice1.nextEntryIds = {"day0_knock"};  // Возврат к началу для повторного прохождения
    choice1.action = [](PlayerState* player) {
        std::cout << "\n=== ПРОЛОГ ЗАВЕРШЁН ===" << std::endl;
        std::cout << "Вы прошли День 0 и узнали один из уроков наставника." << std::endl;
        std::cout << "Можете начать заново и выбрать другую ветку обучения!" << std::endl;

        // Сброс для нового прохождения (опционально)
        // player->resetToStart();
    };
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 1 entry (Customs): " << entry.id << std::endl;
}

// === ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ДЛЯ ШРИФТОВ И ЦВЕТОВ ===

const sf::Font& NotebookScene::getCurrentFont() const {
    switch (m_currentEntry.type) {
        case EntryType::PRESENT:
            return m_fontPresent;
        case EntryType::PAST:
            return m_fontPast;
        case EntryType::THOUGHT:
            return m_fontThought;
        default:
            return m_fontPresent;
    }
}

sf::Color NotebookScene::getCurrentTextColor() const {
    switch (m_currentEntry.type) {
        case EntryType::PRESENT:
            // Приглушенный серо-зеленый для настоящего времени
            return sf::Color(140, 160, 140);  // Комфортный серо-зеленый
        case EntryType::PAST:
            // Приглушенный серо-желтый для воспоминаний
            return sf::Color(180, 170, 130);  // Теплый приглушенный желтый
        case EntryType::THOUGHT:
            // Приглушенный серо-голубой для внутренних мыслей
            return sf::Color(150, 150, 160);  // Мягкий серо-голубой
        default:
            return sf::Color(200, 200, 200);  // Приглушенный серый по умолчанию
    }
}

unsigned int NotebookScene::getCurrentFontSize() const {
    switch (m_currentEntry.type) {
        case EntryType::PRESENT:
            return 24;  // Обычный размер для настоящего
        case EntryType::PAST:
            return 22;  // Чуть меньше для воспоминаний
        case EntryType::THOUGHT:
            return 20;  // Еще меньше для мыслей (более интимное)
        default:
            return 24;
    }
}

std::string NotebookScene::wrapText(const std::string& text, const sf::Font& font, unsigned int fontSize, float maxWidth) const {
    std::string result;
    std::string currentLine;
    std::istringstream stream(text);
    std::string word;

    sf::Text testText(font);
    testText.setCharacterSize(fontSize);

    while (stream >> word) {
        // Проверить, есть ли в слове перевод строки
        size_t newlinePos = word.find('\n');
        if (newlinePos != std::string::npos) {
            // Обработать часть до переноса строки
            std::string beforeNewline = word.substr(0, newlinePos);
            std::string afterNewline = word.substr(newlinePos + 1);

            if (!beforeNewline.empty()) {
                std::string testLine = currentLine.empty() ? beforeNewline : currentLine + " " + beforeNewline;
                testText.setString(utf8(testLine));

                if (testText.getLocalBounds().size.x <= maxWidth) {
                    if (!currentLine.empty()) currentLine += " ";
                    currentLine += beforeNewline;
                } else {
                    if (!currentLine.empty()) result += currentLine + "\n";
                    currentLine = beforeNewline;
                }
            }

            // Добавить текущую строку и начать новую
            result += currentLine + "\n";
            currentLine.clear();

            // Обработать часть после переноса
            if (!afterNewline.empty()) {
                currentLine = afterNewline;
            }
            continue;
        }

        // Проверить, поместится ли слово в текущую строку
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        testText.setString(utf8(testLine));

        if (testText.getLocalBounds().size.x <= maxWidth) {
            // Слово помещается
            if (!currentLine.empty()) {
                currentLine += " ";
            }
            currentLine += word;
        } else {
            // Слово не помещается, начать новую строку
            if (!currentLine.empty()) {
                result += currentLine + "\n";
            }
            currentLine = word;
        }
    }

    // Добавить последнюю строку
    if (!currentLine.empty()) {
        result += currentLine;
    }

    return result;
}

// === ТЕСТОВЫЕ ЗАПИСИ ===

void NotebookScene::loadTestThoughtSystem() {
    NotebookEntry entry;
    entry.id = "test_thought_system";
    entry.type = EntryType::PRESENT;
    entry.text = "🧪 ТЕСТ: Система размышлений\n\n"
                 "Эта запись создана для тестирования ThoughtSystem.\n\n"
                 "Просто НЕ ВЫБИРАЙТЕ ничего и подождите:\n"
                 "• Через 5 секунд появится первая мысль\n"
                 "• Через 10 секунд - вторая мысль\n"
                 "• Через 15 секунд - третья мысль\n\n"
                 "Мысли будут появляться в центре экрана с fade эффектом.\n"
                 "Каждая мысль живет 8 секунд.";
    entry.printSpeed = 100.0f;  // Быстрая печать для теста
    entry.canSkip = true;

    // Добавляем два выбора
    NotebookChoice choice1;
    choice1.text = "Вернуться в главное меню";
    choice1.action = [](PlayerState*) {
        std::cout << "[TEST] Возврат в главное меню" << std::endl;
    };
    choice1.nextEntryIds = {};  // Пустой = возврат в меню
    entry.choices.push_back(choice1);

    NotebookChoice choice2;
    choice2.text = "Повторить тест";
    choice2.action = [](PlayerState*) {
        std::cout << "[TEST] Повтор теста" << std::endl;
    };
    choice2.nextEntryIds = {"test_thought_system"};
    entry.choices.push_back(choice2);

    m_entries[entry.id] = entry;
}

// === ДЕМО-КОНТЕНТ (простой пример для быстрого тестирования) ===
// Эти записи созданы как пример для демонстрации работы системы.
// Пользователь заменит их на свой контент.

void NotebookScene::loadDemo_Start() {
    NotebookEntry entry;
    entry.id = "demo_start";
    entry.type = EntryType::PAST;
    entry.text =
        "1994 год. Осень.\n\n"
        "Я стою у машины. \"машину-2141\".\n"
        "Последний подарок Наставника.\n\n"
        "В кармане 500 рублей.\n"
        "В багажнике — всё, что осталось от прошлой жизни.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    // Автоматический переход к следующей записи
    NotebookChoice auto_continue;
    auto_continue.text = "[Продолжить]";
    auto_continue.nextEntryIds = {"demo_road"};
    auto_continue.action = [](PlayerState*) {};
    entry.addChoice(auto_continue);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_Road() {
    NotebookEntry entry;
    entry.id = "demo_road";
    entry.type = EntryType::PAST;
    entry.text =
        "Дорога. Пустая, серая.\n\n"
        "Я еду в Город N.\n"
        "Там работа. Там новая жизнь.\n\n"
        "Но сначала — долгий путь.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    // Автоматический переход
    NotebookChoice auto_continue;
    auto_continue.text = "[Продолжить]";
    auto_continue.nextEntryIds = {"demo_choice"};
    auto_continue.action = [](PlayerState*) {};
    entry.addChoice(auto_continue);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_Choice() {
    NotebookEntry entry;
    entry.id = "demo_choice";
    entry.type = EntryType::PRESENT;
    entry.text =
        "Дорога раздваивается.\n\n"
        "Слева — федеральная трасса. Быстро, но опасно.\n"
        "Справа — проселок. Медленно, но тихо.\n\n"
        "Бензина хватит на один путь.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    // Выбор 1: Трасса
    NotebookChoice choice1;
    choice1.text = "Трасса (быстро, -15L топлива, -5% энергии)";
    choice1.nextEntryIds = {"demo_highway"};
    choice1.action = [](PlayerState* player) {
        player->addFuel(-15.0f);
        player->modifyEnergy(-5.0f);
        std::cout << "[DEMO] Выбрана трасса: -15L топлива, -5% энергии" << std::endl;
    };
    entry.addChoice(choice1);

    // Выбор 2: Проселок
    NotebookChoice choice2;
    choice2.text = "Проселок (тихо, -10L топлива, -15% энергии)";
    choice2.nextEntryIds = {"demo_backroad"};
    choice2.action = [](PlayerState* player) {
        player->addFuel(-10.0f);
        player->modifyEnergy(-15.0f);
        std::cout << "[DEMO] Выбран проселок: -10L топлива, -15% энергии" << std::endl;
    };
    entry.addChoice(choice2);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_Highway() {
    NotebookEntry entry;
    entry.id = "demo_highway";
    entry.type = EntryType::PAST;
    entry.text =
        "Трасса. Асфальт гудит под колёсами.\n\n"
        "Я давлю на газ. 100. 110. 120.\n"
        "Быстрее. Скорее. Время — деньги.\n\n"
        "Но впереди мигают синие огни...\n\n"
        "[Путь: Трасса — быстро, но опасно]";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    // Переход к финалу
    NotebookChoice auto_continue;
    auto_continue.text = "[Продолжить]";
    auto_continue.nextEntryIds = {"demo_finale"};
    auto_continue.action = [](PlayerState*) {};
    entry.addChoice(auto_continue);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_Backroad() {
    NotebookEntry entry;
    entry.id = "demo_backroad";
    entry.type = EntryType::PAST;
    entry.text =
        "Проселок. Пыль за окном.\n\n"
        "Медленно. Тихо. Никого вокруг.\n"
        "Только поля, леса, деревни.\n\n"
        "Безопасно. Но далеко ли так доеду?\n\n"
        "[Путь: Проселок — тихо, но медленно]";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    // Переход к финалу
    NotebookChoice auto_continue;
    auto_continue.text = "[Продолжить]";
    auto_continue.nextEntryIds = {"demo_finale"};
    auto_continue.action = [](PlayerState*) {};
    entry.addChoice(auto_continue);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_Finale() {
    NotebookEntry entry;
    entry.id = "demo_finale";
    entry.type = EntryType::THOUGHT;
    entry.text =
        "Я думаю о выборе.\n\n"
        "Правильный ли?\n"
        "Успею ли?\n"
        "Доеду ли?\n\n"
        "Наставник говорил:\n"
        "\"Дорога — это выборы. Делай их. Не жалей.\"\n\n"
        "Что ж. Еду дальше.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice continue_demo;
    continue_demo.text = "[Продолжить демо]";
    continue_demo.nextEntryIds = {"demo_rest_stop"};
    entry.addChoice(continue_demo);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_RestStop() {
    NotebookEntry entry;
    entry.id = "demo_rest_stop";
    entry.type = EntryType::PRESENT;
    entry.text =
        "ПРИДОРОЖНОЕ КАФЕ\n"
        "Сейчас\n\n"
        "Останавливаюсь возле старого придорожного кафе.\n"
        "Нужно отдохнуть, подзаправиться.\n\n"
        "В кармане осталось немного денег.\n"
        "Энергия на исходе. Машина требует внимания.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice eat;
    eat.text = "Поесть и отдохнуть (-150₽, +30% энергии)";
    eat.nextEntryIds = {"demo_mechanic_encounter"};
    eat.action = [](PlayerState* ps) {
        ps->addMoney(-150);
        ps->addEnergy(30.0f);
        std::cout << "[DEMO] Поел и отдохнул" << std::endl;
    };
    entry.addChoice(eat);

    NotebookChoice sleep_car;
    sleep_car.text = "Поспать в машине (бесплатно, +20% энергии)";
    sleep_car.nextEntryIds = {"demo_mechanic_encounter"};
    sleep_car.action = [](PlayerState* ps) {
        ps->addEnergy(20.0f);
        std::cout << "[DEMO] Поспал в машине" << std::endl;
    };
    entry.addChoice(sleep_car);

    NotebookChoice keep_going;
    keep_going.text = "Ехать дальше (экономия времени)";
    keep_going.nextEntryIds = {"demo_mechanic_encounter"};
    entry.addChoice(keep_going);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_MechanicEncounter() {
    NotebookEntry entry;
    entry.id = "demo_mechanic_encounter";
    entry.type = EntryType::PRESENT;
    entry.text =
        "ВСТРЕЧА С МЕХАНИКОМ\n"
        "Сейчас\n\n"
        "На стоянке вижу старого механика. Возится с «Жигулями».\n\n"
        "— Эй, парень! — окликает он меня. — Волга у тебя?\n"
        "Гляну, если хочешь. За бутылку.\n\n"
        "Моя Волга действительно стучит подозрительно.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice accept_help;
    accept_help.text = "Согласиться (-50₽, +25% состояние машины)";
    accept_help.nextEntryIds = {"demo_low_fuel"};
    accept_help.action = [](PlayerState* ps) {
        ps->addMoney(-50);
        ps->modifyVehicleCondition(25.0f);
        std::cout << "[DEMO] Механик помог" << std::endl;
    };
    entry.addChoice(accept_help);

    NotebookChoice refuse;
    refuse.text = "Отказаться (сэкономить деньги)";
    refuse.nextEntryIds = {"demo_low_fuel"};
    entry.addChoice(refuse);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_LowFuel() {
    NotebookEntry entry;
    entry.id = "demo_low_fuel";
    entry.type = EntryType::PRESENT;
    entry.text =
        "ДОРОГА. НОЧЬ.\n"
        "Сейчас\n\n"
        "Еду дальше. Стемнело.\n"
        "Дорога пуста. Фары выхватывают асфальт.\n\n"
        "Стрелка указателя топлива ползёт к нулю.\n"
        "Нужно искать заправку.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice continue_road;
    continue_road.text = "[Продолжить путь]";
    continue_road.nextEntryIds = {"demo_final_choice"};
    continue_road.action = [](PlayerState* ps) {
        ps->addFuel(-25.0f);  // Критический расход - вызовет событие
        ps->addEnergy(-15.0f);
        std::cout << "[DEMO] Трата ресурсов для теста событий" << std::endl;
    };
    entry.addChoice(continue_road);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_FinalChoice() {
    NotebookEntry entry;
    entry.id = "demo_final_choice";
    entry.type = EntryType::PRESENT;
    entry.text =
        "РАЗВИЛКА\n"
        "Сейчас\n\n"
        "Впереди развилка.\n\n"
        "Налево — короткий путь через лес. Рискованно, но быстро.\n"
        "Направо — в объезд через город. Безопасно, но дольше.\n\n"
        "Что выбрать?";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice left;
    left.text = "Налево (лес, риск, -10L топлива)";
    left.nextEntryIds = {"demo_forest_path"};
    left.action = [](PlayerState* ps) {
        ps->addFuel(-10.0f);
        ps->addEnergy(-5.0f);
    };
    entry.addChoice(left);

    NotebookChoice right;
    right.text = "Направо (город, безопасно, -20L топлива)";
    right.nextEntryIds = {"demo_city_path"};
    right.action = [](PlayerState* ps) {
        ps->addFuel(-20.0f);
        ps->addEnergy(-10.0f);
    };
    entry.addChoice(right);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_ForestPath() {
    NotebookEntry entry;
    entry.id = "demo_forest_path";
    entry.type = EntryType::PAST;
    entry.text =
        "ЛЕС\n"
        "Тогда, ночью\n\n"
        "Темнота сгущается. Деревья смыкаются над дорогой.\n"
        "Дорога разбита, Волга трясётся на кочках.\n\n"
        "Но я еду. Быстро. Решительно.\n"
        "Как учил наставник — выбрал путь, иди до конца.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice cont;
    cont.text = "[Продолжить]";
    cont.nextEntryIds = {"demo_end"};
    entry.addChoice(cont);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_CityPath() {
    NotebookEntry entry;
    entry.id = "demo_city_path";
    entry.type = EntryType::PAST;
    entry.text =
        "ГОРОД\n"
        "Тогда, ночью\n\n"
        "Огни города. Редкие прохожие.\n"
        "Еду медленно, следя за знаками.\n\n"
        "Безопасно. Надёжно.\n"
        "Может, не самый быстрый путь, но точно доеду.";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice cont;
    cont.text = "[Продолжить]";
    cont.nextEntryIds = {"demo_end"};
    entry.addChoice(cont);

    m_entries[entry.id] = entry;
}

void NotebookScene::loadDemo_End() {
    NotebookEntry entry;
    entry.id = "demo_end";
    entry.type = EntryType::THOUGHT;
    entry.text =
        "═══════════════════════════════════════\n\n"
        "         КОНЕЦ ДЕМОНСТРАЦИИ\n\n"
        "═══════════════════════════════════════\n\n"
        "Вы прошли демо-версию игры!\n\n"
        "ЧТО ВЫ УВИДЕЛИ:\n"
        "• Систему выборов с последствиями\n"
        "• Управление ресурсами (топливо, энергия, деньги)\n"
        "• События, зависящие от состояния ресурсов\n"
        "• Разные типы записей (PRESENT, PAST, THOUGHT)\n"
        "• Ветвление сюжета\n\n"
        "Теперь вы можете:\n"
        "1. Пройти полную версию Дня 0 (\"День 0: Уроки дороги\")\n"
        "2. Создать свой контент\n"
        "3. Вернуться в меню\n\n"
        "Спасибо за игру!";
    entry.printSpeed = 50.0f;
    entry.canSkip = true;

    NotebookChoice menu;
    menu.text = "Вернуться в главное меню";
    menu.nextEntryIds = {};
    menu.action = [](PlayerState*) {
        std::cout << "[DEMO] Конец демо - полная версия" << std::endl;
    };
    entry.addChoice(menu);

    NotebookChoice day0;
    day0.text = "Играть День 0: Уроки дороги";
    day0.nextEntryIds = {"day0_knock"};
    entry.addChoice(day0);

    m_entries[entry.id] = entry;
}

// ============================================================================
// ИНТЕГРАЦИЯ СОБЫТИЙ
// ============================================================================

bool NotebookScene::tryTriggerRandomEvent() {
    // Проверить есть ли уже ожидающее событие
    if (!m_pendingEventId.empty()) {
        return false;
    }

    // Проверить шанс срабатывания (20% по умолчанию)
    float randomChance = static_cast<float>(rand()) / RAND_MAX;
    if (randomChance > m_eventTriggerChance) {
        return false;  // Событие не сработало
    }

    // Получить случайное событие по текущим условиям
    GameEvent* event = m_eventManager.getRandomEvent(
        m_playerState->getFuel(),
        m_playerState->getEnergy(),
        static_cast<int>(m_playerState->getMoney())
    );

    if (event == nullptr) {
        return false;  // Нет подходящих событий
    }

    // Сохранить ID события для показа
    m_pendingEventId = event->id;

    std::cout << "[NotebookScene] Random event triggered: " << event->title
              << " (id: " << event->id << ")" << std::endl;

    return true;
}

NotebookEntry NotebookScene::convertEventToEntry(const GameEvent& event) {
    // Создать запись из события
    NotebookEntry entry(
        "event_" + event.id,  // Префикс для событий
        EntryType::PRESENT,   // События в настоящем времени
        event.description
    );

    // Конвертировать каждый выбор события в выбор блокнота
    int choiceIndex = 0;
    for (const auto& eventChoice : event.choices) {
        NotebookChoice notebookChoice;
        notebookChoice.text = eventChoice.text;

        // Создать уникальный ID для страницы результата
        std::string outcomeId = "event_outcome_" + event.id + "_choice" + std::to_string(choiceIndex);

        // Выбор ведет к странице результата
        notebookChoice.nextEntryIds = {outcomeId};

        // Применить эффекты события через action
        notebookChoice.action = [eventChoice](PlayerState* player) {
            // Применить изменения ресурсов
            player->modifyEnergy(eventChoice.energyChange);
            player->modifyMoney(eventChoice.moneyChange);
            player->addFuel(eventChoice.fuelChange);

            float currentCondition = player->getVehicleCondition();
            player->setVehicleCondition(currentCondition + eventChoice.vehicleConditionChange);

            // Применить изменения отношений с NPC (если есть)
            // TODO: интеграция с NPCManager

            // Добавить/убрать предметы (если есть)
            // TODO: интеграция с инвентарем

            std::cout << "[Event] Applied choice effects:" << std::endl;
            std::cout << "  Energy: " << eventChoice.energyChange << std::endl;
            std::cout << "  Money: " << eventChoice.moneyChange << std::endl;
            std::cout << "  Fuel: " << eventChoice.fuelChange << std::endl;
            std::cout << "  Vehicle: " << eventChoice.vehicleConditionChange << std::endl;
        };

        entry.addChoice(notebookChoice);

        // Создать страницу результата для этого выбора
        if (!eventChoice.outcomeText.empty()) {
            NotebookEntry outcomeEntry(outcomeId, EntryType::PRESENT, eventChoice.outcomeText);
            outcomeEntry.printSpeed = 60.0f;
            outcomeEntry.canSkip = true;

            // Один выбор - продолжить к следующей записи
            NotebookChoice continueChoice;
            continueChoice.text = "[Продолжить]";

            // Использовать сохраненный ID следующей записи
            if (!m_pendingNextEntryId.empty()) {
                continueChoice.nextEntryIds = {m_pendingNextEntryId};
            } else {
                continueChoice.nextEntryIds = {m_currentEntryId};
            }

            outcomeEntry.addChoice(continueChoice);

            // Добавить страницу результата в хранилище
            m_entries[outcomeId] = outcomeEntry;

            std::cout << "[Event] Created outcome page: " << outcomeId << " -> " << m_pendingNextEntryId << std::endl;
        }

        choiceIndex++;
    }

    return entry;
}

void NotebookScene::showEventAsEntry(const std::string& eventId) {
    // Получить событие
    GameEvent* event = m_eventManager.getEvent(eventId);
    if (event == nullptr) {
        std::cerr << "[NotebookScene] Event not found: " << eventId << std::endl;
        return;
    }

    // Конвертировать событие в запись
    NotebookEntry entry = convertEventToEntry(*event);

    // Добавить запись во временное хранилище
    m_entries[entry.id] = entry;

    // Показать запись
    showEntry(entry.id);

    // Пометить событие как сработавшее
    m_eventManager.triggerEvent(eventId);

    // Очистить ожидающее событие
    m_pendingEventId = "";

    std::cout << "[NotebookScene] Showing event as entry: " << event->title << std::endl;
}
