#include "header.h"

Editor::Editor(){
    EditorActive=0;
}

float Editor::getTime(){
return Clock.getElapsedTime().asSeconds();
}

void Editor::init(Configuration& NewConfig, Track& NewTrack, sf::RenderWindow& NewWindow){
    EditorActive=1;
    Config=NewConfig;
    track=&NewTrack;
    Window=&NewWindow;
    sf::Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    OverLay.setPrimitiveType(sf::Lines);
    sf::Vector2u TrackDim=track->getDim();
    int TileSize=track->getSize();
    sf::Vector2f TrackRes=sf::Vector2f(TileSize*TrackDim.x,TileSize*TrackDim.y);
    Scaling=min(Resolution.x/TrackRes.x, Resolution.y/TrackRes.y);
    StartingCircle.setRadius(Scaling*10);
    StartingCircle.setFillColor(sf::Color::Green);
    sf::FloatRect  CircleRect = StartingCircle.getLocalBounds();
    StartingCircle.setOrigin(CircleRect.left + CircleRect.width/2.0f,CircleRect.top  + CircleRect.height/2.0f);
    Clock.restart();

    font.loadFromFile("Aller.ttf");
    ExitMessage.setFont(font); // font is a sf::Font
    ExitMessage.setString("Save File? (Y/N)");
    ExitMessage.setCharacterSize(40*Scaling);
    ExitMessage.setColor(sf::Color::Red);
    sf::FloatRect TextRect = ExitMessage.getLocalBounds();
    ExitMessage.setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top  + TextRect.height/2.0f);
    ExitMessage.setPosition(sf::Vector2f(Resolution.x/2,Resolution.y/2));


}


void Editor::ProcessEvents(sf::Event& Event){
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
                Exit=1;
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
                if(Exit==1)
                {
                   SaveYN=1;
                   SaveSwitch=1;
                }
                break;
            case sf::Keyboard::N:
                if(Exit==1)
                {
                   SaveYN=0;
                   SaveSwitch=1;
                }
                break;
            case sf::Keyboard::S:
                StartSwitch=1;
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

void Editor::Update(float DeltaTime){
    if(ClickLeft==1 || ClickRight==1){
        sf::Vector2u CurrentSquare=getCurrentSquare();
        sf::Vector2u Dim=track->getDim();
        if (CurrentSquare.x<Dim.x && CurrentSquare.y<Dim.y)
        {
            Tile* CurrentTile=track->getTile(CurrentSquare.x,CurrentSquare.y);
            if(CurrentTile->isSquare==1)
            {
                if(ClickLeft==1)
                {
                    CurrentTile->Types=sf::Vector2u(PaintSelection,PaintSelection);
                }
                if(ClickRight==1)
                {
                    CurrentTile->Types=sf::Vector2u(CurrentTile->Types.x,CurrentTile->Types.x);
                    CurrentTile->Orientation=0;
                    CurrentTile->isSquare=0;
                }
            }
            else{
                bool half=getTriangleHalf(CurrentSquare,CurrentTile->Orientation);
                if(half==0){
                    if(ClickLeft==1)
                    {
                        CurrentTile->Types=sf::Vector2u(PaintSelection,CurrentTile->Types.y);
                    }
                }
                else{
                    if(ClickLeft==1)
                    {
                        CurrentTile->Types=sf::Vector2u(CurrentTile->Types.x,PaintSelection);
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
                        CurrentTile->Types=sf::Vector2u(CurrentTile->Types.x,CurrentTile->Types.x);
                        CurrentTile->Orientation=0;
                        CurrentTile->isSquare=1;
                    }
                }
            }

        }
        ClickLeft=0;
        ClickRight=0;
    }

    if(StartSwitch==1)
    {
        track->setStartingPosition(getCurrentSquare());
        StartSwitch=0;
    }

    if(SaveSwitch==1)
    {
        if(SaveYN==1)
        {
            sf::Vector2u TrackDim=track->getDim();
            vector<sf::Vector2u> NewFinishLine;
            for (unsigned int i=0; i<TrackDim.x;++i)
            {
                for (unsigned int j=0; j< TrackDim.y;++j)
                {
                    if(track->getTile(i,j)->Types.x==5)
                        NewFinishLine.push_back(sf::Vector2u(i,j));
                }
            }
            track->setFinishLine(NewFinishLine);
            track->FlushTrack(Config.TrackNumber);
            track->WriteTrack(Config.TrackNumber);
        }
        EditorActive=0;
    }

}

void Editor::Render(){
    sf::Vector2u TrackDim=track->getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
    OverLay.resize((TrackDim.x*TrackDim.y)*12);
    int current=0;
    for (unsigned int i=0; i<TrackDim.x;++i)
    {
        for (unsigned int j=0; j< TrackDim.y;++j)
        {
            sf::Vector2i Pos=sf::Vector2i(i,j);
            if(track->getTile(i,j)->isSquare==1)
            {
                sf::Vertex* tri = &TrackTiles[current * 3];
                sf::Vertex* lines = &OverLay[current * 6];
                sf::Vector2i TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,0,1);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,2,1);
                current++;
            }
            else{
                sf::Vertex* tri= &TrackTiles[current * 3];
                sf::Vertex* lines = &OverLay[current * 6];
                sf::Vector2i TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track->getTile(i,j)->Orientation);
                current++;
                tri = &TrackTiles[current * 3];
                lines = &OverLay[current * 6];
                TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.y);
                PrepareandScaleTriangle(tri,lines,TextPos,Pos,track->getTile(i,j)->Orientation+2);
                current++;
            }
        }
    }
    for(unsigned int i=0; i<(TrackDim.x*TrackDim.y)*12; i++)
    {
        OverLay[i].color=sf::Color::Blue;
    }
    float Size=track->getSize();
    sf::Vector2u Position=track->getStartingPosition();
    StartingCircle.setPosition(sf::Vector2f(Scaling*Size*Position.x+Scaling*Size/2,Scaling*Size*Position.y+Scaling*Size/2));
    Window->draw(TrackTiles,track->getTexture());
    Window->draw(OverLay);
    Window->draw(StartingCircle);
    if(Exit==1)
    {
       Window->draw(ExitMessage);
    }
    Window->display();

    Window->clear();
}

