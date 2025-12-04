#include "UI/HUDPanel.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

HUDPanel::HUDPanel(float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth),
      m_windowHeight(windowHeight),
      m_fontLoaded(false),
      m_topRightEnabled(true),
      m_topRightMoney(0.0f),
      m_topRightFuelCurrent(100.0f),
      m_topRightFuelMax(100.0f),
      m_topRightEnergyCurrent(100.0f),
      m_topRightEnergyMax(100.0f),
      m_topRightCargoCurrent(0.0f),
      m_topRightCargoMax(100.0f),
      m_topRightFuelBar(sf::Vector2f(700.0f, 50.0f), sf::Vector2f(100.0f, 10.0f)),
      m_topRightEnergyBar(sf::Vector2f(700.0f, 85.0f), sf::Vector2f(100.0f, 10.0f)) {
    
    // Try to load default font (SFML 3.x uses openFromFile)
    m_fontLoaded = m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

    if (!m_fontLoaded) {
        std::cerr << "Warning: Could not load HUD font" << std::endl;
    }
    
    initializeLayout();
    initializeTopRightDisplay();
}

void HUDPanel::initializeLayout() {
    // Main HUD background - bottom aligned, spans full width
    float hudHeight = 120.f;
    m_hudBackground.setSize(sf::Vector2f(m_windowWidth, hudHeight));
    m_hudBackground.setPosition(sf::Vector2f(0.f, m_windowHeight - hudHeight));
    m_hudBackground.setFillColor(sf::Color(30, 30, 35, 230));
    m_hudBackground.setOutlineColor(sf::Color(80, 80, 90));
    m_hudBackground.setOutlineThickness(2.f);

    initializeLeftSection();
    initializeCenterSection();
    initializeRightSection();
}

void HUDPanel::initializeLeftSection() {
    float hudTop = m_windowHeight - 120.f;
    float leftX = 20.f;

    // Energy panel
    m_energyPanel.setSize(sf::Vector2f(180.f, 30.f));
    m_energyPanel.setPosition(sf::Vector2f(leftX, hudTop + 10.f));
    m_energyPanel.setFillColor(sf::Color(40, 40, 50));
    m_energyPanel.setOutlineColor(sf::Color(60, 60, 70));
    m_energyPanel.setOutlineThickness(1.f);

    m_energyBar.setSize(sf::Vector2f(170.f, 20.f));
    m_energyBar.setPosition(sf::Vector2f(leftX + 5.f, hudTop + 15.f));
    m_energyBar.setFillColor(sf::Color(100, 200, 100));

    if (m_fontLoaded) {
        m_energyText.emplace(m_font, "ЭНЕРГИЯ / ENERGY", 12);
        m_energyText->setFillColor(sf::Color::White);
        m_energyText->setPosition(sf::Vector2f(leftX, hudTop - 5.f));

        m_energyValue.emplace(m_font, "100%", 14);
        m_energyValue->setFillColor(sf::Color(150, 255, 150));
        m_energyValue->setPosition(sf::Vector2f(leftX + 5.f, hudTop + 12.f));
    }

    // Money panel
    m_moneyPanel.setSize(sf::Vector2f(180.f, 25.f));
    m_moneyPanel.setPosition(sf::Vector2f(leftX, hudTop + 50.f));
    m_moneyPanel.setFillColor(sf::Color(50, 50, 40));
    m_moneyPanel.setOutlineColor(sf::Color(100, 100, 80));
    m_moneyPanel.setOutlineThickness(1.f);

    if (m_fontLoaded) {
        m_moneyText.emplace(m_font, "ДЕНЬГИ:", 12);
        m_moneyText->setFillColor(sf::Color(200, 200, 150));
        m_moneyText->setPosition(sf::Vector2f(leftX + 5.f, hudTop + 52.f));

        m_moneyValue.emplace(m_font, "0 ₽", 14);
        m_moneyValue->setFillColor(sf::Color(255, 255, 150));
        m_moneyValue->setPosition(sf::Vector2f(leftX + 80.f, hudTop + 52.f));
    }

    // Inventory panel
    m_inventoryPanel.setSize(sf::Vector2f(180.f, 25.f));
    m_inventoryPanel.setPosition(sf::Vector2f(leftX, hudTop + 85.f));
    m_inventoryPanel.setFillColor(sf::Color(40, 50, 50));
    m_inventoryPanel.setOutlineColor(sf::Color(80, 100, 100));
    m_inventoryPanel.setOutlineThickness(1.f);

    if (m_fontLoaded) {
        m_inventoryText.emplace(m_font, "ГРУЗ:", 12);
        m_inventoryText->setFillColor(sf::Color(150, 200, 200));
        m_inventoryText->setPosition(sf::Vector2f(leftX + 5.f, hudTop + 87.f));

        m_inventoryValue.emplace(m_font, "0/100", 14);
        m_inventoryValue->setFillColor(sf::Color(200, 255, 255));
        m_inventoryValue->setPosition(sf::Vector2f(leftX + 60.f, hudTop + 87.f));
    }
}

