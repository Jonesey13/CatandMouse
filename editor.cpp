#include "editor.h"


void Editor::init(EditorOptions &NewEditOptions, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames){
    ButtonTexture.loadFromFile("EditorSprites.png");
    TrackTexture.loadFromFile("TrackSprites.png");
    font.loadFromFile("Aller.ttf");
    EditorActive=1;

    frameptr=&frames;
    *frameptr=0;
    cycleptr=&cycles;
    *cycleptr=0;

    ClickLeft=0;
    ClickRight=0;
    ExitFlag=0;
    PreExitFlag=0;
    SaveFlag=0;
    StartFlag=0;
    PaintSelection=0;
    PlayerSelection=0;
    OverLayON=1;



    EditOptions=NewEditOptions;

    if (EditOptions.BlankTrack==1)
    {
        track.SetBlank(EditOptions.NewTrackDim);
    }
    else{
        track.ReadTrack(EditOptions.TrackNumber);
    }

    Window=&NewWindow;
    Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    OverLay.setPrimitiveType(sf::Lines);
    Vector2u TrackDim=track.getDim();
    Vector2u TrackRes=Vector2u(TrackDim.x,TrackDim.y);
    Scaling=min(Resolution.x/static_cast<double>(TrackRes.x), Resolution.y/static_cast<double>(TrackRes.y));
    ScalingTrack=0.85*Scaling;
    ScalingTools=0.15*Scaling;

    ToolBoxes.setPrimitiveType(sf::Quads);
    ToolBoxes.resize(8);
    ToolBoxes[0].position=Vector2f(0.85*Resolution.x,0);
    ToolBoxes[1].position=Vector2f(0.85*Resolution.x,Resolution.y);
    ToolBoxes[2].position=Vector2f(Resolution.x,Resolution.y);
    ToolBoxes[3].position=Vector2f(Resolution.x,0);
    ToolBoxes[4].position=Vector2f(0,0.85*Resolution.y);
    ToolBoxes[5].position=Vector2f(0,Resolution.y);
    ToolBoxes[6].position=Vector2f(Resolution.x,Resolution.y);
    ToolBoxes[7].position=Vector2f(Resolution.x,0.85*Resolution.y);
    for (unsigned i=0; i <ToolBoxes.getVertexCount();i++)
        ToolBoxes[i].color=sf::Color(80,80,80);

    bool *BoolPointer=&OverLayON;
    Clickable<bool> OverLayButton=Clickable<bool>(BoolPointer,vector<Vector2i>{Vector2i(0,1),Vector2i(0,0)},0.05*Resolution.x
                                  , Vector2f(0.9*Resolution.x, 0.1*Resolution.y),vector<string>{"Overlay", "ON/OFF"},0.02*Resolution.x);
    ClickableBools.push_back(OverLayButton);
    unsigned *UnsignedPointer=&track.FinishDirection;
    Clickable<unsigned> DirectionButton=Clickable<unsigned>(UnsignedPointer,vector<Vector2i>{Vector2i(1,0),Vector2i(1,1),Vector2i(1,2),Vector2i(1,3)},0.05*Resolution.x
                                  , Vector2f(0.9*Resolution.x, 0.3*Resolution.y),vector<string>{"Finishing", "Direction"},0.02*Resolution.x);
    ClickableUnsigneds.push_back(DirectionButton);
    UnsignedPointer=&track.TotalStarting;
    vector<Vector2i> textpos(8,Vector2i());
    for (unsigned i=0; i<8; ++i)
    {
        textpos[i]=Vector2i(2,i);
    }
    Clickable<unsigned> TotalPlayersButton=Clickable<unsigned>(UnsignedPointer,textpos,0.05*Resolution.x, Vector2f(0.9*Resolution.x, 0.5*Resolution.y)
                                           ,vector<string>{"No. Starting", "Players"},0.02*Resolution.x,1);
    ClickableUnsigneds.push_back(TotalPlayersButton);

    textpos.resize(5);
    for (unsigned i=0; i<5; ++i)
    {
        textpos[i]=Vector2i(0,i);
    }
    UnsignedPointer=&PaintSelection;
    ClickableSelection=Clickable<unsigned>(UnsignedPointer,textpos,0.05*Resolution.x, Vector2f(0.1*Resolution.x, 0.9*Resolution.y)
                                           ,vector<string>{"Paint Selection"},0.02*Resolution.x);

    sf::CircleShape Circle=sf::CircleShape(50);
    sf::Text text=sf::Text();
    StartingNumbers.clear();
    StartingCircles.clear();
    for (unsigned i=0; i<8 ; ++i)
    {
        StartingCircles.push_back(Circle);
        StartingCircles[i]=sf::CircleShape(50);
        StartingCircles[i].setRadius(Scaling*0.3);
        StartingCircles[i].setFillColor(sf::Color::Green);
        sf::FloatRect  CircleRect = StartingCircles[i].getLocalBounds();
        StartingCircles[i].setOrigin(CircleRect.left + CircleRect.width/2.0f,CircleRect.top  + CircleRect.height/2.0f);

        StartingNumbers.push_back(text);
        ostringstream convert;
        convert<<i+1;
        AdjustText(StartingNumbers[i],convert.str(),&font,0.5*Scaling,sf::Color::Red);
    }
    Clock.restart();

    AdjustText(ExitMessage,"Save File? (Y/N)",&font,Scaling,sf::Color::Yellow,Vector2d(Resolution.x/2,Resolution.y/2));


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
        // Keyboard Input (1 Player Only)
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
        else{
            for (unsigned i=0; i<ClickableBools.size();i++)
            {
                ClickableBools[i].CheckAndUpdate(sf::Mouse::getPosition(*Window));
            }
            for (unsigned i=0; i<ClickableUnsigneds.size();i++)
            {
                ClickableUnsigneds[i].CheckAndUpdate(sf::Mouse::getPosition(*Window));
            }
            ClickableSelection.CheckAndUpdate(sf::Mouse::getPosition(*Window));
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
        if (i<track.TotalStarting)
        {
            Window->draw(StartingCircles[i]);
            Window->draw(StartingNumbers[i]);
        }
    }

    Window->draw(ToolBoxes);
    sf::Texture *TexturePointer=&ButtonTexture;

    for (unsigned i=0; i<ClickableBools.size();i++)
    {
        ClickableBools[i].Render(Window,TexturePointer);;
    }
    for (unsigned i=0; i<ClickableUnsigneds.size();i++)
    {
        ClickableUnsigneds[i].Render(Window,TexturePointer);;
    }
    ClickableSelection.Render(Window,&TrackTexture);

    if(PreExitFlag==1)
    {
       Window->draw(ExitMessage);
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




