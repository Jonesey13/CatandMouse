#include "editor.h"


void Editor::init(EditorOptions &NewEditOptions, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames){
    Window=&NewWindow;
    Resolution=Window->getSize();

    ButtonTexture.loadFromFile("EditorSprites.png");
    TrackTexture.loadFromFile("TrackSprites.png");
    font.loadFromFile("Aller.ttf");
    ToolbarItem::Font=&font;
    ToolbarItem::Window=Window;
    ToolbarItem::TextSize=0.02*Resolution.x;

    EditorActive=1;

    TotalTraps=track.Traps.size();

    frameptr=&frames;
    *frameptr=0;
    cycleptr=&cycles;
    *cycleptr=0;

    EditOptions=NewEditOptions;

    if (EditOptions.BlankTrack==1)
    {
        track.SetBlank(EditOptions.NewTrackDim);
    }
    else{
        track.ReadTrack(EditOptions.TrackNumber);
    }

    RefreshToolbarButtons();



    TrackTiles.setPrimitiveType(sf::Triangles);
    OverLay.setPrimitiveType(sf::Lines);
    RefreshTrackRendering();

    Clock.restart();

}


void Editor::ProcessEvents(sf::Event &Event){
    while (Window->pollEvent(Event))
    {
        switch (Event.type)
        {
        case sf::Event::Closed:
            {
            Window->close();
            break;
            }

        case sf::Event::MouseButtonPressed:
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    ClickLeft=1;
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    ClickRight=1;
                }
            }
            break;
        case sf::Event::KeyPressed:
            switch (Event.key.code)
            {
            case sf::Keyboard::Escape:
                {
                Window->close();
                break;
                }
            case sf::Keyboard::Return:
                PreExitFlag=1;
                break;
            case sf::Keyboard::Num1:
                PaintSelection=0;
                break;
            case sf::Keyboard::Num2:
                PaintSelection=1;
                break;
            case sf::Keyboard::Num3:
                PaintSelection=2;
                break;
            case sf::Keyboard::Num4:
                PaintSelection=3;
                break;
            case sf::Keyboard::Num5:
                PaintSelection=4;
                break;
            case sf::Keyboard::Y:
                if(PreExitFlag==1)
                {
                   ExitFlag=1;
                   SaveFlag=1;
                }
                break;
            case sf::Keyboard::N:
                if(PreExitFlag==1)
                {
                   ExitFlag=1;
                   SaveFlag=0;
                }
                break;
            case sf::Keyboard::S:
                StartFlag=1;
                break;
            case sf::Keyboard::D:
                if (PlayerSelection<track.TotalStarting-1)
                    PlayerSelection++;
                else
                    PlayerSelection=0;
                break;
            case sf::Keyboard::H:
                if(HelpFlag==0)
                    HelpFlag=1;
                else
                    HelpFlag=0;
            default:
                break;
            }
            break;
        case sf::Event::KeyReleased:
            {
            switch (Event.key.code)
            {
            default:
                break;
            }
            break;
            }
        default:
            break;
        }
    }
}

void Editor::Update(double DeltaTime){
    Vector2i LocalMousePosition = sf::Mouse::getPosition(*Window);
    Vector2d MousePosition=Vector2d(LocalMousePosition.x,LocalMousePosition.y)/static_cast<double>(ScalingTrack);
    Vector2u CurrentSquare=Vector2u(MousePosition.x,MousePosition.y);
    Tile* CurrentTile=&track.Tiles[CurrentSquare.x][CurrentSquare.y];
    if(ClickLeft==1 || ClickRight==1){
        Vector2u Dim=track.getDim();
        if (CurrentSquare.x<Dim.x && CurrentSquare.y<Dim.y)
        {
            bool Half=getTriangleHalf(MousePosition,CurrentTile->Orientation);
            if (ToolbarMode==0 || TrapPaintMode==1)
                UpdateClickTrackEdit(CurrentSquare,CurrentTile->Orientation,Half);
            else
                {
                    if (WiringMode==0)
                        UpdateClickTrapEdit(CurrentSquare,CurrentTile->Orientation,Half);
                    else
                    {
                        Vector2u CurrentDouble=Vector2u(2*(MousePosition.x+0.25),2*(MousePosition.y+0.25));
                        Vector2d CurrentPrecise=Vector2d(CurrentDouble.x/2.0,CurrentDouble.y/2.0);
                        UpdateClickWiringEdit(CurrentPrecise);
                    }
                }
        }
        for (auto& index: ToolbarItems[ToolbarMode])
            {
                index->Update(*this);
                if (ToolboxesAltered)
                    {
                        ToolboxesAltered=0;
                        break;
                    }
            }
        ClickLeft=0;
        ClickRight=0;
    }

    if(StartFlag==1)
    {
        track.StartingPositions[PlayerSelection]=CurrentSquare;
        StartFlag=0;
    }

    if(ExitFlag==1)
    {
        if(SaveFlag==1)
        {
            track.FlushTrack(EditOptions.TrackNumber);
            track.WriteTrack(EditOptions.TrackNumber);
        }
        EditorActive=0;
    }


}

