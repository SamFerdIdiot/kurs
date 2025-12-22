// === ВЕТКА В: СПРОСИ БЫВАЛОГО (Командная работа) ===

void NotebookScene::loadDay0_BranchC_Truck() {
    // ЗАПИСЬ 3В: ПОМОЩЬ НА ОБОЧИНЕ
    NotebookEntry entry("day0_branch_c_truck", EntryType::PAST,
        "ВСТРЕЧА: 9:15\n\n"
        "«А у тебя такое было?»\n\n"
        "Денис усмехнулся:\n\n"
        "«Раз сто. Обычно это карбюратор или топливный фильтр. "
        "Но в 70% случаев — плохой бензин. Последняя заправка?»\n\n"
        "Я вспомнил — действительно, заправка была странная. Старая, почти заброшенная.\n\n"
        "«Вот и ответ, — Денис остановился. — Инженер-одиночка — это миф. "
        "Настоящий профессионал умеет использовать коллективный опыт. "
        "Спрашивать — не стыдно. Стыдно — повторять чужие ошибки.»\n\n"
        "Он почистил карбюратор. Машина поехала ровно.\n\n"
        "---\n\n"
        "Грузовик «ЗИЛ» с открытым капотом. Шофёр матерился, копаясь в проводке.\n\n"
        "«Знакомый. — Бросил он. — Поможешь. Но сам договаривайся, что и как.»\n\n"
        "Я поговорил с водителем. Проблема — предохранитель. Запасного нет. "
        "Нашли в «Волге» кусок медной проволоки, поставили на время.\n\n"
        "«Выручили, мужики. — Шофёр вытер руки об брюки. — Давайте, я вас хоть чаем угощу, в той столовой.»");

    entry.date = "Полгода назад, 9:15";
    entry.location = "Обочина трассы";
    entry.mood = "Взаимопомощь";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить]";
    choice1.nextEntryIds = {"day0_branch_c_teahouse"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchC_Teahouse() {
    // ЗАПИСЬ 4В: ЧАЙ И РАЗГОВОРЫ (Выбор артефакта)
    NotebookEntry entry("day0_branch_c_teahouse", EntryType::PAST,
        "ДИАЛОГ: 10:30, ПРИДОРОЖНАЯ СТОЛОВАЯ\n\n"
        "Шофёр оказался бывшим с их завода.\n\n"
        "«Помнишь, как мы тот цех собирали? Без чертежей, на глаз. Но работало — мать его. "
        "Потому что друг друга понимали с полуслова.»\n\n"
        "Они вспоминали. Я слушал. Истории про работу, которая была общим делом.\n\n"
        "«Держи. — Шофёр, уезжая, сунул мне бумажку. — Телёнок мой, там, за бугром работает. "
        "Поможет, если что. Или…» — достал потрёпанную фотку: несколько человек у станка.");

    entry.date = "Полгода назад, 10:30";
    entry.location = "Придорожная столовая";
    entry.mood = "Братство";
    entry.printSpeed = 45.0f;

    // Выбор 1: Взять телефон (контакт)
    NotebookChoice choicePhone;
    choicePhone.text = "Взять телефон (контакт)";
    choicePhone.nextEntryIds = {"day0_provocation"};
    choicePhone.action = [](PlayerState* player) {
        player->addMoney(100.0f);  // Условная ценность контакта
        player->addPrinciple("ask_experienced");  // ПРИНЦИП: Спроси бывалого
        std::cout << "[Day0-C] Получен контакт + Принцип: Спроси бывалого" << std::endl;
    };
    entry.addChoice(choicePhone);

    // Выбор 2: Взять фотографию (память)
    NotebookChoice choicePhoto;
    choicePhoto.text = "Взять фотографию";
    choicePhoto.nextEntryIds = {"day0_provocation"};
    choicePhoto.action = [](PlayerState* player) {
        player->addStoryItem("photo_at_machine");  // Артефакт: Фотка у станка
        player->addPrinciple("ask_experienced");  // ПРИНЦИП: Спроси бывалого
        std::cout << "[Day0-C] Получен артефакт: Фотка у станка + Принцип" << std::endl;
    };
    entry.addChoice(choicePhoto);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch C-2): " << entry.id << std::endl;
}

// === ВЕТКА Г: РЕСУРСОЭФФЕКТИВНОСТЬ (Быстро и по делу) ===

