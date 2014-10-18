#include "editor.h"


void Editor::init(EditorOptions &NewEditOptions, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames){
    Window=&NewWindow;
    Vector2u Resolution=Window->getSize();

    ButtonTexture.loadFromFile("EditorSprites.png");
    TrackTexture.loadFromFile("TrackSprites.png");
    font.loadFromFile("Aller.ttf");
    ToolbarItem::Font=&font;
    ToolbarItem::Window=Window;
    ToolbarItem::TextSize=0.02*Resolution.x;

    EditorActive=1;

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


    TrackTiles.setPrimitiveType(sf::Triangles);
    OverLay.setPrimitiveType(sf::Lines);
    Vector2u TrackDim=track.getDim();
    Vector2u TrackRes=Vector2u(TrackDim.x,TrackDim.y);
    Scaling=min(Resolution.x/static_cast<double>(TrackRes.x), Resolution.y/static_cast<double>(TrackRes.y));
    ScalingTrack=0.85*Scaling;
    ScalingTools=0.15*Scaling;

    sf::RectangleShape Rectangle=CreateRectangle(Vector2d(0.15*Resolution.x,Resolution.y),sf::Color(80,80,80)
                                                , Vector2d (0.85*Resolution.x+0.5*0.15*Resolution.x,0.5*Resolution.y));
    ToolBoxes.push_back(Rectangle);
    Rectangle=CreateRectangle(Vector2d(Resolution.x,0.15*Resolution.y),sf::Color(80,80,80)
                                                , Vector2d (0.5*Resolution.x,0.85*Resolution.y+0.5*0.15*Resolution.y));
    ToolBoxes.push_back(Rectangle);



    ToolbarItems.push_back(make_shared<ToolbarButton>(vector<string>{"Overlay", "ON/OFF"},Vector2d(0.925*Resolution.x, 0.1*Resolution.y)
                                ,0.05*Resolution.x, OverLayAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(0,0),Vector2u(0,1)}, 1));

    ToolbarItems.push_back(make_shared<ToolbarButton>(vector<string>{"Finishing", "Direction"},Vector2d(0.925*Resolution.x, 0.3*Resolution.y)
                                ,0.05*Resolution.x, FinishDirectionAction, ButtonTexture
                                , vector<Vector2u>{Vector2u(1,0),Vector2u(1,1),Vector2u(1,2),Vector2u(1,3)}));

    vector<Vector2u> textpos;
    textpos.resize(5);
    for (unsigned i=0; i<5; ++i)
    {
        textpos[i]=Vector2u(0,i);
    }
    ToolbarItems.push_back(make_shared<ToolbarButtonList>(vector<string>{"Paint", "Selection"},Vector2d(0.3*Resolution.x, 0.925*Resolution.y)
                                ,0.05*Resolution.x, PaintSelectionAction, TrackTexture, textpos));


    ToolbarItems.push_back(make_shared<ToolbarNumber>(vector<string>{"No. Starting", "Players"},Vector2d(0.925*Resolution.x, 0.5*Resolution.y)
                            ,0.05*Resolution.x, track.TotalStarting, 1,8));

    for (unsigned i=0; i<8 ; ++i)
    {
        sf::CircleShape Circle=CreateCircle(0.3*Scaling,sf::Color::Green,Vector2d(0,0));
        StartingCircles.push_back(Circle);

        ostringstream convert;
        convert<<i+1;
        sf::Text text=CreateText(convert.str(),&font,0.5*Scaling,sf::Color::Red,Vector2d(0,0));
        StartingNumbers.push_back(text);
    }
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
            case sf::Keyboard::Num0:
                PaintSelection=0;
                break;
            case sf::Keyboard::Num1:
                PaintSelection=1;
                break;
            case sf::Keyboard::Num2:
                PaintSelection=2;
                break;
            case sf::Keyboard::Num3:
                PaintSelection=3;
                break;
            case sf::Keyboard::Num4:
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
    if(ClickLeft==1 || ClickRight==1){
        Vector2u Dim=track.getDim();
        if (CurrentSquare.x<Dim.x && CurrentSquare.y<Dim.y)
        {
            Tile *CurrentTile=track.getTile(CurrentSquare.x,CurrentSquare.y);
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
                bool half=getTriangleHalf(MousePosition,CurrentTile->Orientation);
                if(half==0){
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

        }
        for (auto& index: ToolbarItems)
            index->Update(*this);
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
    Vector2u Resolution=Window->getSize();
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
        if (i<track.TotalStarting)
        {
            Window->draw(StartingCircles[i]);
            Window->draw(StartingNumbers[i]);
        }
    }

    for(auto index : ToolBoxes)
    {
        Window->draw(index);
    }

    for (auto &index : ToolbarItems)
        index->Render();

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

