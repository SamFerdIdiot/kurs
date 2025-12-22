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

GameEvent createTruckerStories() {
    GameEvent event("trucker_stories",
                    "Дальнобойщик делится опытом",
                    "Дальнобойщик рассказывает байки о дороге: "
                    "'А помню, раз ехал я зимой, трассу замело так, что хоть кол на голове теши...'");

    event.type = EventType::COMPANION;
    event.weight = 0.9f;
    event.oneTimeOnly = false;
    event.associatedNPC = "trucker_petrovich";

    event.condition.requiredNPCsInParty = {"trucker_petrovich"};

    EventChoice listen;
    listen.text = "Послушать байки";
    listen.outcomeText = "Вы узнаёте полезные советы о дороге. Дальнобойщик рад, что вы слушаете!";
    listen.energyChange = 10.0f;
    listen.relationshipChanges["trucker_petrovich"] = 10;

    EventChoice askAdvice;
    askAdvice.text = "Спросить совет по маршруту";
    askAdvice.outcomeText = "Дальнобойщик советует лучший путь. Вы экономите топливо!";
    askAdvice.fuelChange = 5.0f;
    askAdvice.relationshipChanges["trucker_petrovich"] = 15;

    event.choices = {listen, askAdvice};
    return event;
}

GameEvent createStudentPhilosophy() {
    GameEvent event("student_philosophy",
                    "Студент философствует",
                    "Студент достаёт книжку и начинает рассуждать о смысле жизни и дорог: "
                    "'Знаешь, дорога — это метафора нашего пути...'");

    event.type = EventType::COMPANION;
    event.weight = 0.7f;
    event.oneTimeOnly = false;
    event.associatedNPC = "student_lyokha";

    event.condition.requiredNPCsInParty = {"student_lyokha"};

    EventChoice discuss;
    discuss.text = "Поддержать беседу";
    discuss.outcomeText = "Интересный разговор скрашивает дорогу. Вы чувствуете себя бодрее!";
    discuss.energyChange = 15.0f;
    discuss.relationshipChanges["student_lyokha"] = 10;

    EventChoice askHelp;
    askHelp.text = "Попросить почитать карту";
    askHelp.outcomeText = "Студент находит более короткий маршрут!";
    askHelp.fuelChange = 8.0f;
    askHelp.relationshipChanges["student_lyokha"] = 5;

    EventChoice politeDecline;
    politeDecline.text = "Вежливо попросить помолчать";
    politeDecline.outcomeText = "Студент обижается и замолкает.";
    politeDecline.relationshipChanges["student_lyokha"] = -5;

    event.choices = {discuss, askHelp, politeDecline};
    return event;
}

GameEvent createUnemployedAdvice() {
    GameEvent event("unemployed_advice",
                    "Безработный даёт советы по экономии",
                    "Безработный говорит: 'Слушай, я тут знаю один способ сэкономить на заправке. "
                    "Если заправляться утром, топливо холоднее и его больше влезет...'");

    event.type = EventType::COMPANION;
    event.weight = 0.8f;
    event.oneTimeOnly = false;
    event.associatedNPC = "unemployed_vitaliy";

    event.condition.requiredNPCsInParty = {"unemployed_vitaliy"};

    EventChoice listenAdvice;
    listenAdvice.text = "Послушать советы";
    listenAdvice.outcomeText = "Вы узнаёте несколько хитростей экономии!";
    listenAdvice.relationshipChanges["unemployed_vitaliy"] = 10;

    EventChoice askBudget;
    askBudget.text = "Спросить, как растянуть деньги";
    askBudget.outcomeText = "Безработный помогает пересчитать бюджет. Находится лишняя сотня!";
    askBudget.moneyChange = 100;
    askBudget.relationshipChanges["unemployed_vitaliy"] = 15;

    EventChoice ignore;
    ignore.text = "Не слушать, сосредоточиться на дороге";
    ignore.outcomeText = "Безработный замолкает.";

    event.choices = {listenAdvice, askBudget, ignore};
    return event;
}

