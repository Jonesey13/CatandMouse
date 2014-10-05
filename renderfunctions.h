#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

sf::Text CreateText(string Text, sf::Font *font ,double CharacterSize, sf::Color color, Vector2d Position);
sf::Text & AdjustText(sf::Text& Text,string Message="", sf::Font *Font=nullptr,double CharacterSize=10, sf::Color Color=sf::Color(), Vector2d Position=Vector2d(0,0));




#endif //RENDERFUNCTIONS_H
