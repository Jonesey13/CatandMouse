#ifndef HEADER_H
#define HEADER_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <iostream>
#define M_PI 3.14159265358979323846

using namespace std;

//Simple Vector Functions
sf::Vector2f RotateVector(sf::Vector2f input, float rotation);
float DotProduct(sf::Vector2f vector1, sf::Vector2f vector2);

class Configuration
{
public:
    unsigned int NumberOfPlayers=2;
    vector<unsigned int> JoyId={0,0,0,0,0,0,0,0};
    int TrackNumber=1;

    sf::Vector2u NewTrackDim=sf::Vector2u(20,20);
};


class Car
{
    float DrivingForce;     // Acceleration without friction
    float Braking;
    float Reversing;
    sf::Vector2f Velocity;
    float Rotation;
    sf::Vector2f Position;
    float TurningRate;
    static sf::Texture Texture;
    static int Size;
public:
    Car();
    static void initTexture();
    static sf::Texture* getTexture();
    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f newspeed);
    float getRotation();
    void setRotation(float newrotation);
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f newposition);
    void Update(float DeltaTime);
    static int getSize();

    bool VelocitySwitch;
    bool BrakeSwitch;
    bool RPlusSwitch;
    bool RMinusSwitch;
};

class Tile
{
public:
    Tile();
    sf::Vector2i Detection;
    bool isSquare;
    bool Orientation;
    sf::Vector2u Types;
};

class Track
{
    vector<vector<Tile>> Tiles;
    sf::Vector2u Dim;
    sf::Vector2u StartingPosition;
    sf::Texture Texture;
    vector<sf::Vector2u> FinishLine;
    int Size;

public:
    Track();
    void init(int TrackNumber);
    sf::Vector2u getDim();
    int getSize();
    sf::Vector2u getStartingPosition();
    void setStartingPosition(sf::Vector2u NewStarting);
    bool TrackReady=0;
    vector<sf::Vector2u> getFinishLine();
    void setFinishLine(vector<sf::Vector2u> NewFinishLine);
    Tile* getTile(int i, int j);
    sf::Texture* getTexture();
    void ReadTrack(int TrackNumber);
    void WriteTrack(int TrackNumber);
    void FlushTrack(int TrackNumber);
    void SetDetection();
    void SetBlank(sf::Vector2u NewDim);
};


class MenuItem : public sf::Text
{
public:
    int SelectionIndex;
    int flag=0;
};



class Menu
{
    sf::Font font;
    sf::RenderWindow* Window;
    unsigned int TextSize;
    unsigned int Selection=0;
    int Choice=0;
    bool Selected=0;
    int Change=0;
    bool JoyConfig=0;
    bool FlagOn=0;
    vector<unsigned int> RegJoy;
    unsigned int CurrentJoy=0;
    vector<vector<MenuItem>> TextBlock;
public:
    Menu(sf::RenderWindow& NewWindow);
    Configuration Config;
    bool StartGame=0;
    bool StartEditor=0;
    void ProcessEvents(sf::Event& Event);
    void Update();
    void Render();

};



class Game
{
    Configuration Config;
    Track* track;
    vector<Car> Player;
    sf::RenderWindow* Window;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    float Scaling;
    void PrepareandScaleTriangle(sf::Vertex* quad, sf::Vector2i TextPos, sf::Vector2i Pos, int Orientation);


public:
    Game();
    bool GameActive;
    float getTime();
    void init(Configuration& NewConfig, Track& NewTrack, sf::RenderWindow& NewWindow);
    void ProcessEvents(sf::Event& Event);
    void Update(float DeltaTime);
    void Render();
};


class Editor
{
    sf::Font font;
    Configuration Config;
    Track* track;
    sf::RenderWindow* Window;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    sf::VertexArray OverLay;
    float Scaling;
    bool ClickLeft=0;
    bool ClickRight=0;
    bool Exit=0;
    bool SaveYN=0;
    bool StartSwitch=0;
    int PaintSelection=0;
    sf::Text ExitMessage;
    bool SaveSwitch=0;
    void PrepareandScaleTriangle(sf::Vertex* tri, sf::Vertex* lines, sf::Vector2i TextPos,
                                 sf::Vector2i Pos, int Orientation, bool isSquare=0);
    sf::Vector2u getCurrentSquare();
    bool getTriangleHalf(sf::Vector2u Position, bool Orientation);

public:
    Editor();
    bool EditorActive;
    sf::CircleShape StartingCircle=sf::CircleShape(50);
    float getTime();
    void init(Configuration& NewConfig, Track& NewTrack, sf::RenderWindow& NewWindow);
    void ProcessEvents(sf::Event& Event);
    void Update(float DeltaTime);
    void Render();

};


#endif // HEADER_H