void Editor::Render(){
    Vector2u TrackDim=track.getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
    OverLay.resize((TrackDim.x*TrackDim.y)*12);
    unsigned current=0;
    for (unsigned i=0; i<TrackDim.x;++i)
    {
        for (unsigned j=0; j< TrackDim.y;++j)
        {
            Vector2u Pos=Vector2u(i,j);
            if(track.getTile(i,j)->isSquare==1)
            {
                sf::Vertex *tri = &TrackTiles[current * 3];
                sf::Vertex *lines = &OverLay[current * 6];
                Vector2u TextPos=Vector2u(0,track.getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,0,1);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=Vector2u(0,track.getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,2,1);
                current++;
            }
            else{
                sf::Vertex *tri= &TrackTiles[current * 3];
                sf::Vertex *lines = &OverLay[current * 6];
                Vector2u TextPos=Vector2u(0,track.getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track.getTile(i,j)->Orientation);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=Vector2u(0,track.getTile(i,j)->Types.y);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track.getTile(i,j)->Orientation+2);
                current++;
            }
        }
    }
    for(unsigned i=0; i<(TrackDim.x*TrackDim.y)*12; i++)
    {
        OverLay[i].color=sf::Color::Blue;
    }
    for (unsigned i=0; i<track.TotalStarting; i++)
    {
        Vector2u Position=track.StartingPositions[i];
        StartingCircles[i].setPosition(Vector2f(ScalingTrack*Position.x+ScalingTrack*0.5,
                                                ScalingTrack*Position.y+ScalingTrack*0.5));
        if (PlayerSelection==i)
        {
            StartingCircles[i].setFillColor(sf::Color::Yellow);
        }
        else
        {
            StartingCircles[i].setFillColor(sf::Color::Green);
        }
        StartingNumbers[i].setPosition(Vector2f(ScalingTrack*Position.x+ScalingTrack*0.5,
                                                ScalingTrack*Position.y+ScalingTrack*0.5));
    }
    Window->draw(TrackTiles,&TrackTexture);

    if(OverLayON==1)
    {
        Window->draw(OverLay);
    }

    for (unsigned i=0; i<8; i++)
    {
        if (i<track.TotalStarting && ToolbarMode==0)
        {
            Window->draw(StartingCircles[i]);
            Window->draw(StartingNumbers[i]);
        }
    }

    for(auto index : ToolBoxes)
    {
        Window->draw(index);
    }

    for (auto &index : ToolbarItems[ToolbarMode])
        index->Render();

    for (unsigned i=0; i<track.Traps.size();i++)
    {
        for (unsigned j=0; j<track.Traps[i].SwitchPositions.size();j++)
        {
            Window->draw(SwitchCircles[i][j]);
            Window->draw(SwitchBoxes[i][j]);
        }
        for (unsigned j=0; j<track.Traps[i].trapData.size();j++)
        {
            Window->draw(TrapBoxes[i][j]);
        }

        for (unsigned j=0; j<track.Traps[i].Wiring.size();j++)
        {
            Window->draw(WiringLines[i][j]);
        }
    }
    if(InitialStored)
    {
        Vector2i MousePosOrig=sf::Mouse::getPosition(*Window);
        Vector2d MousePos=Vector2d(MousePosOrig.x,MousePosOrig.y);
        sf::VertexArray Line=CreateLine(ScalingTrack*InitialWiringPos, MousePos
                                        ,sf::Color::Red);
        Window->draw(Line);
    }

    if(HelpFlag)
    {
        vector<sf::Text> HelpText;
        unsigned Size=HelpMessages.size();
        double TextSize=Resolution.y/(20.0);
        double Shift=(Size-1)*TextSize/2.0;
        for (unsigned i=0; i<HelpMessages.size();i++)
        {
            HelpText.push_back(CreateText(HelpMessages[i],&font ,TextSize,
                                 sf::Color::Red, Vector2d(Resolution.x/2,Resolution.y/2+i*TextSize-Shift)));
        }
        sf::RectangleShape VictoryRectangle=CreateRectangle(Vector2d(Resolution.x/1.5, TextSize*(Size+0.2)),
                                                            sf::Color::Blue,Vector2d(Resolution.x/2,Resolution.y/2));
        Window->draw(VictoryRectangle);

        for (unsigned i=0; i<HelpMessages.size();i++)
        {
            Window->draw(HelpText[i]);
        }
    }

    if(PreExitFlag==1)
    {
        unsigned SaveTextSize=Resolution.y/(12.f);
        sf::RectangleShape SaveRectangle=CreateRectangle(Vector2d(Resolution.x/1.5, SaveTextSize*1.2),
                                                            sf::Color::Blue,Vector2d(Resolution.x/2,Resolution.y/2));
        Window->draw(SaveRectangle);
        sf::Text SaveMessage=CreateText("Save File? (Y/N)",&font,SaveTextSize,sf::Color::Yellow,Vector2d(Resolution.x/2,Resolution.y/2));
        Window->draw(SaveMessage);
    }
    Window->display();

    Window->clear();
}