void NotebookScene::loadDay0_BranchD_Route() {
    // ЗАПИСЬ 3Г: РАСЧЁТ МАРШРУТА
    NotebookEntry entry("day0_branch_d_route", EntryType::PAST,
        "ЗАДАНИЕ: 8:30\n\n"
        "Я сразу начал проверять: свечи, топливный фильтр, карбюратор. Самые частые причины.\n\n"
        "Карбюратор оказался забит. Я показал Денису.\n\n"
        "«Быстро, — кивнул он. — Но есть проблема. Ты проверил три вещи наугад. "
        "А если бы причина была в четвёртой? Ты потратил бы время впустую.»\n\n"
        "Он почистил карбюратор, продолжая:\n\n"
        "«Принцип Парето: 80% проблем вызваны 20% причин. Ты действовал правильно — "
        "начал с самого вероятного. Но запомни: ресурсоэффективность — это не спешка. "
        "Это умение расставлять приоритеты.»\n\n"
        "---\n\n"
        "«Бензина полбака. Доехать до озера. Проложи маршрут и скажи, как вести, чтобы растянуть.»\n\n"
        "Я предложил прямой путь и постоянную скорость.\n\n"
        "«Прямолинейно. Но ветер боковой, и в конце подъём. Иногда длиннее — выгоднее. "
        "Считай не километры, а литры на конечную точку.»");

    entry.date = "Полгода назад, 8:30";
    entry.location = "Трасса";
    entry.mood = "Расчётливость";
    entry.printSpeed = 45.0f;

    NotebookChoice choice1;
    choice1.text = "[Продолжить]";
    choice1.nextEntryIds = {"day0_branch_d_boys"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-1): " << entry.id << std::endl;
}

void NotebookScene::loadDay0_BranchD_Boys() {
    // ЗАПИСЬ 4Г: МАЛЬЧИШКИ И МЕТАЛЛ (Выбор артефакта)
    NotebookEntry entry("day0_branch_d_boys", EntryType::PAST,
        "ВСТРЕЧА: 10:00, ОПУШКА\n\n"
        "Старый «Москвич» разбирали на запчасти пацаны.\n\n"
        "«Дядь, не мешай! Машина мёртвая!»\n\n"
        "«Можно прогнать, — сказал он. — А можно выторговать что-то полезное.»\n\n"
        "Я поторговался. Купил у них исправный стартер и свечи дешевле, "
        "чем они бы сдали в металлолом.\n\n"
        "«На, ещё вот. — Старший мальчишка сунул мне игрушечную машинку, такую же. — За бесплатно.»");

    entry.date = "Полгода назад, 10:00";
    entry.location = "Опушка";
    entry.mood = "Торговля";
    entry.printSpeed = 45.0f;

    // Выбор 1: Взять стартер и свечи (полезное)
    NotebookChoice choiceStarter;
    choiceStarter.text = "Взять стартер и свечи (полезное)";
    choiceStarter.nextEntryIds = {"day0_provocation"};
    choiceStarter.action = [](PlayerState* player) {
        player->addStoryItem("spare_starter");  // Артефакт: Исправный стартер
        player->addPrinciple("resource_efficient");  // ПРИНЦИП: Ресурсоэффективность
        std::cout << "[Day0-D] Получен артефакт: Стартер + Принцип: Ресурсоэффективность" << std::endl;
    };
    entry.addChoice(choiceStarter);

    // Выбор 2: Взять игрушку (сентименты)
    NotebookChoice choiceToy;
    choiceToy.text = "Взять игрушку (бесполезно, но...)";
    choiceToy.nextEntryIds = {"day0_provocation"};
    choiceToy.action = [](PlayerState* player) {
        player->addMoney(25.0f);  // Меньше выгоды
        player->addPrinciple("resource_efficient");  // Принцип все равно получаем
        std::cout << "[Day0-D] Получена игрушка + Принцип: Ресурсоэффективность" << std::endl;
    };
    entry.addChoice(choiceToy);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Branch D-2): " << entry.id << std::endl;
}

// === ОБЩАЯ ЗАПИСЬ: ПРОВОКАЦИЯ (для всех веток) ===

