#include "renderfunctions.h"

sf::Text CreateText(string Message, sf::Font *Font ,double CharacterSize, sf::Color Color, Vector2d Position){

    sf::Text Text;
    Text.setFont(*Font);
    Text.setString(Message);
    Text.setCharacterSize(CharacterSize);
    Text.setColor(Color);
    sf::FloatRect Rect = Text.getLocalBounds();
    Text.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Text.setPosition(sf::Vector2f(Position.x,Position.y));
    return Text;
}

sf::Text & AdjustText(sf::Text& Text,string Message, sf::Font *Font ,double CharacterSize, sf::Color Color, Vector2d Position){
    Text.setFont(*Font);
    Text.setString(Message);
    Text.setCharacterSize(CharacterSize);
    Text.setColor(Color);
    sf::FloatRect Rect = Text.getLocalBounds();
    Text.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Text.setPosition(sf::Vector2f(Position.x,Position.y));
    return Text;
}
