#ifndef EDITOROPTIONS_H
#define EDITOROPTIONS_H


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

class  EditorOptions
{
public:
    unsigned TrackNumber=1;
    Vector2u NewTrackDim=Vector2u(20,20);
    bool BlankTrack=0;
};



#endif // EDITOROPTIONS_H