void Editor::PrepareandScaleTriangle(sf::Vertex *tri, sf::Vertex *lines ,
                                     Vector2u TextPos, Vector2u Pos, unsigned Orientation, bool isSquare){
    vector<sf::Vertex> quad(4);
    float RenderScaling=ScalingTrack;
    quad[0].position=RenderScaling*Vector2f(Pos.x, Pos.y);
    quad[1].position=RenderScaling*Vector2f(Pos.x, Pos.y+1);
    quad[2].position=RenderScaling*Vector2f(Pos.x+1, Pos.y+1);
    quad[3].position=RenderScaling*Vector2f(Pos.x+1, Pos.y);

    float TextureScaling=RenderSize;
    quad[0].texCoords=TextureScaling*Vector2f(TextPos.x, TextPos.y);
    quad[1].texCoords=TextureScaling*Vector2f(TextPos.x, TextPos.y+1);
    quad[2].texCoords=TextureScaling*Vector2f(TextPos.x+1, TextPos.y+1);
    quad[3].texCoords=TextureScaling*Vector2f(TextPos.x+1, TextPos.y);

    for (unsigned i=0; i<3; i++)
    {
        tri[i]=quad[(i+Orientation)%4];
    }
    lines[0]=quad[Orientation%4];
    lines[1]=quad[(Orientation+1)%4];
    lines[2]=quad[(Orientation+1)%4];
    lines[3]=quad[(Orientation+2)%4];
    if(isSquare==1){
        lines[4]=quad[(Orientation+1)%4];
        lines[5]=quad[(Orientation+2)%4];
    }
    else{
        lines[4]=quad[(Orientation)%4];
        lines[5]=quad[(Orientation+2)%4];
    }
}


