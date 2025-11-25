#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

// ============================================================================
// RESPONSIVE UI LAYOUT SYSTEM
// Адаптивная система разметки для разных разрешений экрана
// ============================================================================

// Константы для разных разрешений экрана
namespace ScreenResolution {
    // MacBook Air M1 (основное разрешение)
    constexpr int MAC_AIR_M1_WIDTH = 1440;
    constexpr int MAC_AIR_M1_HEIGHT = 900;
    
    // Стандартное разрешение игры (базовое)
    constexpr int DEFAULT_WIDTH = 800;
    constexpr int DEFAULT_HEIGHT = 600;
    
    // Альтернативные разрешения
    constexpr int HD_WIDTH = 1280;
    constexpr int HD_HEIGHT = 720;
    
    constexpr int FULL_HD_WIDTH = 1920;
    constexpr int FULL_HD_HEIGHT = 1080;
}

// Типы выравнивания элементов
enum class Alignment {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

// Структура для хранения относительных координат (в процентах)
struct RelativePosition {
    float xPercent;  // 0.0 - 1.0 (0% - 100%)
    float yPercent;  // 0.0 - 1.0 (0% - 100%)
    
    RelativePosition(float x = 0.0f, float y = 0.0f) 
        : xPercent(x), yPercent(y) {}
};

// Структура для хранения отступов
struct Padding {
    float top;
    float right;
    float bottom;
    float left;
    
    Padding(float all = 0.0f) 
        : top(all), right(all), bottom(all), left(all) {}
    
    Padding(float vertical, float horizontal)
        : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    
    Padding(float t, float r, float b, float l)
        : top(t), right(r), bottom(b), left(l) {}
};

// Класс для управления адаптивной разметкой UI
class UILayout {
public:
    // Конструктор с указанием базового и целевого разрешения
    UILayout(int baseWidth = ScreenResolution::DEFAULT_WIDTH, 
             int baseHeight = ScreenResolution::DEFAULT_HEIGHT,
             int targetWidth = ScreenResolution::MAC_AIR_M1_WIDTH,
             int targetHeight = ScreenResolution::MAC_AIR_M1_HEIGHT)
        : m_baseWidth(baseWidth),
          m_baseHeight(baseHeight),
          m_targetWidth(targetWidth),
          m_targetHeight(targetHeight) {
        calculateScaleFactors();
    }
    
    // Установка целевого разрешения
    void setTargetResolution(int width, int height) {
        m_targetWidth = width;
        m_targetHeight = height;
        calculateScaleFactors();
    }
    
    // Получение масштабированных координат
    sf::Vector2f scale(float x, float y) const {
        return sf::Vector2f(x * m_scaleX, y * m_scaleY);
    }
    
    sf::Vector2f scale(const sf::Vector2f& pos) const {
        return scale(pos.x, pos.y);
    }
    
    // Преобразование относительных координат в абсолютные
    sf::Vector2f fromRelative(const RelativePosition& relPos) const {
        return sf::Vector2f(
            relPos.xPercent * m_targetWidth,
            relPos.yPercent * m_targetHeight
        );
    }
    
    // Преобразование абсолютных координат в относительные
    RelativePosition toRelative(float x, float y) const {
        return RelativePosition(
            x / m_targetWidth,
            y / m_targetHeight
        );
    }
    
    // Масштабирование размера
    sf::Vector2f scaleSize(float width, float height) const {
        return sf::Vector2f(width * m_scaleX, height * m_scaleY);
    }
    
    // Масштабирование шрифта
    unsigned int scaleFont(unsigned int baseFontSize) const {
        return static_cast<unsigned int>(baseFontSize * std::min(m_scaleX, m_scaleY));
    }
    
