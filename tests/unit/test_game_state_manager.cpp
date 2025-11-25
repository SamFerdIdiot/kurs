#include <gtest/gtest.h>
#include "GameStateManager.h"
#include <filesystem>

namespace fs = std::filesystem;

// Test fixture for GameStateManager tests
class GameStateManagerTest : public ::testing::Test {
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

// Test new game initialization
TEST_F(GameStateManagerTest, StartNewGame) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    // Should have no save initially
    EXPECT_FALSE(gsm.hasSaveGame());
    
    // Start new game
    gsm.startNewGame(OriginType::EX_RACER, CarType::OLD_SEDAN);
    
    // Should now have a save
    EXPECT_TRUE(gsm.hasSaveGame());
    
    // Verify player state was initialized
    const PlayerState& player = gsm.getPlayerState();
    EXPECT_EQ(player.getOrigin(), OriginType::EX_RACER);
    EXPECT_EQ(player.getCarType(), CarType::OLD_SEDAN);
    EXPECT_EQ(player.getCurrentNodeId(), 0); // Start at node 0
}

// Test load game
TEST_F(GameStateManagerTest, LoadGame) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    // Start and save a game
    gsm.startNewGame(OriginType::NOMAD, CarType::OFFROAD_VAN);
    
    // Modify player state
    gsm.getPlayerState().setMoney(500.0f);
    gsm.getPlayerState().setEnergy(60.0f);
    gsm.getPlayerState().setCurrentNodeId(5);
    gsm.autoSave();
    
    // Create a fresh instance (in real code, this would be on restart)
    // Since it's singleton, we need to reload
    EXPECT_TRUE(gsm.loadGame());
    
    // Verify loaded state
    const PlayerState& player = gsm.getPlayerState();
    EXPECT_EQ(player.getOrigin(), OriginType::NOMAD);
    EXPECT_EQ(player.getCarType(), CarType::OFFROAD_VAN);
    EXPECT_FLOAT_EQ(player.getMoney(), 500.0f);
    EXPECT_FLOAT_EQ(player.getEnergy(), 60.0f);
    EXPECT_EQ(player.getCurrentNodeId(), 5);
}

// Test auto-save
TEST_F(GameStateManagerTest, AutoSave) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    gsm.startNewGame(OriginType::HITCHHIKER, CarType::SPORTS_COUPE);
    
    // Modify state
    gsm.getPlayerState().addMoney(1000.0f);
    
    // Auto-save
    gsm.autoSave();
    
    // Verify backup was created
    EXPECT_TRUE(gsm.getSaveSystem().hasBackup(0));
}

// Test backup loading
TEST_F(GameStateManagerTest, LoadBackup) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    // Create multiple saves
    gsm.startNewGame(OriginType::EX_RACER, CarType::OLD_SEDAN);
    
    // Save state at node 1
    gsm.getPlayerState().setCurrentNodeId(1);
    gsm.getPlayerState().setMoney(100.0f);
    gsm.autoSave();
    
    // Save state at node 2
    gsm.getPlayerState().setCurrentNodeId(2);
    gsm.getPlayerState().setMoney(200.0f);
    gsm.autoSave();
    
    // Save state at node 3
    gsm.getPlayerState().setCurrentNodeId(3);
    gsm.getPlayerState().setMoney(300.0f);
    gsm.autoSave();
    
    // Current state should be node 3
    EXPECT_EQ(gsm.getPlayerState().getCurrentNodeId(), 3);
    EXPECT_FLOAT_EQ(gsm.getPlayerState().getMoney(), 300.0f);
    
    // Load backup 0 (node 2)
    EXPECT_TRUE(gsm.loadBackup(0));
    EXPECT_EQ(gsm.getPlayerState().getCurrentNodeId(), 2);
    EXPECT_FLOAT_EQ(gsm.getPlayerState().getMoney(), 200.0f);
    
    // Load backup 1 (node 1)
    EXPECT_TRUE(gsm.loadBackup(1));
    EXPECT_EQ(gsm.getPlayerState().getCurrentNodeId(), 1);
    EXPECT_FLOAT_EQ(gsm.getPlayerState().getMoney(), 100.0f);
}

// Test get backup list
TEST_F(GameStateManagerTest, GetBackupList) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    gsm.startNewGame(OriginType::NOMAD, CarType::OLD_SEDAN);
    
    // Create 2 more saves (total 3)
    gsm.autoSave();
    gsm.autoSave();
    
    auto backups = gsm.getBackupList();
    ASSERT_EQ(backups.size(), 4); // MAX_BACKUPS = 4
    
    // First two should exist
    EXPECT_TRUE(backups[0].exists);
    EXPECT_TRUE(backups[1].exists);
    
    // Last two should not exist
    EXPECT_FALSE(backups[2].exists);
    EXPECT_FALSE(backups[3].exists);
}

// Test no save on startup
TEST_F(GameStateManagerTest, NoSaveOnStartup) {
    GameStateManager& gsm = GameStateManager::getInstance();
    
    EXPECT_FALSE(gsm.hasSaveGame());
    EXPECT_FALSE(gsm.loadGame());
}
