#include "LocationScene.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include "GameStateManager.h"
#include <iostream>

LocationScene::LocationScene(LocationType locationType, const std::string& locationName)
    : m_locationType(locationType),
      m_locationName(locationName),
      m_isFinished(false),
      m_nextScene(SceneType::TRAVEL_SELECTION),  // [MVP] Return to travel selection / map
      m_showInteractionMenu(false),
      m_fontLoaded(false),
      m_hudPanel(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT),  // Initialize HUD with fixed dimensions
      m_isInShopMode(false),
      m_storeInventory(30, 1000.0f),  // Store has 30 slots, unlimited weight
      m_selectedStoreIndex(0),
      m_selectedPlayerIndex(0),
      m_storeScrollOffset(0),
      m_playerScrollOffset(0),
      m_isStoreListActive(true) {     // Start with store list active

    // [MVP] Advance to next city when arriving at a new location
    auto& playerState = GameStateManager::getInstance().getPlayerState();
    int currentCityIndex = playerState.getCurrentCityIndex();
    std::cout << "[MVP] LocationScene: Arrived at city " << currentCityIndex << std::endl;

    setupLocationElements();
    setupInteractionOptions();

    // Initialize store inventory if this is a store
    if (m_locationType == LocationType::STORE) {
        initializeStoreInventory();
        // Auto-enter shop mode for stores
        enterShopMode();
    }
}

void LocationScene::setupLocationElements() {
    // SFML 3.x: Font loading with FontLoader helper
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "Warning: Could not load font in LocationScene" << std::endl;
    }

    // Setup background with fixed dimensions
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));  // Fixed: Vector2f wrapper
    m_background.setFillColor(sf::Color(60, 60, 70));

    // Setup counter/service area
    m_counterBox.setSize(sf::Vector2f(400.0f, 200.0f));
    m_counterBox.setPosition(sf::Vector2f(483.0f, 200.0f));  // Fixed: Vector2f wrapper
    m_counterBox.setFillColor(sf::Color(80, 80, 90));
    m_counterBox.setOutlineColor(sf::Color(120, 120, 130));
    m_counterBox.setOutlineThickness(3);

    // Setup player character
    m_playerBox.setSize(sf::Vector2f(60.0f, 100.0f));
    m_playerBox.setPosition(sf::Vector2f(300.0f, 500.0f));  // Fixed: Vector2f wrapper
    m_playerBox.setFillColor(sf::Color(100, 150, 200));

    // Setup exit door
    m_exitDoorBox.setSize(sf::Vector2f(100.0f, 150.0f));
    m_exitDoorBox.setPosition(sf::Vector2f(50.0f, 500.0f));  // Fixed: Vector2f wrapper
    m_exitDoorBox.setFillColor(sf::Color(139, 69, 19)); // Brown door
    m_exitDoorBox.setOutlineColor(sf::Color(169, 99, 49));
    m_exitDoorBox.setOutlineThickness(2);

    // SFML 3.x: Setup location name text - requires font in constructor
    std::string displayName = m_locationName.empty() ?
        getLocationDescription() : m_locationName;

    if (m_fontLoaded) {
        m_locationNameText.emplace(m_font, displayName, 28);
        m_locationNameText->setFillColor(sf::Color::White);
        m_locationNameText->setPosition(sf::Vector2f(20.0f, 20.0f));  // Fixed: Vector2f wrapper

        // Setup instructions text
        m_instructionsText.emplace(m_font, "Press E to interact | ESC to leave", 18);
        m_instructionsText->setFillColor(sf::Color(200, 200, 200));
        m_instructionsText->setPosition(sf::Vector2f(20.0f, 700.0f));  // Fixed: Vector2f wrapper

        // Setup status text
        m_statusText.emplace(m_font, "", 20);
        m_statusText->setFillColor(sf::Color::White);
        m_statusText->setPosition(sf::Vector2f(900.0f, 20.0f));  // Fixed: Vector2f wrapper
    }

    // Add location-specific objects based on type
    switch (m_locationType) {
        case LocationType::GAS_STATION: {
            // Add gas pump
            sf::RectangleShape pump;
            pump.setSize(sf::Vector2f(80.0f, 150.0f));
            pump.setPosition(sf::Vector2f(1100.0f, 450.0f));  // Fixed: Vector2f wrapper
            pump.setFillColor(sf::Color(200, 50, 50));
            m_objects.push_back(pump);
            break;
        }
        case LocationType::STORE: {
            // Add shelves
            for (int i = 0; i < 3; ++i) {
                sf::RectangleShape shelf;
                shelf.setSize(sf::Vector2f(150.0f, 200.0f));
                shelf.setPosition(sf::Vector2f(800.0f + i * 180.0f, 250.0f));  // Fixed: Vector2f wrapper
                shelf.setFillColor(sf::Color(100, 70, 50));
                m_objects.push_back(shelf);
            }
            break;
        }
        case LocationType::MOTEL: {
            // Add bed
            sf::RectangleShape bed;
            bed.setSize(sf::Vector2f(200.0f, 120.0f));
            bed.setPosition(sf::Vector2f(900.0f, 300.0f));  // Fixed: Vector2f wrapper
            bed.setFillColor(sf::Color(150, 100, 100));
            m_objects.push_back(bed);
            break;
        }
        default:
            break;
    }
}

