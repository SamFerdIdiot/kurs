#include "GridInventorySystem.h"
#include <algorithm>

// Конструктор
GridInventorySystem::GridInventorySystem(int gridWidth, int gridHeight, float maxWeight)
    : m_gridWidth(gridWidth),
      m_gridHeight(gridHeight),
      m_maxWeight(maxWeight),
      m_nextItemId(1) {
    
    // Инициализация сетки
    m_grid.resize(gridHeight);
    for (int y = 0; y < gridHeight; ++y) {
        m_grid[y].resize(gridWidth);
    }
}

// Добавление предмета
int GridInventorySystem::addItem(const Item& item, int width, int height, int gridX, int gridY, int count) {
    // Проверка веса
    if (getTotalWeight() + item.weight * count > m_maxWeight) {
        return -1; // Превышен лимит веса
    }
    
    // Автопоиск позиции если не указана
    if (gridX == -1 || gridY == -1) {
        if (!findAvailablePosition(width, height, gridX, gridY)) {
            return -1; // Нет свободного места
        }
    }
    
    // Проверка возможности размещения
    if (!canPlaceItem(gridX, gridY, width, height, -1)) {
        return -1;
    }
    
    // Создание предмета
    GridItem gridItem(item, m_nextItemId, width, height);
    gridItem.gridX = gridX;
    gridItem.gridY = gridY;
    gridItem.stackCount = count;
    
    // Занятие ячеек
    occupyCells(m_nextItemId, gridX, gridY, width, height);
    
    // Добавление в список
    m_items.push_back(gridItem);
    
    return m_nextItemId++;
}

// Проверка возможности размещения
bool GridInventorySystem::canPlaceItem(int gridX, int gridY, int width, int height, int ignoreItemId) const {
    // Проверка границ
    if (gridX < 0 || gridY < 0 || gridX + width > m_gridWidth || gridY + height > m_gridHeight) {
        return false;
    }
    
    // Проверка занятости ячеек
    for (int y = gridY; y < gridY + height; ++y) {
        for (int x = gridX; x < gridX + width; ++x) {
            if (m_grid[y][x].occupied && m_grid[y][x].itemId != ignoreItemId) {
                return false;
            }
        }
    }
    
    return true;
}

// Размещение предмета
bool GridInventorySystem::placeItem(int itemId, int gridX, int gridY) {
    // Поиск предмета
    GridItem* item = getItemById(itemId);
    if (!item) {
        return false;
    }
    
    // Получение размеров
    int width, height;
    getActualDimensions(*item, width, height);
    
    // Проверка возможности размещения
    if (!canPlaceItem(gridX, gridY, width, height, itemId)) {
        return false;
    }
    
    // Очистка старых ячеек
    clearCells(itemId);
    
    // Установка новой позиции
    item->gridX = gridX;
    item->gridY = gridY;
    
    // Занятие новых ячеек
    occupyCells(itemId, gridX, gridY, width, height);
    
    return true;
}

// Удаление предмета
bool GridInventorySystem::removeItem(int itemId) {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [itemId](const GridItem& item) { return item.id == itemId; });
    
    if (it == m_items.end()) {
        return false;
    }
    
    // Очистка ячеек
    clearCells(itemId);
    
    // Удаление из списка
    m_items.erase(it);
    
    return true;
}

// Получение предмета в ячейке
GridItem* GridInventorySystem::getItemAtCell(int gridX, int gridY) {
    if (!isValidPosition(gridX, gridY)) {
        return nullptr;
    }
    
    int itemId = m_grid[gridY][gridX].itemId;
    if (itemId == -1) {
        return nullptr;
    }
    
    return getItemById(itemId);
}

const GridItem* GridInventorySystem::getItemAtCell(int gridX, int gridY) const {
    if (!isValidPosition(gridX, gridY)) {
        return nullptr;
    }
    
    int itemId = m_grid[gridY][gridX].itemId;
    if (itemId == -1) {
        return nullptr;
    }
    
    return getItemById(itemId);
}