void HUDPanel::initializeCenterSection() {
    float hudTop = m_windowHeight - 120.f;
    float centerX = m_windowWidth / 2.f - 100.f;

    // Speed panel
    m_speedPanel.setSize(sf::Vector2f(120.f, 40.f));
    m_speedPanel.setPosition(sf::Vector2f(centerX, hudTop + 10.f));
    m_speedPanel.setFillColor(sf::Color(50, 50, 60));
    m_speedPanel.setOutlineColor(sf::Color(100, 100, 120));
    m_speedPanel.setOutlineThickness(2.f);

    if (m_fontLoaded) {
        m_speedText.emplace(m_font, "СКОРОСТЬ", 10);
        m_speedText->setFillColor(sf::Color(150, 150, 150));
        m_speedText->setPosition(sf::Vector2f(centerX + 30.f, hudTop + 12.f));

        m_speedValue.emplace(m_font, "0", 20);
        m_speedValue->setFillColor(sf::Color::White);
        m_speedValue->setPosition(sf::Vector2f(centerX + 40.f, hudTop + 23.f));
    }

    // Odometer panel
    m_odometerPanel.setSize(sf::Vector2f(120.f, 25.f));
    m_odometerPanel.setPosition(sf::Vector2f(centerX, hudTop + 55.f));
    m_odometerPanel.setFillColor(sf::Color(40, 50, 40));
    m_odometerPanel.setOutlineColor(sf::Color(80, 100, 80));
    m_odometerPanel.setOutlineThickness(1.f);

    if (m_fontLoaded) {
        m_odometerText.emplace(m_font, "ПРОБЕГ:", 10);
        m_odometerText->setFillColor(sf::Color(150, 200, 150));
        m_odometerText->setPosition(sf::Vector2f(centerX + 5.f, hudTop + 57.f));

        m_odometerValue.emplace(m_font, "0.0 км", 12);
        m_odometerValue->setFillColor(sf::Color(200, 255, 200));
        m_odometerValue->setPosition(sf::Vector2f(centerX + 55.f, hudTop + 57.f));
    }

    // LCD panel
    m_lcdPanel.setSize(sf::Vector2f(200.f, 30.f));
    m_lcdPanel.setPosition(sf::Vector2f(centerX - 40.f, hudTop + 85.f));
    m_lcdPanel.setFillColor(sf::Color(20, 40, 20));
    m_lcdPanel.setOutlineColor(sf::Color(60, 120, 60));
    m_lcdPanel.setOutlineThickness(2.f);

    if (m_fontLoaded) {
        m_lcdText.emplace(m_font, "READY", 14);
        m_lcdText->setFillColor(sf::Color(100, 255, 100));
        m_lcdText->setPosition(sf::Vector2f(centerX - 25.f, hudTop + 92.f));
    }
}