    // Получение позиции с учетом выравнивания
    sf::Vector2f getAlignedPosition(
        float width, 
        float height, 
        Alignment alignment,
        const Padding& padding = Padding()
    ) const {
        sf::Vector2f pos(0.0f, 0.0f);
        
        switch (alignment) {
            case Alignment::TOP_LEFT:
                pos.x = padding.left;
                pos.y = padding.top;
                break;
            case Alignment::TOP_CENTER:
                pos.x = (m_targetWidth - width) / 2.0f;
                pos.y = padding.top;
                break;
            case Alignment::TOP_RIGHT:
                pos.x = m_targetWidth - width - padding.right;
                pos.y = padding.top;
                break;
            case Alignment::CENTER_LEFT:
                pos.x = padding.left;
                pos.y = (m_targetHeight - height) / 2.0f;
                break;
            case Alignment::CENTER:
                pos.x = (m_targetWidth - width) / 2.0f;
                pos.y = (m_targetHeight - height) / 2.0f;
                break;
            case Alignment::CENTER_RIGHT:
                pos.x = m_targetWidth - width - padding.right;
                pos.y = (m_targetHeight - height) / 2.0f;
                break;
            case Alignment::BOTTOM_LEFT:
                pos.x = padding.left;
                pos.y = m_targetHeight - height - padding.bottom;
                break;
            case Alignment::BOTTOM_CENTER:
                pos.x = (m_targetWidth - width) / 2.0f;
                pos.y = m_targetHeight - height - padding.bottom;
                break;
            case Alignment::BOTTOM_RIGHT:
                pos.x = m_targetWidth - width - padding.right;
                pos.y = m_targetHeight - height - padding.bottom;
                break;
        }
        
        return pos;
    }
    
    // Создание сетки с указанным количеством ячеек
    struct GridCell {
        sf::Vector2f position;
        sf::Vector2f size;
    };
    
    std::vector<GridCell> createGrid(
        int rows, 
        int cols,
        const Padding& padding = Padding(10.0f),
        float spacing = 5.0f
    ) const {
        std::vector<GridCell> grid;
        
        float availableWidth = m_targetWidth - padding.left - padding.right - (cols - 1) * spacing;
        float availableHeight = m_targetHeight - padding.top - padding.bottom - (rows - 1) * spacing;
        
        float cellWidth = availableWidth / cols;
        float cellHeight = availableHeight / rows;
        
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                GridCell cell;
                cell.position.x = padding.left + col * (cellWidth + spacing);
                cell.position.y = padding.top + row * (cellHeight + spacing);
                cell.size.x = cellWidth;
                cell.size.y = cellHeight;
                grid.push_back(cell);
            }
        }
        
        return grid;
    }
    
    // Получение размеров экрана
    int getTargetWidth() const { return m_targetWidth; }
    int getTargetHeight() const { return m_targetHeight; }
    float getScaleX() const { return m_scaleX; }
    float getScaleY() const { return m_scaleY; }
    
private:
    int m_baseWidth;
    int m_baseHeight;
    int m_targetWidth;
    int m_targetHeight;
    float m_scaleX;
    float m_scaleY;
    
    void calculateScaleFactors() {
        m_scaleX = static_cast<float>(m_targetWidth) / m_baseWidth;
        m_scaleY = static_cast<float>(m_targetHeight) / m_baseHeight;
    }
};

// Менеджер для предустановленных разметок
class LayoutPresets {
public:
    // Получить singleton instance
    static LayoutPresets& getInstance() {
        static LayoutPresets instance;
        return instance;
    }
    
    // Получить разметку для конкретной сцены
    UILayout& getLayout(const std::string& sceneName) {
        auto it = m_layouts.find(sceneName);
        if (it != m_layouts.end()) {
            return it->second;
        }
        
        // Если разметка не найдена, создаем новую с дефолтными параметрами
        m_layouts[sceneName] = UILayout();
        return m_layouts[sceneName];
    }
    
    // Установить глобальное целевое разрешение для всех сцен
    void setGlobalTargetResolution(int width, int height) {
        for (auto& pair : m_layouts) {
            pair.second.setTargetResolution(width, height);
        }
        m_globalWidth = width;
        m_globalHeight = height;
    }
    
    // Инициализация предустановок для MacBook Air M1
    void initializeMacAirM1Presets() {
        setGlobalTargetResolution(
            ScreenResolution::MAC_AIR_M1_WIDTH,
            ScreenResolution::MAC_AIR_M1_HEIGHT
        );
    }
    
    int getGlobalWidth() const { return m_globalWidth; }
    int getGlobalHeight() const { return m_globalHeight; }
    
private:
    LayoutPresets() 
        : m_globalWidth(ScreenResolution::DEFAULT_WIDTH),
          m_globalHeight(ScreenResolution::DEFAULT_HEIGHT) {}
    
    std::unordered_map<std::string, UILayout> m_layouts;
    int m_globalWidth;
    int m_globalHeight;
};

// Удобные макросы для использования в коде
#define GET_LAYOUT(scene_name) LayoutPresets::getInstance().getLayout(scene_name)
#define SCALE_POS(layout, x, y) layout.scale(x, y)
#define SCALE_SIZE(layout, w, h) layout.scaleSize(w, h)
#define SCALE_FONT(layout, size) layout.scaleFont(size)

#endif // UI_LAYOUT_H
