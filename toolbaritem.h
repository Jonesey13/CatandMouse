#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <sstream>
#include "renderfunctions.h"

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

class Editor;



class ToolbarItem{
private:
    vector<sf::Text> TitleText;
public:
    virtual ~ToolbarItem()=default;
    ToolbarItem()=default;
    static sf::RenderWindow *Window;
    static sf::Font *Font;
    static unsigned TextSize;

    ToolbarItem( vector<string> NewTitle,Vector2d NewPosition, unsigned NewWidth  );
    virtual void Update(Editor &editor)=0;
    virtual void Render();
    vector<string> Title;
    Vector2d Position;
    unsigned Width;
};

#include "toolbarbutton.h"
#include "toolbarnumber.h"
#include "toolbarbuttonlist.h"

#endif //TOOLBARITEM_H