GameEvent createMechanicTeachSkill() {
    GameEvent event("mechanic_teach_skill",
                    "Михалыч учит ремонту",
                    "На стоянке Михалыч предлагает: 'Давай покажу, как масло менять правильно. "
                    "Пригодится в дороге!'");

    event.type = EventType::COMPANION;
    event.weight = 0.6f;
    event.oneTimeOnly = true;  // Обучение только раз
    event.associatedNPC = "mechanic_mikhalych";

    event.condition.requiredNPCsInParty = {"mechanic_mikhalych"};

    EventChoice learn;
    learn.text = "Согласиться поучиться";
    learn.outcomeText = "Михалыч показывает все тонкости. Теперь вы можете чинить машину эффективнее!";
    learn.energyChange = -10.0f;
    learn.vehicleConditionChange = 5.0f;
    learn.relationshipChanges["mechanic_mikhalych"] = 20;

    EventChoice decline;
    decline.text = "Отказаться, нужно ехать дальше";
    decline.outcomeText = "Михалыч пожимает плечами: 'Как знаешь...'";
    decline.relationshipChanges["mechanic_mikhalych"] = -3;

    event.choices = {learn, decline};
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
// СОБЫТИЯ ДЛЯ ВЕТКИ А: СИСТЕМНЫЙ АНАЛИЗ
// ============================================================================

GameEvent createBranchA_BrokenSign() {
    GameEvent event("branchA_broken_sign",
                    "Сломанный указатель",
                    "На развилке указатель сбит. Налево или направо?");

    event.type = EventType::ROAD;
    event.weight = 1.0f;
    event.oneTimeOnly = false;

    EventChoice analyze;
    analyze.text = "[Системный анализ] Посмотреть на состояние дорог";
    analyze.outcomeText = "Вы анализируете асфальт, следы шин, свежесть разметки. Ответ очевиден!";
    analyze.energyChange = -3.0f;
    analyze.fuelChange = -3.0f;
    // Требует принципа understanding_context

    EventChoice guess;
    guess.text = "Ехать наугад";
    guess.outcomeText = "Вы выбираете наудачу. Повезёт ли?";
    guess.fuelChange = -8.0f;
    guess.vehicleConditionChange = -5.0f;

    EventChoice ask;
    ask.text = "Спросить у местных";
    ask.outcomeText = "Местный житель указывает правильную дорогу.";
    ask.energyChange = -5.0f;
    ask.fuelChange = -4.0f;

    event.choices = {analyze, guess, ask};
    return event;
}

GameEvent createBranchA_EngineNoise() {
    GameEvent event("branchA_engine_noise",
                    "Странный звук двигателя",
                    "Двигатель начал работать по-другому. Не громче, но иначе.");

    event.type = EventType::ROAD;
    event.weight = 0.8f;
    event.oneTimeOnly = false;

    EventChoice analyze;
    analyze.text = "[Системный анализ] Проанализировать симптомы";
    analyze.outcomeText = "Вы вспоминаете урок Дениса: слушаете ритм, сопоставляете звуки. Вывод точен!";
    analyze.vehicleConditionChange = 10.0f;
    analyze.energyChange = -5.0f;

    EventChoice check;
    check.text = "Заглушить и проверить";
    check.outcomeText = "Вы тратите время на осмотр, но находите проблему.";
    check.vehicleConditionChange = 5.0f;
    check.energyChange = -10.0f;

    EventChoice ignore;
    ignore.text = "Ехать дальше";
    ignore.outcomeText = "Звук усиливается. Может, стоило остановиться?";
    ignore.vehicleConditionChange = -10.0f;

    event.choices = {analyze, check, ignore};
    return event;
}

GameEvent createBranchA_WeatherPattern() {
    GameEvent event("branchA_weather_pattern",
                    "Погодные признаки",
                    "Облака собираются, ветер меняется. Что-то надвигается.");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice readSigns;
    readSigns.text = "[Системный анализ] Прочитать приметы, искать укрытие";
    readSigns.outcomeText = "Вы замечаете признаки приближающейся грозы и находите укрытие вовремя!";
    readSigns.energyChange = 5.0f;

    EventChoice speedUp;
    speedUp.text = "Ехать быстрее";
    speedUp.outcomeText = "Вы пытаетесь обогнать непогоду. Рискованно...";
    speedUp.fuelChange = -10.0f;
    speedUp.energyChange = -8.0f;

    EventChoice wait;
    wait.text = "Остановиться и переждать";
    wait.outcomeText = "Вы пережидаете непогоду на стоянке.";
    wait.energyChange = 3.0f;

    event.choices = {readSigns, speedUp, wait};
    return event;
}

GameEvent createBranchA_FuelConsumption() {
    GameEvent event("branchA_fuel_consumption",
                    "Странный расход топлива",
                    "Топливо расходуется быстрее обычного. В чём причина?");

    event.type = EventType::ROAD;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice calculate;
    calculate.text = "[Системный анализ] Вычислить причину по данным";
    calculate.outcomeText = "Вы анализируете скорость, рельеф, ветер. Находите оптимальный режим!";
    calculate.fuelChange = 5.0f;
    calculate.energyChange = -5.0f;

    EventChoice refuel;
    refuel.text = "Залить больше бензина";
    refuel.outcomeText = "Проблема не решена, но бак полон.";
    refuel.moneyChange = -200;
    refuel.fuelChange = 30.0f;

    EventChoice checkTank;
    checkTank.text = "Проверить бак на течь";
    checkTank.outcomeText = "Течи нет, но вы потеряли время.";
    checkTank.energyChange = -10.0f;

    event.choices = {calculate, refuel, checkTank};
    return event;
}

GameEvent createBranchA_MapDiscrepancy() {
    GameEvent event("branchA_map_discrepancy",
                    "Карта не сходится",
                    "Расстояния на карте не совпадают с реальностью. Масштаб неверный?");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice recalculate;
    recalculate.text = "[Системный анализ] Пересчитать с учётом масштаба";
    recalculate.outcomeText = "Вы корректируете расчёты и находите точный маршрут!";
    recalculate.energyChange = -4.0f;
    recalculate.fuelChange = -3.0f;

    EventChoice trustMap;
    trustMap.text = "Довериться карте";
    trustMap.outcomeText = "Карта привела вас не туда. Приходится возвращаться.";
    trustMap.fuelChange = -10.0f;
    trustMap.energyChange = -8.0f;

    EventChoice askDirection;
    askDirection.text = "Спросить дорогу";
    askDirection.outcomeText = "Местный житель помогает сориентироваться.";
    askDirection.energyChange = -5.0f;

    event.choices = {recalculate, trustMap, askDirection};
    return event;
}

// ============================================================================
// СОБЫТИЯ ДЛЯ ВЕТКИ Б: ЧУВСТВО МАТЕРИАЛА
// ============================================================================

GameEvent createBranchB_RoadFeel() {
    GameEvent event("branchB_road_feel",
                    "Чувство дороги",
                    "Асфальт под колёсами изменился. Руль передаёт новые вибрации.");

    event.type = EventType::ROAD;
    event.weight = 1.0f;
    event.oneTimeOnly = false;

    EventChoice feel;
    feel.text = "[Чувство материала] Почувствовать дорогу через руль";
    feel.outcomeText = "Вы чувствуете изменение покрытия и корректируете стиль вождения!";
    feel.vehicleConditionChange = 5.0f;
    feel.energyChange = -3.0f;

    EventChoice slowDown;
    slowDown.text = "Снизить скорость";
    slowDown.outcomeText = "Осторожность не помешает.";
    slowDown.fuelChange = -8.0f;

    EventChoice checkMap;
    checkMap.text = "Проверить карту";
    checkMap.outcomeText = "Карта показывает изменение дорожного покрытия.";
    checkMap.energyChange = -5.0f;

    event.choices = {feel, slowDown, checkMap};
    return event;
}

GameEvent createBranchB_MetalFatigue() {
    GameEvent event("branchB_metal_fatigue",
                    "Усталость металла",
                    "Что-то в подвеске звучит не так. Металл устал?");

    event.type = EventType::ROAD;
    event.weight = 0.8f;
    event.oneTimeOnly = false;

    EventChoice listen;
    listen.text = "[Чувство материала] Прислушаться к звуку";
    listen.outcomeText = "Вы определяете проблемный узел по звуку и вибрации. Урок Дениса помог!";
    listen.vehicleConditionChange = 10.0f;
    listen.energyChange = -5.0f;

    EventChoice inspect;
    inspect.text = "Осмотреть снизу";
    inspect.outcomeText = "Визуальный осмотр показывает износ деталей.";
    inspect.vehicleConditionChange = 5.0f;
    inspect.energyChange = -10.0f;

    EventChoice careful;
    careful.text = "Ехать аккуратнее";
    careful.outcomeText = "Вы снижаете скорость и избегаете ям.";
    careful.fuelChange = -7.0f;

    event.choices = {listen, inspect, careful};
    return event;
}

GameEvent createBranchB_TirePressure() {
    GameEvent event("branchB_tire_pressure",
                    "Давление в шинах",
                    "Машину чуть ведёт в сторону. Что-то с колёсами?");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice feelWheel;
    feelWheel.text = "[Чувство материала] Почувствовать через руль";
    feelWheel.outcomeText = "Руки чувствуют разницу в давлении. Вы подкачиваете нужное колесо!";
    feelWheel.vehicleConditionChange = 8.0f;
    feelWheel.energyChange = -4.0f;

    EventChoice checkGauge;
    checkGauge.text = "Проверить манометром";
    checkGauge.outcomeText = "Манометр показывает разницу в давлении.";
    checkGauge.vehicleConditionChange = 5.0f;
    checkGauge.energyChange = -8.0f;

    EventChoice pumpAll;
    pumpAll.text = "Подкачать все шины";
    pumpAll.outcomeText = "Вы подкачиваете все колёса на всякий случай.";
    pumpAll.vehicleConditionChange = 3.0f;
    pumpAll.energyChange = -12.0f;

    event.choices = {feelWheel, checkGauge, pumpAll};
    return event;
}

GameEvent createBranchB_FuelQuality() {
    GameEvent event("branchB_fuel_quality",
                    "Качество топлива",
                    "После заправки двигатель работает по-другому. Что-то не то с бензином?");

    event.type = EventType::ROAD;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice listenEngine;
    listenEngine.text = "[Чувство материала] Услышать изменение в работе";
    listenEngine.outcomeText = "Вы слышите детонацию. Плохой бензин! Переходите на низкие обороты.";
    listenEngine.vehicleConditionChange = 5.0f;
    listenEngine.energyChange = -4.0f;

    EventChoice drain;
    drain.text = "Слить топливо";
    drain.outcomeText = "Вы сливаете подозрительный бензин и заправляетесь в другом месте.";
    drain.moneyChange = -200;
    drain.energyChange = -15.0f;
    drain.fuelChange = 20.0f;

    EventChoice additive;
    additive.text = "Добавить присадку";
    additive.outcomeText = "Присадка немного помогает.";
    additive.moneyChange = -50;
    additive.vehicleConditionChange = 3.0f;

    event.choices = {listenEngine, drain, additive};
    return event;
}

GameEvent createBranchB_BrakeFeel() {
    GameEvent event("branchB_brake_feel",
                    "Педаль тормоза",
                    "Педаль стала мягче. Что-то изменилось в тормозной системе?");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice feelChange;
    feelChange.text = "[Чувство материала] Почувствовать изменение";
    feelChange.outcomeText = "Нога чувствует изменение сопротивления. Скорее всего, воздух в системе.";
    feelChange.vehicleConditionChange = 8.0f;
    feelChange.energyChange = -5.0f;

    EventChoice checkFluid;
    checkFluid.text = "Проверить тормозную жидкость";
    checkFluid.outcomeText = "Уровень жидкости в норме, но нужна прокачка.";
    checkFluid.vehicleConditionChange = 5.0f;
    checkFluid.energyChange = -10.0f;

    EventChoice slowDriving;
    slowDriving.text = "Ехать медленнее";
    slowDriving.outcomeText = "Вы снижаете скорость и увеличиваете дистанцию.";
    slowDriving.fuelChange = -6.0f;

    event.choices = {feelChange, checkFluid, slowDriving};
    return event;
}

// ============================================================================
// СОБЫТИЯ ДЛЯ ВЕТКИ В: КОМАНДНАЯ РАБОТА
// ============================================================================

GameEvent createBranchC_TruckerAdvice() {
    GameEvent event("branchC_trucker_advice",
                    "Совет дальнобойщика",
                    "Дальнобойщик на стоянке предлагает помощь с маршрутом.");

    event.type = EventType::ROAD;
    event.weight = 1.0f;
    event.oneTimeOnly = false;

    EventChoice askExpert;
    askExpert.text = "[Спросить у опытных] Спросить про дорогу";
    askExpert.outcomeText = "Водитель рассказывает про объезды, посты ГАИ и хорошие заправки!";
    askExpert.fuelChange = -3.0f;
    askExpert.energyChange = -2.0f;

    EventChoice politeRefuse;
    politeRefuse.text = "Отказаться вежливо";
    politeRefuse.outcomeText = "Вы благодарите и едете своим путём.";

    EventChoice askWeather;
    askWeather.text = "Спросить про погоду";
    askWeather.outcomeText = "Он предупреждает о возможном дожде впереди.";
    askWeather.energyChange = -3.0f;

    event.choices = {askExpert, politeRefuse, askWeather};
    return event;
}

GameEvent createBranchC_MechanicHelp() {
    GameEvent event("branchC_mechanic_help",
                    "Помощь механика",
                    "Местный механик предлагает бесплатно проверить машину.");

    event.type = EventType::ROAD;
    event.weight = 0.8f;
    event.oneTimeOnly = false;

    EventChoice askTeach;
    askTeach.text = "[Спросить у опытных] Попросить научить";
    askTeach.outcomeText = "Механик не только проверяет, но и показывает, на что обращать внимание!";
    askTeach.vehicleConditionChange = 15.0f;
    askTeach.energyChange = -8.0f;

    EventChoice justCheck;
    justCheck.text = "Просто проверить";
    justCheck.outcomeText = "Механик быстро осматривает машину и даёт советы.";
    justCheck.vehicleConditionChange = 10.0f;
    justCheck.energyChange = -5.0f;

    EventChoice refuse;
    refuse.text = "Отказаться";
    refuse.outcomeText = "Вы благодарите и уезжаете.";

    event.choices = {askTeach, justCheck, refuse};
    return event;
}

GameEvent createBranchC_LocalKnowledge() {
    GameEvent event("branchC_local_knowledge",
                    "Местные знания",
                    "Старожил предупреждает про опасный участок дороги впереди.");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice listenCarefully;
    listenCarefully.text = "[Спросить у опытных] Выслушать подробно";
    listenCarefully.outcomeText = "Старик рассказывает про все ямы, повороты и опасности. Бесценно!";
    listenCarefully.vehicleConditionChange = 10.0f;
    listenCarefully.energyChange = -5.0f;

    EventChoice thankAndGo;
    thankAndGo.text = "Поблагодарить и ехать";
    thankAndGo.outcomeText = "Вы кивком благодарите и продолжаете путь.";

    EventChoice checkMap;
    checkMap.text = "Проверить по карте";
    checkMap.outcomeText = "На карте ничего подозрительного, но местные знают лучше.";
    checkMap.energyChange = -4.0f;

    event.choices = {listenCarefully, thankAndGo, checkMap};
    return event;
}

GameEvent createBranchC_ConvoyOffer() {
    GameEvent event("branchC_convoy_offer",
                    "Предложение ехать в колонне",
                    "Группа водителей предлагает ехать вместе через сложный участок.");

    event.type = EventType::ROAD;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice joinAndLearn;
    joinAndLearn.text = "[Спросить у опытных] Согласиться и учиться";
    joinAndLearn.outcomeText = "Вы едете в колонне и перенимаете опыт водителей!";
    joinAndLearn.energyChange = 5.0f;
    joinAndLearn.fuelChange = -5.0f;

    EventChoice justJoin;
    justJoin.text = "Согласиться просто";
    justJoin.outcomeText = "Безопаснее ехать вместе.";
    justJoin.energyChange = 3.0f;
    justJoin.fuelChange = -6.0f;

    EventChoice decline;
    decline.text = "Отказаться";
    decline.outcomeText = "Вы предпочитаете ехать в своём темпе.";

    event.choices = {joinAndLearn, justJoin, decline};
    return event;
}

GameEvent createBranchC_RadioCall() {
    GameEvent event("branchC_radio_call",
                    "Разговор по радио",
                    "По рации предупреждают про пост ГАИ впереди.");

    event.type = EventType::ROAD;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice askDetails;
    askDetails.text = "[Спросить у опытных] Спросить детали";
    askDetails.outcomeText = "Водители рассказывают, что проверяют и как себя вести. Полезно!";
    askDetails.energyChange = -3.0f;

    EventChoice thankOnly;
    thankOnly.text = "Поблагодарить";
    thankOnly.outcomeText = "Вы благодарите за предупреждение.";

    EventChoice ignoreRadio;
    ignoreRadio.text = "Игнорировать";
    ignoreRadio.outcomeText = "Вы продолжаете ехать, не обращая внимания на рацию.";

    event.choices = {askDetails, thankOnly, ignoreRadio};
    return event;
}

// ============================================================================
// СОБЫТИЯ ДЛЯ ВЕТКИ Г: РЕСУРСОЭФФЕКТИВНОСТЬ
// ============================================================================

GameEvent createBranchD_FuelChoice() {
    GameEvent event("branchD_fuel_choice",
                    "Выбор топлива",
                    "На заправке два вида бензина: АИ-92 (дешевле) и АИ-95 (дороже).");

    event.type = EventType::RESOURCE;
    event.weight = 1.0f;
    event.oneTimeOnly = false;

    EventChoice calculate;
    calculate.text = "[Ресурсоэффективность] Рассчитать выгоду";
    calculate.outcomeText = "Вы рассчитываете расход и цену. 92-й выгоднее для вашей машины!";
    calculate.moneyChange = -150;
    calculate.fuelChange = 30.0f;
    calculate.energyChange = -3.0f;

    EventChoice cheap;
    cheap.text = "Залить дешёвый";
    cheap.outcomeText = "Экономия прежде всего.";
    cheap.moneyChange = -150;
    cheap.fuelChange = 30.0f;

    EventChoice expensive;
    expensive.text = "Залить дорогой";
    expensive.outcomeText = "Качество важнее цены.";
    expensive.moneyChange = -200;
    expensive.fuelChange = 30.0f;
    expensive.vehicleConditionChange = 3.0f;

    event.choices = {calculate, cheap, expensive};
    return event;
}

GameEvent createBranchD_Shortcut() {
    GameEvent event("branchD_shortcut",
                    "Короткий путь",
                    "Местный показывает короткую дорогу через лес. Экономия 20 км, но грунтовка.");

    event.type = EventType::ROAD;
    event.weight = 0.8f;
    event.oneTimeOnly = false;

    EventChoice weighRisks;
    weighRisks.text = "[Ресурсоэффективность] Взвесить риски и экономию";
    weighRisks.outcomeText = "Вы рассчитываете: экономия топлива vs износ машины. Принимаете взвешенное решение!";
    weighRisks.fuelChange = -8.0f;
    weighRisks.vehicleConditionChange = -3.0f;
    weighRisks.energyChange = -3.0f;

    EventChoice takeShortcut;
    takeShortcut.text = "Ехать коротким путём";
    takeShortcut.outcomeText = "Короче, но тряско.";
    takeShortcut.fuelChange = -8.0f;
    takeShortcut.vehicleConditionChange = -10.0f;

    EventChoice highway;
    highway.text = "Ехать по трассе";
    highway.outcomeText = "Дольше, но безопаснее.";
    highway.fuelChange = -12.0f;

    event.choices = {weighRisks, takeShortcut, highway};
    return event;
}

GameEvent createBranchD_RestStop() {
    GameEvent event("branchD_rest_stop",
                    "Остановка на ночлег",
                    "Можно ночевать в мотеле (100₽) или в машине (бесплатно, но неудобно).");

    event.type = EventType::RESOURCE;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice calculateBalance;
    calculateBalance.text = "[Ресурсоэффективность] Посчитать энергию и деньги";
    calculateBalance.outcomeText = "Вы взвешиваете: отдых важнее денег. Выбираете мотель!";
    calculateBalance.moneyChange = -100;
    calculateBalance.energyChange = 30.0f;

    EventChoice inCar;
    inCar.text = "В машине";
    inCar.outcomeText = "Неудобно, но бесплатно.";
    inCar.energyChange = 10.0f;

    EventChoice motel;
    motel.text = "В мотеле";
    motel.outcomeText = "Комфортный сон стоит денег.";
    motel.moneyChange = -100;
    motel.energyChange = 30.0f;

    event.choices = {calculateBalance, inCar, motel};
    return event;
}

GameEvent createBranchD_RepairChoice() {
    GameEvent event("branchD_repair_choice",
                    "Выбор ремонта",
                    "Нужен мелкий ремонт: в сервисе (200₽, быстро) или самому (бесплатно, но долго).");

    event.type = EventType::RESOURCE;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice evaluateTime;
    evaluateTime.text = "[Ресурсоэффективность] Оценить время vs деньги";
    evaluateTime.outcomeText = "Вы рассчитываете ценность времени. Сделать самому выгоднее!";
    evaluateTime.vehicleConditionChange = 15.0f;
    evaluateTime.energyChange = -15.0f;

    EventChoice service;
    service.text = "В сервисе";
    service.outcomeText = "Быстро и профессионально.";
    service.moneyChange = -200;
    service.vehicleConditionChange = 20.0f;
    service.energyChange = -5.0f;

    EventChoice selfRepair;
    selfRepair.text = "Самому";
    selfRepair.outcomeText = "Долго, зато бесплатно.";
    selfRepair.vehicleConditionChange = 15.0f;
    selfRepair.energyChange = -20.0f;

    event.choices = {evaluateTime, service, selfRepair};
    return event;
}

GameEvent createBranchD_FoodPlanning() {
    GameEvent event("branchD_food_planning",
                    "Планирование еды",
                    "Запас еды кончился. Кафе (50₽, быстро) или магазин (20₽, но дольше)?");

    event.type = EventType::RESOURCE;
    event.weight = 0.7f;
    event.oneTimeOnly = false;

    EventChoice planOptimal;
    planOptimal.text = "[Ресурсоэффективность] Спланировать оптимально";
    planOptimal.outcomeText = "Вы идёте в магазин, покупаете с запасом. Экономия и практичность!";
    planOptimal.moneyChange = -20;
    planOptimal.energyChange = 15.0f;

    EventChoice cafe;
    cafe.text = "В кафе";
    cafe.outcomeText = "Быстро и вкусно.";
    cafe.moneyChange = -50;
    cafe.energyChange = 20.0f;

    EventChoice shop;
    shop.text = "В магазин";
    shop.outcomeText = "Дешевле, но нужно готовить.";
    shop.moneyChange = -20;
    shop.energyChange = 15.0f;

    event.choices = {planOptimal, cafe, shop};
    return event;
}

// ============================================================================
// СЛУЧАЙНЫЕ СОБЫТИЯ (ДЛЯ ЛЮБОЙ ВЕТКИ)
// ============================================================================

GameEvent createRandom_FlatTire() {
    GameEvent event("random_flat_tire",
                    "Прокол колеса",
                    "Шина спустила на неровной дороге. Что делать?");

    event.type = EventType::ROAD;
    event.weight = 0.5f;
    event.oneTimeOnly = false;

    EventChoice changeSelf;
    changeSelf.text = "Поменять самому";
    changeSelf.outcomeText = "Вы меняете колесо. Устали, но справились.";
    changeSelf.energyChange = -10.0f;
    changeSelf.vehicleConditionChange = -5.0f;

    EventChoice askHelp;
    askHelp.text = "Попросить помощи";
    askHelp.outcomeText = "Проезжающий водитель помогает. Быстрее!";
    askHelp.energyChange = -5.0f;

    EventChoice callService;
    callService.text = "Вызвать помощь";
    callService.outcomeText = "Служба быстро меняет колесо.";
    callService.moneyChange = -100;
    callService.energyChange = -3.0f;

    event.choices = {changeSelf, askHelp, callService};
    return event;
}

GameEvent createRandom_Hungry() {
    GameEvent event("random_hungry",
                    "Голод",
                    "Желудок напоминает, что последний раз ели утром.");

    event.type = EventType::RESOURCE;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice eatSupply;
    eatSupply.text = "Съесть запас из багажника";
    eatSupply.outcomeText = "Сухой паёк утоляет голод.";
    eatSupply.energyChange = 10.0f;

    EventChoice wait;
    wait.text = "Потерпеть до города";
    wait.outcomeText = "Вы демонстрируете стойкость.";
    wait.energyChange = -5.0f;

    EventChoice buyCafe;
    buyCafe.text = "Купить в придорожном кафе";
    buyCafe.outcomeText = "Горячая еда поднимает настроение!";
    buyCafe.moneyChange = -50;
    buyCafe.energyChange = 20.0f;

    event.choices = {eatSupply, wait, buyCafe};
    return event;
}

GameEvent createRandom_Rain() {
    GameEvent event("random_rain",
                    "Внезапный ливень",
                    "Небо потемнело, начался сильный дождь. Видимость плохая.");

    event.type = EventType::ROAD;
    event.weight = 0.5f;
    event.oneTimeOnly = false;

    EventChoice waitOut;
    waitOut.text = "Остановиться и переждать";
    waitOut.outcomeText = "Вы пережидаете ливень на стоянке.";
    waitOut.energyChange = 3.0f;

    EventChoice driveSlow;
    driveSlow.text = "Ехать медленно";
    driveSlow.outcomeText = "Осторожная езда в дождь утомляет.";
    driveSlow.energyChange = -10.0f;
    driveSlow.fuelChange = -8.0f;

    EventChoice driveNormal;
    driveNormal.text = "Ехать как обычно";
    driveNormal.outcomeText = "Риск! Машина заносит на лужах.";
    driveNormal.vehicleConditionChange = -10.0f;
    driveNormal.energyChange = -8.0f;

    event.choices = {waitOut, driveSlow, driveNormal};
    return event;
}

GameEvent createRandom_PoliceCheck() {
    GameEvent event("random_police_check",
                    "Проверка ДПС",
                    "Пост ДПС. Инспектор машет остановиться.");

    event.type = EventType::ROAD;
    event.weight = 0.4f;
    event.oneTimeOnly = false;

    EventChoice stopImmediate;
    stopImmediate.text = "Остановиться сразу";
    stopImmediate.outcomeText = "Документы в порядке. Инспектор отпускает.";
    stopImmediate.energyChange = -3.0f;

    EventChoice stopSlow;
    stopSlow.text = "Остановиться, но медленно";
    stopSlow.outcomeText = "Инспектор проверяет документы внимательнее.";
    stopSlow.energyChange = -5.0f;

    EventChoice friendly;
    friendly.text = "Поговорить дружелюбно";
    friendly.outcomeText = "Вежливость располагает. Быстрая проверка.";
    friendly.energyChange = -2.0f;

    event.choices = {stopImmediate, stopSlow, friendly};
    return event;
}

GameEvent createRandom_Hitchhiker() {
    GameEvent event("random_hitchhiker",
                    "Попутчик",
                    "На обочине голосует человек с рюкзаком.");

    event.type = EventType::ENCOUNTER;
    event.weight = 0.5f;
    event.oneTimeOnly = false;

    EventChoice pickup;
    pickup.text = "Подобрать";
    pickup.outcomeText = "Попутчик благодарит и рассказывает интересные истории.";
    pickup.energyChange = 5.0f;
    pickup.fuelChange = -5.0f;

    EventChoice pass;
    pass.text = "Проехать мимо";
    pass.outcomeText = "Вы продолжаете путь в одиночестве.";

    EventChoice pickupAndTalk;
    pickupAndTalk.text = "Подобрать и поговорить";
    pickupAndTalk.outcomeText = "Приятная беседа скрашивает дорогу!";
    pickupAndTalk.energyChange = 10.0f;
    pickupAndTalk.fuelChange = -5.0f;

    event.choices = {pickup, pass, pickupAndTalk};
    return event;
}

GameEvent createRandom_ScenicView() {
    GameEvent event("random_scenic_view",
                    "Красивый вид",
                    "С холма открывается потрясающий вид на долину и реку.");

    event.type = EventType::ROAD;
    event.weight = 0.6f;
    event.oneTimeOnly = false;

    EventChoice stop;
    stop.text = "Остановиться и насладиться";
    stop.outcomeText = "Красота природы поднимает настроение!";
    stop.energyChange = 15.0f;
    stop.fuelChange = -2.0f;

    EventChoice photo;
    photo.text = "Сфотографировать на ходу";
    photo.outcomeText = "Быстрый снимок на память.";
    photo.energyChange = 5.0f;

    EventChoice pass;
    pass.text = "Проехать мимо";
    pass.outcomeText = "Времени нет на остановки.";

    event.choices = {stop, photo, pass};
    return event;
}

// ============================================================================
// СОБЫТИЯ НА ОСНОВЕ РЕСУРСОВ
// ============================================================================

GameEvent createResourceFuelCritical() {
    GameEvent event;
    event.id = "resource_fuel_critical";
    event.type = EventType::RESOURCE;
    event.title = "⛽ КРИТИЧНО: ТОПЛИВО НА ИСХОДЕ";
    event.description = "Стрелка указателя топлива почти на нуле. Осталось максимум 10 литров. "
                       "Если не заправиться сейчас - рискую встать посреди трассы.";
    event.oneTimeOnly = false;

    EventChoice stop;
    stop.text = "Искать заправку прямо сейчас";
    stop.outcomeText = "Нашёл заправку и залил полный бак. Фух, пронесло!";
    stop.fuelChange = 50.0f;
    stop.moneyChange = -500.0f;
    stop.energyChange = -5.0f;

    EventChoice careful;
    careful.text = "Ехать экономично до следующего города";
    careful.outcomeText = "Еду 60 км/ч, никаких резких маневров. Топлива должно хватить.";
    careful.fuelChange = -5.0f;
    careful.energyChange = -10.0f;

    event.choices = {stop, careful};
    return event;
}

GameEvent createResourceFuelLow() {
    GameEvent event;
    event.id = "resource_fuel_low";
    event.type = EventType::RESOURCE;
    event.title = "⛽ Топливо на низком уровне";
    event.description = "В баке меньше 20 литров. Пора подумать о заправке, "
                       "хотя паники ещё нет - до следующего города доеду.";
    event.oneTimeOnly = false;

    EventChoice refuel;
    refuel.text = "Заправиться при первой возможности";
    refuel.outcomeText = "Заправился. Теперь спокойнее.";
    refuel.fuelChange = 40.0f;
    refuel.moneyChange = -400.0f;

    EventChoice wait;
    wait.text = "Подождать до города - там дешевле";
    wait.outcomeText = "Еду дальше, следя за расходом.";
    wait.energyChange = -3.0f;

    event.choices = {refuel, wait};
    return event;
}

GameEvent createResourceEnergyExhausted() {
    GameEvent event;
    event.id = "resource_energy_exhausted";
    event.type = EventType::RESOURCE;
    event.title = "😴 КРИТИЧНО: ИСТОЩЕНИЕ";
    event.description = "Глаза слипаются. Веки тяжелые, как свинец. Реакция замедлена. "
                       "Я вижу дорогу как в тумане. Это опасно - могу уснуть за рулём.";
    event.oneTimeOnly = false;

    EventChoice sleep;
    sleep.text = "Остановиться и поспать (2 часа)";
    sleep.outcomeText = "Проспал 2 часа на обочине. Голова яснее, можно ехать.";
    sleep.energyChange = 50.0f;
    sleep.fuelChange = 0.0f;

    EventChoice coffee;
    coffee.text = "Кофе и в путь";
    coffee.outcomeText = "Крепкий кофе помогает, но ненадолго.";
    coffee.energyChange = 15.0f;
    coffee.moneyChange = -50.0f;

    event.choices = {sleep, coffee};
    return event;
}

GameEvent createResourceEnergyTired() {
    GameEvent event;
    event.id = "resource_energy_tired";
    event.type = EventType::RESOURCE;
    event.title = "😴 Усталость накапливается";
    event.description = "Чувствую усталость. Концентрация падает, хочется отдохнуть. "
                       "Ещё можно ехать, но лучше бы передохнуть.";
    event.oneTimeOnly = false;

    EventChoice rest;
    rest.text = "Остановка на 30 минут";
    rest.outcomeText = "Короткий отдых освежил голову.";
    rest.energyChange = 20.0f;

    EventChoice music;
    music.text = "Включить бодрую музыку и ехать";
    music.outcomeText = "Музыка помогает взбодриться.";
    music.energyChange = 10.0f;

    EventChoice push;
    push.text = "Терпеть до следующего города";
    push.outcomeText = "Еду через силу, стискиваю зубы.";
    push.energyChange = -5.0f;

    event.choices = {rest, music, push};
    return event;
}

GameEvent createResourceMoneyBroke() {
    GameEvent event;
    event.id = "resource_money_broke";
    event.type = EventType::RESOURCE;
    event.title = "💸 КРИТИЧНО: ПОЧТИ БЕЗ ДЕНЕГ";
    event.description = "В кармане меньше 50 рублей. Этого хватит на булку хлеба, не больше. "
                       "Нужно что-то придумать, иначе дальше не доеду.";
    event.oneTimeOnly = false;

    EventChoice work;
    work.text = "Поискать подработку";
    work.outcomeText = "Разгрузил фуру за 200₽. Тяжело, но деньги нужны.";
    work.moneyChange = 200.0f;
    work.energyChange = -25.0f;

    EventChoice sell;
    sell.text = "Продать что-то ненужное";
    sell.outcomeText = "Продал старую куртку попутчику за 100₽.";
    sell.moneyChange = 100.0f;

    EventChoice careful;
    careful.text = "Экономить каждую копейку";
    careful.outcomeText = "Придётся жить впроголодь.";
    careful.energyChange = -10.0f;

    event.choices = {work, sell, careful};
    return event;
}

GameEvent createResourceMoneyLow() {
    GameEvent event;
    event.id = "resource_money_low";
    event.type = EventType::RESOURCE;
    event.title = "💸 Деньги на исходе";
    event.description = "В кармане меньше 200₽. Хватит на пару дней, если экономить. "
                       "Стоит подумать, как пополнить бюджет.";
    event.oneTimeOnly = false;

    EventChoice work;
    work.text = "Поискать мелкую работу";
    work.outcomeText = "Помог на стройке, заработал 300₽.";
    work.moneyChange = 300.0f;
    work.energyChange = -15.0f;

    EventChoice save;
    save.text = "Экономить то, что есть";
    save.outcomeText = "Стараюсь тратить поменьше.";
    save.energyChange = -5.0f;

    event.choices = {work, save};
    return event;
}

GameEvent createResourceVehicleBroken() {
    GameEvent event;
    event.id = "resource_vehicle_broken";
    event.type = EventType::RESOURCE;
    event.title = "🔧 КРИТИЧНО: МАШИНА НА ГРАНИ";
    event.description = "Волга едва тянет. Стуки, скрипы, вибрация. Состояние критическое. "
                       "Если не починить сейчас - встану посреди дороги.";
    event.oneTimeOnly = false;

    EventChoice repair;
    repair.text = "Срочный ремонт на СТО";
    repair.outcomeText = "Отдал 800₽ за ремонт. Машина снова на ходу.";
    repair.vehicleConditionChange = 40.0f;
    repair.moneyChange = -800.0f;
    repair.energyChange = -10.0f;

    EventChoice self;
    self.text = "Попытаться починить самому";
    self.outcomeText = "Ковырялся 2 часа. Вроде стало лучше.";
    self.vehicleConditionChange = 20.0f;
    self.energyChange = -25.0f;

    event.choices = {repair, self};
    return event;
}

GameEvent createResourceVehicleDamaged() {
    GameEvent event;
    event.id = "resource_vehicle_damaged";
    event.type = EventType::RESOURCE;
    event.title = "🔧 Машина повреждена";
    event.description = "Волга работает, но не идеально. Появились странные звуки, "
                       "тяга слабее обычного. Пора бы заехать к механику.";
    event.oneTimeOnly = false;

    EventChoice service;
    service.text = "Заехать на техобслуживание";
    service.outcomeText = "Механик всё проверил и подтянул. Едет лучше.";
    service.vehicleConditionChange = 30.0f;
    service.moneyChange = -400.0f;

    EventChoice check;
    check.text = "Проверить самому, что не так";
    check.outcomeText = "Нашёл пару проблем и устранил.";
    check.vehicleConditionChange = 15.0f;
    check.energyChange = -10.0f;

    EventChoice ignore;
    ignore.text = "Ехать дальше, пока тянет";
    ignore.outcomeText = "Авось доеду...";
    ignore.vehicleConditionChange = -5.0f;

    event.choices = {service, check, ignore};
    return event;
}

GameEvent createResourceMoodDepressed() {
    GameEvent event;
    event.id = "resource_mood_depressed";
    event.type = EventType::RESOURCE;
    event.title = "😔 Депрессия";
    event.description = "Всё валится из рук. Зачем я вообще это затеял? "
                       "Хочется бросить всё и вернуться обратно. Чувствую себя потерянным.";
    event.oneTimeOnly = false;

    EventChoice call;
    call.text = "Позвонить близким";
    call.outcomeText = "Поговорил с семьёй. Стало легче на душе. (+энергия)";
    call.energyChange = 15.0f;
    call.moneyChange = -30.0f;

    EventChoice walk;
    walk.text = "Прогуляться, подышать воздухом";
    walk.outcomeText = "Свежий воздух и смена обстановки помогли. (+энергия)";
    walk.energyChange = 10.0f;

    EventChoice push;
    push.text = "Просто ехать дальше";
    push.outcomeText = "Стискиваю зубы и давлю на газ.";
    push.energyChange = -10.0f;

    event.choices = {call, walk, push};
    return event;
}

GameEvent createResourceMoodLow() {
    GameEvent event;
    event.id = "resource_mood_low";
    event.type = EventType::RESOURCE;
    event.title = "😔 Плохое настроение";
    event.description = "Настроение на нуле. Всё раздражает: дорога, машина, погода. "
                       "Нужно как-то развеяться.";
    event.oneTimeOnly = false;

    EventChoice music;
    music.text = "Включить любимую музыку";
    music.outcomeText = "Знакомые мелодии поднимают настроение. (+энергия)";
    music.energyChange = 5.0f;

    EventChoice cafe;
    cafe.text = "Заехать в кафе, выпить чаю";
    cafe.outcomeText = "Горячий чай и спокойная обстановка - то что нужно.";
    cafe.moneyChange = -80.0f;
    cafe.energyChange = 15.0f;

    EventChoice ignore;
    ignore.text = "Просто ехать дальше";
    ignore.outcomeText = "Надо терпеть, не до настроения.";

    event.choices = {music, cafe, ignore};
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
    eventManager.addEvent(createTruckerStories());
    eventManager.addEvent(createStudentPhilosophy());
    eventManager.addEvent(createUnemployedAdvice());
    eventManager.addEvent(createMechanicTeachSkill());

    // Дорожные события
    eventManager.addEvent(createCarBreakdown());
    eventManager.addEvent(createPoliceStop());
    eventManager.addEvent(createCrossroads());
    eventManager.addEvent(createBadWeather());

    // Ресурсные события
    eventManager.addEvent(createRoadsideFinding());
    eventManager.addEvent(createHitchhikerTrade());

    // СОБЫТИЯ ДЛЯ ВЕТКИ А (Системный анализ)
    eventManager.addEvent(createBranchA_BrokenSign());
    eventManager.addEvent(createBranchA_EngineNoise());
    eventManager.addEvent(createBranchA_WeatherPattern());
    eventManager.addEvent(createBranchA_FuelConsumption());
    eventManager.addEvent(createBranchA_MapDiscrepancy());

    // СОБЫТИЯ ДЛЯ ВЕТКИ Б (Чувство материала)
    eventManager.addEvent(createBranchB_RoadFeel());
    eventManager.addEvent(createBranchB_MetalFatigue());
    eventManager.addEvent(createBranchB_TirePressure());
    eventManager.addEvent(createBranchB_FuelQuality());
    eventManager.addEvent(createBranchB_BrakeFeel());

    // СОБЫТИЯ ДЛЯ ВЕТКИ В (Командная работа)
    eventManager.addEvent(createBranchC_TruckerAdvice());
    eventManager.addEvent(createBranchC_MechanicHelp());
    eventManager.addEvent(createBranchC_LocalKnowledge());
    eventManager.addEvent(createBranchC_ConvoyOffer());
    eventManager.addEvent(createBranchC_RadioCall());

    // СОБЫТИЯ ДЛЯ ВЕТКИ Г (Ресурсоэффективность)
    eventManager.addEvent(createBranchD_FuelChoice());
    eventManager.addEvent(createBranchD_Shortcut());
    eventManager.addEvent(createBranchD_RestStop());
    eventManager.addEvent(createBranchD_RepairChoice());
    eventManager.addEvent(createBranchD_FoodPlanning());

    // СЛУЧАЙНЫЕ СОБЫТИЯ (для всех веток)
    eventManager.addEvent(createRandom_FlatTire());
    eventManager.addEvent(createRandom_Hungry());
    eventManager.addEvent(createRandom_Rain());
    eventManager.addEvent(createRandom_PoliceCheck());
    eventManager.addEvent(createRandom_Hitchhiker());
    eventManager.addEvent(createRandom_ScenicView());

    // СОБЫТИЯ НА ОСНОВЕ РЕСУРСОВ
    eventManager.addEvent(createResourceFuelCritical());
    eventManager.addEvent(createResourceFuelLow());
    eventManager.addEvent(createResourceEnergyExhausted());
    eventManager.addEvent(createResourceEnergyTired());
    eventManager.addEvent(createResourceMoneyBroke());
    eventManager.addEvent(createResourceMoneyLow());
    eventManager.addEvent(createResourceVehicleBroken());
    eventManager.addEvent(createResourceVehicleDamaged());
    eventManager.addEvent(createResourceMoodDepressed());
    eventManager.addEvent(createResourceMoodLow());
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
