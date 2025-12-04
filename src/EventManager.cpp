#include "EventManager.h"
#include "AbilityTreeSystem.h"
#include "AbilityTypes.h"
#include "GameStateManager.h"
#include <algorithm>
#include <random>
#include <ctime>

// Constructor
EventManager::EventManager() {
    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

// Add an event to the pool
void EventManager::addEvent(const GameEvent& event) {
    m_events.push_back(event);
}

// Remove an event by id
void EventManager::removeEvent(const std::string& id) {
    m_events.erase(
        std::remove_if(m_events.begin(), m_events.end(),
            [&id](const GameEvent& event) { return event.id == id; }),
        m_events.end()
    );
}

// Get event by id
GameEvent* EventManager::getEvent(const std::string& id) {
    for (auto& event : m_events) {
        if (event.id == id) {
            return &event;
        }
    }
    return nullptr;
}

// Get all events
const std::vector<GameEvent>& EventManager::getAllEvents() const {
    return m_events;
}

// Check if condition is met for an event
bool EventManager::checkCondition(const EventCondition& condition,
                                 float fuel, float energy, int money,
                                 const std::string& location,
                                 const std::string& roadType) const {
    // Check fuel range
    if (fuel < condition.minFuel || fuel > condition.maxFuel) {
        return false;
    }

    // Check energy range
    if (energy < condition.minEnergy || energy > condition.maxEnergy) {
        return false;
    }

    // Check money range
    if (money < condition.minMoney || money > condition.maxMoney) {
        return false;
    }

    // Check location if specified
    if (!condition.requiredLocation.empty() &&
        condition.requiredLocation != location) {
        return false;
    }

    // Check road type if specified
    if (!condition.requiredRoadType.empty() &&
        condition.requiredRoadType != roadType) {
        return false;
    }

    // Check probability
    float roll = randomFloat(0.0f, 1.0f);
    if (roll > condition.probability) {
        return false;
    }

    return true;
}

// Get random event that meets conditions
GameEvent* EventManager::getRandomEvent(float fuel, float energy, int money,
                                       const std::string& location,
                                       const std::string& roadType) {
    // Collect all eligible events
    std::vector<GameEvent*> eligibleEvents;

    for (auto& event : m_events) {
        if (!event.triggered &&
            checkCondition(event.condition, fuel, energy, money, location, roadType)) {
            eligibleEvents.push_back(&event);
        }
    }

    // Return nullptr if no eligible events
    if (eligibleEvents.empty()) {
        return nullptr;
    }

    // Select random eligible event
    int randomIndex = randomInt(0, static_cast<int>(eligibleEvents.size()) - 1);
    return eligibleEvents[randomIndex];
}

// Mark event as triggered
void EventManager::triggerEvent(const std::string& id) {
    GameEvent* event = getEvent(id);
    if (event) {
        event->triggered = true;
        
        // Call callback if set
        if (m_eventCallback) {
            m_eventCallback(*event);
        }
    }
}

// Reset all triggered flags
void EventManager::resetTriggeredEvents() {
    for (auto& event : m_events) {
        event.triggered = false;
    }
}

// Apply event choice outcome
void EventManager::applyChoice(const EventChoice& choice,
                              float& fuel, float& energy, int& money) {
    fuel = std::max(0.0f, std::min(100.0f, fuel + choice.fuelChange));
    energy = std::max(0.0f, std::min(100.0f, energy + choice.energyChange));
    money = std::max(0, money + choice.moneyChange);
}

// Set callback for when event triggers
void EventManager::setEventCallback(EventCallback callback) {
    m_eventCallback = callback;
}

// Clear all events
void EventManager::clear() {
    m_events.clear();
}

// Initialize default events
void EventManager::initializeDefaultEvents() {
    clear();
    
    // Event 1: Gas Station - Low Fuel
    GameEvent gasStationEvent;
    gasStationEvent.id = "gas_station_low_fuel";
    gasStationEvent.title = "Gas Station";
    gasStationEvent.description = "You see a gas station. Fuel is running low.";
    gasStationEvent.condition.minFuel = 0.0f;
    gasStationEvent.condition.maxFuel = 30.0f;
    gasStationEvent.condition.probability = 0.8f;
    
    EventChoice refuelChoice;
    refuelChoice.text = "Заправиться (500 руб) / Refuel (500 rub)";
    refuelChoice.outcomeText = "Вы заправили бак полностью.\nYou filled up the tank.";
    refuelChoice.fuelChange = 70.0f;
    refuelChoice.moneyChange = -500;
    gasStationEvent.choices.push_back(refuelChoice);
    
    EventChoice skipChoice;
    skipChoice.text = "Проехать мимо / Drive past";
    skipChoice.outcomeText = "Вы решили проехать дальше.\nYou decided to drive on.";
    gasStationEvent.choices.push_back(skipChoice);
    
    addEvent(gasStationEvent);
    
    // Event 2: Hitchhiker
    GameEvent hitchhikerEvent;
    hitchhikerEvent.id = "hitchhiker_road";
    hitchhikerEvent.title = "Попутчик / Hitchhiker";
    hitchhikerEvent.description = "На обочине стоит человек с поднятой рукой.\n"
                                 "A person stands on the roadside with their thumb up.";
    hitchhikerEvent.condition.probability = 0.5f;
    
    EventChoice pickupChoice;
    pickupChoice.text = "Подобрать / Pick up";
    pickupChoice.outcomeText = "Попутчик заплатил за бензин. Вы немного устали от разговоров.\n"
                              "The hitchhiker paid for gas. You're a bit tired from talking.";
    pickupChoice.moneyChange = 200;
    pickupChoice.energyChange = -10.0f;
    hitchhikerEvent.choices.push_back(pickupChoice);
    
    EventChoice ignoreChoice;
    ignoreChoice.text = "Проехать мимо / Drive past";
    ignoreChoice.outcomeText = "Вы проехали мимо.\nYou drove past.";
    hitchhikerEvent.choices.push_back(ignoreChoice);
    
    addEvent(hitchhikerEvent);
    
    // Event 3: Police Checkpoint
    GameEvent policeEvent;
    policeEvent.id = "police_checkpoint";
    policeEvent.title = "Пост ДПС / Police Checkpoint";
    policeEvent.description = "Впереди пост ДПС. Милиционер машет вам остановиться.\n"
                             "A police checkpoint ahead. An officer waves you to stop.";
    policeEvent.condition.probability = 0.4f;
    
    EventChoice cooperateChoice;
    cooperateChoice.text = "Остановиться / Stop";
    cooperateChoice.outcomeText = "Проверка документов прошла быстро.\n"
                                 "Document check was quick.";
    cooperateChoice.energyChange = -5.0f;
    policeEvent.choices.push_back(cooperateChoice);
    
    EventChoice bribeChoice;
    bribeChoice.text = "Дать взятку 300 руб / Bribe 300 rub";
    bribeChoice.outcomeText = "Милиционер махнул рукой: 'Езжай'.\n"
                             "The officer waved: 'Drive on'.";
    bribeChoice.moneyChange = -300;
    policeEvent.choices.push_back(bribeChoice);
    
    addEvent(policeEvent);
    
    // Event 4: Breakdown - Low Energy
    GameEvent breakdownEvent;
    breakdownEvent.id = "car_breakdown";
    breakdownEvent.title = "Поломка / Breakdown";
    breakdownEvent.description = "Машина начинает странно шуметь. Что-то не так.\n"
                                "The car starts making strange noises. Something's wrong.";
    breakdownEvent.condition.minEnergy = 0.0f;
    breakdownEvent.condition.maxEnergy = 40.0f;
    breakdownEvent.condition.probability = 0.6f;
    
    EventChoice repairChoice;
    repairChoice.text = "Починить (800 руб) / Repair (800 rub)";
    repairChoice.outcomeText = "Вы нашли механика. Машина как новая!\n"
                              "You found a mechanic. Car's like new!";
    repairChoice.moneyChange = -800;
    repairChoice.energyChange = 30.0f;
    breakdownEvent.choices.push_back(repairChoice);
    
    EventChoice continueChoice;
    continueChoice.text = "Ехать дальше / Continue";
    continueChoice.outcomeText = "Вы продолжили путь. Шум усилился.\n"
                                "You continued. The noise got louder.";
    continueChoice.energyChange = -15.0f;
    breakdownEvent.choices.push_back(continueChoice);
    
    addEvent(breakdownEvent);
    
    // Event 5: Rest Stop
    GameEvent restEvent;
    restEvent.id = "rest_stop";
    restEvent.title = "Место отдыха / Rest Stop";
    restEvent.description = "Придорожное кафе. Можно перекусить и отдохнуть.\n"
                           "A roadside cafe. You could eat and rest.";
    restEvent.condition.probability = 0.7f;
    
    EventChoice eatChoice;
    eatChoice.text = "Перекусить (150 руб) / Eat (150 rub)";
    eatChoice.outcomeText = "Вы поели и немного отдохнули. Силы восстановлены!\n"
                           "You ate and rested a bit. Energy restored!";
    eatChoice.moneyChange = -150;
    eatChoice.energyChange = 20.0f;
    restEvent.choices.push_back(eatChoice);
    
    EventChoice skipRestChoice;
    skipRestChoice.text = "Ехать дальше / Drive on";
    skipRestChoice.outcomeText = "Вы продолжили путь.\nYou continued driving.";
    restEvent.choices.push_back(skipRestChoice);
    
    addEvent(restEvent);
    
    // Event 6: Tire Blowout - Mechanical
    GameEvent tireEvent;
    tireEvent.id = "tire_blowout";
    tireEvent.title = "Прокол шины / Tire Blowout";
    tireEvent.description = "БАХ! Вы слышите громкий звук. Шина проколота.\n"
                           "BANG! You hear a loud sound. You have a flat tire.";
    tireEvent.condition.probability = 0.3f;
    
    EventChoice fixTireChoice;
    fixTireChoice.text = "Заменить шину самому / Change tire yourself";
    fixTireChoice.outcomeText = "Вы заменили шину. Устали, но едете дальше.\n"
                               "You changed the tire. Tired, but moving on.";
    fixTireChoice.energyChange = -20.0f;
    tireEvent.choices.push_back(fixTireChoice);
    
    EventChoice callMechanicChoice;
    callMechanicChoice.text = "Вызвать механика (400 руб) / Call mechanic (400 rub)";
    callMechanicChoice.outcomeText = "Механик быстро заменил шину.\n"
                                    "Mechanic quickly changed the tire.";
    callMechanicChoice.moneyChange = -400;
    callMechanicChoice.energyChange = -5.0f;
    tireEvent.choices.push_back(callMechanicChoice);
    
    addEvent(tireEvent);
    
    // Event 7: Heavy Rain - Weather
    GameEvent rainEvent;
    rainEvent.id = "heavy_rain";
    rainEvent.title = "Сильный дождь / Heavy Rain";
    rainEvent.description = "Начался ливень. Видимость ухудшилась.\n"
                           "Heavy rain started. Visibility is poor.";
    rainEvent.condition.probability = 0.4f;
    
    EventChoice slowDownChoice;
    slowDownChoice.text = "Ехать медленно / Drive slowly";
    slowDownChoice.outcomeText = "Вы медленно, но безопасно едете под дождем.\n"
                                "You drive slowly but safely in the rain.";
    slowDownChoice.energyChange = -10.0f;
    rainEvent.choices.push_back(slowDownChoice);
    
    EventChoice waitRainChoice;
    waitRainChoice.text = "Переждать (30 мин) / Wait it out (30 min)";
    waitRainChoice.outcomeText = "Вы переждали дождь. Небо прояснилось.\n"
                                "You waited out the rain. Sky cleared up.";
    waitRainChoice.energyChange = -5.0f;
    rainEvent.choices.push_back(waitRainChoice);
    
    EventChoice riskItChoice;
    riskItChoice.text = "Рискнуть и ехать быстро / Risk it and drive fast";
    riskItChoice.outcomeText = "Адреналин! Вы проехали быстро, но очень устали.\n"
                              "Adrenaline! You drove fast but very tired.";
    riskItChoice.energyChange = -25.0f;
    rainEvent.choices.push_back(riskItChoice);
    
    addEvent(rainEvent);
    
    // Event 8: Roadside Vendor - Economic/Social
    GameEvent vendorEvent;
    vendorEvent.id = "roadside_vendor";
    vendorEvent.title = "Торговец / Roadside Vendor";
    vendorEvent.description = "Местный торговец продает домашние пирожки.\n"
                             "A local vendor is selling homemade pies.";
    vendorEvent.condition.probability = 0.5f;
    
    EventChoice buyPiesChoice;
    buyPiesChoice.text = "Купить пирожки (100 руб) / Buy pies (100 rub)";
    buyPiesChoice.outcomeText = "Вкусно! Вы чувствуете прилив сил.\n"
                               "Delicious! You feel energized.";
    buyPiesChoice.moneyChange = -100;
    buyPiesChoice.energyChange = 15.0f;
    vendorEvent.choices.push_back(buyPiesChoice);
    
    EventChoice chatChoice;
    chatChoice.text = "Поговорить / Chat";
    chatChoice.outcomeText = "Продавец рассказал интересные истории о местности.\n"
                            "The vendor told interesting stories about the area.";
    chatChoice.energyChange = 5.0f;
    vendorEvent.choices.push_back(chatChoice);
    
    EventChoice ignoreVendorChoice;
    ignoreVendorChoice.text = "Ехать дальше / Drive on";
    ignoreVendorChoice.outcomeText = "Вы проехали мимо.\nYou drove past.";
    vendorEvent.choices.push_back(ignoreVendorChoice);
    
    addEvent(vendorEvent);
    
    // Event 9: Engine Overheating - Mechanical
    GameEvent overheatingEvent;
    overheatingEvent.id = "engine_overheating";
    overheatingEvent.title = "Перегрев двигателя / Engine Overheating";
    overheatingEvent.description = "Датчик температуры в красной зоне! Двигатель перегревается.\n"
                                  "Temperature gauge in the red! Engine is overheating.";
    overheatingEvent.condition.minEnergy = 0.0f;
    overheatingEvent.condition.maxEnergy = 50.0f;
    overheatingEvent.condition.probability = 0.35f;
    
    EventChoice coolDownChoice;
    coolDownChoice.text = "Остановиться и остыть / Stop and cool down";
    coolDownChoice.outcomeText = "Вы остановились. Двигатель остыл через 20 минут.\n"
                                "You stopped. Engine cooled down in 20 minutes.";
    coolDownChoice.energyChange = 10.0f;
    overheatingEvent.choices.push_back(coolDownChoice);
    
    EventChoice addWaterChoice;
    addWaterChoice.text = "Долить воду в радиатор / Add water to radiator";
    addWaterChoice.outcomeText = "Вы долили воду. Можно ехать дальше.\n"
                                "You added water. Can continue driving.";
    addWaterChoice.energyChange = 5.0f;
    overheatingEvent.choices.push_back(addWaterChoice);
    
    EventChoice ignoreOverheatChoice;
    ignoreOverheatChoice.text = "Игнорировать и ехать / Ignore and drive";
    ignoreOverheatChoice.outcomeText = "ОПАСНО! Двигатель сильно пострадал.\n"
                                      "DANGEROUS! Engine badly damaged.";
    ignoreOverheatChoice.energyChange = -30.0f;
    overheatingEvent.choices.push_back(ignoreOverheatChoice);
    
    addEvent(overheatingEvent);
    
    // Event 10: Toll Road - Economic
    GameEvent tollEvent;
    tollEvent.id = "toll_road";
    tollEvent.title = "Платная дорога / Toll Road";
    tollEvent.description = "Впереди платная трасса. Быстрее, но дороже.\n"
                           "Toll road ahead. Faster, but costs money.";
    tollEvent.condition.minMoney = 200;
    tollEvent.condition.probability = 0.6f;
    
    EventChoice payTollChoice;
    payTollChoice.text = "Заплатить за проезд (200 руб) / Pay toll (200 rub)";
    payTollChoice.outcomeText = "Вы едете по отличной дороге. Быстро и комфортно!\n"
                               "You drive on excellent road. Fast and comfortable!";
    payTollChoice.moneyChange = -200;
    payTollChoice.energyChange = 10.0f;
    tollEvent.choices.push_back(payTollChoice);
    
    EventChoice takeDetourChoice;
    takeDetourChoice.text = "Объехать старой дорогой / Take the old road";
    takeDetourChoice.outcomeText = "Вы едете по ухабистой дороге. Дольше и утомительнее.\n"
                                  "You drive on bumpy road. Longer and tiring.";
    takeDetourChoice.energyChange = -15.0f;
    tollEvent.choices.push_back(takeDetourChoice);
    
    addEvent(tollEvent);
    
    // Event 11: Fellow Traveler - Social
    GameEvent fellowTravelerEvent;
    fellowTravelerEvent.id = "fellow_traveler";
    fellowTravelerEvent.title = "Встреча на дороге / Fellow Traveler";
    fellowTravelerEvent.description = "На парковке другой водитель предлагает путешествовать вместе.\n"
                                     "At the parking lot, another driver suggests traveling together.";
    fellowTravelerEvent.condition.probability = 0.4f;
    
    EventChoice joinConvoyChoice;
    joinConvoyChoice.text = "Присоединиться / Join convoy";
    joinConvoyChoice.outcomeText = "Ехать вместе веселее и безопаснее!\n"
                                  "Traveling together is more fun and safer!";
    joinConvoyChoice.energyChange = 10.0f;
    fellowTravelerEvent.choices.push_back(joinConvoyChoice);
    
    EventChoice travelAloneChoice;
    travelAloneChoice.text = "Ехать одному / Travel alone";
    travelAloneChoice.outcomeText = "Вы предпочитаете одиночество.\n"
                                   "You prefer solitude.";
    fellowTravelerEvent.choices.push_back(travelAloneChoice);
    
    addEvent(fellowTravelerEvent);
    
    // Event 12: Foggy Road - Weather
    GameEvent fogEvent;
    fogEvent.id = "thick_fog";
    fogEvent.title = "Густой туман / Thick Fog";
    fogEvent.description = "Опустился густой туман. Видимость менее 20 метров.\n"
                          "Thick fog descended. Visibility less than 20 meters.";
    fogEvent.condition.probability = 0.3f;
    
    EventChoice crawlChoice;
    crawlChoice.text = "Ползти медленно / Crawl slowly";
    crawlChoice.outcomeText = "Вы осторожно едете. Очень медленно, но безопасно.\n"
                             "You drive carefully. Very slow but safe.";
    crawlChoice.energyChange = -15.0f;
    fogEvent.choices.push_back(crawlChoice);
    
    EventChoice waitFogChoice;
    waitFogChoice.text = "Переждать туман / Wait for fog to clear";
    waitFogChoice.outcomeText = "Вы ждете час. Туман немного рассеялся.\n"
                               "You wait an hour. Fog cleared a bit.";
    waitFogChoice.energyChange = 5.0f;
    fogEvent.choices.push_back(waitFogChoice);
    
    addEvent(fogEvent);
    
    // Event 13: Suspicious Character - Social/Risk
    GameEvent suspiciousEvent;
    suspiciousEvent.id = "suspicious_character";
    suspiciousEvent.title = "Подозрительный тип / Suspicious Character";
    suspiciousEvent.description = "На заправке подозрительный человек просит подвезти.\n"
                                  "At gas station, suspicious person asks for a ride.";
    suspiciousEvent.condition.probability = 0.25f;
    
    EventChoice helpChoice;
    helpChoice.text = "Помочь / Help";
    helpChoice.outcomeText = "Оказалось, это честный человек. Он поблагодарил вас.\n"
                            "Turned out to be an honest person. He thanked you.";
    helpChoice.moneyChange = 150;
    helpChoice.energyChange = -10.0f;
    suspiciousEvent.choices.push_back(helpChoice);
    
    EventChoice refuseChoice;
    refuseChoice.text = "Отказать / Refuse";
    refuseChoice.outcomeText = "Вы отказались. Лучше перестраховаться.\n"
                              "You refused. Better safe than sorry.";
    suspiciousEvent.choices.push_back(refuseChoice);
    
    addEvent(suspiciousEvent);
    
    // Event 14: Fuel Price Surge - Economic
    GameEvent fuelPriceEvent;
    fuelPriceEvent.id = "fuel_price_surge";
    fuelPriceEvent.title = "Взлет цен на бензин / Fuel Price Surge";
    fuelPriceEvent.description = "На заправке цены выросли! Бензин дороже на 50%.\n"
                                "At gas station, prices went up! Fuel 50% more expensive.";
    fuelPriceEvent.condition.minFuel = 0.0f;
    fuelPriceEvent.condition.maxFuel = 40.0f;
    fuelPriceEvent.condition.probability = 0.3f;
    
    EventChoice buyExpensiveFuelChoice;
    buyExpensiveFuelChoice.text = "Купить дорогой бензин (750 руб) / Buy expensive fuel (750 rub)";
    buyExpensiveFuelChoice.outcomeText = "Дорого, но нужно ехать дальше.\n"
                                        "Expensive, but need to keep going.";
    buyExpensiveFuelChoice.fuelChange = 50.0f;
    buyExpensiveFuelChoice.moneyChange = -750;
    fuelPriceEvent.choices.push_back(buyExpensiveFuelChoice);
    
    EventChoice findCheaperChoice;
    findCheaperChoice.text = "Искать дешевле / Search for cheaper";
    findCheaperChoice.outcomeText = "Вы нашли заправку подешевле через 10 км.\n"
                                   "You found cheaper station 10 km away.";
    findCheaperChoice.fuelChange = -5.0f;
    findCheaperChoice.energyChange = -10.0f;
    fuelPriceEvent.choices.push_back(findCheaperChoice);
    
    addEvent(fuelPriceEvent);
    
    // Event 15: Wildlife Crossing - Nature
    GameEvent wildlifeEvent;
    wildlifeEvent.id = "wildlife_crossing";
    wildlifeEvent.title = "Дикие животные / Wildlife Crossing";
    wildlifeEvent.description = "Стадо лосей переходит дорогу!\n"
                               "A herd of moose is crossing the road!";
    wildlifeEvent.condition.probability = 0.2f;
    
    EventChoice stopSafelyChoice;
    stopSafelyChoice.text = "Остановиться и подождать / Stop and wait";
    stopSafelyChoice.outcomeText = "Вы безопасно подождали. Лоси прошли мимо.\n"
                                  "You safely waited. Moose passed by.";
    stopSafelyChoice.energyChange = -5.0f;
    wildlifeEvent.choices.push_back(stopSafelyChoice);
    
    EventChoice honkChoice;
    honkChoice.text = "Посигналить / Honk horn";
    honkChoice.outcomeText = "Лоси испугались и убежали. Путь свободен.\n"
                            "Moose got scared and ran away. Road is clear.";
    wildlifeEvent.choices.push_back(honkChoice);
    
    addEvent(wildlifeEvent);
    
    // Event 16: Pothole Damage - Mechanical
    GameEvent potholeEvent;
    potholeEvent.id = "pothole_damage";
    potholeEvent.title = "Яма на дороге / Pothole";
    potholeEvent.description = "БУМ! Вы въехали в большую яму. Подвеска скрипит.\n"
                              "BOOM! You hit a big pothole. Suspension is creaking.";
    potholeEvent.condition.probability = 0.4f;
    
    EventChoice inspectDamageChoice;
    inspectDamageChoice.text = "Осмотреть повреждения / Inspect damage";
    inspectDamageChoice.outcomeText = "Небольшие повреждения. Можно ехать осторожно.\n"
                                     "Minor damage. Can drive carefully.";
    inspectDamageChoice.energyChange = -10.0f;
    potholeEvent.choices.push_back(inspectDamageChoice);
    
    EventChoice repairSuspensionChoice;
    repairSuspensionChoice.text = "Ремонт подвески (600 руб) / Repair suspension (600 rub)";
    repairSuspensionChoice.outcomeText = "Механик починил подвеску. Едет как новая!\n"
                                        "Mechanic fixed suspension. Rides like new!";
    repairSuspensionChoice.moneyChange = -600;
    repairSuspensionChoice.energyChange = 10.0f;
    potholeEvent.choices.push_back(repairSuspensionChoice);
    
    EventChoice continueAnywayChoice;
    continueAnywayChoice.text = "Ехать как есть / Continue as is";
    continueAnywayChoice.outcomeText = "Вы продолжили путь. Машина трясется.\n"
                                      "You continued. Car is shaking.";
    continueAnywayChoice.energyChange = -20.0f;
    potholeEvent.choices.push_back(continueAnywayChoice);
    
    addEvent(potholeEvent);
    
    // Event 17: Night Driving - Time/Weather
    GameEvent nightEvent;
    nightEvent.id = "night_driving";
    nightEvent.title = "Ночная дорога / Night Driving";
    nightEvent.description = "Солнце село. Дорога темная и пустынная.\n"
                            "Sun has set. Road is dark and deserted.";
    nightEvent.condition.probability = 0.5f;
    
    EventChoice driveNightChoice;
    driveNightChoice.text = "Ехать ночью / Drive at night";
    driveNightChoice.outcomeText = "Вы устали, но доехали до следующего города.\n"
                                  "You're tired but made it to next town.";
    driveNightChoice.energyChange = -25.0f;
    nightEvent.choices.push_back(driveNightChoice);
    
    EventChoice findMotelChoice;
    findMotelChoice.text = "Найти мотель (300 руб) / Find motel (300 rub)";
    findMotelChoice.outcomeText = "Отличный сон! Вы полны сил.\n"
                                 "Great sleep! You're full of energy.";
    findMotelChoice.moneyChange = -300;
    findMotelChoice.energyChange = 30.0f;
    nightEvent.choices.push_back(findMotelChoice);
    
    EventChoice sleepInCarChoice;
    sleepInCarChoice.text = "Спать в машине / Sleep in car";
    sleepInCarChoice.outcomeText = "Неудобно, но вы немного отдохнули.\n"
                                  "Uncomfortable, but you rested a bit.";
    sleepInCarChoice.energyChange = 15.0f;
    nightEvent.choices.push_back(sleepInCarChoice);
    
    addEvent(nightEvent);
    
    // Event 18: Snowy Road - Weather/Seasonal
    GameEvent snowEvent;
    snowEvent.id = "snowy_road";
    snowEvent.title = "Снежная дорога / Snowy Road";
    snowEvent.description = "Начался снегопад. Дорога скользкая.\n"
                           "Snowfall started. Road is slippery.";
    snowEvent.condition.probability = 0.35f;
    
    EventChoice slowSnowChoice;
    slowSnowChoice.text = "Ехать медленно / Drive slowly";
    slowSnowChoice.outcomeText = "Вы осторожно едете по снегу.\n"
                                "You carefully drive through snow.";
    slowSnowChoice.energyChange = -15.0f;
    snowEvent.choices.push_back(slowSnowChoice);
    
    EventChoice chainsChoice;
    chainsChoice.text = "Надеть цепи (200 руб) / Put on chains (200 rub)";
    chainsChoice.outcomeText = "С цепями ехать безопаснее!\n"
                              "With chains it's safer!";
    chainsChoice.moneyChange = -200;
    chainsChoice.energyChange = -5.0f;
    snowEvent.choices.push_back(chainsChoice);
    
    EventChoice waitSnowChoice;
    waitSnowChoice.text = "Переждать / Wait it out";
    waitSnowChoice.outcomeText = "Вы подождали. Снег прекратился.\n"
                                "You waited. Snow stopped.";
    waitSnowChoice.energyChange = 0.0f;
    snowEvent.choices.push_back(waitSnowChoice);
    
    addEvent(snowEvent);
    
    // Event 19: Mechanical Help - Social/Mechanical
    GameEvent helpEvent;
    helpEvent.id = "help_stranded_driver";
    helpEvent.title = "Застрявший водитель / Stranded Driver";
    helpEvent.description = "На обочине сломанная машина. Водитель просит помощи.\n"
                           "Broken down car on roadside. Driver asks for help.";
    helpEvent.condition.probability = 0.4f;
    
    EventChoice helpDriverChoice;
    helpDriverChoice.text = "Помочь / Help";
    helpDriverChoice.outcomeText = "Вы помогли завести машину. Водитель благодарен!\n"
                                  "You helped start the car. Driver is grateful!";
    helpDriverChoice.moneyChange = 300;
    helpDriverChoice.energyChange = -15.0f;
    helpEvent.choices.push_back(helpDriverChoice);
    
    EventChoice callTowTruckChoice;
    callTowTruckChoice.text = "Вызвать эвакуатор для него / Call tow truck for him";
    callTowTruckChoice.outcomeText = "Вы позвонили в службу. Помощь едет.\n"
                                    "You called service. Help is coming.";
    callTowTruckChoice.energyChange = -5.0f;
    helpEvent.choices.push_back(callTowTruckChoice);
    
    EventChoice passHelpChoice;
    passHelpChoice.text = "Проехать мимо / Pass by";
    passHelpChoice.outcomeText = "Вы решили не останавливаться.\n"
                                "You decided not to stop.";
    helpEvent.choices.push_back(passHelpChoice);
    
    addEvent(helpEvent);
    
    // Event 20: Lucky Find - Random Positive
    GameEvent luckyEvent;
    luckyEvent.id = "lucky_find";
    luckyEvent.title = "Удачная находка / Lucky Find";
    luckyEvent.description = "На стоянке вы нашли потерянный кошелек с деньгами.\n"
                            "At parking lot you found a lost wallet with money.";
    luckyEvent.condition.probability = 0.15f;
    
    EventChoice keepMoneyChoice;
    keepMoneyChoice.text = "Оставить деньги себе / Keep the money";
    keepMoneyChoice.outcomeText = "Вы нашли 500 рублей. Повезло!\n"
                                 "You found 500 rubles. Lucky!";
    keepMoneyChoice.moneyChange = 500;
    luckyEvent.choices.push_back(keepMoneyChoice);
    
    EventChoice returnWalletChoice;
    returnWalletChoice.text = "Вернуть хозяину / Return to owner";
    returnWalletChoice.outcomeText = "Хозяин нашелся и дал вам 300 рублей награды.\n"
                                    "Owner found and gave you 300 rubles reward.";
    returnWalletChoice.moneyChange = 300;
    returnWalletChoice.energyChange = 10.0f;
    luckyEvent.choices.push_back(returnWalletChoice);
    
    addEvent(luckyEvent);
    
    // Event 21: Mechanic Hitchhiker - Recruitable NPC
    GameEvent mechanicRecruitEvent;
    mechanicRecruitEvent.id = "hitchhiker_mechanic";
    mechanicRecruitEvent.title = "Автостоп: Механик / Hitchhiker: Mechanic";
    mechanicRecruitEvent.description = "На обочине стоит Михаил с поднятым большим пальцем. Рядом лежит ящик с инструментами.\n"
                                      "On the roadside stands Mikhail with his thumb up. A toolbox lies nearby.";
    mechanicRecruitEvent.condition.probability = 0.25f;
    
    EventChoice recruitMechanicChoice;
    recruitMechanicChoice.text = "Подобрать и взять в команду / Pick up and invite to team";
    recruitMechanicChoice.outcomeText = "Михаил: 'Спасибо! Я механик, помогу с машиной!' Теперь у вас есть личный механик.\n"
                                       "Mikhail: 'Thanks! I'm a mechanic, I'll help with the car!' Now you have a personal mechanic.";
    mechanicRecruitEvent.choices.push_back(recruitMechanicChoice);
    
    EventChoice talkMechanicChoice;
    talkMechanicChoice.text = "Поговорить / Talk";
    talkMechanicChoice.outcomeText = "Михаил рассказал интересные истории о ремонте машин.\n"
                                    "Mikhail told interesting stories about car repairs.";
    talkMechanicChoice.energyChange = 5.0f;
    mechanicRecruitEvent.choices.push_back(talkMechanicChoice);
    
    EventChoice leaveMechanicChoice;
    leaveMechanicChoice.text = "Проехать мимо / Drive past";
    leaveMechanicChoice.outcomeText = "Вы проехали мимо.\n"
                                     "You drove past.";
    mechanicRecruitEvent.choices.push_back(leaveMechanicChoice);
    
    addEvent(mechanicRecruitEvent);
    
    // Event 22: Trader Hitchhiker - Recruitable NPC
    GameEvent traderRecruitEvent;
    traderRecruitEvent.id = "hitchhiker_trader";
    traderRecruitEvent.title = "Автостоп: Торговка / Hitchhiker: Trader";
    traderRecruitEvent.description = "На обочине стоит Сара с большим рюкзаком. Она голосует попутку.\n"
                                    "On the roadside stands Sarah with a large backpack. She's hitchhiking.";
    traderRecruitEvent.condition.probability = 0.25f;
    
    EventChoice recruitTraderChoice;
    recruitTraderChoice.text = "Подобрать и взять в команду / Pick up and invite to team";
    recruitTraderChoice.outcomeText = "Сара: 'Отлично! Я торговка, знаю где купить дёшево!' Сара присоединилась.\n"
                                     "Sarah: 'Great! I'm a trader, I know where to buy cheap!' Sarah joined.";
    traderRecruitEvent.choices.push_back(recruitTraderChoice);
    
    EventChoice talkTraderChoice;
    talkTraderChoice.text = "Поговорить / Talk";
    talkTraderChoice.outcomeText = "Сара рассказала о выгодных торговых путях.\n"
                                   "Sarah told about profitable trade routes.";
    talkTraderChoice.energyChange = 5.0f;
    traderRecruitEvent.choices.push_back(talkTraderChoice);
    
    EventChoice leaveTraderChoice;
    leaveTraderChoice.text = "Проехать мимо / Drive past";
    leaveTraderChoice.outcomeText = "Вы проехали мимо.\n"
                                   "You drove past.";
    traderRecruitEvent.choices.push_back(leaveTraderChoice);
    
    addEvent(traderRecruitEvent);
    
    // Event 23: Navigator Hitchhiker - Recruitable NPC
    GameEvent navigatorRecruitEvent;
    navigatorRecruitEvent.id = "hitchhiker_navigator";
    navigatorRecruitEvent.title = "Автостоп: Штурман / Hitchhiker: Navigator";
    navigatorRecruitEvent.description = "На обочине стоит Джек с картой в руках. Бывший дальнобойщик голосует.\n"
                                       "On the roadside stands Jack with a map in hands. Former truck driver hitchhiking.";
    navigatorRecruitEvent.condition.probability = 0.25f;
    
    EventChoice recruitNavigatorChoice;
    recruitNavigatorChoice.text = "Подобрать и взять в команду / Pick up and invite to team";
    recruitNavigatorChoice.outcomeText = "Джек: 'Спасибо, друг! Знаю все дороги!' Джек присоединился к команде.\n"
                                        "Jack: 'Thanks, buddy! I know all roads!' Jack joined the team.";
    navigatorRecruitEvent.choices.push_back(recruitNavigatorChoice);
    
    EventChoice askDirectionsChoice;
    askDirectionsChoice.text = "Спросить дорогу / Ask for directions";
    askDirectionsChoice.outcomeText = "Джек дал полезные советы о маршруте.\n"
                                     "Jack gave useful advice about the route.";
    askDirectionsChoice.energyChange = 10.0f;
    navigatorRecruitEvent.choices.push_back(askDirectionsChoice);
    
    EventChoice leaveNavigatorChoice;
    leaveNavigatorChoice.text = "Проехать мимо / Drive past";
    leaveNavigatorChoice.outcomeText = "Вы проехали мимо.\n"
                                      "You drove past.";
    navigatorRecruitEvent.choices.push_back(leaveNavigatorChoice);
    
    addEvent(navigatorRecruitEvent);
    
    // Event 24: Medic Hitchhiker - Recruitable NPC
    GameEvent medicRecruitEvent;
    medicRecruitEvent.id = "hitchhiker_medic";
    medicRecruitEvent.title = "Автостоп: Врач / Hitchhiker: Medic";
    medicRecruitEvent.description = "На обочине стоит Эмма с медицинской сумкой. Она путешествует и помогает людям.\n"
                                   "On the roadside stands Emma with a medical bag. She travels and helps people.";
    medicRecruitEvent.condition.probability = 0.25f;
    
    EventChoice recruitMedicChoice;
    recruitMedicChoice.text = "Подобрать и взять в команду / Pick up and invite to team";
    recruitMedicChoice.outcomeText = "Эмма: 'Спасибо! Я врач, помогу всем!' Эмма присоединилась к команде.\n"
                                    "Emma: 'Thanks! I'm a doctor, I'll help everyone!' Emma joined the team.";
    medicRecruitEvent.choices.push_back(recruitMedicChoice);
    
    EventChoice getFirstAidChoice;
    getFirstAidChoice.text = "Попросить аптечку / Ask for first aid";
    getFirstAidChoice.outcomeText = "Эмма дала вам лекарства. Вы чувствуете себя лучше.\n"
                                   "Emma gave you medicine. You feel better.";
    getFirstAidChoice.energyChange = 20.0f;
    medicRecruitEvent.choices.push_back(getFirstAidChoice);
    
    EventChoice leaveMedicChoice;
    leaveMedicChoice.text = "Проехать мимо / Drive past";
    leaveMedicChoice.outcomeText = "Вы проехали мимо.\n"
                                  "You drove past.";
    medicRecruitEvent.choices.push_back(leaveMedicChoice);
    
    addEvent(medicRecruitEvent);
    
    // Event 25: Companion Hitchhiker - Recruitable NPC
    GameEvent companionRecruitEvent;
    companionRecruitEvent.id = "hitchhiker_companion";
    companionRecruitEvent.title = "Автостоп: Попутчик / Hitchhiker: Companion";
    companionRecruitEvent.description = "На обочине стоит Алекс с рюкзаком. Весёлый парень путешествует автостопом.\n"
                                       "On the roadside stands Alex with a backpack. Cheerful guy hitchhiking.";
    companionRecruitEvent.condition.probability = 0.30f;
    
    EventChoice recruitCompanionChoice;
    recruitCompanionChoice.text = "Подобрать и взять в команду / Pick up and invite to team";
    recruitCompanionChoice.outcomeText = "Алекс: 'Ура! Вместе веселее!' Алекс присоединился к команде.\n"
                                        "Alex: 'Hooray! Together is more fun!' Alex joined the team.";
    companionRecruitEvent.choices.push_back(recruitCompanionChoice);
    
    EventChoice chatCompanionChoice;
    chatCompanionChoice.text = "Поболтать / Chat";
    chatCompanionChoice.outcomeText = "Алекс рассказал забавные истории о своих путешествиях.\n"
                                     "Alex told funny stories about his travels.";
    chatCompanionChoice.energyChange = 15.0f;
    companionRecruitEvent.choices.push_back(chatCompanionChoice);
    
    EventChoice leaveCompanionChoice;
    leaveCompanionChoice.text = "Проехать мимо / Drive past";
    leaveCompanionChoice.outcomeText = "Вы проехали мимо.\n"
                                      "You drove past.";
    companionRecruitEvent.choices.push_back(leaveCompanionChoice);
    
    addEvent(companionRecruitEvent);

    // ====== Road-Specific Events ======

    // Highway: Speed trap
    GameEvent speedTrapEvent;
    speedTrapEvent.id = "highway_speed_trap";
    speedTrapEvent.title = "Камера контроля скорости / Speed Camera";
    speedTrapEvent.description = "Вы заметили камеру контроля скорости на шоссе!\n"
                                 "You noticed a speed camera on the highway!";
    speedTrapEvent.condition.requiredRoadType = "highway";
    speedTrapEvent.condition.probability = 0.2f;

    EventChoice speedCameraSlowChoice;
    speedCameraSlowChoice.text = "Сбросить скорость / Slow down";
    speedCameraSlowChoice.outcomeText = "Вы благополучно проехали камеру.\n"
                                        "You safely passed the camera.";
    speedCameraSlowChoice.energyChange = -5.0f;  // Time cost
    speedTrapEvent.choices.push_back(speedCameraSlowChoice);

    EventChoice speedCameraKeepChoice;
    speedCameraKeepChoice.text = "Продолжить с той же скоростью / Keep speed";
    speedCameraKeepChoice.outcomeText = "Камера зафиксировала превышение! Штраф придет по почте.\n"
                                        "Camera caught you speeding! Fine will arrive by mail.";
    speedCameraKeepChoice.moneyChange = -200;
    speedTrapEvent.choices.push_back(speedCameraKeepChoice);

    addEvent(speedTrapEvent);

    // Path: Muddy road
    GameEvent muddyRoadEvent;
    muddyRoadEvent.id = "path_muddy_road";
    muddyRoadEvent.title = "Грязная дорога / Muddy Road";
    muddyRoadEvent.description = "Дорога превратилась в месиво после дождя.\n"
                                 "The road turned into a muddy mess after rain.";
    muddyRoadEvent.condition.requiredRoadType = "path";
    muddyRoadEvent.condition.probability = 0.3f;

    EventChoice carefulChoice;
    carefulChoice.text = "Осторожно проехать / Drive carefully";
    carefulChoice.outcomeText = "Вы медленно, но успешно преодолели грязевой участок.\n"
                                "You slowly but successfully passed the muddy section.";
    carefulChoice.fuelChange = -10.0f;  // More fuel due to difficult terrain
    carefulChoice.energyChange = -5.0f;
    muddyRoadEvent.choices.push_back(carefulChoice);

    EventChoice pushThroughChoice;
    pushThroughChoice.text = "Продавить на скорости / Push through fast";
    pushThroughChoice.outcomeText = "Вы прорвались, но машина вся в грязи и немного повреждена.\n"
                                    "You made it through, but the car is muddy and slightly damaged.";
    pushThroughChoice.fuelChange = -15.0f;
    pushThroughChoice.vehicleConditionChange = -10.0f;
    muddyRoadEvent.choices.push_back(pushThroughChoice);

    addEvent(muddyRoadEvent);

    // Road: Truck convoy
    GameEvent truckConvoyEvent;
    truckConvoyEvent.id = "road_truck_convoy";
    truckConvoyEvent.title = "Колонна грузовиков / Truck Convoy";
    truckConvoyEvent.description = "Впереди медленная колонна грузовиков.\n"
                                   "A slow truck convoy is ahead.";
    truckConvoyEvent.condition.requiredRoadType = "road";
    truckConvoyEvent.condition.probability = 0.25f;

    EventChoice followChoice;
    followChoice.text = "Ехать за колонной / Follow the convoy";
    followChoice.outcomeText = "Вы терпеливо едете за грузовиками.\n"
                               "You patiently drive behind the trucks.";
    followChoice.energyChange = -3.0f;  // Boring but safe
    truckConvoyEvent.choices.push_back(followChoice);

    EventChoice overtakeChoice;
    overtakeChoice.text = "Обогнать / Overtake";
    overtakeChoice.outcomeText = "Рискованный маневр, но вы обогнали колонну!\n"
                                 "Risky maneuver, but you overtook the convoy!";
    overtakeChoice.fuelChange = -8.0f;  // More fuel for acceleration
    overtakeChoice.energyChange = -2.0f;
    truckConvoyEvent.choices.push_back(overtakeChoice);

    addEvent(truckConvoyEvent);

    // Highway: Rest stop
    GameEvent restStopEvent;
    restStopEvent.id = "highway_rest_stop";
    restStopEvent.title = "Придорожная остановка / Rest Stop";
    restStopEvent.description = "Вы видите современную придорожную остановку с кафе.\n"
                                "You see a modern rest stop with a cafe.";
    restStopEvent.condition.requiredRoadType = "highway";
    restStopEvent.condition.probability = 0.15f;

    EventChoice stopForCoffeeChoice;
    stopForCoffeeChoice.text = "Остановиться на кофе / Stop for coffee";
    stopForCoffeeChoice.outcomeText = "Горячий кофе взбодрил вас!\n"
                                      "Hot coffee refreshed you!";
    stopForCoffeeChoice.moneyChange = -50;
    stopForCoffeeChoice.energyChange = 15.0f;
    restStopEvent.choices.push_back(stopForCoffeeChoice);

    EventChoice keepDrivingChoice;
    keepDrivingChoice.text = "Продолжить путь / Keep driving";
    keepDrivingChoice.outcomeText = "Вы решили не останавливаться.\n"
                                    "You decided not to stop.";
    restStopEvent.choices.push_back(keepDrivingChoice);

    addEvent(restStopEvent);

    // ====== NEW EVENTS - Extended System ======

    // Event 26: Bridge Collapse Warning - Path specific
    GameEvent bridgeEvent;
    bridgeEvent.id = "path_bridge_warning";
    bridgeEvent.title = "Опасный мост / Dangerous Bridge";
    bridgeEvent.description = "Старый деревянный мост выглядит ненадежно. Видны трещины.\n"
                              "Old wooden bridge looks unreliable. Cracks are visible.";
    bridgeEvent.condition.requiredRoadType = "path";
    bridgeEvent.condition.probability = 0.25f;

    EventChoice crossSlowlyChoice;
    crossSlowlyChoice.text = "Переехать медленно / Cross slowly";
    crossSlowlyChoice.outcomeText = "Вы осторожно переехали. Мост скрипел, но выдержал.\n"
                                    "You crossed carefully. Bridge creaked but held.";
    crossSlowlyChoice.energyChange = -10.0f;
    bridgeEvent.choices.push_back(crossSlowlyChoice);

    EventChoice findDetourChoice;
    findDetourChoice.text = "Найти объезд / Find detour";
    findDetourChoice.outcomeText = "Вы нашли другой путь. Дольше, но безопаснее.\n"
                                   "You found another way. Longer but safer.";
    findDetourChoice.fuelChange = -15.0f;
    findDetourChoice.energyChange = -5.0f;
    bridgeEvent.choices.push_back(findDetourChoice);

    EventChoice riskCrossChoice;
    riskCrossChoice.text = "Рискнуть на скорости / Risk crossing fast";
    riskCrossChoice.outcomeText = "ОПАСНО! Мост треснул, но вы успели! Адреналин!\n"
                                  "DANGEROUS! Bridge cracked but you made it! Adrenaline!";
    riskCrossChoice.energyChange = -20.0f;
    riskCrossChoice.vehicleConditionChange = -15.0f;
    bridgeEvent.choices.push_back(riskCrossChoice);

    addEvent(bridgeEvent);

    // Event 27: Highway Construction - Highway specific
    GameEvent constructionEvent;
    constructionEvent.id = "highway_construction";
    constructionEvent.title = "Дорожные работы / Road Construction";
    constructionEvent.description = "Впереди ремонт дороги. Движение по одной полосе.\n"
                                    "Road work ahead. Traffic moving in one lane.";
    constructionEvent.condition.requiredRoadType = "highway";
    constructionEvent.condition.probability = 0.3f;

    EventChoice waitInLineChoice;
    waitInLineChoice.text = "Стоять в пробке / Wait in traffic";
    waitInLineChoice.outcomeText = "Вы простояли 30 минут. Утомительно.\n"
                                   "You waited 30 minutes. Tiring.";
    waitInLineChoice.energyChange = -15.0f;
    constructionEvent.choices.push_back(waitInLineChoice);

    EventChoice talkToWorkersChoice;
    talkToWorkersChoice.text = "Поговорить с рабочими / Talk to workers";
    talkToWorkersChoice.outcomeText = "Рабочие показали короткий объезд!\n"
                                      "Workers showed you a shortcut!";
    talkToWorkersChoice.energyChange = -5.0f;
    constructionEvent.choices.push_back(talkToWorkersChoice);

    addEvent(constructionEvent);

    // Event 28: Stray Dogs - Road specific
    GameEvent dogsEvent;
    dogsEvent.id = "road_stray_dogs";
    dogsEvent.title = "Стая бродячих собак / Stray Dogs Pack";
    dogsEvent.description = "На дороге стая собак. Они не агрессивны, но блокируют путь.\n"
                           "A pack of dogs on the road. Not aggressive but blocking the way.";
    dogsEvent.condition.requiredRoadType = "road";
    dogsEvent.condition.probability = 0.2f;

    EventChoice honkDogsChoice;
    honkDogsChoice.text = "Посигналить / Honk";
    honkDogsChoice.outcomeText = "Собаки убежали с дороги.\n"
                                 "Dogs ran off the road.";
    dogsEvent.choices.push_back(honkDogsChoice);

    EventChoice feedDogsChoice;
    feedDogsChoice.text = "Покормить (50 руб) / Feed them (50 rub)";
    feedDogsChoice.outcomeText = "Собаки радостно съели еду и ушли. Вы чувствуете себя хорошо.\n"
                                 "Dogs happily ate and left. You feel good.";
    feedDogsChoice.moneyChange = -50;
    feedDogsChoice.energyChange = 10.0f;
    dogsEvent.choices.push_back(feedDogsChoice);

    EventChoice driveAroundChoice;
    driveAroundChoice.text = "Объехать по обочине / Drive around";
    driveAroundChoice.outcomeText = "Вы объехали собак по грунту.\n"
                                    "You drove around on dirt.";
    driveAroundChoice.vehicleConditionChange = -5.0f;
    dogsEvent.choices.push_back(driveAroundChoice);

    addEvent(dogsEvent);

    // Event 29: Abandoned Car - General
    GameEvent abandonedCarEvent;
    abandonedCarEvent.id = "abandoned_car";
    abandonedCarEvent.title = "Брошенная машина / Abandoned Car";
    abandonedCarEvent.description = "На обочине стоит старая брошенная машина.\n"
                                    "An old abandoned car sits on the roadside.";
    abandonedCarEvent.condition.probability = 0.2f;

    EventChoice searchCarChoice;
    searchCarChoice.text = "Обыскать машину / Search the car";
    searchCarChoice.outcomeText = "Вы нашли канистру с бензином и немного денег!\n"
                                  "You found a gas canister and some money!";
    searchCarChoice.fuelChange = 15.0f;
    searchCarChoice.moneyChange = 150;
    searchCarChoice.energyChange = -5.0f;
    abandonedCarEvent.choices.push_back(searchCarChoice);

    EventChoice takePartsChoice;
    takePartsChoice.text = "Снять запчасти / Take spare parts";
    takePartsChoice.outcomeText = "Вы сняли полезные детали. Можно продать позже.\n"
                                  "You took useful parts. Can sell later.";
    takePartsChoice.moneyChange = 300;
    takePartsChoice.energyChange = -15.0f;
    abandonedCarEvent.choices.push_back(takePartsChoice);

    EventChoice ignoreCarChoice;
    ignoreCarChoice.text = "Проехать мимо / Drive past";
    ignoreCarChoice.outcomeText = "Вы решили не останавливаться.\n"
                                  "You decided not to stop.";
    abandonedCarEvent.choices.push_back(ignoreCarChoice);

    addEvent(abandonedCarEvent);

    // Event 30: Radio Contest - General
    GameEvent radioContestEvent;
    radioContestEvent.id = "radio_contest";
    radioContestEvent.title = "Конкурс на радио / Radio Contest";
    radioContestEvent.description = "По радио объявили конкурс! Позвоните и ответьте на вопрос.\n"
                                    "A radio contest announced! Call and answer the question.";
    radioContestEvent.condition.probability = 0.15f;

    EventChoice callRadioChoice;
    callRadioChoice.text = "Позвонить / Call in";
    callRadioChoice.outcomeText = "Вы ответили правильно и выиграли 1000 рублей!\n"
                                  "You answered correctly and won 1000 rubles!";
    callRadioChoice.moneyChange = 1000;
    callRadioChoice.energyChange = 5.0f;
    radioContestEvent.choices.push_back(callRadioChoice);

    EventChoice ignoreRadioChoice;
    ignoreRadioChoice.text = "Не звонить / Don't call";
    ignoreRadioChoice.outcomeText = "Вы продолжили слушать музыку.\n"
                                    "You continued listening to music.";
    radioContestEvent.choices.push_back(ignoreRadioChoice);

    addEvent(radioContestEvent);

    // Event 31: Scenic Viewpoint - Highway specific
    GameEvent viewpointEvent;
    viewpointEvent.id = "highway_scenic_viewpoint";
    viewpointEvent.title = "Смотровая площадка / Scenic Viewpoint";
    viewpointEvent.description = "Указатель на красивую смотровую площадку.\n"
                                 "Sign pointing to a beautiful scenic viewpoint.";
    viewpointEvent.condition.requiredRoadType = "highway";
    viewpointEvent.condition.probability = 0.2f;

    EventChoice visitViewpointChoice;
    visitViewpointChoice.text = "Посетить / Visit";
    visitViewpointChoice.outcomeText = "Потрясающий вид! Вы отдохнули и сделали фото.\n"
                                       "Amazing view! You rested and took photos.";
    visitViewpointChoice.energyChange = 20.0f;
    viewpointEvent.choices.push_back(visitViewpointChoice);

    EventChoice skipViewpointChoice;
    skipViewpointChoice.text = "Ехать дальше / Drive on";
    skipViewpointChoice.outcomeText = "Некогда любоваться видами.\n"
                                      "No time for sightseeing.";
    viewpointEvent.choices.push_back(skipViewpointChoice);

    addEvent(viewpointEvent);

    // Event 32: Farmers Market - Road specific
    GameEvent farmersMarketEvent;
    farmersMarketEvent.id = "road_farmers_market";
    farmersMarketEvent.title = "Фермерский рынок / Farmers Market";
    farmersMarketEvent.description = "Придорожный фермерский рынок. Свежие продукты!\n"
                                     "Roadside farmers market. Fresh produce!";
    farmersMarketEvent.condition.requiredRoadType = "road";
    farmersMarketEvent.condition.probability = 0.25f;

    EventChoice buyFoodChoice;
    buyFoodChoice.text = "Купить еду (200 руб) / Buy food (200 rub)";
    buyFoodChoice.outcomeText = "Свежие овощи и хлеб! Очень вкусно и полезно.\n"
                                "Fresh vegetables and bread! Very tasty and healthy.";
    buyFoodChoice.moneyChange = -200;
    buyFoodChoice.energyChange = 25.0f;
    farmersMarketEvent.choices.push_back(buyFoodChoice);

    EventChoice bargainChoice;
    bargainChoice.text = "Поторговаться / Bargain";
    bargainChoice.outcomeText = "Продавец дал скидку! Вы купили еду дешевле.\n"
                                "Seller gave discount! You bought food cheaper.";
    bargainChoice.moneyChange = -100;
    bargainChoice.energyChange = 20.0f;
    farmersMarketEvent.choices.push_back(bargainChoice);

    EventChoice skipMarketChoice;
    skipMarketChoice.text = "Ехать дальше / Drive on";
    skipMarketChoice.outcomeText = "Вы продолжили путь.\n"
                                   "You continued on.";
    farmersMarketEvent.choices.push_back(skipMarketChoice);

    addEvent(farmersMarketEvent);

    // Event 33: Rockslide - Path specific
    GameEvent rockslideEvent;
    rockslideEvent.id = "path_rockslide";
    rockslideEvent.title = "Камнепад / Rockslide";
    rockslideEvent.description = "Камни упали на дорогу! Путь частично перекрыт.\n"
                                 "Rocks fell on the road! Path partially blocked.";
    rockslideEvent.condition.requiredRoadType = "path";
    rockslideEvent.condition.probability = 0.2f;

    EventChoice clearRocksChoice;
    clearRocksChoice.text = "Расчистить путь / Clear the path";
    clearRocksChoice.outcomeText = "Вы убрали камни. Очень устали, но путь свободен.\n"
                                   "You cleared rocks. Very tired but path is clear.";
    clearRocksChoice.energyChange = -30.0f;
    rockslideEvent.choices.push_back(clearRocksChoice);

    EventChoice squeezeByChoice;
    squeezeByChoice.text = "Протиснуться / Squeeze by";
    squeezeByChoice.outcomeText = "Вы проехали между камнями. Машина поцарапана.\n"
                                  "You drove between rocks. Car is scratched.";
    squeezeByChoice.vehicleConditionChange = -10.0f;
    rockslideEvent.choices.push_back(squeezeByChoice);

    EventChoice turnBackChoice;
    turnBackChoice.text = "Вернуться назад / Turn back";
    turnBackChoice.outcomeText = "Вы развернулись. Придется искать другой путь.\n"
                                 "You turned back. Need to find another way.";
    turnBackChoice.fuelChange = -20.0f;
    turnBackChoice.energyChange = -10.0f;
    rockslideEvent.choices.push_back(turnBackChoice);

    addEvent(rockslideEvent);

    // Event 34: Street Racing - Highway specific
    GameEvent streetRacingEvent;
    streetRacingEvent.id = "highway_street_racing";
    streetRacingEvent.title = "Уличные гонщики / Street Racers";
    streetRacingEvent.description = "Группа молодых гонщиков вызывает вас на гонку!\n"
                                    "A group of young racers challenges you to a race!";
    streetRacingEvent.condition.requiredRoadType = "highway";
    streetRacingEvent.condition.probability = 0.15f;

    EventChoice acceptRaceChoice;
    acceptRaceChoice.text = "Принять вызов / Accept challenge";
    acceptRaceChoice.outcomeText = "Вы выиграли гонку! Гонщики уважают вас. Но машина пострадала.\n"
                                   "You won the race! Racers respect you. But car suffered.";
    acceptRaceChoice.moneyChange = 500;
    acceptRaceChoice.fuelChange = -30.0f;
    acceptRaceChoice.energyChange = -20.0f;
    acceptRaceChoice.vehicleConditionChange = -20.0f;
    streetRacingEvent.choices.push_back(acceptRaceChoice);

    EventChoice declineRaceChoice;
    declineRaceChoice.text = "Отказаться / Decline";
    declineRaceChoice.outcomeText = "Вы отказались от гонки. Безопасность важнее.\n"
                                    "You declined the race. Safety first.";
    streetRacingEvent.choices.push_back(declineRaceChoice);

    addEvent(streetRacingEvent);

    // Event 35: Mysterious Stranger - General
    GameEvent mysteriousStrangerEvent;
    mysteriousStrangerEvent.id = "mysterious_stranger";
    mysteriousStrangerEvent.title = "Таинственный незнакомец / Mysterious Stranger";
    mysteriousStrangerEvent.description = "На парковке человек в капюшоне предлагает выгодную сделку.\n"
                                          "At parking lot, a hooded figure offers a lucrative deal.";
    mysteriousStrangerEvent.condition.probability = 0.1f;

    EventChoice acceptDealChoice;
    acceptDealChoice.text = "Принять сделку / Accept deal";
    acceptDealChoice.outcomeText = "Сделка оказалась честной! Вы получили 800 рублей.\n"
                                   "Deal was legit! You got 800 rubles.";
    acceptDealChoice.moneyChange = 800;
    mysteriousStrangerEvent.choices.push_back(acceptDealChoice);

    EventChoice refuseDealChoice;
    refuseDealChoice.text = "Отказаться / Refuse";
    refuseDealChoice.outcomeText = "Лучше не рисковать с незнакомцами.\n"
                                   "Better not risk with strangers.";
    mysteriousStrangerEvent.choices.push_back(refuseDealChoice);

    addEvent(mysteriousStrangerEvent);

    // Event 36: Roadside Memorial - General
    GameEvent memorialEvent;
    memorialEvent.id = "roadside_memorial";
    memorialEvent.title = "Придорожный памятник / Roadside Memorial";
    memorialEvent.description = "Крест и цветы на обочине. Памятник погибшим в аварии.\n"
                                "Cross and flowers on roadside. Memorial for accident victims.";
    memorialEvent.condition.probability = 0.15f;

    EventChoice payRespectsChoice;
    payRespectsChoice.text = "Остановиться и почтить память / Stop and pay respects";
    payRespectsChoice.outcomeText = "Вы остановились на минуту молчания. Напоминание о безопасности.\n"
                                    "You stopped for a minute of silence. Reminder about safety.";
    payRespectsChoice.energyChange = -5.0f;
    memorialEvent.choices.push_back(payRespectsChoice);

    EventChoice drivePastMemorialChoice;
    drivePastMemorialChoice.text = "Проехать мимо / Drive past";
    drivePastMemorialChoice.outcomeText = "Вы продолжили путь.\n"
                                          "You continued on.";
    memorialEvent.choices.push_back(drivePastMemorialChoice);

    addEvent(memorialEvent);

    // Event 37: Fuel Smugglers - Path specific
    GameEvent smugglersEvent;
    smugglersEvent.id = "path_fuel_smugglers";
    smugglersEvent.title = "Контрабандисты / Fuel Smugglers";
    smugglersEvent.description = "На глухой дороге встретили контрабандистов. Предлагают дешевый бензин.\n"
                                 "On a remote road you met smugglers. They offer cheap gas.";
    smugglersEvent.condition.requiredRoadType = "path";
    smugglersEvent.condition.minFuel = 0.0f;
    smugglersEvent.condition.maxFuel = 40.0f;
    smugglersEvent.condition.probability = 0.15f;

    EventChoice buySmuggledFuelChoice;
    buySmuggledFuelChoice.text = "Купить бензин (250 руб) / Buy fuel (250 rub)";
    buySmuggledFuelChoice.outcomeText = "Дешево! Но качество под вопросом.\n"
                                        "Cheap! But quality is questionable.";
    buySmuggledFuelChoice.fuelChange = 40.0f;
    buySmuggledFuelChoice.moneyChange = -250;
    buySmuggledFuelChoice.vehicleConditionChange = -5.0f;  // Bad fuel
    smugglersEvent.choices.push_back(buySmuggledFuelChoice);

    EventChoice reportSmugglersChoice;
    reportSmugglersChoice.text = "Сообщить полиции / Report to police";
    reportSmugglersChoice.outcomeText = "Полиция благодарна. Вы получили награду.\n"
                                        "Police are grateful. You got reward.";
    reportSmugglersChoice.moneyChange = 400;
    smugglersEvent.choices.push_back(reportSmugglersChoice);

    EventChoice ignoreSmugglers;
    ignoreSmugglers.text = "Проехать мимо / Drive past";
    ignoreSmugglers.outcomeText = "Вы не стали связываться.\n"
                                  "You didn't get involved.";
    smugglersEvent.choices.push_back(ignoreSmugglers);

    addEvent(smugglersEvent);

    // Event 38: Emergency Vehicle - Highway specific
    GameEvent emergencyEvent;
    emergencyEvent.id = "highway_emergency_vehicle";
    emergencyEvent.title = "Машина скорой помощи / Ambulance";
    emergencyEvent.description = "Скорая помощь с мигалками едет сзади!\n"
                                 "Ambulance with sirens is behind you!";
    emergencyEvent.condition.requiredRoadType = "highway";
    emergencyEvent.condition.probability = 0.2f;

    EventChoice pullOverChoice;
    pullOverChoice.text = "Уступить дорогу / Pull over";
    pullOverChoice.outcomeText = "Вы пропустили скорую. Они спешат спасать жизнь.\n"
                                 "You let ambulance pass. They're rushing to save a life.";
    emergencyEvent.choices.push_back(pullOverChoice);

    EventChoice followAmbulanceChoice;
    followAmbulanceChoice.text = "Следовать за ней / Follow it";
    followAmbulanceChoice.outcomeText = "Вы едете за скорой. Проехали все пробки быстро!\n"
                                        "You follow ambulance. Passed all traffic jams quickly!";
    followAmbulanceChoice.fuelChange = -10.0f;
    emergencyEvent.choices.push_back(followAmbulanceChoice);

    addEvent(emergencyEvent);

    // Event 39: Flat Tire Stranger Help - General
    GameEvent flatTireHelpEvent;
    flatTireHelpEvent.id = "flat_tire_help";
    flatTireHelpEvent.title = "Прокол! / Flat Tire!";
    flatTireHelpEvent.description = "У вас спустило колесо! Проблема.\n"
                                    "You got a flat tire! Problem.";
    flatTireHelpEvent.condition.probability = 0.25f;

    EventChoice fixYourselfChoice;
    fixYourselfChoice.text = "Починить самому / Fix yourself";
    fixYourselfChoice.outcomeText = "Вы заменили колесо. Устали и испачкались.\n"
                                    "You changed the wheel. Tired and dirty.";
    fixYourselfChoice.energyChange = -25.0f;
    flatTireHelpEvent.choices.push_back(fixYourselfChoice);

    EventChoice askStrangerHelpChoice;
    askStrangerHelpChoice.text = "Попросить помощи у проезжающих / Ask passing drivers";
    askStrangerHelpChoice.outcomeText = "Добрый водитель остановился и помог! Вы благодарны.\n"
                                        "Kind driver stopped and helped! You're grateful.";
    askStrangerHelpChoice.energyChange = -10.0f;
    flatTireHelpEvent.choices.push_back(askStrangerHelpChoice);

    EventChoice callRoadsideChoice;
    callRoadsideChoice.text = "Вызвать помощь на дороге (350 руб) / Call roadside (350 rub)";
    callRoadsideChoice.outcomeText = "Служба приехала и быстро все починила.\n"
                                     "Service arrived and fixed everything quickly.";
    callRoadsideChoice.moneyChange = -350;
    callRoadsideChoice.energyChange = -5.0f;
    flatTireHelpEvent.choices.push_back(callRoadsideChoice);

    addEvent(flatTireHelpEvent);

    // Event 40: Lost Tourist - Road specific
    GameEvent lostTouristEvent;
    lostTouristEvent.id = "road_lost_tourist";
    lostTouristEvent.title = "Потерявшийся турист / Lost Tourist";
    lostTouristEvent.description = "Иностранный турист просит помочь найти дорогу.\n"
                                   "Foreign tourist asks for help finding the way.";
    lostTouristEvent.condition.requiredRoadType = "road";
    lostTouristEvent.condition.probability = 0.2f;

    EventChoice helpTouristChoice;
    helpTouristChoice.text = "Помочь найти дорогу / Help find the way";
    helpTouristChoice.outcomeText = "Турист благодарен! Дал вам 300 рублей за помощь.\n"
                                    "Tourist is grateful! Gave you 300 rubles for help.";
    helpTouristChoice.moneyChange = 300;
    helpTouristChoice.energyChange = -5.0f;
    lostTouristEvent.choices.push_back(helpTouristChoice);

    EventChoice giveTouristMapChoice;
    giveTouristMapChoice.text = "Дать карту / Give map";
    giveTouristMapChoice.outcomeText = "Турист взял карту и поехал дальше.\n"
                                       "Tourist took map and drove on.";
    lostTouristEvent.choices.push_back(giveTouristMapChoice);

    EventChoice ignoreTouristChoice;
    ignoreTouristChoice.text = "Извиниться и уехать / Apologize and leave";
    ignoreTouristChoice.outcomeText = "У вас нет времени.\n"
                                      "You don't have time.";
    lostTouristEvent.choices.push_back(ignoreTouristChoice);

    addEvent(lostTouristEvent);

    // Event 41: Old Friend Encounter - General
    GameEvent oldFriendEvent;
    oldFriendEvent.id = "old_friend_encounter";
    oldFriendEvent.title = "Старый друг / Old Friend";
    oldFriendEvent.description = "На заправке вы встретили старого друга!\n"
                                 "At gas station you met an old friend!";
    oldFriendEvent.condition.probability = 0.1f;

    EventChoice catchUpChoice;
    catchUpChoice.text = "Поболтать / Catch up";
    catchUpChoice.outcomeText = "Отличный разговор! Друг угостил вас кофе.\n"
                                "Great conversation! Friend treated you to coffee.";
    catchUpChoice.energyChange = 20.0f;
    oldFriendEvent.choices.push_back(catchUpChoice);

    EventChoice quickHiChoice;
    quickHiChoice.text = "Быстро поздороваться / Quick hello";
    quickHiChoice.outcomeText = "Вы быстро поздоровались и поехали дальше.\n"
                                "You quickly said hello and drove on.";
    quickHiChoice.energyChange = 5.0f;
    oldFriendEvent.choices.push_back(quickHiChoice);

    addEvent(oldFriendEvent);

    // ====== NPC COMPANION EVENTS ======
    // These events require specific NPCs in party

    // Event 42: Mechanic Fixes Car - Requires Mechanic NPC
    GameEvent mechanicFixEvent;
    mechanicFixEvent.id = "companion_mechanic_fix";
    mechanicFixEvent.title = "Механик помогает / Mechanic Helps";
    mechanicFixEvent.description = "Михаил (ваш механик) заметил проблему с двигателем.\n"
                                   "Mikhail (your mechanic) noticed an engine problem.";
    mechanicFixEvent.condition.minEnergy = 0.0f;
    mechanicFixEvent.condition.maxEnergy = 60.0f;
    mechanicFixEvent.condition.probability = 0.4f;

    EventChoice mechanicFixChoice;
    mechanicFixChoice.text = "Позволить Михаилу починить / Let Mikhail fix it";
    mechanicFixChoice.outcomeText = "Михаил быстро все починил! Машина работает отлично.\n"
                                    "Mikhail fixed it quickly! Car works great.";
    mechanicFixChoice.energyChange = 30.0f;
    mechanicFixChoice.vehicleConditionChange = 20.0f;
    mechanicFixEvent.choices.push_back(mechanicFixChoice);

    EventChoice ignoreFixChoice;
    ignoreFixChoice.text = "Отказаться от помощи / Refuse help";
    ignoreFixChoice.outcomeText = "Вы решили не тратить время.\n"
                                  "You decided not to waste time.";
    mechanicFixEvent.choices.push_back(ignoreFixChoice);

    addEvent(mechanicFixEvent);

    // Event 43: Trader Finds Deal - Requires Trader NPC
    GameEvent traderDealEvent;
    traderDealEvent.id = "companion_trader_deal";
    traderDealEvent.title = "Сара нашла сделку / Sarah Found a Deal";
    traderDealEvent.description = "Сара (торговка) знает где купить топливо дешево!\n"
                                  "Sarah (trader) knows where to buy fuel cheap!";
    traderDealEvent.condition.minFuel = 20.0f;
    traderDealEvent.condition.maxFuel = 60.0f;
    traderDealEvent.condition.probability = 0.35f;

    EventChoice traderBuyChoice;
    traderBuyChoice.text = "Купить топливо (300 руб за 50л) / Buy fuel (300 rub for 50L)";
    traderBuyChoice.outcomeText = "Отличная сделка! Топливо куплено дешево.\n"
                                  "Great deal! Fuel bought cheap.";
    traderBuyChoice.fuelChange = 50.0f;
    traderBuyChoice.moneyChange = -300;
    traderDealEvent.choices.push_back(traderBuyChoice);

    EventChoice traderSkipChoice;
    traderSkipChoice.text = "Не покупать / Don't buy";
    traderSkipChoice.outcomeText = "Вы решили не покупать.\n"
                                   "You decided not to buy.";
    traderDealEvent.choices.push_back(traderSkipChoice);

    addEvent(traderDealEvent);

    // Event 44: Navigator Shortcut - Requires Navigator NPC
    GameEvent navigatorShortcutEvent;
    navigatorShortcutEvent.id = "companion_navigator_shortcut";
    navigatorShortcutEvent.title = "Джек знает короткий путь / Jack Knows a Shortcut";
    navigatorShortcutEvent.description = "Джек (штурман) предлагает короткий путь через лес.\n"
                                        "Jack (navigator) suggests a shortcut through the forest.";
    navigatorShortcutEvent.condition.probability = 0.3f;

    EventChoice takeShortcutChoice;
    takeShortcutChoice.text = "Поехать коротким путем / Take the shortcut";
    takeShortcutChoice.outcomeText = "Вы сэкономили топливо! Джек знает дороги.\n"
                                     "You saved fuel! Jack knows the roads.";
    takeShortcutChoice.fuelChange = 15.0f;
    navigatorShortcutEvent.choices.push_back(takeShortcutChoice);

    EventChoice takeMainRoadChoice;
    takeMainRoadChoice.text = "Ехать основной дорогой / Take main road";
    takeMainRoadChoice.outcomeText = "Вы поехали привычным путем.\n"
                                     "You took the usual route.";
    navigatorShortcutEvent.choices.push_back(takeMainRoadChoice);

    addEvent(navigatorShortcutEvent);

    // Event 45: Medic Helps Fatigue - Requires Medic NPC
    GameEvent medicHelpEvent;
    medicHelpEvent.id = "companion_medic_help";
    medicHelpEvent.title = "Эмма дает лекарство / Emma Gives Medicine";
    medicHelpEvent.description = "Эмма (врач) заметила, что вы устали. Предлагает витамины.\n"
                                 "Emma (doctor) noticed you're tired. Offers vitamins.";
    medicHelpEvent.condition.minEnergy = 0.0f;
    medicHelpEvent.condition.maxEnergy = 40.0f;
    medicHelpEvent.condition.probability = 0.45f;

    EventChoice takeMedicineChoice;
    takeMedicineChoice.text = "Принять витамины / Take vitamins";
    takeMedicineChoice.outcomeText = "Витамины помогли! Вы чувствуете прилив сил.\n"
                                     "Vitamins helped! You feel energized.";
    takeMedicineChoice.energyChange = 35.0f;
    medicHelpEvent.choices.push_back(takeMedicineChoice);

    EventChoice refuseMedicineChoice;
    refuseMedicineChoice.text = "Отказаться / Refuse";
    refuseMedicineChoice.outcomeText = "Вы решили справиться сами.\n"
                                       "You decided to manage yourself.";
    medicHelpEvent.choices.push_back(refuseMedicineChoice);

    addEvent(medicHelpEvent);

    // Event 46: Companion Stories - Any companion
    GameEvent companionStoriesEvent;
    companionStoriesEvent.id = "companion_stories";
    companionStoriesEvent.title = "Истории попутчика / Companion Stories";
    companionStoriesEvent.description = "Алекс рассказывает веселые истории о путешествиях.\n"
                                        "Alex tells funny travel stories.";
    companionStoriesEvent.condition.probability = 0.3f;

    EventChoice listenStoriesChoice;
    listenStoriesChoice.text = "Слушать истории / Listen to stories";
    listenStoriesChoice.outcomeText = "Вы посмеялись! Настроение улучшилось.\n"
                                      "You laughed! Mood improved.";
    listenStoriesChoice.energyChange = 15.0f;
    companionStoriesEvent.choices.push_back(listenStoriesChoice);

    EventChoice ignoreStoriesChoice;
    ignoreStoriesChoice.text = "Сконцентрироваться на дороге / Focus on road";
    ignoreStoriesChoice.outcomeText = "Вы сосредоточились на вождении.\n"
                                      "You focused on driving.";
    companionStoriesEvent.choices.push_back(ignoreStoriesChoice);

    addEvent(companionStoriesEvent);

    // Event 47: Team Meal - Multiple companions
    GameEvent teamMealEvent;
    teamMealEvent.id = "companion_team_meal";
    teamMealEvent.title = "Совместный обед / Team Meal";
    teamMealEvent.description = "Команда предлагает остановиться и пообедать вместе.\n"
                                "Team suggests stopping for a meal together.";
    teamMealEvent.condition.probability = 0.25f;

    EventChoice teamEatChoice;
    teamEatChoice.text = "Пообедать вместе (150 руб) / Eat together (150 rub)";
    teamEatChoice.outcomeText = "Приятный обед с командой! Все отдохнули и пообщались.\n"
                                "Pleasant meal with team! Everyone rested and chatted.";
    teamEatChoice.moneyChange = -150;
    teamEatChoice.energyChange = 30.0f;
    teamMealEvent.choices.push_back(teamEatChoice);

    EventChoice skipMealChoice;
    skipMealChoice.text = "Пропустить обед / Skip meal";
    skipMealChoice.outcomeText = "Вы решили ехать дальше.\n"
                                 "You decided to keep driving.";
    teamMealEvent.choices.push_back(skipMealChoice);

    addEvent(teamMealEvent);
}

// Add ability perk choices to event
// Injects perk-based choices into event based on available perks
void EventManager::addPerkChoicesToEvent(GameEvent* event, float playerFuel, float playerEnergy, int playerMoney) {
    if (!event) return;
    
    // Get ability tree system
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    
    // Determine which resource preservation types are relevant for this event
    // by analyzing the existing choices
    std::vector<ResourcePreservationType> relevantTypes;
    
    for (const auto& choice : event->choices) {
        if (choice.energyChange < 0) {
            relevantTypes.push_back(ResourcePreservationType::CHARACTER_ENERGY);
        }
        if (choice.fuelChange < 0) {
            relevantTypes.push_back(ResourcePreservationType::FUEL);
        }
        if (choice.moneyChange < 0) {
            relevantTypes.push_back(ResourcePreservationType::MONEY);
        }
        // Note: Vehicle condition would need car damage tracking
    }
    
    // Remove duplicates
    std::sort(relevantTypes.begin(), relevantTypes.end());
    relevantTypes.erase(std::unique(relevantTypes.begin(), relevantTypes.end()), relevantTypes.end());
    
    // For each relevant type, get available perks
    for (auto preservationType : relevantTypes) {
        auto perks = abilitySystem.getActivePerksForEvent(preservationType);
        
        for (const auto* perk : perks) {
            if (!perk) continue;
            
            // Check if perk is unlocked and has charges
            if (!abilitySystem.isAbilityUnlocked(perk->getId())) continue;
            if (!abilitySystem.hasCharges(perk->getId())) continue;
            
            // Check if player has resources to use perk
            const auto& effect = perk->getEffect();
            if (effect.energyCost > playerEnergy) continue;
            if (effect.moneyCost > playerMoney) continue;

            // [MVP] Disabled - Item requirement checks (uncomment to enable)
            /*
            // Check item requirements via InventorySystem
            const auto& inventory = GameStateManager::getInstance().getPlayerState().getInventory();
            bool hasAllItems = true;
            for (const auto& itemName : effect.itemRequirements) {
                if (!inventory.hasItem(itemName, 1)) {
                    hasAllItems = false;
                    break;
                }
            }
            if (!hasAllItems) continue;
            */

            // Create perk choice
            EventChoice perkChoice;
            perkChoice.isPerkChoice = true;
            perkChoice.perkId = perk->getId();
            
            // Build choice text with perk icon
            perkChoice.text = "💡 [" + perk->getName() + "] ";
            
            // Add effect description based on preservation type
            switch (preservationType) {
                case ResourcePreservationType::CHARACTER_ENERGY:
                    perkChoice.text += "Сохранить энергию / Preserve energy (+" +
                                      std::to_string(static_cast<int>(effect.preserveAmount)) + ")";
                    perkChoice.energyChange = effect.preserveAmount;
                    break;
                case ResourcePreservationType::FUEL:
                    perkChoice.text += "Сохранить топливо / Preserve fuel (+" +
                                      std::to_string(static_cast<int>(effect.preserveAmount)) + "л)";
                    perkChoice.fuelChange = effect.preserveAmount;
                    break;
                case ResourcePreservationType::MONEY:
                    perkChoice.text += "Сохранить деньги / Preserve money (+" +
                                      std::to_string(effect.preserveAmount) + "₽)";
                    perkChoice.moneyChange = static_cast<int>(effect.preserveAmount);
                    break;
                case ResourcePreservationType::VEHICLE_CONDITION:
                    perkChoice.text += "Сохранить состояние / Preserve condition (+" +
                                      std::to_string(static_cast<int>(effect.preserveAmount)) + "%)";
                    perkChoice.vehicleConditionChange = effect.preserveAmount;
                    break;
                case ResourcePreservationType::NONE:
                    // No preservation effect for this perk
                    break;
            }
            
            // Add charge info (вместо cooldown)
            int currentCharges = abilitySystem.getCurrentCharges(perk->getId());
            int maxCharges = abilitySystem.getMaxCharges(perk->getId());
            perkChoice.text += " [Заряды: " + std::to_string(currentCharges) + "/" + 
                              std::to_string(maxCharges) + "]";
            
            // Add costs
            if (effect.energyCost > 0) {
                perkChoice.energyChange -= effect.energyCost;
            }
            if (effect.moneyCost > 0) {
                perkChoice.moneyChange -= effect.moneyCost;
            }
            
            // Outcome text
            perkChoice.outcomeText = "Вы использовали способность: " + perk->getName() + "\n" +
                                    "You used ability: " + perk->getName();
            
            // Add choice to event
            event->choices.push_back(perkChoice);
        }
    }
}

// Random number generation helper
float EventManager::randomFloat(float min, float max) const {
    float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return min + random * (max - min);
}

int EventManager::randomInt(int min, int max) const {
    return min + (std::rand() % (max - min + 1));
}
    
