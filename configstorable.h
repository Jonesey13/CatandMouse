#ifndef CONFIGSTORABLE_H
#define CONFIGSTORABLE_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <fstream>


using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;
class Configuration;

class  ConfigStorable
{
public:
    virtual ~ConfigStorable()=default;
    ConfigStorable()=default;
    ConfigStorable(string NewSignature,Configuration &NewConfig): Sig(NewSignature), Config(&NewConfig){};
    string Sig;
    Configuration *Config;
    virtual void ReadFromStream(ifstream &inputstream)=0;
    virtual void WriteToStream(ofstream &outputstream)=0;
};



#endif // CONFIGSTORABLE_H
