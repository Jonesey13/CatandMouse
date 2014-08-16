#include "header.h"

Game::Game(){
    GameActive=0;
}

float Game::getTime(){
return Clock.getElapsedTime().asSeconds();
}

void Game::init(Configuration& NewConfig, Track& NewTrack, sf::RenderWindow& NewWindow){
    GameActive=1;
    Config=NewConfig;
    track=&NewTrack;
    Window=&NewWindow;
    sf::Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    sf::Vector2u TrackDim=track->getDim();
    int TileSize=track->getSize();
    sf::Vector2f TrackRes=sf::Vector2f(TileSize*TrackDim.x,TileSize*TrackDim.y);
    Scaling=min(Resolution.x/TrackRes.x, Resolution.y/TrackRes.y);

    Car::initTexture();
    Car car;
    for(unsigned int i=0; i< Config.NumberOfPlayers; i++){
        Player.push_back(car);
        int Size=track->getSize();
        int StartingX=track->getStartingPosition().x*Size;
        int StartingY=track->getStartingPosition().y*Size;
        Player[i].setPosition(sf::Vector2f(StartingX+Size/2,StartingY+Size/2+Size*i));
    }
    Clock.restart();
}


void Game::ProcessEvents(sf::Event& Event){
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

void Game::Update(float DeltaTime){
    for (unsigned int i=0; i<Player.size() ; i++)
    {
        Player[i].Update(DeltaTime);
    }
}

void Game::Render(){
    sf::Vector2u TrackDim=track->getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
    int current=0;
    for (unsigned int i=0; i<TrackDim.x;++i)
    {
        for (unsigned int j=0; j< TrackDim.y;++j)
        {
            sf::Vector2i Pos=sf::Vector2i(i,j);
            if(track->getTile(i,j)->isSquare==1)
            {
                sf::Vertex* tri = &TrackTiles[current * 3];
                sf::Vector2i TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,TextPos,Pos,0);
                current++;
                tri = &TrackTiles[current * 3];
                TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,TextPos,Pos,2);
                current++;
            }
            else{
                sf::Vertex* tri= &TrackTiles[current * 3];
                sf::Vector2i TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.x);
                PrepareandScaleTriangle(tri,TextPos,Pos,track->getTile(i,j)->Orientation);
                current++;
                tri = &TrackTiles[current * 3];
                TextPos=sf::Vector2i(0,track->getTile(i,j)->Types.y);
                PrepareandScaleTriangle(tri,TextPos,Pos,track->getTile(i,j)->Orientation+2);
                current++;
            }
        }
    }
    Window->draw(TrackTiles,track->getTexture());

    sf::Sprite PlayerSprite;
    int Size=Car::getSize();
    PlayerSprite.setTexture(*Car::getTexture());
    PlayerSprite.setOrigin(Size/2,Size/2);
    for (unsigned int i=0; i<Player.size() ; i++)
    {
        PlayerSprite.setPosition(Scaling*Player[i].getPosition());
        PlayerSprite.setScale(Scaling,Scaling);
        PlayerSprite.setTextureRect(sf::IntRect(i*Size, 0, Size, Size));
        PlayerSprite.setRotation(360*Player[i].getRotation());
        Window->draw(PlayerSprite);
    }
    Window->display();
    Window->clear();
}

void Game::PrepareandScaleTriangle(sf::Vertex* tri, sf::Vector2i TextPos, sf::Vector2i Pos, int Orientation){
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

}
