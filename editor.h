#ifndef EDITOR_H
#define EDITOR_H

#include "editoroptions.h"
#include "track.h"
#include "toolbaritem.h"
#include "functions.h"
#include "renderfunctions.h"

#include <memory>
#include <iostream>
#include <sstream>



class Editor
{
private:
    sf::Font font;
    sf::Texture ButtonTexture;
    sf::Texture TrackTexture;
    EditorOptions EditOptions;
    Track track;
    Vector2u Resolution;
    sf::RenderWindow *Window;
    unsigned *frameptr=nullptr;
    unsigned *cycleptr=nullptr;
    sf::Clock Clock;
    sf::VertexArray TrackTiles;
    sf::VertexArray OverLay;

    vector<vector<shared_ptr<ToolbarItem>>> ToolbarItems;
    unsigned ToolbarMode=0;
    unsigned CurrentTrap=0;
    unsigned TotalTraps;

    unsigned RenderSize=32;

    bool SwitchMode=0;;
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
    bool HelpFlag=0;
    unsigned PaintSelection=0;
    unsigned PlayerSelection=0;
    void PrepareandScaleTriangle(sf::Vertex *tri, sf::Vertex *lines, Vector2u TextPos,
                                 Vector2u Pos, unsigned Orientation, bool isSquare=0);
    vector<sf::Text> StartingNumbers;
    vector<sf::CircleShape> StartingCircles;
    vector<sf::RectangleShape> ToolBoxes;
    vector<string> HelpMessages;
    vector<vector<sf::CircleShape>> SwitchCircles;
    vector<vector<sf::VertexArray>> SwitchBoxes;

    void TrapModeOnOff(unsigned &RenderIndex);
    void OverLayAction(unsigned &RenderIndex);
    void SwitchModeAction(unsigned &RenderIndex);
    void FinishDirectionAction(unsigned &RenderIndex);
    void PaintSelectionAction(unsigned &RenderIndex);
    void RefreshTrackRendering();
    void RefreshToolbarButtons();
    void ChangeTotalTraps();


public:
    Editor(){};
    bool EditorActive=0;
    double getTime(){return Clock.getElapsedTime().asSeconds();};
    void init(EditorOptions &NewEditOptions, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames);
    void ProcessEvents(sf::Event &Event);
    void Update(double DeltaTime);
    void UpdateClickTrackEdit(Vector2u CurrentSquare, bool Half);
    void UpdateClickTrapEdit(Vector2u CurrentSquare, bool Half);
    void Render();

};




#endif // EDITOR_H
