#ifndef NEWPAUSEITEM_H
#define NEWPAUSEITEM_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <functional>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

class Game;


class NewPauseItem{
public:
    NewPauseItem(string Text1, std::function<void()> SelectionFunction): Item(Text1), ActionFunction(SelectionFunction){};
    string Item;
    std::function<void()> ActionFunction;
};




#endif // NEWPAUSEITEM_H

