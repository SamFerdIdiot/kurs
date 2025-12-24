#ifndef THOUGHT_SYSTEM_H
#define THOUGHT_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>







class ThoughtSystem {
public:



    ThoughtSystem();






    void update(float deltaTime, float timeSinceChoice);






    void render(sf::RenderWindow& window, const sf::Font& font);




    void reset();





    void spawnThought(const std::string& text);





    void enableTestMode(bool enabled = true);

private:



    struct ThoughtBubble {
        std::string text;
        float lifetime;
        float maxLifetime;
        float alpha;
        sf::Vector2f position;
    };


    std::vector<ThoughtBubble> m_activeThoughts;
    float m_thoughtTimer;
    float m_nextThoughtDelay;
    int m_thoughtsSpawned;
    bool m_testMode;


    static constexpr float THOUGHT_LIFETIME = 8.0f;
    static constexpr float FADE_IN_TIME = 0.5f;
    static constexpr float FADE_OUT_TIME = 1.0f;


    static constexpr float FIRST_THOUGHT_DELAY = 30.0f;
    static constexpr float SECOND_THOUGHT_DELAY = 60.0f;
    static constexpr float THIRD_THOUGHT_DELAY = 90.0f;


    static constexpr float TEST_FIRST_THOUGHT_DELAY = 5.0f;
    static constexpr float TEST_SECOND_THOUGHT_DELAY = 10.0f;
    static constexpr float TEST_THIRD_THOUGHT_DELAY = 15.0f;


    std::vector<std::string> m_generalThoughts;




    std::string getRandomThought() const;




    void updateThoughtAlpha(ThoughtBubble& thought) const;




    void initializeThoughts();
};

#endif
