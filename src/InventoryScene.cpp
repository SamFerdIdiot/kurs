#include "InventoryScene.h"
#include "GameStateManager.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>
#include <sstream>
#include <iomanip>

InventoryScene::InventoryScene()
    : m_selectedIndex(0),
      m_scrollOffset(0),
      m_isFinished(false),
      m_nextScene(SceneType::INVENTORY),
      m_returnToScene(SceneType::NODE),
      m_fontLoaded(false) {

    // SFML 3.x: Font loading with FontLoader helper
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "InventoryScene: WARNING - Could not load any font!" << std::endl;
    }

    // Fixed layout for 1440x900
    const float SCREEN_WIDTH = 1440.0f;
    const float SCREEN_HEIGHT = 900.0f;

    // Semi-transparent background overlay
    m_backgroundBox.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_backgroundBox.setFillColor(sf::Color(30, 30, 46, 230));

    // List container (main panel)
    m_listContainer.setSize(sf::Vector2f(900.0f, 600.0f));
    m_listContainer.setPosition(sf::Vector2f(270.0f, 150.0f));
    m_listContainer.setFillColor(UI::Color::BACKGROUND_DARK);
    m_listContainer.setOutlineColor(UI::Color::ACCENT_GREEN);
    m_listContainer.setOutlineThickness(3.0f);

    // Header bar (for column labels)
    m_headerBar.setSize(sf::Vector2f(900.0f, 50.0f));
    m_headerBar.setPosition(sf::Vector2f(270.0f, 150.0f));
    m_headerBar.setFillColor(sf::Color(50, 50, 60));
    m_headerBar.setOutlineColor(UI::Color::ACCENT_GREEN);
    m_headerBar.setOutlineThickness(2.0f);

    // Selection highlight bar
    m_selectionHighlight.setSize(sf::Vector2f(880.0f, m_itemHeight - 4.0f));
    m_selectionHighlight.setFillColor(UI::Color::SELECTION_HIGHLIGHT);

    // SFML 3.x: Text initialization (requires font in constructor)
    if (m_fontLoaded) {
        // Title
        m_titleText.emplace(m_font, "INVENTORY", UI::FONT_SIZE_HUGE);
        m_titleText->setFillColor(UI::Color::ACCENT_YELLOW);
        m_titleText->setPosition(sf::Vector2f(600.0f, 80.0f));

        // Header labels
        m_headerNameText.emplace(m_font, "NAME", UI::FONT_SIZE_NORMAL);
        m_headerNameText->setFillColor(UI::Color::TEXT_PRIMARY);
        m_headerNameText->setPosition(sf::Vector2f(300.0f, 163.0f));

        m_headerWeightText.emplace(m_font, "WEIGHT", UI::FONT_SIZE_NORMAL);
        m_headerWeightText->setFillColor(UI::Color::TEXT_PRIMARY);
        m_headerWeightText->setPosition(sf::Vector2f(750.0f, 163.0f));

        m_headerValueText.emplace(m_font, "VALUE", UI::FONT_SIZE_NORMAL);
        m_headerValueText->setFillColor(UI::Color::TEXT_PRIMARY);
        m_headerValueText->setPosition(sf::Vector2f(1000.0f, 163.0f));

        // Total weight display
        m_totalWeightText.emplace(m_font, "Total: 0.0 kg", UI::FONT_SIZE_NORMAL);
        m_totalWeightText->setFillColor(UI::Color::ACCENT_BLUE);
        m_totalWeightText->setPosition(sf::Vector2f(900.0f, 770.0f));

        // Controls help text
        m_controlsText.emplace(m_font, "[W/S] Navigate  [Enter] Use  [Esc] Close", UI::FONT_SIZE_SMALL);
        m_controlsText->setFillColor(UI::Color::TEXT_SECONDARY);
        m_controlsText->setPosition(sf::Vector2f(270.0f, 770.0f));
    }

    // Initialize total weight
    updateTotalWeight();
}

