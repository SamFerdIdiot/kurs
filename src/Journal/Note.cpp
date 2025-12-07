#include "Journal/Note.h"
#include "PlayerState.h"
#include "UIConstants.h"
#include <iostream>

// Helper function to convert UTF-8 string to SFML string
static sf::String utf8(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

void Note::render(sf::RenderWindow& window, const sf::Font& font, int selectedChoiceIndex) {
    // Базовая реализация рендеринга записки
    // Наследники могут переопределить для кастомизации

    const float PADDING = 50.0f;
    const float LINE_SPACING = 1.3f;

    // === ФОН ЗАПИСКИ (цвет старой бумаги) ===
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(UI::SCREEN_WIDTH * 0.9f, UI::SCREEN_HEIGHT * 0.85f));
    background.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f, UI::SCREEN_HEIGHT * 0.05f));
    background.setFillColor(sf::Color(245, 235, 220));  // Старая бумага
    background.setOutlineColor(sf::Color(100, 90, 80));
    background.setOutlineThickness(2.0f);
    window.draw(background);

    float yPos = UI::SCREEN_HEIGHT * 0.05f + PADDING;

    // === ЗАГОЛОВОК ===
    sf::Text titleText(font);
    titleText.setString(utf8(getTitle()));
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color(50, 40, 30));  // Темно-коричневый
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    window.draw(titleText);

    yPos += 50.0f;

    // === ОПИСАНИЕ ===
    sf::Text descText(font);
    descText.setString(utf8(getDescription()));
    descText.setCharacterSize(18);
    descText.setFillColor(sf::Color(50, 40, 30));
    descText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
    descText.setLineSpacing(LINE_SPACING);

    // Обернуть текст по ширине
    float maxWidth = UI::SCREEN_WIDTH * 0.8f;
    // TODO: Реализовать word wrap если текст слишком длинный

    window.draw(descText);

    // Приблизительная высота описания (предполагаем ~6 строк)
    yPos += 200.0f;

    // === ВАРИАНТЫ ВЫБОРА ===
    auto choices = getChoices();
    if (!choices.empty()) {
        // Заголовок выборов
        sf::Text choicesHeader(font);
        choicesHeader.setString(utf8("ВАШ ВЫБОР:"));
        choicesHeader.setCharacterSize(20);
        choicesHeader.setFillColor(sf::Color(100, 50, 50));  // Красноватый
        choicesHeader.setStyle(sf::Text::Bold);
        choicesHeader.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
        window.draw(choicesHeader);

        yPos += 40.0f;

        // Отрисовка каждого выбора
        for (size_t i = 0; i < choices.size(); i++) {
            const auto& choice = choices[i];

            // Подсветка выбранного варианта
            if (static_cast<int>(i) == selectedChoiceIndex) {
                sf::RectangleShape highlight;
                highlight.setSize(sf::Vector2f(UI::SCREEN_WIDTH * 0.75f, 30.0f));
                highlight.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING - 10.0f, yPos - 5.0f));
                highlight.setFillColor(sf::Color(255, 240, 200, 150));  // Светло-желтая подсветка
                window.draw(highlight);
            }

            // Стрелка для выбранного варианта
            std::string arrow = (static_cast<int>(i) == selectedChoiceIndex) ? "▸ " : "  ";

            // Номер варианта
            std::string choiceText = arrow + "[" + std::to_string(i + 1) + "] " + choice.text;

            sf::Text choiceTextSf(font);
            choiceTextSf.setString(utf8(choiceText));
            choiceTextSf.setCharacterSize(18);

            // Цвет зависит от доступности
            if (choice.isDisabled) {
                choiceTextSf.setFillColor(sf::Color(150, 140, 130));  // Серый для недоступных
            } else {
                choiceTextSf.setFillColor(sf::Color(50, 40, 30));
            }

            choiceTextSf.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING, yPos));
            window.draw(choiceTextSf);

            // Причина недоступности
            if (choice.isDisabled && !choice.disabledReason.empty()) {
                sf::Text reasonText(font);
                reasonText.setString(utf8("  ⚠️ " + choice.disabledReason));
                reasonText.setCharacterSize(14);
                reasonText.setFillColor(sf::Color(180, 50, 50));  // Красный
                reasonText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.05f + PADDING + 30.0f, yPos + 22.0f));
                window.draw(reasonText);

                yPos += 50.0f;  // Больше места для двустрочного варианта
            } else {
                yPos += 35.0f;
            }
        }
    }

    // === ПОДСКАЗКА УПРАВЛЕНИЯ ===
    yPos = UI::SCREEN_HEIGHT * 0.85f + 10.0f;
    sf::Text hintText(font);
    hintText.setString(utf8("[Используйте 1-9 или стрелки + Enter]"));
    hintText.setCharacterSize(14);
    hintText.setFillColor(sf::Color(120, 110, 100));  // Светло-коричневый
    hintText.setPosition(sf::Vector2f(UI::SCREEN_WIDTH * 0.5f - 200.0f, yPos));
    window.draw(hintText);
}
