#include "Notebook/ThoughtSystem.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>


static sf::String utf8(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

ThoughtSystem::ThoughtSystem()
    : m_thoughtTimer(0.0f)
    , m_nextThoughtDelay(FIRST_THOUGHT_DELAY)
    , m_thoughtsSpawned(0)
    , m_testMode(false)
{

    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    initializeThoughts();
}

void ThoughtSystem::update(float deltaTime, float timeSinceChoice) {

    m_thoughtTimer += deltaTime;


    if (timeSinceChoice >= m_nextThoughtDelay && m_thoughtsSpawned < 3) {
        spawnThought(getRandomThought());
        m_thoughtsSpawned++;


        if (m_thoughtsSpawned == 1) {
            m_nextThoughtDelay = m_testMode ? TEST_SECOND_THOUGHT_DELAY : SECOND_THOUGHT_DELAY;
        } else if (m_thoughtsSpawned == 2) {
            m_nextThoughtDelay = m_testMode ? TEST_THIRD_THOUGHT_DELAY : THIRD_THOUGHT_DELAY;
        }
    }


    for (auto& thought : m_activeThoughts) {
        thought.lifetime += deltaTime;
        updateThoughtAlpha(thought);
    }


    m_activeThoughts.erase(
        std::remove_if(m_activeThoughts.begin(), m_activeThoughts.end(),
            [](const ThoughtBubble& t) { return t.lifetime >= t.maxLifetime; }),
        m_activeThoughts.end()
    );
}

void ThoughtSystem::render(sf::RenderWindow& window, const sf::Font& font) {
    const float screenWidth = static_cast<float>(window.getSize().x);
    const float screenHeight = static_cast<float>(window.getSize().y);

    for (const auto& thought : m_activeThoughts) {

        sf::Text text(font, utf8(thought.text), 32);

        text.setFillColor(sf::Color(40, 40, 40, static_cast<uint8_t>(thought.alpha)));


        sf::FloatRect bounds = text.getLocalBounds();
        float textWidth = bounds.size.x;
        float textHeight = bounds.size.y;

        float textPosX = (screenWidth - textWidth) / 2.0f;
        float textPosY = (screenHeight - textHeight) / 2.0f;

        text.setPosition(sf::Vector2f(textPosX, textPosY));


        sf::RectangleShape background;
        background.setSize(sf::Vector2f(textWidth + 40.0f, textHeight + 40.0f));
        background.setPosition(sf::Vector2f(
            textPosX - 20.0f,
            textPosY - 20.0f
        ));
        background.setFillColor(sf::Color(30, 30, 30, static_cast<uint8_t>(thought.alpha * 0.7f)));

        window.draw(background);
        window.draw(text);
    }
}

void ThoughtSystem::reset() {
    m_thoughtTimer = 0.0f;
    m_nextThoughtDelay = m_testMode ? TEST_FIRST_THOUGHT_DELAY : FIRST_THOUGHT_DELAY;
    m_thoughtsSpawned = 0;
    m_activeThoughts.clear();
}

void ThoughtSystem::spawnThought(const std::string& text) {
    ThoughtBubble thought;
    thought.text = text;
    thought.lifetime = 0.0f;
    thought.maxLifetime = THOUGHT_LIFETIME;
    thought.alpha = 0.0f;
    thought.position = sf::Vector2f(0.0f, 0.0f);

    m_activeThoughts.push_back(thought);
}

std::string ThoughtSystem::getRandomThought() const {
    if (m_generalThoughts.empty()) {
        return "...";
    }

    int index = std::rand() % m_generalThoughts.size();
    return m_generalThoughts[index];
}

void ThoughtSystem::updateThoughtAlpha(ThoughtBubble& thought) const {

    if (thought.lifetime < FADE_IN_TIME) {
        thought.alpha = (thought.lifetime / FADE_IN_TIME) * 255.0f;
    }

    else if (thought.lifetime > thought.maxLifetime - FADE_OUT_TIME) {
        float timeLeft = thought.maxLifetime - thought.lifetime;
        thought.alpha = (timeLeft / FADE_OUT_TIME) * 255.0f;
    }

    else {
        thought.alpha = 255.0f;
    }


    thought.alpha = std::max(0.0f, std::min(255.0f, thought.alpha));
}

void ThoughtSystem::initializeThoughts() {

    m_generalThoughts = {
        "В кармане осталось 500 рублей...",
        "Как там мама?",
        "Наставник научил меня всему...",
        "А что меня ждет в Городе N?",
        "Машина работает хорошо.",
        "Доеду ли я до места?",
        "Надо экономить топливо.",
        "Что бы сделал Наставник?",
        "Не хочу его подводить...",
        "Дорога длинная...",
        "Нужно быть осторожным.",
        "Главное — доехать."
    };
}

void ThoughtSystem::enableTestMode(bool enabled) {
    m_testMode = enabled;


    if (m_thoughtsSpawned == 0) {
        m_nextThoughtDelay = m_testMode ? TEST_FIRST_THOUGHT_DELAY : FIRST_THOUGHT_DELAY;
    } else if (m_thoughtsSpawned == 1) {
        m_nextThoughtDelay = m_testMode ? TEST_SECOND_THOUGHT_DELAY : SECOND_THOUGHT_DELAY;
    } else if (m_thoughtsSpawned == 2) {
        m_nextThoughtDelay = m_testMode ? TEST_THIRD_THOUGHT_DELAY : THIRD_THOUGHT_DELAY;
    }
}
