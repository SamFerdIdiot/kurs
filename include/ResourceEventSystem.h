#pragma once

#include "PlayerState.h"
#include "EventManager.h"
#include <string>
#include <vector>







class ResourceEventSystem {
public:



    struct ResourceThresholds {

        float fuel_critical = 20.0f;
        float fuel_low = 35.0f;
        float fuel_very_low = 10.0f;


        float energy_exhausted = 15.0f;
        float energy_tired = 30.0f;
        float energy_very_tired = 10.0f;


        float money_broke = 50.0f;
        float money_low = 200.0f;


        float vehicle_broken = 25.0f;
        float vehicle_damaged = 50.0f;


        float mood_depressed = 20.0f;
        float mood_low = 40.0f;
    };






    ResourceEventSystem(PlayerState* playerState, EventManager* eventManager);





    std::string checkResourcesAndTriggerEvents();






    std::string checkSpecificResource(const std::string& resourceType);





    std::string getResourceWarning() const;





    bool hasCriticalResources() const;




    void setThresholds(const ResourceThresholds& thresholds);

private:
    PlayerState* m_playerState;
    EventManager* m_eventManager;
    ResourceThresholds m_thresholds;


    bool m_fuelWarningShown = false;
    bool m_energyWarningShown = false;
    bool m_moneyWarningShown = false;
    bool m_vehicleWarningShown = false;




    std::string checkFuel();




    std::string checkEnergy();




    std::string checkMoney();




    std::string checkVehicle();




    std::string checkMood();




    void resetWarningFlags();
};