void HUDPanel::initializeRightSection() {
    float hudTop = m_windowHeight - 120.f;
    float rightX = m_windowWidth - 200.f;

    // Fuel panel
    m_fuelPanel.setSize(sf::Vector2f(180.f, 30.f));
    m_fuelPanel.setPosition(sf::Vector2f(rightX, hudTop + 10.f));
    m_fuelPanel.setFillColor(sf::Color(50, 40, 40));
    m_fuelPanel.setOutlineColor(sf::Color(100, 80, 80));
    m_fuelPanel.setOutlineThickness(1.f);

    m_fuelBar.setSize(sf::Vector2f(170.f, 20.f));
    m_fuelBar.setPosition(sf::Vector2f(rightX + 5.f, hudTop + 15.f));
    m_fuelBar.setFillColor(sf::Color(200, 150, 50));

    if (m_fontLoaded) {
        m_fuelText.emplace(m_font, "ТОПЛИВО / FUEL", 12);
        m_fuelText->setFillColor(sf::Color::White);
        m_fuelText->setPosition(sf::Vector2f(rightX, hudTop - 5.f));

        m_fuelValue.emplace(m_font, "100%", 14);
        m_fuelValue->setFillColor(sf::Color(255, 200, 100));
        m_fuelValue->setPosition(sf::Vector2f(rightX + 5.f, hudTop + 12.f));
    }

    // Temperature panel
    m_tempPanel.setSize(sf::Vector2f(180.f, 30.f));
    m_tempPanel.setPosition(sf::Vector2f(rightX, hudTop + 50.f));
    m_tempPanel.setFillColor(sf::Color(40, 40, 50));
    m_tempPanel.setOutlineColor(sf::Color(80, 80, 100));
    m_tempPanel.setOutlineThickness(1.f);

    m_tempBar.setSize(sf::Vector2f(85.f, 20.f));
    m_tempBar.setPosition(sf::Vector2f(rightX + 5.f, hudTop + 55.f));
    m_tempBar.setFillColor(sf::Color(100, 150, 200));

    if (m_fontLoaded) {
        m_tempText.emplace(m_font, "ТЕМП:", 12);
        m_tempText->setFillColor(sf::Color(150, 150, 200));
        m_tempText->setPosition(sf::Vector2f(rightX + 5.f, hudTop + 52.f));

        m_tempValue.emplace(m_font, "50%", 12);
        m_tempValue->setFillColor(sf::Color(200, 200, 255));
        m_tempValue->setPosition(sf::Vector2f(rightX + 95.f, hudTop + 55.f));
    }

    // Warning indicators
    m_warningEngine.setSize(sf::Vector2f(55.f, 20.f));
    m_warningEngine.setPosition(sf::Vector2f(rightX + 5.f, hudTop + 90.f));
    m_warningEngine.setFillColor(sf::Color(60, 40, 40));

    m_warningFuel.setSize(sf::Vector2f(55.f, 20.f));
    m_warningFuel.setPosition(sf::Vector2f(rightX + 65.f, hudTop + 90.f));
    m_warningFuel.setFillColor(sf::Color(60, 50, 40));

    m_warningCheck.setSize(sf::Vector2f(55.f, 20.f));
    m_warningCheck.setPosition(sf::Vector2f(rightX + 125.f, hudTop + 90.f));
    m_warningCheck.setFillColor(sf::Color(60, 60, 40));

    if (m_fontLoaded) {
        m_warningEngineText.emplace(m_font, "ENG", 10);
        m_warningEngineText->setFillColor(sf::Color(150, 100, 100));
        m_warningEngineText->setPosition(sf::Vector2f(rightX + 15.f, hudTop + 93.f));

        m_warningFuelText.emplace(m_font, "FUEL", 10);
        m_warningFuelText->setFillColor(sf::Color(200, 150, 100));
        m_warningFuelText->setPosition(sf::Vector2f(rightX + 72.f, hudTop + 93.f));

        m_warningCheckText.emplace(m_font, "CHK", 10);
        m_warningCheckText->setFillColor(sf::Color(200, 200, 100));
        m_warningCheckText->setPosition(sf::Vector2f(rightX + 133.f, hudTop + 93.f));
    }
}

