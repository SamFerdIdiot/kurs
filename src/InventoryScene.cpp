#include "InventoryScene.h"
#include "GameStateManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

InventoryScene::InventoryScene()
    : m_cellSize(80.0f),
      m_gridStartX(100.0f),
      m_gridStartY(150.0f),
      m_hoveredCellX(-1),
      m_hoveredCellY(-1),
      m_draggedItemId(-1),
      m_isDragging(false),
      m_isFinished(false),
      m_nextScene(SceneType::INVENTORY),
      m_returnToScene(SceneType::NODE),
      m_fontLoaded(false) {
    
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("InventoryScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Загрузка шрифта с fallback опциями
    if (m_font.loadFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "InventoryScene: WARNING - Could not load any font!" << std::endl;
    }
    
    // Инициализация GridInventorySystem
    m_gridInventory = std::make_unique<GridInventorySystem>(6, 4, 64.0f);
    
    // Инициализация UI элементов with responsive scaling
    // Полупрозрачный фон
    m_backgroundBox.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_backgroundBox.setFillColor(sf::Color(30, 30, 46, 230)); // Темный с прозрачностью
    
    // Контейнер сетки
    m_gridContainer.setSize(SCALE_SIZE(layout, 520.0f, 380.0f));
    m_gridContainer.setPosition(SCALE_POS(layout, 90.0f, 140.0f));
    m_gridContainer.setFillColor(sf::Color(62, 62, 78)); // Medium gray
    m_gridContainer.setOutlineColor(sf::Color(94, 126, 160)); // Light blue
    m_gridContainer.setOutlineThickness(2.0f);
    
    // Панель деталей
    m_detailPanel.setSize(SCALE_SIZE(layout, 150.0f, 380.0f));
    m_detailPanel.setPosition(SCALE_POS(layout, 630.0f, 140.0f));
    m_detailPanel.setFillColor(sf::Color(62, 62, 78));
    m_detailPanel.setOutlineColor(sf::Color(94, 126, 160));
    m_detailPanel.setOutlineThickness(2.0f);
    
    // Панель статистики
    m_statsPanel.setSize(SCALE_SIZE(layout, 690.0f, 50.0f));
    m_statsPanel.setPosition(SCALE_POS(layout, 90.0f, 75.0f));
    m_statsPanel.setFillColor(sf::Color(62, 62, 78));
    m_statsPanel.setOutlineColor(sf::Color(94, 126, 160));
    m_statsPanel.setOutlineThickness(2.0f);
    
    // Кнопка закрытия
    m_closeButton.setSize(SCALE_SIZE(layout, 100.0f, 40.0f));
    m_closeButton.setPosition(SCALE_POS(layout, 680.0f, 20.0f));
    m_closeButton.setFillColor(sf::Color(244, 67, 54)); // Red
    m_closeButton.setOutlineColor(sf::Color::White);
    m_closeButton.setOutlineThickness(2.0f);
    
    // Заголовок
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 25.0f));
    m_titleText.setString("INVENTORY");
    
    // Текст кнопки закрытия
    m_closeButtonText.setFont(m_font);
    m_closeButtonText.setCharacterSize(SCALE_FONT(layout, 20));
    m_closeButtonText.setFillColor(sf::Color::White);
    m_closeButtonText.setPosition(SCALE_POS(layout, 690.0f, 28.0f));
    m_closeButtonText.setString("CLOSE");
    
    // Текст статистики
    m_statsText.setFont(m_font);
    m_statsText.setCharacterSize(SCALE_FONT(layout, 18));
    m_statsText.setFillColor(sf::Color(204, 204, 204));
    m_statsText.setPosition(SCALE_POS(layout, 100.0f, 85.0f));
    
    // Текст деталей
    m_detailText.setFont(m_font);
    m_detailText.setCharacterSize(SCALE_FONT(layout, 14));
    m_detailText.setFillColor(sf::Color(204, 204, 204));
    m_detailText.setPosition(SCALE_POS(layout, 640.0f, 150.0f));
    m_detailText.setString("Select an item\nto view details");
    
    // Текст управления
    m_controlsText.setFont(m_font);
    m_controlsText.setCharacterSize(SCALE_FONT(layout, 14));
    m_controlsText.setFillColor(sf::Color(150, 150, 150));
    m_controlsText.setPosition(SCALE_POS(layout, 100.0f, 540.0f));
    m_controlsText.setString("Left Click: Pick/Place | Right Click: Rotate | ESC: Close");
    
    // Инициализация сетки
    initializeGridCells();
    
    // Обновление статистики
    updateStats();
    
    // Добавление тестовых предметов
    addTestItems();
}

