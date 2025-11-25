#include "RestScene.h"
#include "AbilityTreeSystem.h"
#include <iostream>
#include <random>

RestScene::RestScene()
    : m_playerState(nullptr), m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU), m_selectedOption(0),
      m_hoveredOption(-1), m_fontLoaded(false) {
    setupUI();
    initializeOptions();
}

RestScene::RestScene(PlayerState* playerState)
    : m_playerState(playerState), m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU), m_selectedOption(0),
      m_hoveredOption(-1), m_fontLoaded(false) {
    setupUI();
    initializeOptions();
}

void RestScene::setPlayerState(PlayerState* playerState) {
    m_playerState = playerState;
}

void RestScene::setupUI() {
    // Загрузка шрифта
    if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else {
        std::cerr << "Failed to load font for RestScene" << std::endl;
    }
    
    // Фон
    m_background.setSize(sf::Vector2f(1280.0f, 720.0f));
    m_background.setFillColor(sf::Color(20, 20, 30));  // Темный фон
    
    // Меню бокс
    m_menuBox.setSize(sf::Vector2f(800.0f, 500.0f));
    m_menuBox.setPosition(240.0f, 110.0f);
    m_menuBox.setFillColor(sf::Color(40, 40, 50, 230));
    m_menuBox.setOutlineThickness(3.0f);
    m_menuBox.setOutlineColor(sf::Color(0, 255, 0));  // Зелёная рамка (Pip-Boy стиль)
    
    // Заголовок
    if (m_fontLoaded) {
        m_titleText.setFont(m_font);
        m_titleText.setString(L"REST");
        m_titleText.setCharacterSize(48);
        m_titleText.setFillColor(sf::Color(0, 255, 0));
        m_titleText.setPosition(400.0f, 130.0f);
        
        // Статус текст
        m_statusText.setFont(m_font);
        m_statusText.setCharacterSize(20);
        m_statusText.setFillColor(sf::Color(200, 200, 200));
        m_statusText.setPosition(280.0f, 200.0f);
    }
}

void RestScene::initializeOptions() {
    m_restOptions.clear();
    
    // Опция 1: Отдых в машине (Car Rest)
    // Восстанавливает энергию, шанс негативных статусов, тратит прочность машины
    RestOption carRest;
    carRest.name = L"Rest in Car";
    carRest.description = L"Restore energy (+30), chance of negative effects, vehicle wear (-5%)";
    carRest.cost = 0;  // Бесплатно
    carRest.energyRestore = 30.0f;
    carRest.timePass = 240.0f;  // 4 часа
    m_restOptions.push_back(carRest);
    
    // Опция 2: Отдых в мотеле (Motel Rest)
    // Полностью восстанавливает всё, даёт положительные баффы, стоит денег
    RestOption motelRest;
    motelRest.name = L"Rest at Motel";
    motelRest.description = L"Full restoration of energy, health, morale. Positive buffs. (-$300)";
    motelRest.cost = 300;  // Стоит 300 рублей
    motelRest.energyRestore = 100.0f;  // Полное восстановление
    motelRest.timePass = 480.0f;  // 8 часов
    m_restOptions.push_back(motelRest);
    
    // Опция 3: Пропустить / Skip
    RestOption skip;
    skip.name = L"Continue without resting";
    skip.description = L"Cancel rest and return to road";
    skip.cost = 0;
    skip.energyRestore = 0.0f;
    skip.timePass = 0.0f;
    m_restOptions.push_back(skip);
    
    // Создать UI элементы для опций
    m_optionBoxes.clear();
    m_optionTexts.clear();
    
    float yPos = 280.0f;
    for (size_t i = 0; i < m_restOptions.size(); ++i) {
        // Бокс опции
        sf::RectangleShape box;
        box.setSize(sf::Vector2f(760.0f, 100.0f));
        box.setPosition(260.0f, yPos);
        box.setFillColor(sf::Color(60, 60, 70, 200));
        box.setOutlineThickness(2.0f);
        box.setOutlineColor(sf::Color(100, 100, 110));
        m_optionBoxes.push_back(box);
        
        // Текст опции
        if (m_fontLoaded) {
            sf::Text text;
            text.setFont(m_font);
            text.setString(m_restOptions[i].name);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color(220, 220, 220));
            text.setPosition(280.0f, yPos + 10.0f);
            m_optionTexts.push_back(text);
        }
        
        yPos += 110.0f;
    }
}

void RestScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
            m_selectedOption = (m_selectedOption > 0) ? m_selectedOption - 1 : m_restOptions.size() - 1;
        }
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            m_selectedOption = (m_selectedOption + 1) % m_restOptions.size();
        }
        else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
            applyRestOption(m_restOptions[m_selectedOption]);
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            m_isFinished = true;
            m_nextScene = SceneType::NODE;
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        updateOptionHover(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (m_hoveredOption >= 0 && m_hoveredOption < static_cast<int>(m_restOptions.size())) {
                applyRestOption(m_restOptions[m_hoveredOption]);
            }
        }
    }
}

void RestScene::updateOptionHover(const sf::Vector2i& mousePos) {
    m_hoveredOption = -1;
    
    for (size_t i = 0; i < m_optionBoxes.size(); ++i) {
        if (m_optionBoxes[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), 
                                                        static_cast<float>(mousePos.y))) {
            m_hoveredOption = static_cast<int>(i);
            m_selectedOption = static_cast<int>(i);
            break;
        }
    }
}

