#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

enum Action: unsigned {ACCELERATE,BRAKE};

#include "storable.h"

class Configuration
{
public:
    Configuration();
    unsigned NumberOfPlayers=2;
    vector<int> JoyId={-1,-1,-1,-1,-1,-1,-1,-1};
    vector<map<unsigned,Action>> ButtonsToActions;

    unsigned TrackNumber=1;
    unsigned NumberOfLaps=3;

    void Save();
    void Load();
};



#endif // CONFIGURATION_H
