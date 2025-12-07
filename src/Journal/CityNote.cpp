#include "Journal/CityNote.h"
#include <sstream>
#include <iostream>

CityNote::CityNote(const std::string& cityId, PlayerState* playerState)
    : Note("city_" + cityId),
      m_cityId(cityId),
      m_playerState(playerState),
      m_selectedAction(Action::NONE) {
}

std::string CityNote::getTitle() const {
    return getCityName() + " - Дневник путешественника";
}

std::string CityNote::getCityName() const {
    // Маппинг ID городов на названия
    if (m_cityId == "moscow") return "МОСКВА";
    if (m_cityId == "spb") return "САНКТ-ПЕТЕРБУРГ";
    if (m_cityId == "vladivostok") return "ВЛАДИВОСТОК";
    if (m_cityId == "kazan") return "КАЗАНЬ";
    if (m_cityId == "ekaterinburg") return "ЕКАТЕРИНБУРГ";
    if (m_cityId == "novosibirsk") return "НОВОСИБИРСК";

    // По умолчанию - преобразовать ID с заглавной буквы
    std::string name = m_cityId;
    if (!name.empty()) {
        name[0] = std::toupper(name[0]);
    }
    return name;
}

std::string CityNote::getDescription() const {
    return generateCityDescription();
}

std::string CityNote::generateCityDescription() const {
    std::stringstream ss;

    // Дневниковая запись от первого лица
    ss << "Я въехал в " << getCityName() << ". ";

    // Атмосфера на основе времени суток
    int hour = 12;  // TODO: Получить из PlayerState

    if (hour >= 0 && hour < 6) {
        ss << "Глубокая ночь. Город спит, улицы пусты. ";
        ss << "Только редкие фонари мерцают в темноте.";
    } else if (hour >= 6 && hour < 12) {
        ss << "Утро. Город просыпается. ";
        ss << "Люди спешат по своим делам, не обращая внимания на чужака.";
    } else if (hour >= 12 && hour < 18) {
        ss << "День в разгаре. ";
        ss << "Шум, движение, суета. Типичный российский город.";
    } else {
        ss << "Вечер. Зажигаются огни. ";
        ss << "Город постепенно засыпает.";
    }

    ss << "\n\n";

    // Мысли героя о своем состоянии
    if (m_playerState) {
        if (m_playerState->getFuel() < 20.0f) {
            ss << "Надо заправиться, бензина почти не осталось. ";
        }
        if (m_playerState->getEnergy() < 30.0f) {
            ss << "Я устал. Нужно отдохнуть. ";
        }
        if (m_playerState->getMoney() < 200.0f) {
            ss << "Деньги на исходе...";
        }
    }

    ss << "\n\nЧто мне делать?";

    return ss.str();
}

std::vector<NoteChoice> CityNote::getChoices() const {
    std::vector<NoteChoice> choices;
    buildChoices(choices);
    return choices;
}

