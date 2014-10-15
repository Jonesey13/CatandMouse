#ifndef EDITOR_H
#define EDITOR_H

#include "editoroptions.h"
#include "track.h"
#include "clickable.h"
#include "functions.h"
#include "renderfunctions.h"


#include <iostream>
#include <sstream>



class Editor
{
    sf::Font font;
    sf::Texture ButtonTexture;
    sf::Texture TrackTexture;
    EditorOptions EditOptions;
    Track track;
    sf::RenderWindow *Window;
    unsigned *frameptr=nullptr;
    unsigned *cycleptr=nullptr;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    sf::VertexArray OverLay;
    vector<Clickable<bool>> ClickableBools;
    vector<Clickable<unsigned>> ClickableUnsigneds;
    Clickable<unsigned> ClickableSelection;
    unsigned RenderSize=32;
    bool OverLayON=1;
    double Scaling=0;
    double ScalingTrack=0;
    double ScalingTools=0;
    bool ClickLeft=0;
    bool ClickRight=0;
    bool PreExitFlag=0;
    bool ExitFlag=0;
    bool StartFlag=0;
    bool SaveFlag=0;
    unsigned PaintSelection;
    unsigned PlayerSelection;
    sf::Text ExitMessage;
    void PrepareandScaleTriangle(sf::Vertex *tri, sf::Vertex *lines, Vector2u TextPos,
                                 Vector2u Pos, unsigned Orientation, bool isSquare=0);
    vector<sf::Text> StartingNumbers;
    vector<sf::CircleShape> StartingCircles;
    vector<sf::RectangleShape> ToolBoxes;

public:
    Editor(){};
    bool EditorActive=0;
    double getTime(){return Clock.getElapsedTime().asSeconds();};
    void init(EditorOptions &NewEditOptions, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames);
    void ProcessEvents(sf::Event &Event);
    void Update(double DeltaTime);
    void Render();
};




#endif // EDITOR_H