void HUDPanel::update(const HUDData& data) {
    m_data = data;
    updateBars();

    if (!m_fontLoaded) {
        return;
    }

    // Update energy value
    if (m_energyValue) {
        std::ostringstream energyStream;
        energyStream << std::fixed << std::setprecision(0) << m_data.energy << "%";
        m_energyValue->setString(energyStream.str());
    }

    // Update money value
    if (m_moneyValue) {
        std::ostringstream moneyStream;
        moneyStream << std::fixed << std::setprecision(0) << m_data.money << " ₽";
        m_moneyValue->setString(moneyStream.str());
    }

    // Update inventory value
    if (m_inventoryValue) {
        std::ostringstream invStream;
        invStream << std::fixed << std::setprecision(1) << m_data.inventoryWeight
                  << "/" << m_data.maxInventoryWeight;
        m_inventoryValue->setString(invStream.str());
    }

    // Update speed value
    if (m_speedValue) {
        std::ostringstream speedStream;
        speedStream << std::fixed << std::setprecision(0) << m_data.speed;
        m_speedValue->setString(speedStream.str());
    }

    // Update odometer value
    if (m_odometerValue) {
        std::ostringstream odoStream;
        odoStream << std::fixed << std::setprecision(1) << m_data.odometer << " км";
        m_odometerValue->setString(odoStream.str());
    }

    // Update LCD text
    if (m_lcdText) {
        m_lcdText->setString(m_data.lcdText);
    }

    // Update fuel value
    if (m_fuelValue) {
        std::ostringstream fuelStream;
        fuelStream << std::fixed << std::setprecision(0) << m_data.fuel << "%";
        m_fuelValue->setString(fuelStream.str());
    }

    // Update temperature value
    if (m_tempValue) {
        std::ostringstream tempStream;
        tempStream << std::fixed << std::setprecision(0) << m_data.temperature << "%";
        m_tempValue->setString(tempStream.str());
    }

    // Update warning indicators
    if (m_data.engineWarning) {
        m_warningEngine.setFillColor(sf::Color(200, 50, 50));
        if (m_warningEngineText) m_warningEngineText->setFillColor(sf::Color::White);
    } else {
        m_warningEngine.setFillColor(sf::Color(60, 40, 40));
        if (m_warningEngineText) m_warningEngineText->setFillColor(sf::Color(150, 100, 100));
    }

    if (m_data.fuelWarning) {
        m_warningFuel.setFillColor(sf::Color(200, 150, 50));
        if (m_warningFuelText) m_warningFuelText->setFillColor(sf::Color::White);
    } else {
        m_warningFuel.setFillColor(sf::Color(60, 50, 40));
        if (m_warningFuelText) m_warningFuelText->setFillColor(sf::Color(200, 150, 100));
    }

    if (m_data.checkEngine) {
        m_warningCheck.setFillColor(sf::Color(200, 200, 50));
        if (m_warningCheckText) m_warningCheckText->setFillColor(sf::Color::White);
    } else {
        m_warningCheck.setFillColor(sf::Color(60, 60, 40));
        if (m_warningCheckText) m_warningCheckText->setFillColor(sf::Color(200, 200, 100));
    }
}

void HUDPanel::updateBars() {
    // Update energy bar width
    float energyWidth = (m_data.energy / 100.f) * 170.f;
    m_energyBar.setSize(sf::Vector2f(energyWidth, 20.f));
    
    // Change energy color based on level
    if (m_data.energy < 20.f) {
        m_energyBar.setFillColor(sf::Color(200, 50, 50));
    } else if (m_data.energy < 50.f) {
        m_energyBar.setFillColor(sf::Color(200, 150, 50));
    } else {
        m_energyBar.setFillColor(sf::Color(100, 200, 100));
    }
    
    // Update fuel bar width
    float fuelWidth = (m_data.fuel / 100.f) * 170.f;
    m_fuelBar.setSize(sf::Vector2f(fuelWidth, 20.f));
    
    // Change fuel color based on level
    if (m_data.fuel < 20.f) {
        m_fuelBar.setFillColor(sf::Color(200, 50, 50));
    } else if (m_data.fuel < 50.f) {
        m_fuelBar.setFillColor(sf::Color(200, 150, 50));
    } else {
        m_fuelBar.setFillColor(sf::Color(200, 200, 50));
    }
    
    // Update temperature bar width
    float tempWidth = (m_data.temperature / 100.f) * 85.f;
    m_tempBar.setSize(sf::Vector2f(tempWidth, 20.f));
    
    // Change temperature color based on level
    if (m_data.temperature > 80.f) {
        m_tempBar.setFillColor(sf::Color(200, 50, 50));
    } else if (m_data.temperature > 60.f) {
        m_tempBar.setFillColor(sf::Color(200, 150, 50));
    } else {
        m_tempBar.setFillColor(sf::Color(100, 150, 200));
    }
}

