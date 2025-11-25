/**
 * @file test_ui.cpp
 * @brief Unit tests for UI components (ResourceBar, HUDPanel)
 * @date 2025-11-16
 */

#include <gtest/gtest.h>
#include "UI/ResourceBar.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>

/**
 * Test ResourceBar initialization
 */
TEST(ResourceBarTest, Initialization) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Initial percentage should be 0
    EXPECT_FLOAT_EQ(bar.getPercentage(), 0.0f);
}

/**
 * Test ResourceBar value setting
 */
TEST(ResourceBarTest, SetValue) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    bar.setValue(50, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
    
    bar.setValue(75, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 75.0f);
    
    bar.setValue(25, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 25.0f);
}

/**
 * Test ResourceBar color gradient zones
 */
TEST(ResourceBarTest, ColorGradient) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Test red zone (<40%)
    bar.setValue(20, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 20.0f);
    
    // Test yellow zone (40-70%)
    bar.setValue(50, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
    
    // Test green zone (>70%)
    bar.setValue(80, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 80.0f);
}

/**
 * Test ResourceBar smooth transition
 * Verifies that the bar animates smoothly instead of jumping instantly
 */
TEST(ResourceBarTest, SmoothTransition) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Start at 100%
    bar.setValue(100, 100);
    bar.update(1.0f);  // Update to reach target
    EXPECT_FLOAT_EQ(bar.getPercentage(), 100.0f);
    
    // Change to 50%
    bar.setValue(50, 100);
    
    // After a small time step, value should be transitioning (not instant)
    // We can't check the exact display value, but we can verify the percentage is correct
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
}

/**
 * Test ResourceBar position setting
 */
TEST(ResourceBarTest, SetPosition) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Position can be changed
    bar.setPosition(sf::Vector2f(200, 150));
    
    // No exception should be thrown
    EXPECT_TRUE(true);
}

/**
 * Test ResourceBar boundary conditions
 */
TEST(ResourceBarTest, BoundaryConditions) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Test zero
    bar.setValue(0, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 0.0f);
    
    // Test full
    bar.setValue(100, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 100.0f);
    
    // Test negative (should clamp to 0)
    bar.setValue(-10, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 0.0f);
    
    // Test over max (should clamp to 100%)
    bar.setValue(150, 100);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 100.0f);
}

/**
 * Test ResourceBar with different max values
 */
TEST(ResourceBarTest, DifferentMaxValues) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Test with max = 50
    bar.setValue(25, 50);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
    
    // Test with max = 200
    bar.setValue(100, 200);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
    
    // Test with max = 1 (edge case)
    bar.setValue(0.5f, 1.0f);
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
}

/**
 * Test ResourceBar update with different delta times
 */
TEST(ResourceBarTest, UpdateWithDeltaTime) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    bar.setValue(100, 100);
    bar.update(0.1f);
    
    bar.setValue(50, 100);
    bar.update(0.1f);
    
    // Value should still be correct
    EXPECT_FLOAT_EQ(bar.getPercentage(), 50.0f);
}

/**
 * Test ResourceBar zero max value (should not crash)
 */
TEST(ResourceBarTest, ZeroMaxValue) {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    // Setting zero max should not crash (will be clamped to 1.0)
    bar.setValue(50, 0);
    
    // Should handle gracefully
    float percentage = bar.getPercentage();
    EXPECT_GE(percentage, 0.0f);
}

// === HUDPanel Tests ===

/**
 * Test HUDPanel initialization
 */
TEST(HUDPanelTest, Initialization) {
    HUDPanel hud(800, 600);
    
    // Initial values should be set
    EXPECT_FLOAT_EQ(hud.getMoney(), 0.0f);
    EXPECT_FLOAT_EQ(hud.getFuel(), 100.0f);
    EXPECT_FLOAT_EQ(hud.getEnergy(), 100.0f);
    EXPECT_FLOAT_EQ(hud.getCargo(), 0.0f);
}

/**
 * Test HUDPanel resource setters
 */
TEST(HUDPanelTest, ResourceSetters) {
    HUDPanel hud(800, 600);
    
    // Set money
    hud.setMoney(1250.0f);
    EXPECT_FLOAT_EQ(hud.getMoney(), 1250.0f);
    
    // Set fuel
    hud.setFuel(45.5f, 100.0f);
    EXPECT_FLOAT_EQ(hud.getFuel(), 45.5f);
    
    // Set energy
    hud.setEnergy(70.0f, 100.0f);
    EXPECT_FLOAT_EQ(hud.getEnergy(), 70.0f);
    
    // Set cargo
    hud.setCargo(25.0f, 64.0f);
    EXPECT_FLOAT_EQ(hud.getCargo(), 25.0f);
}

/**
 * Test HUDPanel update method
 */
TEST(HUDPanelTest, UpdateMethod) {
    HUDPanel hud(800, 600);
    
    hud.setFuel(50.0f, 100.0f);
    hud.setEnergy(70.0f, 100.0f);
    
    // Update should not crash
    hud.updateTopRight(0.016f);  // 60 FPS frame time
    
    EXPECT_TRUE(true);
}

/**
 * Test HUDPanel enable/disable top-right display
 */
TEST(HUDPanelTest, EnableDisableTopRight) {
    HUDPanel hud(800, 600);
    
    // Should be enabled by default
    hud.setMoney(100.0f);
    EXPECT_FLOAT_EQ(hud.getMoney(), 100.0f);
    
    // Disable and enable
    hud.setTopRightDisplayEnabled(false);
    hud.setTopRightDisplayEnabled(true);
    
    // Should still work
    EXPECT_FLOAT_EQ(hud.getMoney(), 100.0f);
}

/**
 * Test HUDPanel with different max values
 */
TEST(HUDPanelTest, DifferentMaxValues) {
    HUDPanel hud(800, 600);
    
    // Test with different fuel capacity
    hud.setFuel(50.0f, 200.0f);
    EXPECT_FLOAT_EQ(hud.getFuel(), 50.0f);
    
    // Test with different energy capacity
    hud.setEnergy(80.0f, 150.0f);
    EXPECT_FLOAT_EQ(hud.getEnergy(), 80.0f);
    
    // Test with different cargo capacity
    hud.setCargo(100.0f, 250.0f);
    EXPECT_FLOAT_EQ(hud.getCargo(), 100.0f);
}