void InventoryScene::initializeGridCells() {
    m_gridCells.clear();
    
    int gridWidth = m_gridInventory->getGridWidth();
    int gridHeight = m_gridInventory->getGridHeight();
    
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            sf::RectangleShape cell(sf::Vector2f(m_cellSize - 2.0f, m_cellSize - 2.0f));
            float posX = m_gridStartX + x * m_cellSize + 1.0f;
            float posY = m_gridStartY + y * m_cellSize + 1.0f;
            cell.setPosition(posX, posY);
            cell.setFillColor(sf::Color(40, 40, 40)); // Dark gray for empty
            cell.setOutlineColor(sf::Color(62, 62, 78)); // Medium gray border
            cell.setOutlineThickness(1.0f);
            m_gridCells.push_back(cell);
        }
    }
}

void InventoryScene::addTestItems() {
    // Добавление нескольких тестовых предметов для демонстрации
    Item item1;
    item1.name = "Canned Food";
    item1.weight = 0.5f;
    item1.value = 10.0f;
    m_gridInventory->addItem(item1, 1, 1, 0, 0, 1);
    
    Item item2;
    item2.name = "Water Bottle";
    item2.weight = 1.0f;
    item2.value = 5.0f;
    m_gridInventory->addItem(item2, 1, 1, 1, 0, 1);
    
    Item item3;
    item3.name = "First Aid Kit";
    item3.weight = 2.0f;
    item3.value = 50.0f;
    m_gridInventory->addItem(item3, 2, 1, 0, 1, 1);
    
    Item item4;
    item4.name = "Fuel Can";
    item4.weight = 5.0f;
    item4.value = 30.0f;
    m_gridInventory->addItem(item4, 2, 2, 3, 0, 1);
}

void InventoryScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_isFinished = true;
            m_nextScene = m_returnToScene;
        }
    }
    
    if (event.type == sf::Event::MouseMoved) {
        m_mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        updateHoveredCell();
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            handleLeftClick();
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            handleRightClick();
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (m_isDragging) {
                stopDragging();
            }
        }
    }
}

void InventoryScene::update(float deltaTime) {
    // Обновление подсвеченной ячейки при движении мыши
    updateHoveredCell();
}

void InventoryScene::updateHoveredCell() {
    // Преобразование позиции мыши в координаты сетки
    int gridX, gridY;
    screenToGrid(m_mousePosition.x, m_mousePosition.y, gridX, gridY);
    
    // Проверка, находится ли мышь в границах сетки
    if (gridX >= 0 && gridX < m_gridInventory->getGridWidth() &&
        gridY >= 0 && gridY < m_gridInventory->getGridHeight()) {
        m_hoveredCellX = gridX;
        m_hoveredCellY = gridY;
    } else {
        m_hoveredCellX = -1;
        m_hoveredCellY = -1;
    }
}

void InventoryScene::handleLeftClick() {
    if (m_isDragging) {
        // Попытка разместить предмет
        if (m_hoveredCellX >= 0 && m_hoveredCellY >= 0) {
            if (m_gridInventory->placeItem(m_draggedItemId, m_hoveredCellX, m_hoveredCellY)) {
                // Успешное размещение
                m_isDragging = false;
                m_draggedItemId = -1;
                updateStats();
            }
        }
    } else {
        // Попытка поднять предмет
        if (m_hoveredCellX >= 0 && m_hoveredCellY >= 0) {
            GridItem* item = m_gridInventory->getItemAtCell(m_hoveredCellX, m_hoveredCellY);
            if (item) {
                startDragging(item->id);
            }
        }
    }
}

void InventoryScene::handleRightClick() {
    // Поворот предмета
    if (m_isDragging) {
        m_gridInventory->rotateItem(m_draggedItemId);
    } else if (m_hoveredCellX >= 0 && m_hoveredCellY >= 0) {
        GridItem* item = m_gridInventory->getItemAtCell(m_hoveredCellX, m_hoveredCellY);
        if (item) {
            m_gridInventory->rotateItem(item->id);
        }
    }
}

void InventoryScene::startDragging(int itemId) {
    m_isDragging = true;
    m_draggedItemId = itemId;
}

