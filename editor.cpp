#include "header.h"

Editor::Editor(){
    EditorActive=0;
}

double Editor::getTime(){
    return Clock.getElapsedTime().asSeconds();
}

void Editor::init(Configuration &NewConfig, Track &NewTrack, sf::RenderWindow &NewWindow){
    ButtonTexture.loadFromFile("EditorSprites.png");
    font.loadFromFile("Aller.ttf");
    EditorActive=1;
    ClickLeft=0;
    ClickRight=0;
    ExitFlag=0;
    PreExitFlag=0;
    SaveFlag=0;
    StartFlag=0;
    PaintSelection=0;
    PlayerSelection=0;
    OverLayON=1;
    Config=NewConfig;
    track=&NewTrack;
    Window=&NewWindow;
    Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    OverLay.setPrimitiveType(sf::Lines);
    Vector2u TrackDim=track->getDim();
    unsigned TileSize=track->getSize();
    Vector2u TrackRes=Vector2u(TileSize*TrackDim.x,TileSize*TrackDim.y);
    Scaling=min((double)Resolution.x/(double)TrackRes.x, (double)Resolution.y/(double)TrackRes.y);
    ScalingTrack=0.85d*Scaling;
    ScalingTools=0.15d*Scaling;

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
    sf::Font *FontPointer=&font;
    Clickable<bool> OverLayButton=Clickable<bool>(BoolPointer,vector<Vector2i>{Vector2i(0,1),Vector2i(0,0)},0.05*Resolution.x
                                  , Vector2f(0.9*Resolution.x, 0.1*Resolution.y),FontPointer,vector<string>{"Overlay", "ON/OFF"},0.02*Resolution.x,32);
    ClickableBools.push_back(OverLayButton);
    unsigned *UnsignedPointer=&track->FinishDirection;
    Clickable<unsigned> DirectionButton=Clickable<unsigned>(UnsignedPointer,vector<Vector2i>{Vector2i(1,0),Vector2i(1,1),Vector2i(1,2),Vector2i(1,3)},0.05*Resolution.x
                                  , Vector2f(0.9*Resolution.x, 0.3*Resolution.y),FontPointer,vector<string>{"Finishing", "Direction"},0.02*Resolution.x,32);
    ClickableUnsigneds.push_back(DirectionButton);
    UnsignedPointer=&track->TotalStarting;
    vector<Vector2i> textpos(8,Vector2i());
    for (unsigned i=0; i<8; ++i)
    {
        textpos[i]=Vector2i(2,i);
    }
    Clickable<unsigned> TotalPlayersButton=Clickable<unsigned>(UnsignedPointer,textpos,0.05*Resolution.x, Vector2f(0.9*Resolution.x, 0.5*Resolution.y)
                                           ,FontPointer,vector<string>{"No. Starting", "Players"},0.02*Resolution.x,32,1);
    ClickableUnsigneds.push_back(TotalPlayersButton);

    textpos.resize(5);
    for (unsigned i=0; i<5; ++i)
    {
        textpos[i]=Vector2i(0,i);
    }
    UnsignedPointer=&PaintSelection;
    ClickableSelection=Clickable<unsigned>(UnsignedPointer,textpos,0.05*Resolution.x, Vector2f(0.1*Resolution.x, 0.9*Resolution.y)
                                           ,FontPointer,vector<string>{"Paint Selection"},0.02*Resolution.x,32);

    sf::CircleShape Circle=sf::CircleShape(50);
    sf::Text text=sf::Text();
    StartingNumbers.clear();
    StartingCircles.clear();
    for (unsigned i=0; i<8 ; ++i)
    {
        StartingCircles.push_back(Circle);
        StartingCircles[i]=sf::CircleShape(50);
        StartingCircles[i].setRadius(Scaling*10);
        StartingCircles[i].setFillColor(sf::Color::Green);
        sf::FloatRect  CircleRect = StartingCircles[i].getLocalBounds();
        StartingCircles[i].setOrigin(CircleRect.left + CircleRect.width/2.0f,CircleRect.top  + CircleRect.height/2.0f);

        StartingNumbers.push_back(text);
        StartingNumbers[i].setFont(font);
        ostringstream convert;
        convert<<i+1;
        StartingNumbers[i].setString(convert.str());
        StartingNumbers[i].setCharacterSize(15*Scaling);
        StartingNumbers[i].setColor(sf::Color::Red);
        sf::FloatRect TextRect = StartingNumbers[i].getLocalBounds();
        StartingNumbers[i].setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top  + TextRect.height/2.0f);
    }
    Clock.restart();


    ExitMessage.setFont(font);
    ExitMessage.setString("Save File? (Y/N)");
    ExitMessage.setCharacterSize(40*Scaling);
    ExitMessage.setColor(sf::Color::Yellow);
    sf::FloatRect TextRect = ExitMessage.getLocalBounds();
    ExitMessage.setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top  + TextRect.height/2.0f);
    ExitMessage.setPosition(sf::Vector2f(Resolution.x/2,Resolution.y/2));


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
            case sf::Keyboard::Num5:
                PaintSelection=5;
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
                if (PlayerSelection<track->TotalStarting-1)
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
    if(ClickLeft==1 || ClickRight==1){
        Vector2u CurrentSquare=getCurrentSquare();
        Vector2u Dim=track->getDim();
        if (CurrentSquare.x<Dim.x && CurrentSquare.y<Dim.y)
        {
            Tile *CurrentTile=track->getTile(CurrentSquare.x,CurrentSquare.y);
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
                bool half=getTriangleHalf(CurrentSquare,CurrentTile->Orientation);
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
        track->StartingPositions[PlayerSelection]=getCurrentSquare();
        StartFlag=0;
    }

    if(ExitFlag==1)
    {
        if(SaveFlag==1)
        {
            track->FlushTrack(Config.TrackNumber);
            track->WriteTrack(Config.TrackNumber);
        }
        EditorActive=0;
    }

}

