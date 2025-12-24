#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "GameTypes.h"
#include "InventorySystem.h"
#include <memory>












class PlayerState {
public:
    PlayerState();


    float getEnergy() const { return m_energy; }
    void setEnergy(float energy);
    void addEnergy(float amount);
    void modifyEnergy(float amount) { addEnergy(amount); }
    bool hasEnergy() const { return m_energy > 0.0f; }


    float getMoney() const { return m_money; }
    void setMoney(float money);
    void addMoney(float amount);
    void modifyMoney(float amount) { addMoney(amount); }
    bool canAfford(float cost) const { return m_money >= cost; }


    float getFuel() const { return m_fuel; }
    void setFuel(float fuel);
    void addFuel(float amount);
    bool hasFuel() const { return m_fuel > 0.0f; }


    float getVehicleCondition() const { return m_vehicleCondition; }
    void setVehicleCondition(float condition);
    void modifyVehicleCondition(float amount);


    float getMood() const { return m_mood; }
    void setMood(float mood);
    void modifyMood(float amount);


    int getReputation() const { return m_reputation; }
    void setReputation(int reputation);
    void modifyReputation(int amount);


    OriginType getOrigin() const { return m_origin; }
    void setOrigin(OriginType origin) { m_origin = origin; }

    CarType getCarType() const { return m_carType; }
    void setCarType(CarType carType) { m_carType = carType; }


    void initializeResources();
    void initializeResources(OriginType origin, CarType carType);


    bool isGameOver() const;


    int getCurrentNodeId() const { return m_currentNodeId; }
    void setCurrentNodeId(int nodeId) { m_currentNodeId = nodeId; }


    int getCurrentCityIndex() const { return m_currentCityIndex; }
    void setCurrentCityIndex(int index) { m_currentCityIndex = index; }
    void advanceToNextCity() { m_currentCityIndex++; }


    float getTotalPlayTime() const { return m_totalPlayTime; }
    void updatePlayTime(float deltaTime) { m_totalPlayTime += deltaTime; }
    void resetPlayTime() { m_totalPlayTime = 0.0f; }


    std::string getCurrentNotebookEntryId() const { return m_currentNotebookEntryId; }
    void setCurrentNotebookEntryId(const std::string& entryId) { m_currentNotebookEntryId = entryId; }


    InventorySystem& getInventory() { return m_inventory; }
    const InventorySystem& getInventory() const { return m_inventory; }
    void initializeStartingInventory();





    void addPrinciple(const std::string& principleId);
    void addTrait(const std::string& traitId);
    void addStoryItem(const std::string& itemId);

    bool hasPrinciple(const std::string& principleId) const;
    bool hasTrait(const std::string& traitId) const;
    bool hasStoryItem(const std::string& itemId) const;

    const std::vector<std::string>& getPrinciples() const { return m_principles; }
    const std::vector<std::string>& getTraits() const { return m_traits; }
    const std::vector<std::string>& getStoryItems() const { return m_storyItems; }

    void clearPrinciples() { m_principles.clear(); }
    void clearTraits() { m_traits.clear(); }
    void clearStoryItems() { m_storyItems.clear(); }

private:

    float m_energy;
    float m_money;
    float m_fuel;
    float m_vehicleCondition;
    float m_mood;
    int m_reputation;
    OriginType m_origin;
    CarType m_carType;
    InventorySystem m_inventory;


    int m_currentCityIndex;
    float m_totalPlayTime;
    int m_currentNodeId;
    std::string m_currentNotebookEntryId;


    std::vector<std::string> m_principles;
    std::vector<std::string> m_traits;
    std::vector<std::string> m_storyItems;
};

#endif