void CityNote::buildChoices(std::vector<NoteChoice>& choices) const {
    if (!m_playerState) {
        return;
    }

    // === ЗАПРАВКА ===
    const int GAS_COST = 500;
    const float FULL_TANK = 100.0f;

    if (m_playerState->getMoney() >= GAS_COST) {
        choices.push_back(NoteChoice(
            "Заправиться (" + std::to_string(GAS_COST) + "₽)",
            "Я заправил полный бак. Теперь можно ехать дальше.",
            [this](PlayerState* ps) {
                const_cast<CityNote*>(this)->m_selectedAction = Action::GAS_STATION;
            }
        ));
    } else {
        NoteChoice gasChoice;
        gasChoice.text = "Заправиться (" + std::to_string(GAS_COST) + "₽)";
        gasChoice.isDisabled = true;
        gasChoice.disabledReason = "Не хватает денег";
        choices.push_back(gasChoice);
    }

    // === РАБОТА ===
    choices.push_back(NoteChoice(
        "Поискать работу",
        "Надо заработать денег. Пойду поищу, кому нужны рабочие руки...",
        [this](PlayerState* ps) {
            const_cast<CityNote*>(this)->m_selectedAction = Action::WORK;
        }
    ));

    // === МОТЕЛЬ ===
    const int MOTEL_COST = 1000;

    if (m_playerState->getMoney() >= MOTEL_COST && m_playerState->getEnergy() < 80.0f) {
        choices.push_back(NoteChoice(
            "Снять номер в мотеле (" + std::to_string(MOTEL_COST) + "₽)",
            "Я снял номер и хорошо выспался. Чувствую себя гораздо лучше.",
            [this](PlayerState* ps) {
                const_cast<CityNote*>(this)->m_selectedAction = Action::MOTEL;
            }
        ));
    } else if (m_playerState->getMoney() < MOTEL_COST) {
        NoteChoice motelChoice;
        motelChoice.text = "Снять номер в мотеле (" + std::to_string(MOTEL_COST) + "₽)";
        motelChoice.isDisabled = true;
        motelChoice.disabledReason = "Не хватает денег";
        choices.push_back(motelChoice);
    }

    // === МАГАЗИН ===
    choices.push_back(NoteChoice(
        "Зайти в магазин",
        "Пойду куплю что-нибудь в дорогу...",
        [this](PlayerState* ps) {
            const_cast<CityNote*>(this)->m_selectedAction = Action::SHOP;
        }
    ));

    // === ПРОДОЛЖИТЬ ПУТЬ ===
    if (m_playerState->getFuel() > 20.0f) {
        choices.push_back(NoteChoice(
            "Ехать дальше",
            "Пора в дорогу. Завожу двигатель...",
            [this](PlayerState* ps) {
                const_cast<CityNote*>(this)->m_selectedAction = Action::TRAVEL;
            }
        ));
    } else {
        NoteChoice travelChoice;
        travelChoice.text = "Ехать дальше";
        travelChoice.isDisabled = true;
        travelChoice.disabledReason = "Бензина не хватит";
        choices.push_back(travelChoice);
    }
}

void CityNote::executeChoice(int choiceIndex, PlayerState* playerState) {
    auto choices = getChoices();

    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(choices.size())) {
        std::cerr << "Invalid choice index: " << choiceIndex << std::endl;
        return;
    }

    const auto& choice = choices[choiceIndex];

    // Пропустить недоступные варианты
    if (choice.isDisabled) {
        std::cout << "[CityNote] Choice disabled: " << choice.disabledReason << std::endl;
        return;
    }

    // Выполнить действие выбора (устанавливает m_selectedAction)
    if (choice.action) {
        choice.action(playerState);
    }

    // Применить специфичные эффекты в зависимости от действия
    switch (m_selectedAction) {
        case Action::GAS_STATION:
            playerState->modifyMoney(-500.0f);
            playerState->setFuel(100.0f);
            std::cout << "[CityNote] Refueled to 100L, spent 500₽" << std::endl;
            setFinished(true);  // Закончили с этой запиской
            break;

        case Action::WORK:
            std::cout << "[CityNote] Selected WORK - will show WorkNote" << std::endl;
            setFinished(true);
            break;

        case Action::MOTEL:
            playerState->modifyMoney(-1000.0f);
            playerState->setEnergy(100.0f);
            std::cout << "[CityNote] Rested at motel, spent 1000₽" << std::endl;
            setFinished(true);
            break;

        case Action::SHOP:
            std::cout << "[CityNote] Selected SHOP - will show ShopNote" << std::endl;
            setFinished(true);
            break;

        case Action::TRAVEL:
            std::cout << "[CityNote] Selected TRAVEL - will show TravelNote" << std::endl;
            setFinished(true);
            break;

        default:
            break;
    }

    std::cout << "[CityNote] Executed choice " << choiceIndex << ": "
              << choice.text << std::endl;
}