void LocationScene::setupInteractionOptions() {
    // This would setup the interaction menu
    // For now, we'll handle interactions directly
}

void LocationScene::handleInput(const sf::Event& event) {
    // If in shop mode, handle shop-specific input
    if (m_isInShopMode) {
        handleShopInput(event);
        return;
    }

    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            handleLeave();
        } else if (keyPressed->code == sf::Keyboard::Key::E) {
            // Interact with location based on type
            switch (m_locationType) {
                case LocationType::GAS_STATION:
                    handleRefuel();
                    break;
                case LocationType::STORE:
                    handleShop();
                    break;
                case LocationType::GARAGE:
                case LocationType::MECHANIC:
                    handleRepair();
                    break;
                case LocationType::DINER:
                    handleEat();
                    break;
                case LocationType::MOTEL:
                    handleRest();
                    break;
                default:
                    break;
            }
        }
    }
}

void LocationScene::update(float deltaTime) {
    // [MVP] Disabled - Status display (uncomment to enable)
    /*
    // Update status display (SFML 3.x: check if optional text exists)
    if (m_statusText) {
        std::string status = "Money: $" + std::to_string(static_cast<int>(m_playerState.getMoney())) +
                            " | Fuel: " + std::to_string(static_cast<int>(m_playerState.getFuel())) + "L" +
                            " | Energy: " + std::to_string(static_cast<int>(m_playerState.getEnergy())) + "%";
        m_statusText->setString(status);
    }
    */

    // [MVP] Disabled - HUD update (uncomment to enable)
    /*
    // Update HUD
    m_hudPanel.setMoney(m_playerState.getMoney());
    m_hudPanel.setFuel(m_playerState.getFuel(), 100.0f);
    m_hudPanel.setEnergy(m_playerState.getEnergy(), 100.0f);
    float cargoWeight = m_playerState.getInventory().getTotalWeight();
    m_hudPanel.setCargo(cargoWeight, 100.0f);
    m_hudPanel.updateTopRight(deltaTime);
    */
}

void LocationScene::render(sf::RenderWindow& window) {
    // If in shop mode, render shop UI instead
    if (m_isInShopMode) {
        renderShopUI(window);
        return;
    }

    // Draw background
    window.draw(m_background);

    // Draw counter
    window.draw(m_counterBox);

    // Draw objects
    for (const auto& obj : m_objects) {
        window.draw(obj);
    }

    // Draw exit door
    window.draw(m_exitDoorBox);

    // Draw player
    window.draw(m_playerBox);

    // [MVP] ACTIVE - Location UI
    if (m_fontLoaded) {
        if (m_locationNameText) {
            window.draw(*m_locationNameText);
        }
        if (m_instructionsText) {
            window.draw(*m_instructionsText);
        }
        // [MVP] Disabled - Status text (uncomment to enable)
        /*
        if (m_statusText) {
            window.draw(*m_statusText);
        }
        */
    }

    // [MVP] Disabled - HUD rendering (uncomment to enable)
    /*
    m_hudPanel.render(window);
    */
}

SceneType LocationScene::getNextScene() const {
    return m_nextScene;
}

