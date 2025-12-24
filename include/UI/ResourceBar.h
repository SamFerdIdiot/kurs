#ifndef RESOURCEBAR_H
#define RESOURCEBAR_H

#include <SFML/Graphics.hpp>












class ResourceBar {
public:




    ResourceBar(sf::Vector2f pos, sf::Vector2f size);






    void setValue(float current, float max);




    void setPosition(sf::Vector2f pos);





    void update(float deltaTime);




    void draw(sf::RenderWindow& window);



    float getPercentage() const;

private:

    sf::RectangleShape m_border;
    sf::RectangleShape m_background;
    sf::RectangleShape m_fill;


    sf::Vector2f m_position;
    sf::Vector2f m_size;


    float m_currentValue;
    float m_maxValue;
    float m_displayValue;


    static constexpr float TRANSITION_SPEED = 5.0f;
    static constexpr float BORDER_THICKNESS = 2.0f;




    sf::Color getBorderColor() const;





    sf::Color getFillColor(float percentage) const;


    void updateVisuals();
};

#endif