void Editor::UpdateClickTrackEdit(Vector2u CurrentSquare, bool Orientation, bool Half){
    Tile *CurrentTile=track.getTile(CurrentSquare.x,CurrentSquare.y);
    bool Available=1;
    TrapData *SelectedTrapPos=nullptr;
    Trap* SelectedTrap;
    for( auto& trap : track.Traps)
    {
        vector<Vector2u> SwitchPos=trap.SwitchPositions;
        if(find(SwitchPos.begin(),SwitchPos.end(),CurrentSquare)!=SwitchPos.end())
           {
               Available=0;
           }
        if(ClickRight)
            {
            vector<Vector2u> TrapPos;
            for (unsigned i=0; i<trap.trapData.size(); i++)
            {
                if(CurrentTile->isSquare || trap.trapData[i].Half==Half || ClickRight)
                    TrapPos.push_back(trap.trapData[i].Square);
            }
            if(find(TrapPos.begin(),TrapPos.end(),CurrentSquare)!=TrapPos.end())
               {
                   Available=0;
               }
           }
        if(ClickLeft)
        {
            vector<TrapData> &AllTrapData=trap.trapData;
            TrapData CurrentTriangle;
            CurrentTriangle.Square=CurrentSquare;
            CurrentTriangle.Half=Half;
            auto it=find_if(AllTrapData.begin(),AllTrapData.end(),[&](const TrapData &First)
                        {return (First.Square==CurrentTriangle.Square) && (First.Half==CurrentTriangle.Half);});
            if(it!=AllTrapData.end())
            {
                SelectedTrapPos=&(*it);
                SelectedTrap=&trap;
            }
            if (CurrentTile->isSquare)
            {
                it=find_if(AllTrapData.begin(),AllTrapData.end(),[&](const TrapData &First)
                            {return (First.Square==CurrentTriangle.Square) && (First.Half!=CurrentTriangle.Half);});
                if(it!=AllTrapData.end())
                {
                    SelectedTrapPos=&(*it);
                    SelectedTrap=&trap;
                }
            }
        }
    }
    if (Available)
    {
        if(CurrentTile->isSquare==1)
        {
            if(ClickLeft==1)
            {
                CurrentTile->Types=Vector2u(PaintSelection,PaintSelection);
            }
            if(ClickRight==1)
            {
                CurrentTile->Types=Vector2u(CurrentTile->Types.x,CurrentTile->Types.x);
                CurrentTile->Orientation=0;
                CurrentTile->isSquare=0;
            }
        }
        else{
            if(Half==0){
                if(ClickLeft==1)
                {
                    CurrentTile->Types=Vector2u(PaintSelection,CurrentTile->Types.y);
                }
            }
            else{
                if(ClickLeft==1)
                {
                    CurrentTile->Types=Vector2u(CurrentTile->Types.x,PaintSelection);
                }
            }
            if(ClickRight==1)
            {
                if(CurrentTile->Orientation==0)
                {
                    CurrentTile->Orientation=1;
                    CurrentTile->isSquare=0;
                }
                else{
                    CurrentTile->Types=Vector2u(CurrentTile->Types.x,CurrentTile->Types.x);
                    CurrentTile->Orientation=0;
                    CurrentTile->isSquare=1;
                }
            }
        }
        if(SelectedTrapPos)
        {
            if (SelectedTrap->MainSwitch==0)
                SelectedTrapPos->Types.x=PaintSelection;
            else
                SelectedTrapPos->Types.y=PaintSelection;
        }
    }
}