bool LocationScene::isFinished() const {
    return m_isFinished;
}

// [MVP] Disabled - Service interactions (uncomment to enable)
/*
// Interaction handlers
void LocationScene::handleRefuel() {
    float fuelCost = 50.0f; // Cost per liter
    float fuelAmount = 50.0f; // Amount to refuel
    float totalCost = fuelAmount * fuelCost;

    if (m_playerState.canAfford(totalCost)) {
        m_playerState.addMoney(-totalCost);
        m_playerState.addFuel(fuelAmount);
        std::cout << "Refueled " << fuelAmount << "L for $" << totalCost << std::endl;
    } else {
        std::cout << "Not enough money to refuel!" << std::endl;
    }
}
*/

void LocationScene::handleRefuel() {
    // [MVP] Disabled - Resource management not active
}

void LocationScene::handleShop() {
    // Enter shop mode
    std::cout << "Opening store..." << std::endl;
    enterShopMode();
}

void LocationScene::handleRepair() {
    // [MVP] Disabled - Resource management not active
}

void LocationScene::handleEat() {
    // [MVP] Disabled - Resource management not active
}

void LocationScene::handleRest() {
    // [MVP] Disabled - Resource management not active
}

void LocationScene::handleTalk() {
    // Simple talk interaction - placeholder for future dialogue system
    std::cout << "Talking to NPC - simple dialogue placeholder" << std::endl;
    // Will implement simple dialogue later (Phase 5.3)
}

void LocationScene::handleLeave() {
    std::cout << "Leaving location" << std::endl;
    m_isFinished = true;
}

// ============================================================================
// SHOP SYSTEM IMPLEMENTATION
// ============================================================================

void LocationScene::initializeStoreInventory() {
    // Create sample items for the store
    std::cout << "Initializing store inventory..." << std::endl;

    // Food items
    Item bread("Bread", "Fresh bread", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 50, 0.5f, true, 10);
    m_storeInventory.addItem(bread, 10);

    Item water("Water", "Bottle of water", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 30, 1.0f, true, 20);
    m_storeInventory.addItem(water, 20);

    Item canned("Canned Food", "Canned beans", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 80, 0.8f, true, 15);
    m_storeInventory.addItem(canned, 15);

    // Tools
    Item toolkit("Toolkit", "Basic repair kit", ItemCategory::TOOL, ItemRarity::UNCOMMON, 500, 3.0f, false, 1);
    m_storeInventory.addItem(toolkit, 3);

    Item flashlight("Flashlight", "LED flashlight", ItemCategory::TOOL, ItemRarity::COMMON, 150, 0.5f, false, 1);
    m_storeInventory.addItem(flashlight, 5);

    Item map("Map", "Road map", ItemCategory::TOOL, ItemRarity::COMMON, 100, 0.2f, false, 1);
    m_storeInventory.addItem(map, 2);

    // Equipment
    Item spareTire("Spare Tire", "Emergency tire", ItemCategory::EQUIPMENT, ItemRarity::UNCOMMON, 800, 15.0f, false, 1);
    m_storeInventory.addItem(spareTire, 2);

    Item roofRack("Roof Rack", "Extra storage", ItemCategory::EQUIPMENT, ItemRarity::RARE, 1200, 10.0f, false, 1);
    m_storeInventory.addItem(roofRack, 1);

    // Junk (valuable but no use)
    Item scrap("Scrap Metal", "Valuable scrap", ItemCategory::JUNK, ItemRarity::COMMON, 20, 2.0f, true, 50);
    m_storeInventory.addItem(scrap, 10);

    std::cout << "Store inventory initialized with items" << std::endl;
}

void LocationScene::enterShopMode() {
    m_isInShopMode = true;
    m_selectedStoreIndex = 0;
    m_selectedPlayerIndex = 0;
    m_storeScrollOffset = 0;
    m_playerScrollOffset = 0;
    m_isStoreListActive = true;  // Start with store list selected
    std::cout << "[SHOP] Entered shop mode" << std::endl;
}

void LocationScene::exitShopMode() {
    m_isInShopMode = false;
    std::cout << "[SHOP] Exited shop mode" << std::endl;
}

