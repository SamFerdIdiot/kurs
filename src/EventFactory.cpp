#include "EventFactory.h"

namespace EventFactory {

// ============================================================================
// События встреч с компаньонами (ENCOUNTER)
// ============================================================================

GameEvent createMeetMechanic() {
    GameEvent event("meet_mechanic",
                    "Сломанная машина на обочине",
                    "На обочине стоит старая Волга с открытым капотом. "
                    "Пожилой мужчина в замасленной спецовке возится с двигателем.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "mechanic_mikhalych";

    // Условия: нет механика в команде, есть место
    event.condition.maxPartySize = 2; // Меньше 3 = есть место

    // Вариант 1: Помочь и пригласить
    EventChoice help;
    help.text = "Остановиться и помочь с ремонтом";
    help.outcomeText = "Вы помогли Михалычу починить машину. Он благодарен и готов присоединиться к вам!";
    help.energyChange = -5.0f;
    help.recruitNPC = "mechanic_mikhalych";
    help.relationshipChanges["mechanic_mikhalych"] = 20;

    // Вариант 2: Просто помочь
    EventChoice helpOnly;
    helpOnly.text = "Помочь, но ехать дальше";
    helpOnly.outcomeText = "Михалыч благодарит вас. Может, ещё встретитесь на дороге.";
    helpOnly.energyChange = -5.0f;
    helpOnly.moneyChange = 50; // Михалыч даёт немного денег в благодарность
    helpOnly.relationshipChanges["mechanic_mikhalych"] = 10;

    // Вариант 3: Проехать мимо
    EventChoice ignore;
    ignore.text = "Проехать мимо";
    ignore.outcomeText = "Вы продолжаете путь. Михалыч остаётся у своей машины.";

    event.choices = {help, helpOnly, ignore};
    return event;
}

GameEvent createMeetUnemployed() {
    GameEvent event("meet_unemployed",
                    "Попутчик на автовокзале",
                    "На автовокзале к вам подходит мужчина в потрёпанном костюме с портфелем. "
                    "Он представляется Виталием и спрашивает, не подбросите ли до соседнего города.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "unemployed_vitaliy";
    event.condition.maxPartySize = 2;

    EventChoice accept;
    accept.text = "Согласиться подвезти";
    accept.outcomeText = "Виталий с радостью садится в машину. Он знает много о ценах и экономии!";
    accept.recruitNPC = "unemployed_vitaliy";
    accept.relationshipChanges["unemployed_vitaliy"] = 15;

    EventChoice acceptForMoney;
    acceptForMoney.text = "Взять плату за проезд (100₽)";
    acceptForMoney.outcomeText = "Виталий неохотно платит, но всё же садится в машину.";
    acceptForMoney.moneyChange = 100;
    acceptForMoney.recruitNPC = "unemployed_vitaliy";
    acceptForMoney.relationshipChanges["unemployed_vitaliy"] = -5;

    EventChoice refuse;
    refuse.text = "Отказать";
    refuse.outcomeText = "Виталий кивает и отходит искать другой транспорт.";

    event.choices = {accept, acceptForMoney, refuse};
    return event;
}

GameEvent createMeetPunk() {
    GameEvent event("meet_punk",
                    "Панк на заправке",
                    "На заправке вы видите молодого парня с ирокезом и гитарой. "
                    "Он пытается собрать денег на дорогу, играя на гитаре.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "punk_vasya";
    event.condition.maxPartySize = 2;

    EventChoice invite;
    invite.text = "Предложить ехать вместе";
    invite.outcomeText = "Вася радостно соглашается! Наконец-то не одному в дороге!";
    invite.recruitNPC = "punk_vasya";
    invite.relationshipChanges["punk_vasya"] = 25;

    EventChoice giveMoney;
    giveMoney.text = "Дать денег (50₽) и уехать";
    giveMoney.outcomeText = "Вася благодарит и продолжает играть.";
    giveMoney.moneyChange = -50;
    giveMoney.relationshipChanges["punk_vasya"] = 10;

    EventChoice ignore;
    ignore.text = "Пройти мимо";
    ignore.outcomeText = "Вы заправляетесь и уезжаете.";

    event.choices = {invite, giveMoney, ignore};
    return event;
}

GameEvent createMeetGranny() {
    GameEvent event("meet_granny",
                    "Бабушка на обочине",
                    "Пожилая женщина с большой сумкой голосует на обочине. "
                    "Она везёт гостинцы внукам в соседний город.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "granny_galina";
    event.condition.maxPartySize = 2;

    EventChoice help;
    help.text = "Подвезти бабушку";
    help.outcomeText = "Галина Петровна благодарно садится. Она тут же угощает вас пирожками!";
    help.recruitNPC = "granny_galina";
    help.energyChange = 10.0f; // Сразу пирожки!
    help.relationshipChanges["granny_galina"] = 30;

    EventChoice refuse;
    refuse.text = "Извиниться и уехать";
    refuse.outcomeText = "Бабушка понимающе кивает и продолжает ждать.";

    event.choices = {help, refuse};
    return event;
}

GameEvent createMeetTrucker() {
    GameEvent event("meet_trucker",
                    "Дальнобойщик в придорожном кафе",
                    "В придорожном кафе за соседним столиком сидит опытный дальнобойщик. "
                    "Он пьёт чай из термоса и слушает радио.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "trucker_petrovich";
    event.condition.maxPartySize = 2;

    EventChoice invite;
    invite.text = "Предложить ехать вместе";
    invite.outcomeText = "Петрович соглашается составить компанию. Он знает все дороги!";
    invite.recruitNPC = "trucker_petrovich";
    invite.relationshipChanges["trucker_petrovich"] = 20;

    EventChoice chatOnly;
    chatOnly.text = "Поболтать и узнать про дорогу";
    chatOnly.outcomeText = "Петрович рассказывает полезные советы о маршруте.";
    chatOnly.energyChange = 5.0f; // Отдохнули за чаем
    chatOnly.relationshipChanges["trucker_petrovich"] = 10;

    EventChoice leave;
    leave.text = "Поесть и уехать";
    leave.outcomeText = "Вы перекусываете и продолжаете путь.";
    leave.energyChange = 10.0f;

    event.choices = {invite, chatOnly, leave};
    return event;
}

GameEvent createMeetStudent() {
    GameEvent event("meet_student",
                    "Студент-автостопщик",
                    "Молодой парень с рюкзаком голосует на трассе. "
                    "Студент едет на каникулы домой.");

    event.type = EventType::ENCOUNTER;
    event.weight = 1.0f;
    event.oneTimeOnly = true;
    event.associatedNPC = "student_lyokha";
    event.condition.maxPartySize = 2;

    EventChoice pickup;
    pickup.text = "Подобрать студента";
    pickup.outcomeText = "Лёха благодарно садится. Он молод и энергичен, может помочь в пути!";
    pickup.recruitNPC = "student_lyokha";
    pickup.relationshipChanges["student_lyokha"] = 20;

    EventChoice ignore;
    ignore.text = "Проехать мимо";
    ignore.outcomeText = "Студент машет рукой вслед.";

    event.choices = {pickup, ignore};
    return event;
}

// ============================================================================
// События с компаньонами (COMPANION)
// ============================================================================

GameEvent createMechanicRepairOffer() {
    GameEvent event("mechanic_repair_offer",
                    "Михалыч предлагает помощь",
                    "Михалыч осматривает машину и говорит: 'Слушай, я тут заметил, "
                    "у одного попутчика проблемы с двигателем. Могу помочь, если хочешь.'");

    event.type = EventType::COMPANION;
    event.weight = 0.5f;
    event.oneTimeOnly = false;
    event.associatedNPC = "mechanic_mikhalych";

    // Условие: Механик в команде, есть другие NPC, состояние машины < 80%
    event.condition.requiredNPCsInParty = {"mechanic_mikhalych"};
    event.condition.minPartySize = 2;

    EventChoice accept;
    accept.text = "Попросить Михалыча помочь";
    accept.outcomeText = "Михалыч чинит машины попутчиков. Все довольны!";
    accept.vehicleConditionChange = 15.0f;
    accept.relationshipChanges["mechanic_mikhalych"] = 5;
    accept.energyChange = -10.0f;

    EventChoice decline;
    decline.text = "Не стоит, справимся";
    decline.outcomeText = "Михалыч кивает и продолжает путь.";

    event.choices = {accept, decline};
    return event;
}

GameEvent createPunkConcertEvent() {
    GameEvent event("punk_concert",
                    "Вася хочет выступить",
                    "Панк Вася предлагает: 'Слушай, тут заправка. "
                    "Могу сыграть пару песен, люди денег дадут. Как думаешь?'");

    event.type = EventType::COMPANION;
    event.weight = 0.8f;
    event.oneTimeOnly = false;
    event.associatedNPC = "punk_vasya";

    event.condition.requiredNPCsInParty = {"punk_vasya"};

    EventChoice allow;
    allow.text = "Разрешить концерт";
    allow.outcomeText = "Вася играет, собирает деньги. Правда, охранник потребовал откат...";
    allow.moneyChange = 150; // 200₽ - 50₽ откат
    allow.relationshipChanges["punk_vasya"] = 10;

    EventChoice forbid;
    forbid.text = "Не стоит рисковать";
    forbid.outcomeText = "Вася расстроен, но соглашается.";
    forbid.relationshipChanges["punk_vasya"] = -5;

    event.choices = {allow, forbid};
    return event;
}

GameEvent createGrannyStoriesEvent() {
    GameEvent event("granny_stories",
                    "Бабушка рассказывает истории",
                    "Галина Петровна начинает рассказывать о своей молодости и угощает пирожками. "
                    "Атмосфера в машине становится теплее.");

    event.type = EventType::COMPANION;
    event.weight = 1.0f;
    event.oneTimeOnly = false;
    event.associatedNPC = "granny_galina";

    event.condition.requiredNPCsInParty = {"granny_galina"};

    EventChoice listen;
    listen.text = "Послушать истории";
    listen.outcomeText = "Вы отдыхаете душой. Бабушка угощает всех пирожками!";
    listen.energyChange = 15.0f;
    listen.relationshipChanges["granny_galina"] = 10;

    EventChoice politeDecline;
    politeDecline.text = "Вежливо отказаться, нужно сосредоточиться на дороге";
    politeDecline.outcomeText = "Бабушка понимает и продолжает вязать.";

    event.choices = {listen, politeDecline};
    return event;
}

GameEvent createCompanionConflict() {
    GameEvent event("companion_conflict",
                    "Конфликт в команде",
                    "Между попутчиками начинается спор о том, куда ехать дальше. "
                    "Атмосфера накаляется.");

    event.type = EventType::COMPANION;
    event.weight = 0.3f;
    event.oneTimeOnly = false;

    event.condition.minPartySize = 2;

    EventChoice mediate;
    mediate.text = "Вмешаться и помирить всех";
    mediate.outcomeText = "Вы находите компромисс. Все успокаиваются.";
    mediate.energyChange = -10.0f;
    // Небольшое улучшение отношений со всеми (нужно динамически добавлять)

    EventChoice ignore;
    ignore.text = "Не вмешиваться";
    ignore.outcomeText = "Спор утихает сам собой, но осадок остался.";
    // Небольшое ухудшение отношений

    event.choices = {mediate, ignore};
    return event;
}

// ============================================================================
// Дорожные события (ROAD)
// ============================================================================

GameEvent createCarBreakdown() {
    GameEvent event("car_breakdown",
                    "Поломка на трассе",
                    "Машина начинает странно дёргаться. Что-то не так с двигателем!");

    event.type = EventType::ROAD;
    event.weight = 0.5f;
    event.oneTimeOnly = false;

    EventChoice tryRepair;
    tryRepair.text = "Попробовать починить самому";
    tryRepair.outcomeText = "После часа возни машина снова заводится. Но ремонт не идеальный.";
    tryRepair.energyChange = -20.0f;
    tryRepair.vehicleConditionChange = 10.0f;

    EventChoice mechanicHelp;
    mechanicHelp.text = "Попросить механика (если есть)";
    mechanicHelp.outcomeText = "Механик быстро и профессионально чинит поломку!";
    mechanicHelp.energyChange = -5.0f;
    mechanicHelp.vehicleConditionChange = 25.0f;
    // Доступен только если есть механик

    EventChoice callService;
    callService.text = "Вызвать эвакуатор (500₽)";
    callService.outcomeText = "Эвакуатор отвозит вас на ближайшую СТО.";
    callService.moneyChange = -500;
    callService.energyChange = -10.0f;
    callService.vehicleConditionChange = 40.0f;

    event.choices = {tryRepair, mechanicHelp, callService};
    return event;
}

GameEvent createPoliceStop() {
    GameEvent event("police_stop",
                    "Остановка ГАИ",
                    "Сотрудник ГАИ останавливает вашу машину для проверки документов.");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice documents;
    documents.text = "Показать документы";
    documents.outcomeText = "Документы в порядке. Инспектор желает счастливого пути.";

    EventChoice bribe;
    bribe.text = "Дать взятку (100₽)";
    bribe.outcomeText = "Инспектор кивает и отпускает без проверки.";
    bribe.moneyChange = -100;

    EventChoice argue;
    argue.text = "Поспорить с инспектором";
    argue.outcomeText = "После долгих препирательств вас всё же отпускают, но настроение испорчено.";
    argue.energyChange = -15.0f;

    event.choices = {documents, bribe, argue};
    return event;
}

GameEvent createCrossroads() {
    GameEvent event("crossroads",
                    "Развилка дорог",
                    "Вы подъезжаете к развилке. Налево - короткий путь через плохую дорогу, "
                    "направо - объезд по трассе.");

    event.type = EventType::ROAD;
    event.weight = 1.0f;
    event.oneTimeOnly = false;

    EventChoice shortcut;
    shortcut.text = "Ехать напрямик (короче, но хуже дорога)";
    shortcut.outcomeText = "Вы экономите время, но машина изрядно потрёпана.";
    shortcut.fuelChange = -5.0f;
    shortcut.vehicleConditionChange = -15.0f;

    EventChoice highway;
    highway.text = "Ехать по трассе (дольше, но безопаснее)";
    highway.outcomeText = "Долгий, но спокойный путь. Машина в порядке.";
    highway.fuelChange = -10.0f;

    event.choices = {shortcut, highway};
    return event;
}

GameEvent createBadWeather() {
    GameEvent event("bad_weather",
                    "Непогода",
                    "Начинается сильный дождь. Видимость на дороге ухудшается.");

    event.type = EventType::ROAD;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice continue_driving;
    continue_driving.text = "Продолжать ехать осторожно";
    continue_driving.outcomeText = "Вы медленно, но верно продолжаете путь.";
    continue_driving.energyChange = -10.0f;
    continue_driving.fuelChange = -8.0f; // Медленнее = больше расход

    EventChoice wait;
    wait.text = "Переждать на стоянке";
    wait.outcomeText = "Через час дождь стихает. Можно ехать дальше.";
    wait.energyChange = 5.0f; // Отдохнули

    event.choices = {continue_driving, wait};
    return event;
}

// ============================================================================
// Ресурсные события (RESOURCE)
// ============================================================================

GameEvent createRoadsideFinding() {
    GameEvent event("roadside_finding",
                    "Находка на обочине",
                    "На обочине вы замечаете забытую сумку. Остановиться?");

    event.type = EventType::RESOURCE;
    event.weight = 0.4f;
    event.oneTimeOnly = false;

    EventChoice take;
    take.text = "Остановиться и проверить";
    take.outcomeText = "В сумке немного денег и полезные вещи!";
    take.moneyChange = 200;

    EventChoice ignore;
    ignore.text = "Проехать мимо";
    ignore.outcomeText = "Вы продолжаете путь.";

    event.choices = {take, ignore};
    return event;
}

GameEvent createHitchhikerTrade() {
    GameEvent event("hitchhiker_trade",
                    "Попутчик предлагает обмен",
                    "Попутчик предлагает обменять канистру бензина на 150₽.");

    event.type = EventType::RESOURCE;
    event.weight = 0.5f;
    event.oneTimeOnly = false;

    EventChoice buy;
    buy.text = "Купить бензин (150₽)";
    buy.outcomeText = "Вы заправляетесь. Неплохая сделка!";
    buy.moneyChange = -150;
    buy.fuelChange = 20.0f;

    EventChoice decline;
    decline.text = "Отказаться";
    decline.outcomeText = "Вы вежливо отказываетесь.";

    event.choices = {buy, decline};
    return event;
}

// ============================================================================
// Утилиты
// ============================================================================

void initializeAllEvents(EventManager& eventManager) {
    // Встречи с компаньонами
    eventManager.addEvent(createMeetMechanic());
    eventManager.addEvent(createMeetUnemployed());
    eventManager.addEvent(createMeetPunk());
    eventManager.addEvent(createMeetGranny());
    eventManager.addEvent(createMeetTrucker());
    eventManager.addEvent(createMeetStudent());

    // События с компаньонами
    eventManager.addEvent(createMechanicRepairOffer());
    eventManager.addEvent(createPunkConcertEvent());
    eventManager.addEvent(createGrannyStoriesEvent());
    eventManager.addEvent(createCompanionConflict());

    // Дорожные события
    eventManager.addEvent(createCarBreakdown());
    eventManager.addEvent(createPoliceStop());
    eventManager.addEvent(createCrossroads());
    eventManager.addEvent(createBadWeather());

    // Ресурсные события
    eventManager.addEvent(createRoadsideFinding());
    eventManager.addEvent(createHitchhikerTrade());
}

std::vector<GameEvent> createEncounterEvents() {
    return {
        createMeetMechanic(),
        createMeetUnemployed(),
        createMeetPunk(),
        createMeetGranny(),
        createMeetTrucker(),
        createMeetStudent()
    };
}

std::vector<GameEvent> createRoadEvents() {
    return {
        createCarBreakdown(),
        createPoliceStop(),
        createCrossroads(),
        createBadWeather()
    };
}

std::vector<GameEvent> createCompanionEvents() {
    return {
        createMechanicRepairOffer(),
        createPunkConcertEvent(),
        createGrannyStoriesEvent(),
        createCompanionConflict()
    };
}

} // namespace EventFactory
