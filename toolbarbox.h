#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;


class ToolbarButton{
public:
    virtual ~ToolbarButton()=default;
    ToolbarButton()=default;
    virtual void Update(Vector2d MousePosition)=0;
    virtual void Render()=0;

};



#endif //TOOLBARBUTTON_H