void RestScene::applyRestOption(const RestOption& option) {
    if (!m_playerState) {
        std::cerr << "PlayerState is null in RestScene!" << std::endl;
        m_isFinished = true;
        m_nextScene = SceneType::NODE;
        return;
    }
    
    // Опция 3 - Skip (продолжить без отдыха)
    if (option.timePass == 0.0f && option.energyRestore == 0.0f) {
        m_isFinished = true;
        m_nextScene = SceneType::NODE;
        return;
    }
    
    // Опция 1: Отдых в машине
    if (option.cost == 0 && option.energyRestore < 50.0f) {
        // Восстановить энергию (частично)
        float currentEnergy = m_playerState->getEnergy();
        m_playerState->setEnergy(std::min(100.0f, currentEnergy + option.energyRestore));
        
        // Износ машины (тратит 5% состояния)
        float vehicleCondition = m_playerState->getVehicleCondition();
        m_playerState->setVehicleCondition(std::max(0.0f, vehicleCondition - 5.0f));
        
        // Шанс негативного статуса (30% шанс)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        
        if (dis(gen) <= 30) {
            // Негативный статус: усталость (снижение макс. энергии на 10 на короткое время)
            // TODO: Добавить систему статусов/баффов когда она будет реализована
            std::cout << "Negative effect: Uncomfortable sleep. Max energy -10 for next day." << std::endl;
        }
        
        // Восстановить заряды способностей
        AbilityTreeSystem::getInstance().restoreCharges();
        
        std::cout << "Car rest complete. Energy restored, ability charges restored." << std::endl;
    }
    // Опция 2: Отдых в мотеле
    else if (option.cost > 0) {
        // Проверить наличие денег
        if (m_playerState->getMoney() < option.cost) {
            std::cout << "Not enough money for motel rest!" << std::endl;
            // Можно показать сообщение об ошибке
            return;
        }
        
        // Списать деньги
        m_playerState->modifyMoney(-option.cost);
        
        // Полное восстановление энергии
        m_playerState->setEnergy(100.0f);
        
        // Восстановить здоровье (если есть система здоровья)
        // TODO: Восстановить здоровье когда система будет реализована
        
        // Восстановить мораль (если есть система морали)
        // TODO: Восстановить мораль когда система будет реализована
        
        // Положительный бафф: +10% к эффективности на следующий день
        // TODO: Добавить систему баффов когда она будет реализована
        std::cout << "Positive effect: Excellent sleep. +10% efficiency for next day." << std::endl;
        
        // Восстановить заряды способностей
        AbilityTreeSystem::getInstance().restoreCharges();
        
        std::cout << "Motel rest complete. Everything fully restored, ability charges restored." << std::endl;
    }
    
    // Обновить статус текст
    if (m_fontLoaded) {
        std::wstring status = L"Rest complete.\nEnergy: " + std::to_wstring(static_cast<int>(m_playerState->getEnergy())) + L"/100\n";
        status += L"Money: $" + std::to_wstring(static_cast<int>(m_playerState->getMoney())) + L"\n";
        status += L"Vehicle Condition: " + std::to_wstring(static_cast<int>(m_playerState->getVehicleCondition())) + L"%";
        m_statusText.setString(status);
    }
    
    // Завершить сцену
    m_isFinished = true;
    m_nextScene = SceneType::NODE;
}

void RestScene::update(float deltaTime) {
    // Обновить выделение опции
    for (size_t i = 0; i < m_optionBoxes.size(); ++i) {
        if (static_cast<int>(i) == m_selectedOption) {
            m_optionBoxes[i].setOutlineColor(sf::Color(0, 255, 0));  // Зелёная рамка для выбранной
            m_optionBoxes[i].setOutlineThickness(3.0f);
            if (i < m_optionTexts.size()) {
                m_optionTexts[i].setFillColor(sf::Color(0, 255, 0));  // Зелёный текст
            }
        } else {
            m_optionBoxes[i].setOutlineColor(sf::Color(100, 100, 110));
            m_optionBoxes[i].setOutlineThickness(2.0f);
            if (i < m_optionTexts.size()) {
                m_optionTexts[i].setFillColor(sf::Color(220, 220, 220));
            }
        }
    }
    
    // Обновить статус текст если есть PlayerState
    if (m_playerState && m_fontLoaded) {
        std::wstring status = L"Current Status:\n";
        status += L"Energy: " + std::to_wstring(static_cast<int>(m_playerState->getEnergy())) + L"/100\n";
        status += L"Money: $" + std::to_wstring(static_cast<int>(m_playerState->getMoney())) + L"\n";
        status += L"Vehicle Condition: " + std::to_wstring(static_cast<int>(m_playerState->getVehicleCondition())) + L"%\n\n";
        status += L"Choose rest method:";
        m_statusText.setString(status);
    }
}

void RestScene::render(sf::RenderWindow& window) {
    // Рендер фона
    window.draw(m_background);
    
    // Рендер меню бокса
    window.draw(m_menuBox);
    
    // Рендер заголовка
    if (m_fontLoaded) {
        window.draw(m_titleText);
        window.draw(m_statusText);
    }
    
    // Рендер опций
    for (size_t i = 0; i < m_optionBoxes.size(); ++i) {
        window.draw(m_optionBoxes[i]);
        
        if (i < m_optionTexts.size()) {
            window.draw(m_optionTexts[i]);
            
            // Рендер описания для выбранной опции
            if (static_cast<int>(i) == m_selectedOption && m_fontLoaded) {
                sf::Text descText;
                descText.setFont(m_font);
                descText.setString(m_restOptions[i].description);
                descText.setCharacterSize(16);
                descText.setFillColor(sf::Color(180, 180, 180));
                descText.setPosition(280.0f, m_optionBoxes[i].getPosition().y + 45.0f);
                window.draw(descText);
            }
        }
    }
}

SceneType RestScene::getNextScene() const {
    return m_nextScene;
}

bool RestScene::isFinished() const {
    return m_isFinished;
}
