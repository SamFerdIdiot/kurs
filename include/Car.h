#ifndef CAR_H
#define CAR_H

#include "GameTypes.h"
#include "InventorySystem.h"
#include <string>

















class Car {
public:



    Car(CarType carType = CarType::UNKNOWN);


    CarType getType() const { return m_type; }


    void setType(CarType carType);




    float getFuel() const { return m_fuel; }


    float getFuelCapacity() const { return m_fuelCapacity; }


    float getFuelEfficiency() const;


    void setFuel(float fuel);



    float addFuel(float amount);






    float calculateFuelConsumption(float distance, float terrainModifier = 1.0f) const;






    bool consumeFuel(float distance, float terrainModifier = 1.0f);






    bool hasFuelForDistance(float distance, float terrainModifier = 1.0f) const;




    float getCargoCapacity() const { return m_cargoCapacity; }


    float getCurrentCargoWeight() const { return m_currentCargoWeight; }


    void setCurrentCargoWeight(float weight);


    float getRemainingCargoCapacity() const;





    bool canAddCargo(float additionalWeight) const;



    bool isOverloaded() const;




    float getCondition() const { return m_condition; }


    void setCondition(float condition);




    void repair(float amount);





    void applyWear(float distance, float terrainModifier = 1.0f);





    float getEffectiveFuelEfficiency() const;



    float getSpeedModifier() const;



    float getBreakdownChance() const;






    void syncCargoWeight(const InventorySystem& inventory);





    bool canCarryInventory(const InventorySystem& inventory) const;




    std::string getTypeName() const;


    std::string getDescription() const;


    std::string getStatusString() const;

private:
    CarType m_type;


    float m_fuel;
    float m_fuelCapacity;
    float m_baseFuelEfficiency;


    float m_cargoCapacity;
    float m_currentCargoWeight;


    float m_condition;


    float m_baseSpeed;
    float m_durability;


    void initializeProperties();


    void clampFuel();


    void clampCondition();
};

#endif
