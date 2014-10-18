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
sf::RectangleShape CreateRectangle(Vector2d RectangleSize,sf::Color color, Vector2d Position);
sf::CircleShape CreateCircle(double Radius,sf::Color color, Vector2d Position);
sf::VertexArray CreateLineBox(Vector2d Position, double width, sf::Color Color);
sf::VertexArray CreateTexturedBox(Vector2d Postion,double Width, Vector2u TexturePosition, float RenderSize);

sf::Text & AdjustText(sf::Text& Text,string Message="", sf::Font *Font=nullptr,double CharacterSize=10, sf::Color Color=sf::Color(), Vector2d Position=Vector2d(0,0));
sf::Text & RepositionText(sf::Text& Text, Vector2d Position);

Vector2d ScalePosition(Vector2d position, double Scaling, bool paddingdim, unsigned padding);
Vector2f ScalePosition(Vector2f position, double Scaling, bool paddingdim, unsigned padding);


#endif //RENDERFUNCTIONS_H