void NotebookScene::loadDay0_Provocation() {
    // ЗАПИСЬ 5: ПРОВОКАЦИЯ
    NotebookEntry entry("day0_provocation", EntryType::PAST,
        "ВОСПОМИНАНИЕ: ПОЛГОДА НАЗАД, 8:15\n\n"
        "— Что, обдумываешь моё предложение? — спросил он, не глядя. — "
        "Не тормози. Я не вечный. Нужен человек, который головой работает, а не ждёт указаний.");

    entry.date = "Полгода назад, 8:15";
    entry.location = "Волга";
    entry.mood = "Напряжённый";
    entry.printSpeed = 45.0f;

    // Выбор 1: Огрызнуться
    NotebookChoice choice1;
    choice1.text = "«Сам ты вечно ворчишь.»";
    choice1.nextEntryIds = {"day0_garage_finale"};
    choice1.action = [](PlayerState* player) {
        player->addTrait("impulsive");  // Черта: импульсивный
        std::cout << "[Day0] Игрок огрызнулся (черта: импульсивный)" << std::endl;
    };
    entry.addChoice(choice1);

    // Выбор 2: Подумать
    NotebookChoice choice2;
    choice2.text = "«Я думаю.»";
    choice2.nextEntryIds = {"day0_garage_finale"};
    choice2.action = [](PlayerState* player) {
        player->addTrait("patient");  // Черта: терпеливый
        std::cout << "[Day0] Игрок подумал (черта: терпеливый)" << std::endl;
    };
    entry.addChoice(choice2);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Provocation): " << entry.id << std::endl;
}

// === ФИНАЛ: ГАРАЖ (общий для всех веток) ===

void NotebookScene::loadDay0_GarageFinale() {
    // ЗАПИСЬ 6: ПЕРЕДАЧА ИНСТРУМЕНТА (в гараже)
    NotebookEntry entry("day0_garage_finale", EntryType::PAST,
        "ВОСПОМИНАНИЕ: ПОЛГОДА НАЗАД, ВЕЧЕР, ГАРАЖ\n\n"
        "Он подвёл меня к «Москвичу».\n\n"
        "— Вот. Твой. Я всё, что мог, поправил. Остальное — на тебе. — "
        "Сунул ключи и потрёпанный конверт. — Документы. Контракт с заводом там, за границей. "
        "Адрес общежития для рабочих. Больше у меня для тебя ничего нет.\n\n"
        "Положил руку на крышу, смотрел куда-то мимо.\n\n"
        "— Механизм ломается, когда используется не по делу. — Он отвёл взгляд. — "
        "Своё дело найдёшь — выживешь. Не найдёшь… Ну, хоть попробуешь. "
        "Пиши, как устроился. Если будет что писать.\n\n"
        "Развернулся и пошёл прочь, не прощаясь.");

    entry.date = "Полгода назад, вечер";
    entry.location = "Гараж Дениса";
    entry.mood = "Прощание";
    entry.printSpeed = 35.0f;  // Медленнее для драматизма

    NotebookChoice choice1;
    choice1.text = "[Продолжить в настоящее время]";
    choice1.nextEntryIds = {"day0_transition_present"};
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Garage Finale): " << entry.id << std::endl;
}

// === ПЕРЕХОД В НАСТОЯЩЕЕ ===

void NotebookScene::loadDay0_TransitionPresent() {
    // ЗАПИСЬ 7: НАСТОЯЩЕЕ ВРЕМЯ. ДОРОГА
    NotebookEntry entry("day0_transition_present", EntryType::PRESENT,
        "Сегодня. Дорога.\n\n"
        "И вот сейчас, в этом безымянном коридоре между странами, "
        "его голос в голове чёток, как никогда. Не наставник из кино. "
        "Просто уставший мужик, который передал мне то, что считал нужным.\n\n"
        "В бардачке — артефакт с того дня. Конверт. Ключи от машины, "
        "которая теперь и дом, и средство передвижения, и единственный актив.\n\n"
        "Он остался там. Я еду сюда. Не за счастьем. За шансом. За работой. "
        "За возможностью не сломаться.\n\n"
        "Впереди — неизвестность. Заграничный завод. Чужая страна. "
        "Общежитие для таких же, как я.\n\n"
        "А между прошлым и будущим — только этот гул мотора, дорога и его последний вопрос, "
        "который теперь звучит во мне самом:\n\n"
        "«Ну что, выживешь?»");

    entry.date = "Сегодня";
    entry.location = "В пути";
    entry.mood = "Решимость";
    entry.printSpeed = 40.0f;

    NotebookChoice choice1;
    choice1.text = "[Закончить пролог]";
    choice1.nextEntryIds = {};  // Пустое = возврат в главное меню
    entry.addChoice(choice1);

    m_entries[entry.id] = entry;
    std::cout << "[NotebookScene] Loaded Day 0 entry (Transition to Present): " << entry.id << std::endl;
}
