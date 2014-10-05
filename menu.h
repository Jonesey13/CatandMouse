#ifndef MENU_H
#define MENU_H

#include "menuitem.h"
#include "configuration.h"
#include "editoroptions.h"
#include "track.h"


#include <vector>
#include <string>
#include <sstream>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;


class Menu
{
    typedef void (Menu::*voidptr)();
    sf::Font font;
    sf::RenderWindow *Window;
    unsigned TextSize;
    unsigned Selection=0;
    unsigned Choice=0;
    bool Selected=0;
    unsigned Change=0;
    bool JoyConfig=0;
    bool ButtonConfig=0;
    Action CurrentAction=ACCELERATE;
    vector<unsigned> RegJoy;
    unsigned JoyItem=0;
    unsigned CurrentPlayer=1;
    vector<string> MenuTitles;
    vector<vector<MenuItem>> Items;
    Vector2u Resolution;

    void ActionStartGame();
    void ActionJoyConfig();
    void ActionAccelerateSet();
    void ActionBrakeSet();
    void ActionEditorMenu1();
    void ActionEditorMenu2();
    void ActionEditorRunCurrent();
    void ActionEditorRunNew();
    void ActionReturn();
    void ActionOptionsMenu();
    void ActionControllerMenu();
    void ActionSaveOptions();
    void ActionLoadOptions();
    void NoAction(){};

    void ClearVariables(){JoyConfig=0; ButtonConfig=0;}


public:
    Menu(sf::RenderWindow &NewWindow);
    Configuration Config;
    EditorOptions EditOptions;
    bool StartGame=0;
    bool StartEditor=0;
    void ProcessEvents(sf::Event &Event);
    void Update();
    void Render();

};




#endif // MENU_H