void Editor::PrepareandScaleTriangle(sf::Vertex* tri, sf::Vertex* lines ,
                                     sf::Vector2i TextPos, sf::Vector2i Pos, int Orientation, bool isSquare){
    int Size=track->getSize();
    vector<sf::Vertex> quad(4);
    quad[0].position=Scaling*sf::Vector2f(Pos.x*Size, Pos.y*Size);
    quad[1].position=Scaling*sf::Vector2f(Pos.x*Size, (Pos.y+1)*Size);
    quad[2].position=Scaling*sf::Vector2f((Pos.x+1)*Size, (Pos.y+1)*Size);
    quad[3].position=Scaling*sf::Vector2f((Pos.x+1)*Size, Pos.y*Size);

    quad[0].texCoords=sf::Vector2f(TextPos.x*Size, TextPos.y*Size);
    quad[1].texCoords=sf::Vector2f(TextPos.x*Size, (TextPos.y+1)*Size);
    quad[2].texCoords=sf::Vector2f((TextPos.x+1)*Size, (TextPos.y+1)*Size);
    quad[3].texCoords=sf::Vector2f((TextPos.x+1)*Size, TextPos.y*Size);

    for (unsigned int i=0; i<3; i++)
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


sf::Vector2u Editor::getCurrentSquare(){
    sf::Vector2i localPosition = sf::Mouse::getPosition(*Window);
    int Size=track->getSize();
    int x=localPosition.x/(Scaling*Size);
    int y=localPosition.y/(Scaling*Size);
    return sf::Vector2u(x,y);
}


bool Editor::getTriangleHalf(sf::Vector2u Position, bool Orientation){
    sf::Vector2i LocalPosition = sf::Mouse::getPosition(*Window);
    sf::Vector2f CorrectedPosition=sf::Vector2f((float)(LocalPosition.x)/Scaling,(float)(LocalPosition.y)/Scaling);
    float component;

    float Size=track->getSize();
    if (Orientation==0){
        sf::Vector2f Position1=sf::Vector2f(Size*Position.x,Size*(Position.y+1));
        sf::Vector2f Position2=sf::Vector2f(Size*(Position.x+1),Size*Position.y);
        sf::Vector2f Position3=sf::Vector2f(Size*Position.x,Size*Position.y);
        sf::Vector2f Normal=Position2-Position1;
        component=DotProduct(CorrectedPosition-Position3,Normal);
    }
    else{
        sf::Vector2f Position1=sf::Vector2f(Size*(Position.x+1),Size*(Position.y+1));
        sf::Vector2f Position2=sf::Vector2f(Size*Position.x,Size*Position.y);
        sf::Vector2f Position3=sf::Vector2f(Size*Position.x,Size*(Position.y+1));
        sf::Vector2f Normal=Position2-Position1;
        component=DotProduct(CorrectedPosition-Position3,Normal);
    }
    return (component>0);
}
