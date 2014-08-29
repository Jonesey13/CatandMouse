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
//#include <Windows.h>
//#include <chrono>
//#include <thread>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#define M_PI 3.14159265358979323846

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

#include "clickable.h"


//Simple Vector Functions
Vector2d RotateVector(Vector2d input, double rotation);
double DotProduct(Vector2d vector1, Vector2d vector2);
Vector2f Double2Float(Vector2d input);

class Configuration
{
public:
    // Global Settings
    unsigned NumberOfPlayers=2;
    vector<unsigned> JoyId={0,0,0,0,0,0,0,0};
    unsigned TrackNumber=1;

    //Editor Options
    Vector2u NewTrackDim=Vector2u(20,20);
};


class Car
{
    static sf::Texture Texture;
    static unsigned Size;
public:
    Car();
    static void init();
    static sf::Texture *getTexture();
    static unsigned getSize();

    double DrivingForce;
    double Braking;
    double Reversing;
    Vector2d Velocity;
    double Rotation;
    Vector2d Position;
    double TurningRate;
    bool VelocitySwitch;
    bool BrakeSwitch;
    bool RPlusSwitch;
    bool RMinusSwitch;

    void Update(double DeltaTime);
};

class Tile
{
public:
    sf::Vector2<vector<unsigned>> Detection;
    bool isSquare=1;
    bool Orientation=0;
    Vector2u Types=sf::Vector2u();
};

class Track
{
    vector<vector<Tile>> Tiles;
    Vector2u Dim;
    sf::Texture Texture;
    unsigned Size;
    enum PROPERTIES {FALL, NORMTRACTION, SLIDING, FINISH, WALL};

public:
    Track();
    void init(unsigned TrackNumber);
    vector<Vector2u> StartingPositions;
    Vector2u getDim();
    unsigned getSize();
    Tile *getTile(unsigned i, unsigned j);
    sf::Texture *getTexture();
    bool TrackReady=0;
    unsigned FinishDirection=0;
    unsigned TotalStarting=4;

    void SetBlank(sf::Vector2u NewDim);
    void ReadTrack(unsigned TrackNumber);
    void WriteTrack(unsigned TrackNumber);
    void FlushTrack(unsigned TrackNumber);
    void RefreshDetection();
};


class MenuItem : public sf::Text
{
public:
    unsigned SelectionIndex;
    unsigned flag=0;
};



class Menu
{
    sf::Font font;
    sf::RenderWindow *Window;
    unsigned TextSize;
    unsigned Selection=0;
    unsigned Choice=0;
    bool Selected=0;
    unsigned Change=0;
    bool JoyConfig=0;
    bool FlagOn=0;
    vector<unsigned> RegJoy;
    unsigned CurrentJoy=0;
    vector<vector<MenuItem>> TextBlock;
public:
    Menu(sf::RenderWindow &NewWindow);
    Configuration Config;
    bool StartGame=0;
    bool StartEditor=0;
    void ProcessEvents(sf::Event &Event);
    void Update();
    void Render();

};



class Game
{
    Configuration Config;
    Track *track;
    vector<Car> Player;
    sf::RenderWindow *Window;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    double Scaling;
    void PrepareandScaleTriangle(sf::Vertex *quad, Vector2i TextPos, Vector2i Pos, unsigned Orientation);


public:
    Game();

    bool GameActive;
    double getTime();
    void init(Configuration &NewConfig, Track &NewTrack, sf::RenderWindow &NewWindow);
    void ProcessEvents(sf::Event &Event);
    void Update(double DeltaTime);
    void Render();
};





class Editor
{
    sf::Font font;
    sf::Texture ButtonTexture;
    Configuration Config;
    Track *track;
    sf::RenderWindow *Window;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    sf::VertexArray OverLay;
    vector<Clickable<bool>> ClickableBools;
    vector<Clickable<unsigned>> ClickableUnsigneds;
    Clickable<unsigned> ClickableSelection;
    bool OverLayON;
    double Scaling;
    double ScalingTrack;
    double ScalingTools;
    bool ClickLeft;
    bool ClickRight;
    bool PreExitFlag;
    bool ExitFlag;
    bool StartFlag;
    bool SaveFlag;
    unsigned PaintSelection;
    unsigned PlayerSelection;
    sf::Text ExitMessage;
    void PrepareandScaleTriangle(sf::Vertex *tri, sf::Vertex *lines, Vector2u TextPos,
                                 Vector2u Pos, unsigned Orientation, bool isSquare=0);
    Vector2u getCurrentSquare();
    bool getTriangleHalf(Vector2u Position, bool Orientation);
    vector<sf::Text> StartingNumbers;
    vector<sf::CircleShape> StartingCircles;
    sf::VertexArray ToolBoxes;

public:
    Editor();
    bool EditorActive;

    double getTime();
    void init(Configuration &NewConfig, Track &NewTrack, sf::RenderWindow &NewWindow);
    void ProcessEvents(sf::Event &Event);
    void Update(double DeltaTime);
    void Render();

};


#endif // HEADER_H