void InventoryScene::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::W:
            case sf::Keyboard::Key::Up:
                selectPrevious();
                break;

            case sf::Keyboard::Key::S:
            case sf::Keyboard::Key::Down:
                selectNext();
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                useSelectedItem();
                break;

            case sf::Keyboard::Key::Escape:
                m_isFinished = true;
                m_nextScene = m_returnToScene;
                break;

            default:
                break;
        }
    }
}

void InventoryScene::update(float deltaTime) {
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Count non-empty slots
    int nonEmptyCount = 0;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            nonEmptyCount++;
        }
    }

    // Update selection highlight position
    if (nonEmptyCount > 0 && m_selectedIndex >= 0 && m_selectedIndex < nonEmptyCount) {
        int displayIndex = m_selectedIndex - m_scrollOffset;
        float highlightY = m_listStartY + (displayIndex * m_itemHeight) + 2.0f;
        m_selectionHighlight.setPosition(sf::Vector2f(280.0f, highlightY));
    }
}

void InventoryScene::render(sf::RenderWindow& window) {
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Draw background overlay
    window.draw(m_backgroundBox);

    // Draw main panels
    window.draw(m_listContainer);
    window.draw(m_headerBar);

    // Draw selection highlight (check if there are any non-empty items)
    bool hasItems = false;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            hasItems = true;
            break;
        }
    }
    if (hasItems) {
        window.draw(m_selectionHighlight);
    }

    // Draw text elements
    if (m_fontLoaded) {
        if (m_titleText) window.draw(*m_titleText);
        if (m_headerNameText) window.draw(*m_headerNameText);
        if (m_headerWeightText) window.draw(*m_headerWeightText);
        if (m_headerValueText) window.draw(*m_headerValueText);
        if (m_totalWeightText) window.draw(*m_totalWeightText);
        if (m_controlsText) window.draw(*m_controlsText);
    }

    // Draw item list
    renderItemList(window);
}

SceneType InventoryScene::getNextScene() const {
    return m_nextScene;
}

bool InventoryScene::isFinished() const {
    return m_isFinished;
}

void InventoryScene::selectPrevious() {
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Count non-empty slots
    int nonEmptyCount = 0;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            nonEmptyCount++;
        }
    }

    if (nonEmptyCount == 0) return;

    m_selectedIndex--;
    if (m_selectedIndex < 0) {
        m_selectedIndex = nonEmptyCount - 1;
    }

    // Adjust scroll offset
    if (m_selectedIndex < m_scrollOffset) {
        m_scrollOffset = m_selectedIndex;
    }
    if (m_selectedIndex >= m_scrollOffset + m_maxVisibleItems) {
        m_scrollOffset = m_selectedIndex - m_maxVisibleItems + 1;
    }
}

void InventoryScene::selectNext() {
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Count non-empty slots
    int nonEmptyCount = 0;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            nonEmptyCount++;
        }
    }

    if (nonEmptyCount == 0) return;

    m_selectedIndex++;
    if (m_selectedIndex >= nonEmptyCount) {
        m_selectedIndex = 0;
    }

    // Adjust scroll offset
    if (m_selectedIndex < m_scrollOffset) {
        m_scrollOffset = 0;
    }
    if (m_selectedIndex >= m_scrollOffset + m_maxVisibleItems) {
        m_scrollOffset = m_selectedIndex - m_maxVisibleItems + 1;
    }
}

void InventoryScene::useSelectedItem() {
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Get list of non-empty slots
    std::vector<int> nonEmptySlots;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            nonEmptySlots.push_back(i);
        }
    }

    if (nonEmptySlots.empty() || m_selectedIndex < 0 || m_selectedIndex >= static_cast<int>(nonEmptySlots.size())) {
        return;
    }

    int actualSlotIndex = nonEmptySlots[m_selectedIndex];
    const InventorySlot& slot = playerState.getInventory().getSlot(actualSlotIndex);

    // [MVP] Simple message for now - actual item usage not implemented
    std::cout << "Using item: " << slot.item.name << std::endl;
}

