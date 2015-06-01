#ifndef MENUITEM_H
#define MENUITEM_H

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

class Menu;


class MenuItem{
public:
    typedef void (Menu::*MenuVoidptr)();


    MenuItem(string Text1, string Text2, MenuVoidptr SelectionFunction)
    : Item(Text1), FlagMessage(Text2), HasFlag(true), ActionFunction(SelectionFunction){};

    MenuItem(string Text, unsigned &Target, MenuVoidptr SelectionFunction)
    : Item(Text), HasNumber(true), NumberPointer(&Target), ActionFunction(SelectionFunction) {};

    MenuItem(string Text, unsigned &Target, unsigned NewMin, unsigned NewMax, MenuVoidptr SelectionFunction)
    : Item(Text), HasNumber(true),  Minimum(NewMin), Maximum(NewMax), NumberPointer(&Target), ActionFunction(SelectionFunction) {};

    MenuItem(string Text, MenuVoidptr SelectionFunction)
    : Item(Text), ActionFunction(SelectionFunction) {};

    MenuItem(string Text1, string Text2, unsigned &Target, MenuVoidptr SelectionFunction)
    : Item(Text1), FlagMessage(Text2), HasFlag(true), HasFlagNumber(true), FlagPointer(&Target), ActionFunction(SelectionFunction) {};


    string Item;
    string FlagMessage;
    bool HasFlag=0;
    bool Flag=0;
    bool HasNumber=0;
    unsigned Minimum=1;
    unsigned Maximum=10;


    unsigned *NumberPointer=nullptr;
    bool HasFlagNumber=0;
    unsigned *FlagPointer=nullptr;

    MenuVoidptr ActionFunction;
};




#endif // MENUITEM_H
