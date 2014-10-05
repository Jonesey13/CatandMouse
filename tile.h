#ifndef TILE_H
#define TILE_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <set>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;
using Vector2b=sf::Vector2<bool>;

class Tile
{
public:
    sf::Vector2<set<string>> Detection=sf::Vector2<set<string>>(set<string>(),set<string>());
    bool isSquare=1;
    bool Orientation=0;
    Vector2u Types=sf::Vector2u();
};

#endif // TILE_H