void InventoryScene::renderItemList(sf::RenderWindow& window) {
    if (!m_fontLoaded) return;

    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Get list of non-empty slots from real inventory
    std::vector<int> nonEmptySlots;
    for (int i = 0; i < playerState.getInventory().getSlotCount(); ++i) {
        if (!playerState.getInventory().getSlot(i).isEmpty) {
            nonEmptySlots.push_back(i);
        }
    }

    if (nonEmptySlots.empty()) {
        // Show empty message
        sf::Text emptyText(m_font, "[ Inventory is empty ]", UI::FONT_SIZE_NORMAL);
        emptyText.setFillColor(UI::Color::TEXT_SECONDARY);
        emptyText.setPosition(sf::Vector2f(600.0f, 400.0f));
        window.draw(emptyText);
        return;
    }

    // Calculate visible range
    int startIdx = m_scrollOffset;
    int endIdx = std::min(startIdx + m_maxVisibleItems, static_cast<int>(nonEmptySlots.size()));

    for (int i = startIdx; i < endIdx; ++i) {
        int displayIdx = i - m_scrollOffset;
        float yPos = m_listStartY + (displayIdx * m_itemHeight);

        int actualSlotIndex = nonEmptySlots[i];
        const InventorySlot& slot = playerState.getInventory().getSlot(actualSlotIndex);

        // Item name
        sf::Text nameText(m_font, slot.item.name, UI::FONT_SIZE_NORMAL);
        nameText.setFillColor(i == m_selectedIndex ? UI::Color::ACCENT_YELLOW : UI::Color::TEXT_PRIMARY);
        nameText.setPosition(sf::Vector2f(300.0f, yPos + 8.0f));
        window.draw(nameText);

        // Item weight
        std::ostringstream weightStr;
        weightStr << std::fixed << std::setprecision(1) << slot.item.weight << " kg";
        sf::Text weightText(m_font, weightStr.str(), UI::FONT_SIZE_NORMAL);
        weightText.setFillColor(i == m_selectedIndex ? UI::Color::ACCENT_YELLOW : UI::Color::TEXT_SECONDARY);
        weightText.setPosition(sf::Vector2f(750.0f, yPos + 8.0f));
        window.draw(weightText);

        // Item value
        std::ostringstream valueStr;
        valueStr << "$" << slot.item.value;
        sf::Text valueText(m_font, valueStr.str(), UI::FONT_SIZE_NORMAL);
        valueText.setFillColor(i == m_selectedIndex ? UI::Color::ACCENT_YELLOW : UI::Color::TEXT_SECONDARY);
        valueText.setPosition(sf::Vector2f(1000.0f, yPos + 8.0f));
        window.draw(valueText);

        // Stack count (if stackable and > 1)
        if (slot.item.isStackable && slot.count > 1) {
            std::ostringstream countStr;
            countStr << "x" << slot.count;
            sf::Text countText(m_font, countStr.str(), UI::FONT_SIZE_SMALL);
            countText.setFillColor(UI::Color::TEXT_SECONDARY);
            countText.setPosition(sf::Vector2f(650.0f, yPos + 10.0f));
            window.draw(countText);
        }
    }

    // Scroll indicator
    if (static_cast<int>(nonEmptySlots.size()) > m_maxVisibleItems) {
        sf::Text scrollText(m_font,
            "(" + std::to_string(m_selectedIndex + 1) + "/" + std::to_string(nonEmptySlots.size()) + ")",
            UI::FONT_SIZE_SMALL);
        scrollText.setFillColor(UI::Color::TEXT_SECONDARY);
        scrollText.setPosition(sf::Vector2f(1100.0f, m_listStartY - 30.0f));
        window.draw(scrollText);
    }
}

void InventoryScene::updateTotalWeight() {
    if (!m_fontLoaded || !m_totalWeightText) return;

    auto& playerState = GameStateManager::getInstance().getPlayerState();
    float totalWeight = playerState.getInventory().getTotalWeight();
    float maxWeight = playerState.getInventory().getMaxWeight();

    std::ostringstream weightStr;
    weightStr << "Total: " << std::fixed << std::setprecision(1) << totalWeight << "/" << maxWeight << " kg";
    m_totalWeightText->setString(weightStr.str());
}

