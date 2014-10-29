#ifndef STORABLE_H
#define STORABLE_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>



using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;


class  Storable
{
public:
    virtual ~Storable()=default;
    Storable()=default;
    Storable(string NewSignature): Sig(NewSignature){};
    string Sig;
    virtual void ReadFromStream(ifstream &inputstream)=0;
    virtual void WriteToStream(ofstream &outputstream)=0;
};


#include "storablesingle.h"
#include "storablevector.h"
#include "storablevector2.h"
#include "storablevectorvector2.h"
#include "storabletilemap.h"
#include "storablecontroller.h"
#include "storabletraplist.h"


#endif // STORABLE_H

