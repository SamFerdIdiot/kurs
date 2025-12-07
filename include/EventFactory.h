#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

#include "EventManager.h"
#include <vector>
#include <memory>

// Фабрика для создания предопределённых событий
// Содержит русские события для дорожного путешествия по России 90-х
namespace EventFactory {
    // ============================================================================
    // События встреч с компаньонами (ENCOUNTER)
    // ============================================================================

    // Встреча с Механиком Михалычем на трассе
    GameEvent createMeetMechanic();

    // Встреча с Безработным Виталием на автовокзале
    GameEvent createMeetUnemployed();

    // Встреча с Панком Васей на заправке
    GameEvent createMeetPunk();

    // Встреча с Бабушкой Галиной на обочине
    GameEvent createMeetGranny();

    // Встреча с Дальнобойщиком Петровичем в кафе
    GameEvent createMeetTrucker();

    // Встреча со Студентом Лёхой на трассе
    GameEvent createMeetStudent();

    // ============================================================================
    // События с компаньонами (COMPANION)
    // ============================================================================

    // Механик предлагает починить машину попутчиков
    GameEvent createMechanicRepairOffer();

    // Панк хочет устроить концерт на заправке
    GameEvent createPunkConcertEvent();

    // Бабушка делится историями и пирожками
    GameEvent createGrannyStoriesEvent();

    // Конфликт между компаньонами
    GameEvent createCompanionConflict();

    // ============================================================================
    // Дорожные события (ROAD)
    // ============================================================================

    // Поломка машины на трассе
    GameEvent createCarBreakdown();

    // ГАИ остановка
    GameEvent createPoliceStop();

    // Перекресток - выбор пути
    GameEvent createCrossroads();

    // Плохая погода
    GameEvent createBadWeather();

    // ============================================================================
    // Ресурсные события (RESOURCE)
    // ============================================================================

    // Находка на обочине
    GameEvent createRoadsideFinding();

    // Попутчик предлагает обмен
    GameEvent createHitchhikerTrade();

    // ============================================================================
    // Утилиты
    // ============================================================================

    // Создать все базовые события и добавить в EventManager
    void initializeAllEvents(EventManager& eventManager);

    // Создать только события встреч с компаньонами
    std::vector<GameEvent> createEncounterEvents();

    // Создать только дорожные события
    std::vector<GameEvent> createRoadEvents();

    // Создать только события с компаньонами
    std::vector<GameEvent> createCompanionEvents();
}

#endif // EVENT_FACTORY_H
