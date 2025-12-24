#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

#include "EventManager.h"
#include <vector>
#include <memory>



namespace EventFactory {





    GameEvent createMeetMechanic();


    GameEvent createMeetUnemployed();


    GameEvent createMeetPunk();


    GameEvent createMeetGranny();


    GameEvent createMeetTrucker();


    GameEvent createMeetStudent();






    GameEvent createMechanicRepairOffer();


    GameEvent createPunkConcertEvent();


    GameEvent createGrannyStoriesEvent();


    GameEvent createCompanionConflict();


    GameEvent createTruckerStories();


    GameEvent createStudentPhilosophy();


    GameEvent createUnemployedAdvice();


    GameEvent createMechanicTeachSkill();






    GameEvent createCarBreakdown();


    GameEvent createPoliceStop();


    GameEvent createCrossroads();


    GameEvent createBadWeather();






    GameEvent createRoadsideFinding();


    GameEvent createHitchhikerTrade();


    GameEvent createOutOfFuel();






    void initializeAllEvents(EventManager& eventManager);


    std::vector<GameEvent> createEncounterEvents();


    std::vector<GameEvent> createRoadEvents();


    std::vector<GameEvent> createCompanionEvents();
}

#endif
