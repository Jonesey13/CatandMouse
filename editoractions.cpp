#include "editor.h"


void Editor::RefreshToolbarButtons(){
    ToolBoxes.clear();
    sf::RectangleShape Rectangle=CreateRectangle(Vector2d(0.15*Resolution.x,Resolution.y),sf::Color(80,80,80)
                                                , Vector2d (0.85*Resolution.x+0.5*0.15*Resolution.x,0.5*Resolution.y));
    ToolBoxes.push_back(Rectangle);
    Rectangle=CreateRectangle(Vector2d(Resolution.x,0.15*Resolution.y),sf::Color(80,80,80)
                                                , Vector2d (0.5*Resolution.x,0.85*Resolution.y+0.5*0.15*Resolution.y));
    ToolBoxes.push_back(Rectangle);


    ToolbarItems.clear();
    ToolbarItems.resize(2);

    ToolbarItems[0].push_back(make_shared<ToolbarButton>(vector<string>{"Overlay", "ON/OFF"},Vector2d(0.925*Resolution.x, 0.1*Resolution.y)
                                ,0.05*Resolution.x, &Editor::OverLayAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, 1));
    ToolbarItems[1].push_back(make_shared<ToolbarButton>(vector<string>{"Overlay", "ON/OFF"},Vector2d(0.925*Resolution.x, 0.1*Resolution.y)
                                ,0.05*Resolution.x, &Editor::OverLayAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, 1));

    ToolbarItems[0].push_back(make_shared<ToolbarButton>(vector<string>{"Finishing", "Direction"},Vector2d(0.925*Resolution.x, 0.3*Resolution.y)
                                ,0.05*Resolution.x, &Editor::FinishDirectionAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(1,0),Vector2u(1,1),Vector2u(1,2),Vector2u(1,3)}));

    vector<Vector2u> textpos;
    textpos.resize(5);
    for (unsigned i=0; i<5; ++i)
    {
        textpos[i]=Vector2u(0,i);
    }

    ToolbarItems[0].push_back(make_shared<ToolbarButtonList>(vector<string>{"Paint", "Selection"},Vector2d(0.3*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, &Editor::PaintSelectionAction, TrackTexture, textpos,0,&PaintSelection));
    ToolbarItems[1].push_back(make_shared<ToolbarButtonList>(vector<string>{"Paint", "Selection"},Vector2d(0.3*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, &Editor::PaintSelectionAction, TrackTexture, textpos,0,&PaintSelection));


    ToolbarItems[0].push_back(make_shared<ToolbarNumber>(vector<string>{"No. Starting", "Players"},Vector2d(0.925*Resolution.x, 0.5*Resolution.y)
                                                        ,0.05*Resolution.x, track.TotalStarting, 1,8));

    ToolbarItems[0].push_back(make_shared<ToolbarNumber>(vector<string>{"Width"},Vector2d(0.925*Resolution.x, 0.65*Resolution.y)
                                                        ,0.05*Resolution.x, track.Dim.x, 1,50,&Editor::RefreshTrackRendering));

    ToolbarItems[0].push_back(make_shared<ToolbarNumber>(vector<string>{"Height"},Vector2d(0.925*Resolution.x, 0.8*Resolution.y)
                                                        ,0.05*Resolution.x, track.Dim.y, 1,50,&Editor::RefreshTrackRendering));

    ToolbarItems[0].push_back(make_shared<ToolbarItem>(vector<string>{"H=Help"},Vector2d(0.925*Resolution.x, 0.95*Resolution.y)
                                                        ,0.05*Resolution.x));
    ToolbarItems[0].push_back(make_shared<ToolbarButton>(vector<string>{"Trap Edit Mode"},Vector2d(0.8*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, &Editor::TrapModeOnOff, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0)}, 0));
    ToolbarItems[1].push_back(make_shared<ToolbarButton>(vector<string>{"Trap Edit Mode"},Vector2d(0.8*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, &Editor::TrapModeOnOff, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,1)}, 0));
    ToolbarItems[1].push_back(make_shared<ToolbarNumber>(vector<string>{"Current Trap"},Vector2d(0.925*Resolution.x, 0.65*Resolution.y)
                                                        ,0.05*Resolution.x, CurrentTrap, 0,TotalTraps-1,&Editor::ChangeCurrentTrap));
    ToolbarItems[1].push_back(make_shared<ToolbarNumber>(vector<string>{"Total Traps"},Vector2d(0.925*Resolution.x, 0.5*Resolution.y)
                                                        ,0.05*Resolution.x, TotalTraps, 1,50,&Editor::ChangeTotalTraps));

    ToolbarItems[1].push_back(make_shared<ToolbarButton>(vector<string>{"Edit Switches", "ON/OFF"},Vector2d(0.925*Resolution.x, 0.25*Resolution.y)
                                ,0.05*Resolution.x, &Editor::SwitchModeAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, 0));
    ToolbarItems[1].push_back(make_shared<ToolbarButton>(vector<string>{"Trap", "ON/OFF"},Vector2d(0.925*Resolution.x, 0.38*Resolution.y)
                                ,0.05*Resolution.x, &Editor::TrapONOFFAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, track.Traps[CurrentTrap].MainSwitch));
    ToolbarItems[1].push_back(make_shared<ToolbarButton>(vector<string>{"Wiring Mode", "ON/OFF"},Vector2d(0.6*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, &Editor::WiringModeAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, 0));

    HelpMessages={"Press Enter to Exit or Save",
                    "Num1-Num5 sets the paint tile",
                    "S sets the current player position",
                    "D switches player",
                    "H to close this message"};
}



void Editor::ChangeTotalTraps(){
    track.Traps.resize(TotalTraps);
    if (CurrentTrap>=TotalTraps)
    {
        CurrentTrap=TotalTraps-1;
    }
    RefreshToolbarButtons();
    ToolboxesAltered=1;
}

void Editor::SwitchModeAction(unsigned &RenderIndex){
    SwitchMode=!SwitchMode;
    RenderIndex=SwitchMode;

}

void Editor::TrapModeOnOff(unsigned &RenderIndex){
    if(ToolbarMode==0)
        ToolbarMode=1;
    else
        ToolbarMode=0;
}

void Editor::OverLayAction(unsigned& RenderIndex){
    OverLayON=!OverLayON;
    RenderIndex=OverLayON;
}

void Editor::FinishDirectionAction(unsigned& RenderIndex){
    unsigned &Direction=track.FinishDirection;
    if (Direction!=3)
        Direction++;
    else
        Direction=0;
    RenderIndex=Direction;
}


void Editor::PaintSelectionAction(unsigned &RenderIndex){
    PaintSelection=RenderIndex;
}

void Editor::TrapONOFFAction(unsigned &RenderIndex){
    track.Traps[CurrentTrap].MainSwitch=!track.Traps[CurrentTrap].MainSwitch;
    RenderIndex=track.Traps[CurrentTrap].MainSwitch;
    track.FlipTrap(CurrentTrap, track.Traps[CurrentTrap].MainSwitch);
    RefreshTrackRendering();
}

void Editor::ChangeCurrentTrap(){
    RefreshToolbarButtons();
    RefreshTrackRendering();
    ToolboxesAltered=1;
}

void Editor::WiringModeAction(unsigned &RenderIndex){
    WiringMode=!WiringMode;
    RenderIndex=WiringMode;
    RefreshTrackRendering();
    InitialStored=0;
}