void InventoryScene::stopDragging() {
    if (!m_isDragging) return;
    
    // Попытка разместить предмет на текущей позиции
    if (m_hoveredCellX >= 0 && m_hoveredCellY >= 0) {
        if (!m_gridInventory->placeItem(m_draggedItemId, m_hoveredCellX, m_hoveredCellY)) {
            // Если не удалось разместить, предмет остается на старом месте
            // (GridInventorySystem автоматически управляет этим)
        }
    }
    
    m_isDragging = false;
    m_draggedItemId = -1;
    updateStats();
}

void InventoryScene::updateStats() {
    float currentWeight = m_gridInventory->getTotalWeight();
    float maxWeight = m_gridInventory->getMaxWeight();
    
    std::ostringstream ss;
    ss << "Weight: " << std::fixed << std::setprecision(1) << currentWeight 
       << " / " << maxWeight << " kg";
    m_statsText.setString(ss.str());
}

void InventoryScene::render(sf::RenderWindow& window) {
    // Рендер фона
    window.draw(m_backgroundBox);
    
    // Рендер панелей
    window.draw(m_gridContainer);
    window.draw(m_detailPanel);
    window.draw(m_statsPanel);
    window.draw(m_closeButton);
    
    // Рендер текстов
    window.draw(m_titleText);
    window.draw(m_closeButtonText);
    window.draw(m_statsText);
    window.draw(m_detailText);
    window.draw(m_controlsText);
    
    // Рендер сетки
    renderGrid(window);
    
    // Рендер предметов
    renderItems(window);
    
    // Рендер перетаскиваемого предмета
    if (m_isDragging) {
        renderDraggedItem(window);
    }
    
    // Рендер деталей предмета
    renderItemDetails(window);
}

void InventoryScene::renderGrid(sf::RenderWindow& window) {
    int gridWidth = m_gridInventory->getGridWidth();
    int gridHeight = m_gridInventory->getGridHeight();
    
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int index = y * gridWidth + x;
            
            // Проверка, занята ли ячейка
            const GridItem* item = m_gridInventory->getItemAtCell(x, y);
            
            if (x == m_hoveredCellX && y == m_hoveredCellY) {
                // Подсвеченная ячейка
                if (m_isDragging) {
                    // Проверка, можно ли разместить здесь
                    const GridItem* draggedItem = m_gridInventory->getItemById(m_draggedItemId);
                    if (draggedItem) {
                        int width = draggedItem->isRotated ? draggedItem->gridHeight : draggedItem->gridWidth;
                        int height = draggedItem->isRotated ? draggedItem->gridWidth : draggedItem->gridHeight;
                        
                        if (m_gridInventory->canPlaceItem(x, y, width, height, m_draggedItemId)) {
                            m_gridCells[index].setFillColor(sf::Color(76, 175, 80, 100)); // Green
                        } else {
                            m_gridCells[index].setFillColor(sf::Color(244, 67, 54, 100)); // Red
                        }
                    }
                } else {
                    m_gridCells[index].setFillColor(sf::Color(94, 126, 160, 100)); // Light blue
                }
            } else if (item && !m_isDragging) {
                m_gridCells[index].setFillColor(sf::Color(62, 62, 78)); // Medium gray
            } else {
                m_gridCells[index].setFillColor(sf::Color(40, 40, 40)); // Dark gray
            }
            
            window.draw(m_gridCells[index]);
        }
    }
}

void InventoryScene::renderItems(sf::RenderWindow& window) {
    const std::vector<GridItem>& items = m_gridInventory->getItems();
    
    for (const GridItem& gridItem : items) {
        // Не рисуем предмет, который перетаскивается
        if (m_isDragging && gridItem.id == m_draggedItemId) {
            continue;
        }
        
        // Вычисление размеров с учетом поворота
        int width = gridItem.isRotated ? gridItem.gridHeight : gridItem.gridWidth;
        int height = gridItem.isRotated ? gridItem.gridWidth : gridItem.gridHeight;
        
        float screenX, screenY;
        gridToScreen(gridItem.gridX, gridItem.gridY, screenX, screenY);
        
        // Рендер прямоугольника предмета
        sf::RectangleShape itemRect(sf::Vector2f(
            width * m_cellSize - 4.0f,
            height * m_cellSize - 4.0f
        ));
        itemRect.setPosition(screenX + 2.0f, screenY + 2.0f);
        
        // Цвет в зависимости от размера
        if (width == 1 && height == 1) {
            itemRect.setFillColor(sf::Color(76, 175, 80, 200)); // Green - small
        } else if (width == 2 && height == 1) {
            itemRect.setFillColor(sf::Color(255, 193, 7, 200)); // Yellow - medium
        } else if (width == 1 && height == 2) {
            itemRect.setFillColor(sf::Color(255, 193, 7, 200)); // Yellow - medium
        } else if (width == 2 && height == 2) {
            itemRect.setFillColor(sf::Color(255, 152, 0, 200)); // Orange - large
        } else {
            itemRect.setFillColor(sf::Color(244, 67, 54, 200)); // Red - very large
        }
        
        itemRect.setOutlineColor(sf::Color::White);
        itemRect.setOutlineThickness(2.0f);
        window.draw(itemRect);
        
        // Рендер имени предмета (если помещается)
        sf::Text itemName;
        itemName.setFont(m_font);
        itemName.setString(gridItem.item.name);
        itemName.setCharacterSize(12);
        itemName.setFillColor(sf::Color::White);
        itemName.setPosition(screenX + 5.0f, screenY + 5.0f);
        window.draw(itemName);
    }
}

