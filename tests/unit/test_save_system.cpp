#include <gtest/gtest.h>
#include "SaveSystem.h"
#include "PlayerState.h"
#include <filesystem>

namespace fs = std::filesystem;

// Test fixture for SaveSystem tests
class SaveSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing saves before each test
        if (fs::exists("saves")) {
            fs::remove_all("saves");
        }
    }
    
    void TearDown() override {
        // Clean up saves after each test
        if (fs::exists("saves")) {
            fs::remove_all("saves");
        }
    }
};

// Test basic save and load
TEST_F(SaveSystemTest, SaveAndLoad) {
    SaveSystem saveSystem;
    
    // Create a player state with specific values
    PlayerState originalState;
    originalState.setEnergy(75.0f);
    originalState.setMoney(500.0f);
    originalState.setFuel(60.0f);
    originalState.setOrigin(OriginType::EX_RACER);
    originalState.setCarType(CarType::OLD_SEDAN);
    originalState.setCurrentNodeId(5);
    
    // Save the state
    ASSERT_TRUE(saveSystem.saveGame(originalState));
    ASSERT_TRUE(saveSystem.hasSaveFile());
    
    // Load the state into a new player state
    PlayerState loadedState;
    ASSERT_TRUE(saveSystem.loadGame(loadedState));
    
    // Verify loaded values match original
    EXPECT_FLOAT_EQ(loadedState.getEnergy(), 75.0f);
    EXPECT_FLOAT_EQ(loadedState.getMoney(), 500.0f);
    EXPECT_FLOAT_EQ(loadedState.getFuel(), 60.0f);
    EXPECT_EQ(loadedState.getOrigin(), OriginType::EX_RACER);
    EXPECT_EQ(loadedState.getCarType(), CarType::OLD_SEDAN);
    EXPECT_EQ(loadedState.getCurrentNodeId(), 5);
}

// Test backup rotation
TEST_F(SaveSystemTest, BackupRotation) {
    SaveSystem saveSystem;
    
    // Create and save first state
    PlayerState state1;
    state1.setMoney(100.0f);
    state1.setCurrentNodeId(1);
    ASSERT_TRUE(saveSystem.saveGame(state1));
    
    // Save second state - should create backup of first
    PlayerState state2;
    state2.setMoney(200.0f);
    state2.setCurrentNodeId(2);
    ASSERT_TRUE(saveSystem.saveGame(state2));
    
    // Check that backup exists
    EXPECT_TRUE(saveSystem.hasBackup(0));
    
    // Load from backup 0 - should be state1
    PlayerState loadedBackup;
    ASSERT_TRUE(saveSystem.loadBackup(loadedBackup, 0));
    EXPECT_FLOAT_EQ(loadedBackup.getMoney(), 100.0f);
    EXPECT_EQ(loadedBackup.getCurrentNodeId(), 1);
    
    // Load main save - should be state2
    PlayerState loadedMain;
    ASSERT_TRUE(saveSystem.loadGame(loadedMain));
    EXPECT_FLOAT_EQ(loadedMain.getMoney(), 200.0f);
    EXPECT_EQ(loadedMain.getCurrentNodeId(), 2);
}