void Editor::UpdateClickTrapEdit(Vector2u CurrentSquare,bool Orientation, bool Half){
    Tile* CurrentTile=track.getTile(CurrentSquare.x,CurrentSquare.y);
    if(ClickLeft && SwitchMode)
    {
        bool Available=1;
        for( auto& trap : track.Traps)
        {
            vector<Vector2u> SwitchPos=trap.SwitchPositions;
            if(find(SwitchPos.begin(),SwitchPos.end(),CurrentSquare)!=SwitchPos.end())
            {
               Available=0;
            }
            vector<Vector2u> TrapPos;
            for (unsigned i=0; i<trap.trapData.size(); i++)
            {
                TrapPos.push_back(trap.trapData[i].Square);
            }
            if(find(TrapPos.begin(),TrapPos.end(),CurrentSquare)!=TrapPos.end())
            {
               Available=0;
            }
        }
        if (Available==1)
        {
            CurrentTile->isSquare=1;
            CurrentTile->Types=Vector2u(1,1);
            track.Traps[CurrentTrap].SwitchPositions.push_back(CurrentSquare);
        }
    }
    if(ClickLeft && !SwitchMode)
    {
        bool Available=1;
        for( auto& trap : track.Traps)
        {
            vector<Vector2u> SwitchPos=trap.SwitchPositions;
            if(find(SwitchPos.begin(),SwitchPos.end(),CurrentSquare)!=SwitchPos.end())
               {
                   Available=0;
               }
            vector<Vector2u> TrapPos;
            for (unsigned i=0; i<trap.trapData.size(); i++)
            {
                if(trap.trapData[i].Half==Half)
                    TrapPos.push_back(trap.trapData[i].Square);
            }
            if(find(TrapPos.begin(),TrapPos.end(),CurrentSquare)!=TrapPos.end())
               {
                   Available=0;
               }
        }
        if (Available==1)
        {
            bool MainSwitch=track.Traps[CurrentTrap].MainSwitch;
            if(CurrentTile->isSquare)
                CurrentTile->Types=Vector2u(MainSwitch,MainSwitch);
            else
            {
                if (Half==1)
                    CurrentTile->Types.y=MainSwitch;
                else
                    CurrentTile->Types.x=MainSwitch;
            }
            TrapData NewData;
            NewData.Square=CurrentSquare;
            NewData.Half=Half;
            NewData.Types=Vector2u(0,1);
            track.Traps[CurrentTrap].trapData.push_back(NewData);
        }
    }
    if(ClickRight && SwitchMode)
    {
        Trap& trap=track.Traps[CurrentTrap];
        vector<Vector2u> &SwitchPos=trap.SwitchPositions;

        auto it=find(SwitchPos.begin(),SwitchPos.end(),CurrentSquare);
        if (it!=SwitchPos.end())
        {
            SwitchPos.erase(it);
        }
    }
    if(ClickRight && !SwitchMode)
    {
        Trap& trap=track.Traps[CurrentTrap];
        vector<TrapData> &AllTrapData=trap.trapData;
        TrapData CurrentTriangle;
        CurrentTriangle.Square=CurrentSquare;
        CurrentTriangle.Half=Half;


        auto it=find_if(AllTrapData.begin(),AllTrapData.end(),[&](const TrapData &First)
                        {return (First.Square==CurrentTriangle.Square) && (First.Half==CurrentTriangle.Half);});
        if (it!=AllTrapData.end())
        {
            AllTrapData.erase(it);
        }
        if (CurrentTile->isSquare)
        {
            CurrentTriangle.Half=!Half;
            it=find_if(AllTrapData.begin(),AllTrapData.end(),[&](const TrapData &First)
                        {return (First.Square==CurrentTriangle.Square) && (First.Half==CurrentTriangle.Half);});
            if (it!=AllTrapData.end())
            {
                AllTrapData.erase(it);
            }
        }
    }

    RefreshTrackRendering();

}

void Editor::UpdateClickWiringEdit(Vector2d Position){
    if (ClickLeft)
        {
        if (InitialStored==0)
        {
            InitialWiringPos=Position;
            InitialStored=1;
        }
        else
        {
            Vector2d FinalWiringPos=Position;
            Trap& trap=track.Traps[CurrentTrap];
            trap.Wiring.push_back(pair<Vector2d,Vector2d>(InitialWiringPos,FinalWiringPos));
            InitialStored=0;
        }

        }
    if (ClickRight)
    {
        if (InitialStored==1)
        {
            InitialStored=0;
        }
        if (InitialStored==0)
        {
            Trap& trap=track.Traps[CurrentTrap];
            auto it=find_if(trap.Wiring.begin(),trap.Wiring.end(),
                            [&](pair<Vector2d,Vector2d> Wire)
                            {return (DotProduct(Position-Wire.first,Position-Wire.first)<0.0001) ||
                                    (DotProduct(Position-Wire.second,Position-Wire.second)<0.0001);});
            if (it!=trap.Wiring.end())
            {
                trap.Wiring.erase(it);
            }
        }

    }
    RefreshTrackRendering();
}


