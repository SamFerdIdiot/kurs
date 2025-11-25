#ifndef HUDPANEL_HPP
#define HUDPANEL_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "UI/ResourceBar.h"

// HUDPanel - Retro driving game style HUD panel (Jalopy-inspired)
// 
// This class implements TWO HUD displays:
// 1. Bottom-aligned panel with three main sections (existing):
//    - Left: Energy/Status indicators
//    - Center: Odometer and LCD screen
//    - Right: Car indicators (fuel, temperature, etc.)
//
// 2. Top-right resource display (Phase 4 addition):
//    - Money: 💰 1,250₽ (yellow text)
//    - Fuel: ⛽ 45.5/100L (orange text, with progress bar)
//    - Energy: ⚡ 70/100 (cyan text, with progress bar)
//    - Cargo: 📦 25/64 (white text)
//
// The HUD uses pixel/bitmap fonts and retro styling with rectangles and text.

class HUDPanel {
public:
    // HUD data structure for updating displayed values
    struct HUDData {
        float speed;           // Current speed in km/h
        float fuel;            // Fuel level (0-100%)
        float energy;          // Energy level (0-100%)
        float money;           // Money amount
        float temperature;     // Engine temperature (0-100%)
        float odometer;        // Total distance traveled in km
        std::string lcdText;   // Text to display on LCD screen
        bool engineWarning;    // Engine warning indicator
        bool fuelWarning;      // Low fuel warning
        bool checkEngine;      // Check engine light
        float inventoryWeight; // Current inventory weight
        float maxInventoryWeight; // Maximum inventory weight capacity
        
        HUDData() 
            : speed(0.0f), fuel(100.0f), energy(100.0f), money(0.0f),
              temperature(50.0f), odometer(0.0f), lcdText("READY"),
              engineWarning(false), fuelWarning(false), checkEngine(false),
              inventoryWeight(0.0f), maxInventoryWeight(100.0f) {}
    };

    // Constructor
    // windowWidth, windowHeight - dimensions of the game window
    HUDPanel(float windowWidth, float windowHeight);
    
    // Update HUD with new data
    void update(const HUDData& data);
    
    // Render the HUD panel
    void render(sf::RenderWindow& window);
    
    // Set custom font (should be pixel/bitmap font)
    bool setFont(const std::string& fontPath);
    
    // === Phase 4: Top-right resource display methods ===
    
    // Update individual resources for top-right display
    void setMoney(float amount);
    void setFuel(float current, float max);
    void setEnergy(float current, float max);
    void setCargo(float current, float max);
    
    // Enable/disable top-right display (default: enabled)
    void setTopRightDisplayEnabled(bool enabled);
    
    // Update animation for resource bars (call every frame)
    void updateTopRight(float deltaTime);
    
    // Get current resource values
    float getMoney() const { return m_topRightMoney; }
    float getFuel() const { return m_topRightFuelCurrent; }
    float getEnergy() const { return m_topRightEnergyCurrent; }
    float getCargo() const { return m_topRightCargoCurrent; }
    
private:
    // Window dimensions
    float m_windowWidth;
    float m_windowHeight;
    
    // HUD data
    HUDData m_data;
    
    // Font for text rendering
    sf::Font m_font;
    bool m_fontLoaded;
    
    // === Main HUD background ===
    sf::RectangleShape m_hudBackground;
    
    // === Left section: Energy/Status ===
    sf::RectangleShape m_energyPanel;
    sf::RectangleShape m_energyBar;
    sf::Text m_energyText;
    sf::Text m_energyValue;
    
    sf::RectangleShape m_moneyPanel;
    sf::Text m_moneyText;
    sf::Text m_moneyValue;
    
    sf::RectangleShape m_inventoryPanel;
    sf::Text m_inventoryText;
    sf::Text m_inventoryValue;
    
    // === Center section: Odometer and LCD ===
    sf::RectangleShape m_odometerPanel;
    sf::Text m_odometerText;
    sf::Text m_odometerValue;
    
    sf::RectangleShape m_lcdPanel;
    sf::Text m_lcdText;
    
    sf::RectangleShape m_speedPanel;
    sf::Text m_speedText;
    sf::Text m_speedValue;
    
    // === Right section: Car indicators ===
    sf::RectangleShape m_fuelPanel;
    sf::RectangleShape m_fuelBar;
    sf::Text m_fuelText;
    sf::Text m_fuelValue;
    
    sf::RectangleShape m_tempPanel;
    sf::RectangleShape m_tempBar;
    sf::Text m_tempText;
    sf::Text m_tempValue;
    
    // Warning indicators
    sf::RectangleShape m_warningEngine;
    sf::Text m_warningEngineText;
    
    sf::RectangleShape m_warningFuel;
    sf::Text m_warningFuelText;
    
    sf::RectangleShape m_warningCheck;
    sf::Text m_warningCheckText;
    
    // Helper functions
    void initializeLayout();
    void initializeLeftSection();
    void initializeCenterSection();
    void initializeRightSection();
    void updateBars();
    
    // === Phase 4: Top-right resource display ===
    bool m_topRightEnabled;
    
    // Resource values
    float m_topRightMoney;
    float m_topRightFuelCurrent;
    float m_topRightFuelMax;
    float m_topRightEnergyCurrent;
    float m_topRightEnergyMax;
    float m_topRightCargoCurrent;
    float m_topRightCargoMax;
    
    // Resource bars
    ResourceBar m_topRightFuelBar;
    ResourceBar m_topRightEnergyBar;
    
    // Text elements
    sf::Text m_topRightMoneyText;
    sf::Text m_topRightFuelText;
    sf::Text m_topRightEnergyText;
    sf::Text m_topRightCargoText;
    
    // Helper functions for top-right display
    void initializeTopRightDisplay();
    void updateTopRightDisplay();
    void renderTopRightDisplay(sf::RenderWindow& window);
};

#endif // HUDPANEL_HPP