void HUDPanel::render(sf::RenderWindow& window) {
    // Draw main background
    window.draw(m_hudBackground);

    // Draw left section
    window.draw(m_energyPanel);
    window.draw(m_energyBar);
    window.draw(m_moneyPanel);
    window.draw(m_inventoryPanel);

    if (m_fontLoaded) {
        if (m_energyText) window.draw(*m_energyText);
        if (m_energyValue) window.draw(*m_energyValue);
        if (m_moneyText) window.draw(*m_moneyText);
        if (m_moneyValue) window.draw(*m_moneyValue);
        if (m_inventoryText) window.draw(*m_inventoryText);
        if (m_inventoryValue) window.draw(*m_inventoryValue);
    }

    // Draw center section
    window.draw(m_speedPanel);
    window.draw(m_odometerPanel);
    window.draw(m_lcdPanel);

    if (m_fontLoaded) {
        if (m_speedText) window.draw(*m_speedText);
        if (m_speedValue) window.draw(*m_speedValue);
        if (m_odometerText) window.draw(*m_odometerText);
        if (m_odometerValue) window.draw(*m_odometerValue);
        if (m_lcdText) window.draw(*m_lcdText);
    }

    // Draw right section
    window.draw(m_fuelPanel);
    window.draw(m_fuelBar);
    window.draw(m_tempPanel);
    window.draw(m_tempBar);
    window.draw(m_warningEngine);
    window.draw(m_warningFuel);
    window.draw(m_warningCheck);

    if (m_fontLoaded) {
        if (m_fuelText) window.draw(*m_fuelText);
        if (m_fuelValue) window.draw(*m_fuelValue);
        if (m_tempText) window.draw(*m_tempText);
        if (m_tempValue) window.draw(*m_tempValue);
        if (m_warningEngineText) window.draw(*m_warningEngineText);
        if (m_warningFuelText) window.draw(*m_warningFuelText);
        if (m_warningCheckText) window.draw(*m_warningCheckText);
    }

    // Draw top-right display
    if (m_topRightEnabled) {
        renderTopRightDisplay(window);
    }
}

bool HUDPanel::setFont(const std::string& fontPath) {
    // SFML 3.x uses openFromFile instead of loadFromFile
    m_fontLoaded = m_font.openFromFile(fontPath);

    if (m_fontLoaded) {
        // Reinitialize layout with new font
        initializeLayout();
        initializeTopRightDisplay();
    }

    return m_fontLoaded;
}

// === Phase 4: Top-right resource display implementation ===