void LocationScene::handleShopInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Escape:
                exitShopMode();
                break;

            case sf::Keyboard::Key::Tab:
                // Switch between store and inventory lists
                m_isStoreListActive = !m_isStoreListActive;
                std::cout << "[SHOP] Switched to " << (m_isStoreListActive ? "STORE" : "INVENTORY") << " list" << std::endl;
                break;

            case sf::Keyboard::Key::W:
            case sf::Keyboard::Key::Up:
                // Move selection up
                if (m_isStoreListActive) {
                    if (m_selectedStoreIndex > 0) {
                        m_selectedStoreIndex--;
                        // Scroll up if needed
                        if (m_selectedStoreIndex < m_storeScrollOffset) {
                            m_storeScrollOffset = m_selectedStoreIndex;
                        }
                    }
                } else {
                    if (m_selectedPlayerIndex > 0) {
                        m_selectedPlayerIndex--;
                        if (m_selectedPlayerIndex < m_playerScrollOffset) {
                            m_playerScrollOffset = m_selectedPlayerIndex;
                        }
                    }
                }
                break;

            case sf::Keyboard::Key::S:
            case sf::Keyboard::Key::Down:
                // Move selection down
                if (m_isStoreListActive) {
                    int storeSlotCount = m_storeInventory.getSlotCount();
                    if (m_selectedStoreIndex < storeSlotCount - 1) {
                        m_selectedStoreIndex++;
                        // Scroll down if needed (show 10 items at once)
                        if (m_selectedStoreIndex >= m_storeScrollOffset + 10) {
                            m_storeScrollOffset = m_selectedStoreIndex - 9;
                        }
                    }
                } else {
                    auto& playerState = GameStateManager::getInstance().getPlayerState();
                    int playerSlotCount = playerState.getInventory().getSlotCount();
                    if (m_selectedPlayerIndex < playerSlotCount - 1) {
                        m_selectedPlayerIndex++;
                        if (m_selectedPlayerIndex >= m_playerScrollOffset + 10) {
                            m_playerScrollOffset = m_selectedPlayerIndex - 9;
                        }
                    }
                }
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                // Buy or sell
                if (m_isStoreListActive) {
                    buySelectedItem();
                } else {
                    sellSelectedItem();
                }
                break;

            default:
                break;
        }
    }
}

void LocationScene::buySelectedItem() {
    auto& playerState = GameStateManager::getInstance().getPlayerState();
    const InventorySlot& storeSlot = m_storeInventory.getSlot(m_selectedStoreIndex);

    if (storeSlot.isEmpty) {
        std::cout << "[SHOP] No item in selected slot" << std::endl;
        return;
    }

    const Item& item = storeSlot.item;
    int price = item.value;

    // Check if player can afford
    if (!playerState.canAfford(static_cast<float>(price))) {
        std::cout << "[SHOP] Not enough money! Need " << price << ", have " << playerState.getMoney() << std::endl;
        return;
    }

    // Check weight limit
    float currentWeight = playerState.getInventory().getTotalWeight();
    float maxWeight = playerState.getInventory().getMaxWeight();
    if (currentWeight + item.weight > maxWeight) {
        std::cout << "[SHOP] Too heavy! Weight: " << (currentWeight + item.weight) << "/" << maxWeight << std::endl;
        return;
    }

    // Try to add to player inventory
    if (playerState.getInventory().addItem(item, 1)) {
        // Success! Deduct money and remove from store
        playerState.addMoney(-static_cast<float>(price));
        m_storeInventory.removeItem(m_selectedStoreIndex, 1);
        std::cout << "[SHOP] Bought " << item.name << " for " << price << " rubles" << std::endl;
    } else {
        std::cout << "[SHOP] Inventory full!" << std::endl;
    }
}

void LocationScene::sellSelectedItem() {
    auto& playerState = GameStateManager::getInstance().getPlayerState();
    const InventorySlot& playerSlot = playerState.getInventory().getSlot(m_selectedPlayerIndex);

    if (playerSlot.isEmpty) {
        std::cout << "[SHOP] No item in selected slot" << std::endl;
        return;
    }

    const Item& item = playerSlot.item;
    int sellPrice = item.value / 2;  // Sell for half price

    // Add item to store and give money to player
    if (m_storeInventory.addItem(item, 1)) {
        playerState.getInventory().removeItem(m_selectedPlayerIndex, 1);
        playerState.addMoney(static_cast<float>(sellPrice));
        std::cout << "[SHOP] Sold " << item.name << " for " << sellPrice << " rubles" << std::endl;
    } else {
        std::cout << "[SHOP] Store inventory full!" << std::endl;
    }
}

