#include "EventManager.h"
#include "AbilityTreeSystem.h"
#include "AbilityTypes.h"
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
                                 const std::string& location) const {
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
    
    // Check probability
    float roll = randomFloat(0.0f, 1.0f);
    if (roll > condition.probability) {
        return false;
    }
    
    return true;
}

// Get random event that meets conditions
GameEvent* EventManager::getRandomEvent(float fuel, float energy, int money,
                                       const std::string& location) {
    // Collect all eligible events
    std::vector<GameEvent*> eligibleEvents;
    
    for (auto& event : m_events) {
        if (!event.triggered && 
            checkCondition(event.condition, fuel, energy, money, location)) {
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
            // TODO: Check item requirements when inventory system is integrated
            
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
                    // TODO: Add vehicle condition change when Car damage system is implemented
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