void HUDPanel::initializeTopRightDisplay() {
    if (!m_fontLoaded) {
        return;
    }

    float rightX = 650.0f;  // Top-right corner start position
    float startY = 10.0f;
    float lineSpacing = 25.0f;

    // Money text: 💰 1,250₽
    m_topRightMoneyText.emplace(m_font, "💰 0₽", 14);
    m_topRightMoneyText->setFillColor(sf::Color(255, 215, 0));  // Gold color
    m_topRightMoneyText->setPosition(sf::Vector2f(rightX, startY));

    // Fuel text: ⛽ 45.5/100L (above fuel bar)
    m_topRightFuelText.emplace(m_font, "⛽ 100.0/100L", 14);
    m_topRightFuelText->setFillColor(sf::Color(255, 165, 0));  // Orange color
    m_topRightFuelText->setPosition(sf::Vector2f(rightX, startY + lineSpacing));

    // Fuel bar position (below fuel text)
    m_topRightFuelBar.setPosition(sf::Vector2f(rightX + 100, startY + lineSpacing + 5));

    // Energy text: ⚡ 70/100 (above energy bar)
    m_topRightEnergyText.emplace(m_font, "⚡ 100/100", 14);
    m_topRightEnergyText->setFillColor(sf::Color(0, 255, 255));  // Cyan color
    m_topRightEnergyText->setPosition(sf::Vector2f(rightX, startY + lineSpacing * 2 + 10));

    // Energy bar position (below energy text)
    m_topRightEnergyBar.setPosition(sf::Vector2f(rightX + 100, startY + lineSpacing * 2 + 15));

    // Cargo text: 📦 25/64
    m_topRightCargoText.emplace(m_font, "📦 0/100", 14);
    m_topRightCargoText->setFillColor(sf::Color::White);
    m_topRightCargoText->setPosition(sf::Vector2f(rightX, startY + lineSpacing * 3 + 20));

    // Initialize resource values
    updateTopRightDisplay();
}

void HUDPanel::setMoney(float amount) {
    m_topRightMoney = amount;
    updateTopRightDisplay();
}

void HUDPanel::setFuel(float current, float max) {
    m_topRightFuelCurrent = current;
    m_topRightFuelMax = max;
    m_topRightFuelBar.setValue(current, max);
    updateTopRightDisplay();
}

void HUDPanel::setEnergy(float current, float max) {
    m_topRightEnergyCurrent = current;
    m_topRightEnergyMax = max;
    m_topRightEnergyBar.setValue(current, max);
    updateTopRightDisplay();
}

void HUDPanel::setCargo(float current, float max) {
    m_topRightCargoCurrent = current;
    m_topRightCargoMax = max;
    updateTopRightDisplay();
}

void HUDPanel::setTopRightDisplayEnabled(bool enabled) {
    m_topRightEnabled = enabled;
}

void HUDPanel::updateTopRight(float deltaTime) {
    if (!m_topRightEnabled) {
        return;
    }
    
    // Update resource bar animations
    m_topRightFuelBar.update(deltaTime);
    m_topRightEnergyBar.update(deltaTime);
}

void HUDPanel::updateTopRightDisplay() {
    if (!m_fontLoaded) {
        return;
    }

    // Update money text
    if (m_topRightMoneyText) {
        std::ostringstream moneyStream;
        moneyStream << "💰 " << std::fixed << std::setprecision(0) << m_topRightMoney << "₽";
        m_topRightMoneyText->setString(moneyStream.str());
    }

    // Update fuel text
    if (m_topRightFuelText) {
        std::ostringstream fuelStream;
        fuelStream << "⛽ " << std::fixed << std::setprecision(1)
                   << m_topRightFuelCurrent << "/" << m_topRightFuelMax << "L";
        m_topRightFuelText->setString(fuelStream.str());
    }

    // Update energy text
    if (m_topRightEnergyText) {
        std::ostringstream energyStream;
        energyStream << "⚡ " << std::fixed << std::setprecision(0)
                     << m_topRightEnergyCurrent << "/" << m_topRightEnergyMax;
        m_topRightEnergyText->setString(energyStream.str());
    }

    // Update cargo text
    if (m_topRightCargoText) {
        std::ostringstream cargoStream;
        cargoStream << "📦 " << std::fixed << std::setprecision(0)
                    << m_topRightCargoCurrent << "/" << m_topRightCargoMax;
        m_topRightCargoText->setString(cargoStream.str());
    }
}

void HUDPanel::renderTopRightDisplay(sf::RenderWindow& window) {
    if (!m_fontLoaded) {
        return;
    }

    // Draw text elements
    if (m_topRightMoneyText) window.draw(*m_topRightMoneyText);
    if (m_topRightFuelText) window.draw(*m_topRightFuelText);
    if (m_topRightEnergyText) window.draw(*m_topRightEnergyText);
    if (m_topRightCargoText) window.draw(*m_topRightCargoText);

    // Draw resource bars
    m_topRightFuelBar.draw(window);
    m_topRightEnergyBar.draw(window);
}