void LocationScene::renderShopUI(sf::RenderWindow& window) {
    if (!m_fontLoaded) return;

    auto& playerState = GameStateManager::getInstance().getPlayerState();

    // Background
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    bg.setFillColor(UI::Color::BACKGROUND_DARK);
    window.draw(bg);

    // Title
    sf::Text titleText(m_font, "STORE", UI::FONT_SIZE_HUGE);
    titleText.setFillColor(UI::Color::ACCENT_YELLOW);
    titleText.setPosition(sf::Vector2f(50.0f, 20.0f));
    window.draw(titleText);

    // Player resources (money and weight)
    float money = playerState.getMoney();
    float currentWeight = playerState.getInventory().getTotalWeight();
    float maxWeight = playerState.getInventory().getMaxWeight();

    std::string resourcesStr = "Money: " + std::to_string(static_cast<int>(money)) + " RUB  |  " +
                               "Weight: " + std::to_string(static_cast<int>(currentWeight)) + "/" +
                               std::to_string(static_cast<int>(maxWeight)) + " kg";
    sf::Text resourcesText(m_font, resourcesStr, UI::FONT_SIZE_NORMAL);
    resourcesText.setFillColor(UI::Color::ACCENT_BLUE);
    resourcesText.setPosition(sf::Vector2f(50.0f, 80.0f));
    window.draw(resourcesText);

    // Instructions
    sf::Text instructionsText(m_font, "[Tab] Switch List  [W/S] Navigate  [Enter] Buy/Sell  [Esc] Exit", UI::FONT_SIZE_SMALL);
    instructionsText.setFillColor(UI::Color::TEXT_SECONDARY);
    instructionsText.setPosition(sf::Vector2f(50.0f, 820.0f));
    window.draw(instructionsText);

    // Two columns: Store items (left) and Player inventory (right)
    float leftX = 50.0f;
    float rightX = 750.0f;
    float startY = 150.0f;
    float columnWidth = 640.0f;
    float rowHeight = 60.0f;
    int maxVisibleRows = 10;

    // ===== LEFT COLUMN: STORE ITEMS =====
    // Column header
    sf::RectangleShape storeHeader;
    storeHeader.setSize(sf::Vector2f(columnWidth, 50.0f));
    storeHeader.setPosition(sf::Vector2f(leftX, startY - 50.0f));
    storeHeader.setFillColor(m_isStoreListActive ? UI::Color::ACCENT_BLUE : UI::Color::BACKGROUND_LIGHT);
    window.draw(storeHeader);

    sf::Text storeHeaderText(m_font, "STORE ITEMS (BUY)", UI::FONT_SIZE_SUBTITLE);
    storeHeaderText.setFillColor(UI::Color::TEXT_PRIMARY);
    storeHeaderText.setPosition(sf::Vector2f(leftX + 10.0f, startY - 45.0f));
    window.draw(storeHeaderText);

    // Render store items
    for (int i = 0; i < maxVisibleRows; ++i) {
        int slotIndex = m_storeScrollOffset + i;
        if (slotIndex >= m_storeInventory.getSlotCount()) break;

        const InventorySlot& slot = m_storeInventory.getSlot(slotIndex);
        float y = startY + i * rowHeight;

        // Item background
        sf::RectangleShape itemBg;
        itemBg.setSize(sf::Vector2f(columnWidth, rowHeight - 5.0f));
        itemBg.setPosition(sf::Vector2f(leftX, y));

        if (m_isStoreListActive && slotIndex == m_selectedStoreIndex) {
            itemBg.setFillColor(UI::Color::ACCENT_YELLOW);
            itemBg.setOutlineColor(UI::Color::TEXT_PRIMARY);
            itemBg.setOutlineThickness(2.0f);
        } else {
            itemBg.setFillColor(UI::Color::BACKGROUND_LIGHT);
        }
        window.draw(itemBg);

        if (!slot.isEmpty) {
            // Item name
            std::string itemStr = slot.item.name + " x" + std::to_string(slot.count);
            sf::Text itemText(m_font, itemStr, UI::FONT_SIZE_NORMAL);
            itemText.setFillColor(UI::Color::TEXT_PRIMARY);
            itemText.setPosition(sf::Vector2f(leftX + 10.0f, y + 5.0f));
            window.draw(itemText);

            // Item price and weight
            std::string priceStr = std::to_string(slot.item.value) + " RUB  " +
                                   std::to_string(static_cast<int>(slot.item.weight * 10) / 10.0f) + " kg";
            sf::Text priceText(m_font, priceStr, UI::FONT_SIZE_SMALL);
            priceText.setFillColor(UI::Color::TEXT_SECONDARY);
            priceText.setPosition(sf::Vector2f(leftX + 10.0f, y + 32.0f));
            window.draw(priceText);
        }
    }

    // ===== RIGHT COLUMN: PLAYER INVENTORY =====
    // Column header
    sf::RectangleShape invHeader;
    invHeader.setSize(sf::Vector2f(columnWidth, 50.0f));
    invHeader.setPosition(sf::Vector2f(rightX, startY - 50.0f));
    invHeader.setFillColor(!m_isStoreListActive ? UI::Color::ACCENT_BLUE : UI::Color::BACKGROUND_LIGHT);
    window.draw(invHeader);

    sf::Text invHeaderText(m_font, "YOUR INVENTORY (SELL)", UI::FONT_SIZE_SUBTITLE);
    invHeaderText.setFillColor(UI::Color::TEXT_PRIMARY);
    invHeaderText.setPosition(sf::Vector2f(rightX + 10.0f, startY - 45.0f));
    window.draw(invHeaderText);

    // Render player inventory
    for (int i = 0; i < maxVisibleRows; ++i) {
        int slotIndex = m_playerScrollOffset + i;
        if (slotIndex >= playerState.getInventory().getSlotCount()) break;

        const InventorySlot& slot = playerState.getInventory().getSlot(slotIndex);
        float y = startY + i * rowHeight;

        // Item background
        sf::RectangleShape itemBg;
        itemBg.setSize(sf::Vector2f(columnWidth, rowHeight - 5.0f));
        itemBg.setPosition(sf::Vector2f(rightX, y));

        if (!m_isStoreListActive && slotIndex == m_selectedPlayerIndex) {
            itemBg.setFillColor(UI::Color::ACCENT_YELLOW);
            itemBg.setOutlineColor(UI::Color::TEXT_PRIMARY);
            itemBg.setOutlineThickness(2.0f);
        } else {
            itemBg.setFillColor(UI::Color::BACKGROUND_LIGHT);
        }
        window.draw(itemBg);

        if (!slot.isEmpty) {
            // Item name
            std::string itemStr = slot.item.name + " x" + std::to_string(slot.count);
            sf::Text itemText(m_font, itemStr, UI::FONT_SIZE_NORMAL);
            itemText.setFillColor(UI::Color::TEXT_PRIMARY);
            itemText.setPosition(sf::Vector2f(rightX + 10.0f, y + 5.0f));
            window.draw(itemText);

            // Sell price and weight
            int sellPrice = slot.item.value / 2;
            std::string priceStr = std::to_string(sellPrice) + " RUB  " +
                                   std::to_string(static_cast<int>(slot.item.weight * 10) / 10.0f) + " kg";
            sf::Text priceText(m_font, priceStr, UI::FONT_SIZE_SMALL);
            priceText.setFillColor(UI::Color::TEXT_SECONDARY);
            priceText.setPosition(sf::Vector2f(rightX + 10.0f, y + 32.0f));
            window.draw(priceText);
        }
    }
}

std::string LocationScene::getLocationDescription() const {
    switch (m_locationType) {
        case LocationType::HOME:
            return "Home";
        case LocationType::GAS_STATION:
            return "Gas Station";
        case LocationType::STORE:
            return "Store";
        case LocationType::GARAGE:
            return "Garage";
        case LocationType::MOTEL:
            return "Motel";
        case LocationType::DINER:
            return "Diner";
        case LocationType::MECHANIC:
            return "Mechanic Shop";
        default:
            return "Unknown Location";
    }
}
