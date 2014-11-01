#ifndef GAME_H
#define GAME_H

#include "configuration.h"
#include "race.h"
#include "pauseitem.h"
#include "renderfunctions.h"

class Game
{
    Configuration Config;
    Race race;
    sf::Font font=sf::Font();
    Track *track;
    unsigned *frameptr=nullptr;
    unsigned *cycleptr=nullptr;
    vector<Car*> Player;
    vector<PauseItem> PauseOptions;
    vector<vector<sf::CircleShape>> SwitchCircles;
    vector<vector<sf::VertexArray>> TrapBoxes;
    vector<vector<sf::VertexArray>> WiringLines;
    bool Pause=0;
    unsigned Selection=0;
    unsigned Selected=0;


    unsigned UpdateRate=0;

    Vector2u Resolution;
    unsigned PauseTextSize=0;
    sf::Texture CarTexture;
    sf::Texture TrackTexture;
    sf::RenderWindow *Window;
    sf::VertexArray TrackTiles;
    double Scaling;
    unsigned padding;
    bool paddingdim;
    unsigned RenderSize=32;
    sf::Clock Clock;
    void PrepareandScaleTriangle(sf::Vertex *quad, const Vector2i &TextPos, const Vector2i &Pos, const unsigned &Orientation);
    void ActionUnpause();
    void ActionExit();
    void RefreshRendering();


public:
    Game();

    bool GameActive;
    void init(Configuration &NewConfig, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames);
    void ProcessEvents(sf::Event &Event);
    void Update(double DeltaTime);
    double getTime();
    void Render();
};




#endif // GAME_H