// Test multiple backups
TEST_F(SaveSystemTest, MultipleBackups) {
    SaveSystem saveSystem;
    
    // Create 6 saves to test backup rotation
    for (int i = 0; i < 6; i++) {
        PlayerState state;
        state.setMoney(static_cast<float>(i * 100));
        state.setCurrentNodeId(i);
        ASSERT_TRUE(saveSystem.saveGame(state));
    }
    
    // Should have main save + 4 backups
    EXPECT_TRUE(saveSystem.hasSaveFile());
    EXPECT_TRUE(saveSystem.hasBackup(0));
    EXPECT_TRUE(saveSystem.hasBackup(1));
    EXPECT_TRUE(saveSystem.hasBackup(2));
    EXPECT_TRUE(saveSystem.hasBackup(3));
    
    // Main save should be the last one (i=5)
    PlayerState mainState;
    ASSERT_TRUE(saveSystem.loadGame(mainState));
    EXPECT_FLOAT_EQ(mainState.getMoney(), 500.0f);
    EXPECT_EQ(mainState.getCurrentNodeId(), 5);
    
    // Backup 0 should be i=4
    PlayerState backup0;
    ASSERT_TRUE(saveSystem.loadBackup(backup0, 0));
    EXPECT_FLOAT_EQ(backup0.getMoney(), 400.0f);
    EXPECT_EQ(backup0.getCurrentNodeId(), 4);
    
    // Backup 3 should be i=1 (oldest backup, i=0 was rotated out)
    PlayerState backup3;
    ASSERT_TRUE(saveSystem.loadBackup(backup3, 3));
    EXPECT_FLOAT_EQ(backup3.getMoney(), 100.0f);
    EXPECT_EQ(backup3.getCurrentNodeId(), 1);
}

// Test loading non-existent save
TEST_F(SaveSystemTest, LoadNonExistentSave) {
    SaveSystem saveSystem;
    PlayerState state;
    
    EXPECT_FALSE(saveSystem.hasSaveFile());
    EXPECT_FALSE(saveSystem.loadGame(state));
}

// Test loading non-existent backup
TEST_F(SaveSystemTest, LoadNonExistentBackup) {
    SaveSystem saveSystem;
    PlayerState state;
    
    EXPECT_FALSE(saveSystem.hasBackup(0));
    EXPECT_FALSE(saveSystem.loadBackup(state, 0));
}

// Test invalid backup index
TEST_F(SaveSystemTest, InvalidBackupIndex) {
    SaveSystem saveSystem;
    PlayerState state;
    
    EXPECT_FALSE(saveSystem.hasBackup(-1));
    EXPECT_FALSE(saveSystem.hasBackup(4)); // MAX_BACKUPS = 4, valid indices are 0-3
    EXPECT_FALSE(saveSystem.loadBackup(state, -1));
    EXPECT_FALSE(saveSystem.loadBackup(state, 4));
}

// Test clear all saves
TEST_F(SaveSystemTest, ClearAllSaves) {
    SaveSystem saveSystem;
    
    // Create some saves
    PlayerState state;
    state.setMoney(100.0f);
    ASSERT_TRUE(saveSystem.saveGame(state));
    state.setMoney(200.0f);
    ASSERT_TRUE(saveSystem.saveGame(state));
    state.setMoney(300.0f);
    ASSERT_TRUE(saveSystem.saveGame(state));
    
    EXPECT_TRUE(saveSystem.hasSaveFile());
    EXPECT_TRUE(saveSystem.hasBackup(0));
    
    // Clear all saves
    saveSystem.clearAllSaves();
    
    EXPECT_FALSE(saveSystem.hasSaveFile());
    EXPECT_FALSE(saveSystem.hasBackup(0));
    EXPECT_FALSE(saveSystem.hasBackup(1));
}

// Test get backup list
TEST_F(SaveSystemTest, GetBackupList) {
    SaveSystem saveSystem;
    
    // Create 3 saves
    for (int i = 0; i < 3; i++) {
        PlayerState state;
        state.setMoney(static_cast<float>(i * 100));
        ASSERT_TRUE(saveSystem.saveGame(state));
    }
    
    auto backupList = saveSystem.getBackupList();
    ASSERT_EQ(backupList.size(), 4); // MAX_BACKUPS = 4
    
    // First two backups should exist
    EXPECT_TRUE(backupList[0].exists);
    EXPECT_TRUE(backupList[1].exists);
    
    // Last two should not exist (only created 3 saves total)
    EXPECT_FALSE(backupList[2].exists);
    EXPECT_FALSE(backupList[3].exists);
}
