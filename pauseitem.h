#ifndef PAUSEITEM_H
#define PAUSEITEM_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

class Game;


class PauseItem{
public:
    typedef void (Game::*voidptr)();
    PauseItem(string Text1, voidptr SelectionFunction): Item(Text1), ActionFunction(SelectionFunction){};
    string Item;
    voidptr ActionFunction;
};




#endif // PAUSEITEM_H