void InventoryScene::renderDraggedItem(sf::RenderWindow& window) {
    const GridItem* draggedItem = m_gridInventory->getItemById(m_draggedItemId);
    if (!draggedItem) return;
    
    // Вычисление размеров с учетом поворота
    int width = draggedItem->isRotated ? draggedItem->gridHeight : draggedItem->gridWidth;
    int height = draggedItem->isRotated ? draggedItem->gridWidth : draggedItem->gridHeight;
    
    // Рендер предмета на позиции мыши
    sf::RectangleShape itemRect(sf::Vector2f(
        width * m_cellSize - 4.0f,
        height * m_cellSize - 4.0f
    ));
    
    // Центрирование на мыши
    itemRect.setPosition(
        m_mousePosition.x - (width * m_cellSize) / 2.0f,
        m_mousePosition.y - (height * m_cellSize) / 2.0f
    );
    
    // Полупрозрачный
    itemRect.setFillColor(sf::Color(255, 255, 255, 150));
    itemRect.setOutlineColor(sf::Color::White);
    itemRect.setOutlineThickness(2.0f);
    window.draw(itemRect);
}

void InventoryScene::renderItemDetails(sf::RenderWindow& window) {
    // Получение предмета под курсором
    const GridItem* hoveredItem = nullptr;
    if (m_hoveredCellX >= 0 && m_hoveredCellY >= 0) {
        hoveredItem = m_gridInventory->getItemAtCell(m_hoveredCellX, m_hoveredCellY);
    }
    
    if (hoveredItem) {
        std::ostringstream ss;
        ss << "Item Details:\n\n";
        ss << "Name:\n" << hoveredItem->item.name << "\n\n";
        ss << "Weight:\n" << std::fixed << std::setprecision(1) << hoveredItem->item.weight << " kg\n\n";
        ss << "Value:\n" << static_cast<int>(hoveredItem->item.value) << " RUB\n\n";
        ss << "Size:\n" << hoveredItem->gridWidth << "x" << hoveredItem->gridHeight << " cells";
        if (hoveredItem->isRotated) {
            ss << "\n(Rotated)";
        }
        
        m_detailText.setString(ss.str());
    } else {
        m_detailText.setString("Select an item\nto view details");
    }
}

bool InventoryScene::isMouseInInventoryBounds(int mouseX, int mouseY) const {
    float gridEndX = m_gridStartX + m_gridInventory->getGridWidth() * m_cellSize;
    float gridEndY = m_gridStartY + m_gridInventory->getGridHeight() * m_cellSize;
    
    return mouseX >= m_gridStartX && mouseX <= gridEndX &&
           mouseY >= m_gridStartY && mouseY <= gridEndY;
}

void InventoryScene::screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const {
    gridX = static_cast<int>((screenX - m_gridStartX) / m_cellSize);
    gridY = static_cast<int>((screenY - m_gridStartY) / m_cellSize);
    
    // Проверка границ
    if (gridX < 0 || gridX >= m_gridInventory->getGridWidth() ||
        gridY < 0 || gridY >= m_gridInventory->getGridHeight()) {
        gridX = -1;
        gridY = -1;
    }
}

void InventoryScene::gridToScreen(int gridX, int gridY, float& screenX, float& screenY) const {
    screenX = m_gridStartX + gridX * m_cellSize;
    screenY = m_gridStartY + gridY * m_cellSize;
}

SceneType InventoryScene::getNextScene() const {
    return m_nextScene;
}

bool InventoryScene::isFinished() const {
    return m_isFinished;
}