void Editor::RefreshTrackRendering(){
    track.Reshape();
    Vector2u TrackDim=track.getDim();
    Vector2u TrackRes=Vector2u(TrackDim.x,TrackDim.y);
    Scaling=min(Resolution.x/static_cast<double>(TrackRes.x), Resolution.y/static_cast<double>(TrackRes.y));
    ScalingTrack=0.85*Scaling;
    ScalingTools=0.15*Scaling;

    for (auto &index :track.StartingPositions)
    {
        if (index.x>=TrackDim.x)
            index.x=TrackDim.x-1;
        if (index.y>=TrackDim.y)
            index.y=TrackDim.y-1;
    }

    StartingCircles.clear();
    StartingNumbers.clear();
    for (unsigned i=0; i<8 ; ++i)
    {
        sf::CircleShape Circle=CreateCircle(0.3*Scaling,sf::Color::Green,Vector2d(0,0));
        StartingCircles.push_back(Circle);

        ostringstream convert;
        convert<<i+1;
        sf::Text text=CreateText(convert.str(),&font,0.5*Scaling,sf::Color::Red,Vector2d(0,0));
        StartingNumbers.push_back(text);
    }

    SwitchCircles.clear();
    SwitchBoxes.clear();
    TrapBoxes.clear();
    WiringLines.clear();
    SwitchCircles.resize(track.Traps.size());
    SwitchBoxes.resize(track.Traps.size());
    TrapBoxes.resize(track.Traps.size());
    WiringLines.resize(track.Traps.size());
    for (unsigned i=0; i<track.Traps.size(); i++)
    {
        Trap& trap=track.Traps[i];
        SwitchCircles[i].resize(trap.SwitchPositions.size());
        SwitchBoxes[i].resize(trap.SwitchPositions.size());
        TrapBoxes[i].resize(trap.trapData.size());
        WiringLines[i].resize(trap.Wiring.size());
        bool MainSwitch=trap.MainSwitch;
        for (unsigned j=0; j<trap.SwitchPositions.size(); j++)
        {
            Vector2d SwitchPosition=ScalingTrack*Vector2d(trap.SwitchPositions[j].x+0.5, trap.SwitchPositions[j].y+0.5);
            if (MainSwitch==0)
                SwitchCircles[i][j]=CreateCircle(0.3*Scaling,sf::Color::Yellow,SwitchPosition);
            else
                SwitchCircles[i][j]=CreateCircle(0.3*Scaling,sf::Color::Green,SwitchPosition);
            if (CurrentTrap==i)
                SwitchBoxes[i][j]=CreateLineBox(SwitchPosition, ScalingTrack, sf::Color::Yellow);
            else
                SwitchBoxes[i][j]=CreateLineBox(SwitchPosition, ScalingTrack, sf::Color::Red);
        }
        for (unsigned j=0; j<trap.trapData.size(); j++)
        {
            Vector2u TrapPosUnsigned=trap.trapData[j].Square;
            Tile* CurrentTile=track.getTile(TrapPosUnsigned.x,TrapPosUnsigned.y);
            bool isSquare=CurrentTile->isSquare;
            unsigned Orientation=CurrentTile->Orientation;
            bool Half=trap.trapData[j].Half;
            Vector2d TrapPosition=ScalingTrack*Vector2d(TrapPosUnsigned.x+0.5, TrapPosUnsigned.y+0.5);
            if (CurrentTrap==i && isSquare)
                TrapBoxes[i][j]=CreateLineBox(TrapPosition, ScalingTrack, sf::Color::Yellow);
            if (CurrentTrap!=i && isSquare)
                TrapBoxes[i][j]=CreateLineBox(TrapPosition, ScalingTrack, sf::Color::Red);
            if (CurrentTrap==i && !isSquare)
                TrapBoxes[i][j]=CreateLineTriangle(TrapPosition,Orientation,Half, ScalingTrack, sf::Color::Yellow);
            if (CurrentTrap!=i && !isSquare)
                TrapBoxes[i][j]=CreateLineTriangle(TrapPosition,Orientation,Half, ScalingTrack, sf::Color::Red);
        }
        for (unsigned j=0; j<trap.Wiring.size(); j++)
        {
            Vector2d StartPos=trap.Wiring[j].first;
            Vector2d EndPos=trap.Wiring[j].second;
            StartPos=ScalingTrack*StartPos;
            EndPos=ScalingTrack*EndPos;
            if(CurrentTrap!=i)
                WiringLines[i][j]=CreateLine(StartPos,EndPos,sf::Color::Red);
            else
                WiringLines[i][j]=CreateLine(StartPos,EndPos,sf::Color::Yellow);
        }
    }

}