// Получение предмета по ID
GridItem* GridInventorySystem::getItemById(int itemId) {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [itemId](const GridItem& item) { return item.id == itemId; });
    
    return (it != m_items.end()) ? &(*it) : nullptr;
}

const GridItem* GridInventorySystem::getItemById(int itemId) const {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [itemId](const GridItem& item) { return item.id == itemId; });
    
    return (it != m_items.end()) ? &(*it) : nullptr;
}

// Поворот предмета
bool GridInventorySystem::rotateItem(int itemId) {
    GridItem* item = getItemById(itemId);
    if (!item) {
        return false;
    }
    
    // Переключение поворота
    item->isRotated = !item->isRotated;
    
    // Получение новых размеров
    int width, height;
    getActualDimensions(*item, width, height);
    
    // Проверка возможности размещения с новыми размерами
    if (!canPlaceItem(item->gridX, item->gridY, width, height, itemId)) {
        // Откат поворота
        item->isRotated = !item->isRotated;
        return false;
    }
    
    // Обновление ячеек
    clearCells(itemId);
    occupyCells(itemId, item->gridX, item->gridY, width, height);
    
    return true;
}

// Поиск свободного места
bool GridInventorySystem::findAvailablePosition(int width, int height, int& outX, int& outY) const {
    for (int y = 0; y <= m_gridHeight - height; ++y) {
        for (int x = 0; x <= m_gridWidth - width; ++x) {
            if (canPlaceItem(x, y, width, height, -1)) {
                outX = x;
                outY = y;
                return true;
            }
        }
    }
    return false;
}

// Получение общего веса
float GridInventorySystem::getTotalWeight() const {
    float totalWeight = 0.0f;
    for (const auto& item : m_items) {
        totalWeight += item.item.weight * item.stackCount;
    }
    return totalWeight;
}

// Получение максимального веса
float GridInventorySystem::getMaxWeight() const {
    return m_maxWeight;
}

// Установка максимального веса
void GridInventorySystem::setMaxWeight(float maxWeight) {
    m_maxWeight = maxWeight;
}

// Проверка наличия предмета
bool GridInventorySystem::hasItem(const std::string& itemName, int minCount) const {
    return getItemCount(itemName) >= minCount;
}

// Подсчёт предметов
int GridInventorySystem::getItemCount(const std::string& itemName) const {
    int count = 0;
    for (const auto& item : m_items) {
        if (item.item.name == itemName) {
            count += item.stackCount;
        }
    }
    return count;
}

// Очистка инвентаря
void GridInventorySystem::clear() {
    m_items.clear();
    for (int y = 0; y < m_gridHeight; ++y) {
        for (int x = 0; x < m_gridWidth; ++x) {
            m_grid[y][x].occupied = false;
            m_grid[y][x].itemId = -1;
        }
    }
}

// Удаление предмета (drop)
bool GridInventorySystem::dropItem(int itemId) {
    return removeItem(itemId);
}

// Проверка валидности позиции
bool GridInventorySystem::isValidPosition(int gridX, int gridY) const {
    return gridX >= 0 && gridX < m_gridWidth && gridY >= 0 && gridY < m_gridHeight;
}

// Занятие ячеек
void GridInventorySystem::occupyCells(int itemId, int gridX, int gridY, int width, int height) {
    for (int y = gridY; y < gridY + height; ++y) {
        for (int x = gridX; x < gridX + width; ++x) {
            m_grid[y][x].occupied = true;
            m_grid[y][x].itemId = itemId;
        }
    }
}

// Очистка ячеек
void GridInventorySystem::clearCells(int itemId) {
    for (int y = 0; y < m_gridHeight; ++y) {
        for (int x = 0; x < m_gridWidth; ++x) {
            if (m_grid[y][x].itemId == itemId) {
                m_grid[y][x].occupied = false;
                m_grid[y][x].itemId = -1;
            }
        }
    }
}

// Получение фактических размеров с учётом поворота
void GridInventorySystem::getActualDimensions(const GridItem& item, int& width, int& height) const {
    if (item.isRotated) {
        width = item.gridHeight;
        height = item.gridWidth;
    } else {
        width = item.gridWidth;
        height = item.gridHeight;
    }
}