void Editor::Render(){
    Vector2u TrackDim=track->getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
    OverLay.resize((TrackDim.x*TrackDim.y)*12);
    unsigned current=0;
    for (unsigned i=0; i<TrackDim.x;++i)
    {
        for (unsigned j=0; j< TrackDim.y;++j)
        {
            Vector2u Pos=Vector2u(i,j);
            if(track->getTile(i,j)->isSquare==1)
            {
                sf::Vertex *tri = &TrackTiles[current * 3];
                sf::Vertex *lines = &OverLay[current * 6];
                Vector2u TextPos=Vector2u(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,0,1);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=Vector2u(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,2,1);
                current++;
            }
            else{
                sf::Vertex *tri= &TrackTiles[current * 3];
                sf::Vertex *lines = &OverLay[current * 6];
                Vector2u TextPos=Vector2u(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track->getTile(i,j)->Orientation);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=Vector2u(0,track->getTile(i,j)->Types.y);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track->getTile(i,j)->Orientation+2);
                current++;
            }
        }
    }
    for(unsigned i=0; i<(TrackDim.x*TrackDim.y)*12; i++)
    {
        OverLay[i].color=sf::Color::Blue;
    }
    unsigned Size=track->getSize();
    for (unsigned i=0; i<track->TotalStarting; i++)
    {
        Vector2u Position=track->StartingPositions[i];
        StartingCircles[i].setPosition(Vector2f(ScalingTrack*Size*Position.x+ScalingTrack*Size/2,
                                                ScalingTrack*Size*Position.y+ScalingTrack*Size/2));
        if (PlayerSelection==i)
        {
            StartingCircles[i].setFillColor(sf::Color::Yellow);
        }
        else
        {
            StartingCircles[i].setFillColor(sf::Color::Green);
        }
        StartingNumbers[i].setPosition(Vector2f(ScalingTrack*Size*Position.x+ScalingTrack*Size/2,
                                                ScalingTrack*Size*Position.y+ScalingTrack*Size/2));
    }
    Window->draw(TrackTiles,track->getTexture());

    if(OverLayON==1)
    {
        Window->draw(OverLay);
    }

    for (unsigned i=0; i<8; i++)
    {
        if (i<track->TotalStarting)
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
    ClickableSelection.Render(Window,track->getTexture());

    if(PreExitFlag==1)
    {
       Window->draw(ExitMessage);
    }
    Window->display();

    Window->clear();
}

void Editor::PrepareandScaleTriangle(sf::Vertex *tri, sf::Vertex *lines ,
                                     Vector2u TextPos, Vector2u Pos, unsigned Orientation, bool isSquare){
    unsigned Size=track->getSize();
    vector<sf::Vertex> quad(4);
    quad[0].position=(float)ScalingTrack*Vector2f(Pos.x*Size, Pos.y*Size);
    quad[1].position=(float)ScalingTrack*Vector2f(Pos.x*Size, (Pos.y+1)*Size);
    quad[2].position=(float)ScalingTrack*Vector2f((Pos.x+1)*Size, (Pos.y+1)*Size);
    quad[3].position=(float)ScalingTrack*Vector2f((Pos.x+1)*Size, Pos.y*Size);

    quad[0].texCoords=Vector2f(TextPos.x*Size, TextPos.y*Size);
    quad[1].texCoords=Vector2f(TextPos.x*Size, (TextPos.y+1)*Size);
    quad[2].texCoords=Vector2f((TextPos.x+1)*Size, (TextPos.y+1)*Size);
    quad[3].texCoords=Vector2f((TextPos.x+1)*Size, TextPos.y*Size);

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


Vector2u Editor::getCurrentSquare(){
    Vector2i localPosition = sf::Mouse::getPosition(*Window);
    unsigned Size=track->getSize();
    unsigned x=localPosition.x/(ScalingTrack*Size);
    unsigned y=localPosition.y/(ScalingTrack*Size);
    return sf::Vector2u(x,y);
}


bool Editor::getTriangleHalf(Vector2u Position, bool Orientation){
    Vector2i LocalPosition = sf::Mouse::getPosition(*Window);
    Vector2d CorrectedPosition=Vector2d((double)(LocalPosition.x)/ScalingTrack,(double)(LocalPosition.y)/ScalingTrack);
    double component;
    unsigned Size=track->getSize();
    if (Orientation==0){
        Vector2d Position1=Vector2d(Size*Position.x,Size*(Position.y+1));
        Vector2d Position2=Vector2d(Size*(Position.x+1),Size*Position.y);
        Vector2d Position3=Vector2d(Size*Position.x,Size*Position.y);
        Vector2d Normal=Position2-Position1;
        component=DotProduct(CorrectedPosition-Position3,Normal);
    }
    else{
        Vector2d Position1=Vector2d(Size*(Position.x+1),Size*(Position.y+1));
        Vector2d Position2=Vector2d(Size*Position.x,Size*Position.y);
        Vector2d Position3=Vector2d(Size*Position.x,Size*(Position.y+1));
        Vector2d Normal=Position2-Position1;
        component=DotProduct(CorrectedPosition-Position3,Normal);
    }
    return (component>0);
}
