#include "header.h"

Game::Game(){
    GameActive=0;
}

double Game::getTime(){
    return Clock.getElapsedTime().asSeconds();
}

void Game::init(Configuration &NewConfig, Track &NewTrack, sf::RenderWindow &NewWindow){
    GameActive=1;
    Config=NewConfig;
    track=&NewTrack;
    Window=&NewWindow;
    Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    Vector2u TrackDim=track->getDim();
    int TileSize=track->getSize();
    Vector2d TrackRes=Vector2d(TileSize*TrackDim.x,TileSize*TrackDim.y);
    Scaling=min(Resolution.x/TrackRes.x, Resolution.y/TrackRes.y);

    Car::init();
    Car car;
    for(unsigned i=0; i< Config.NumberOfPlayers; i++){
        Player.push_back(car);
        unsigned Size=track->getSize();
        unsigned StartingX=track->StartingPositions[0].x*Size;
        unsigned StartingY=track->StartingPositions[0].y*Size;
        Player[i].Position=Vector2d(StartingX+Size/2,StartingY+Size/2+Size*i);
    }
    Clock.restart();
}


void Game::ProcessEvents(sf::Event &Event){
    while (Window->pollEvent(Event))
    {
        switch (Event.type)
        {
        case sf::Event::Closed:
            {
            Window->close();
            break;
            }
        // Keyboard Input (1 Player Only)
        case sf::Event::KeyPressed:
            switch (Event.key.code)
            {
            case sf::Keyboard::Escape:
                {
                Window->close();
                break;
                }
            case sf::Keyboard::W:
                Player[0].VelocitySwitch=1;
                break;
            case sf::Keyboard::S:
                Player[0].BrakeSwitch=1;
                break;
            case sf::Keyboard::A:
                Player[0].RMinusSwitch=1;
                break;
            case sf::Keyboard::D:
                Player[0].RPlusSwitch=1;
                break;
            default:
                break;
            }
            break;
        case sf::Event::KeyReleased:
            {
            switch (Event.key.code)
            {
            case sf::Keyboard::W:
                Player[0].VelocitySwitch=0;
                break;
            case sf::Keyboard::S:
                Player[0].BrakeSwitch=0;
                break;
            case sf::Keyboard::A:
                Player[0].RMinusSwitch=0;
                break;
            case sf::Keyboard::D:
                Player[0].RPlusSwitch=0;
                break;
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

void Game::Update(double DeltaTime){
    for (unsigned i=0; i<Player.size() ; i++)
    {
        Player[i].Update(DeltaTime);
    }
}

void Game::Render(){
    Vector2u TrackDim=track->getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
    unsigned current=0;
    for (unsigned i=0; i<TrackDim.x;++i)
    {
        for (unsigned j=0; j< TrackDim.y;++j)
        {
            Vector2i Pos=Vector2i(i,j);
            sf::Vertex *tri= &TrackTiles[current * 3];
            Vector2i TextPos=Vector2i(0,track->getTile(i,j)->Types.x);
            PrepareandScaleTriangle(tri,TextPos,Pos,track->getTile(i,j)->Orientation);
            current++;
            tri = &TrackTiles[current * 3];
            TextPos=Vector2i(0,track->getTile(i,j)->Types.y);
            PrepareandScaleTriangle(tri,TextPos,Pos,track->getTile(i,j)->Orientation+2);
            current++;
        }
    }
    Window->draw(TrackTiles,track->getTexture());

    sf::Sprite PlayerSprite;
    unsigned Size=Car::getSize();
    PlayerSprite.setTexture(*Car::getTexture());
    PlayerSprite.setOrigin(Size/2,Size/2);
    for (unsigned i=0; i<Player.size() ; i++)
    {
        PlayerSprite.setPosition(Double2Float(Scaling*Player[i].Position));
        PlayerSprite.setScale(Scaling,Scaling);
        PlayerSprite.setTextureRect(sf::IntRect(i*Size, 0, Size, Size));
        PlayerSprite.setRotation(360*Player[i].Rotation);
        Window->draw(PlayerSprite);
    }
    Window->display();
    Window->clear();
}

void Game::PrepareandScaleTriangle(sf::Vertex *tri, Vector2i TextPos, Vector2i Pos, unsigned Orientation){
    unsigned Size=track->getSize();
    vector<sf::Vertex> quad(4);
    quad[0].position=Double2Float(Scaling*Vector2d(Pos.x*Size, Pos.y*Size));
    quad[1].position=Double2Float(Scaling*Vector2d(Pos.x*Size, (Pos.y+1)*Size));
    quad[2].position=Double2Float(Scaling*Vector2d((Pos.x+1)*Size, (Pos.y+1)*Size));
    quad[3].position=Double2Float(Scaling*Vector2d((Pos.x+1)*Size, Pos.y*Size));

    quad[0].texCoords=Vector2f(TextPos.x*Size, TextPos.y*Size);
    quad[1].texCoords=Vector2f(TextPos.x*Size, (TextPos.y+1)*Size);
    quad[2].texCoords=Vector2f((TextPos.x+1)*Size, (TextPos.y+1)*Size);
    quad[3].texCoords=Vector2f((TextPos.x+1)*Size, TextPos.y*Size);

    for (unsigned i=0; i<3; i++)
    {
        tri[i]=quad[(i+Orientation)%4];
    }

}
