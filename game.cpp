#include "game.h"

Game::Game(){
    GameActive=0;
}

double Game::getTime(){
    return Clock.getElapsedTime().asSeconds();
}


void Game::init(Configuration &NewConfig, sf::RenderWindow &NewWindow,unsigned &cycles, unsigned &frames){
    GameActive=1;
    CarTexture.loadFromFile("CarSprites.png");
    TrackTexture.loadFromFile("TrackSprites.png");
    font.loadFromFile("Aller.ttf");
    Config=NewConfig;
    Window=&NewWindow;
    race.init(Config);

    track=&race.track;
    frameptr=&frames;
    *frameptr=0;
    cycleptr=&cycles;
    *cycleptr=0;

    Player.resize(race.Player.size());
    for (unsigned i=0; i<race.Player.size();i++)
    {
        Player[i]=&race.Player[i];
    }

    PauseItem Option=PauseItem("Resume",ActionUnpause);
    PauseOptions.push_back(Option);
    Option=PauseItem("Exit",ActionExit);
    PauseOptions.push_back(Option);

    Vector2u Resolution=Window->getSize();
    TrackTiles.setPrimitiveType(sf::Triangles);
    Vector2u TrackDim=track->getDim();
    Vector2d TrackRes=Vector2d(TrackDim.x,TrackDim.y);
    Scaling=min(Resolution.x/TrackRes.x, Resolution.y/TrackRes.y);

    if ((Resolution.x/TrackRes.x)>(Resolution.y/TrackRes.y))
    {
        paddingdim=0;
        padding=(Resolution.x-Scaling*TrackRes.x)*0.5;
    }
    else{
        paddingdim=1;
        padding=(Resolution.y-Scaling*TrackRes.y)*0.5;
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
                Pause=1;
                break;
                }
            case sf::Keyboard::W:
                Player[0]->VelocitySwitch=1;
                break;
            case sf::Keyboard::S:
                Player[0]->BrakeSwitch=1;
                break;
            case sf::Keyboard::A:
                Player[0]->RMinusSwitch=1;
                break;
            case sf::Keyboard::D:
                Player[0]->RPlusSwitch=1;
                break;
            case sf::Keyboard::Up:
                if(Pause==1)
                {
                    if(Selection>0)
                    {
                        Selection--;
                    }
                }
                break;
            case sf::Keyboard::Down:
                if(Pause==1)
                {
                    if(Selection<PauseOptions.size()-1)
                    {
                        Selection++;
                    }
                }
                break;
            case sf::Keyboard::Return:
                if(Pause==1)
                {
                    Selected=1;
                }
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
                Player[0]->VelocitySwitch=0;
                break;
            case sf::Keyboard::S:
                Player[0]->BrakeSwitch=0;
                break;
            case sf::Keyboard::A:
                Player[0]->RMinusSwitch=0;
                break;
            case sf::Keyboard::D:
                Player[0]->RPlusSwitch=0;
                break;
            default:
                break;
            }
            break;
            }
        case sf::Event::JoystickButtonPressed:
            {
                unsigned CurrentJoy=Event.joystickButton.joystickId;
                unsigned CurrentButton=Event.joystickButton.button;
                int CurrentPlayer=Config.JoyId[CurrentJoy];
                int NumberOfPlayers=Config.NumberOfPlayers;
                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
                {
                    map<unsigned, Action>* ButtonsToActions=&Config.ButtonsToActions[CurrentPlayer];
                    if (ButtonsToActions->find(CurrentButton)!=ButtonsToActions->end())
                    {
                        Action action = (*ButtonsToActions)[CurrentButton];
                        if (action==ACCELERATE)
                            Player[CurrentPlayer]->VelocitySwitch=1;
                        if (action==BRAKE)
                            Player[CurrentPlayer]->BrakeSwitch=1;
                    }
                }

            }
            break;
        case sf::Event::JoystickButtonReleased:
            {
                unsigned CurrentJoy=Event.joystickButton.joystickId;
                unsigned CurrentButton=Event.joystickButton.button;
                int CurrentPlayer=Config.JoyId[CurrentJoy];
                int NumberOfPlayers=Config.NumberOfPlayers;
                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
                {
                    map<unsigned, Action>* ButtonsToActions=&Config.ButtonsToActions[CurrentPlayer];
                    if (ButtonsToActions->find(CurrentButton)!=ButtonsToActions->end())
                    {
                        Action action = (*ButtonsToActions)[CurrentButton];
                        if (action==ACCELERATE)
                            Player[CurrentPlayer]->VelocitySwitch=0;
                        if (action==BRAKE)
                            Player[CurrentPlayer]->BrakeSwitch=0;
                    }
                }
            }
            break;
        case sf::Event::JoystickMoved:
            {
                unsigned currentjoy=Event.joystickMove.joystickId;
                int CurrentPlayer=Config.JoyId[currentjoy];
                int NumberOfPlayers=Config.NumberOfPlayers;
                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
                {
                    Car* ThisPlayer=Player[Config.JoyId[currentjoy]];
                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)<-50
                       && ThisPlayer->RMinusSwitch==0)
                    {
                        ThisPlayer->RMinusSwitch=1;
                    }
                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)>50
                       && ThisPlayer->RPlusSwitch==0)
                    {
                        ThisPlayer->RPlusSwitch=1;
                    }
                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)>-50
                       && sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)<50)
                    {
                        ThisPlayer->RMinusSwitch=0;
                        ThisPlayer->RPlusSwitch=0;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

void Game::Update(double DeltaTime){
    if (Pause==0)
    {
        race.Update(DeltaTime);
    }
    else{
        if(Selected==1)
        {
            (this->*PauseOptions[Selection].ActionFunction)();
            Selected=0;
        }
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
    Window->draw(TrackTiles,&TrackTexture);

    sf::Sprite PlayerSprite;
    PlayerSprite.setTexture(CarTexture);
    PlayerSprite.setOrigin(RenderSize/2,RenderSize/2);
    for (unsigned i=0; i<Player.size() ; i++)
    {
        Vector2d PlayerPosition=Player[i]->Position;
        PlayerPosition=ScalePosition(PlayerPosition, Scaling, paddingdim, padding);
        PlayerSprite.setPosition(Double2Float(PlayerPosition));
        if (Player[i]->DeathSwitch==1)
        {
            double DeathScaling=Scaling*(Car::DeathDuration-Player[i]->DeathTime)/Car::DeathDuration;
            PlayerSprite.setScale(DeathScaling/RenderSize,DeathScaling/RenderSize);
        }
        else{
            PlayerSprite.setScale(Scaling/RenderSize,Scaling/RenderSize);
        }
        PlayerSprite.setTextureRect(sf::IntRect(i*RenderSize, 0, RenderSize, RenderSize));
        PlayerSprite.setRotation(360*Player[i]->Rotation);
        Window->draw(PlayerSprite);
    }

    if (race.Victory==1)
    {
        ostringstream convert;
        convert<<"Player "<<race.VictorNumber<<" Has Won!";
        Vector2u Resolution=Window->getSize();
        unsigned VictoryTextSize=Resolution.y/(12.f);
        sf::RectangleShape VictoryRectangle=CreateRectangle(Vector2d(Resolution.x/1.5, VictoryTextSize*1.2),
                                                            sf::Color::Blue,Vector2d(Resolution.x/2,Resolution.y/4));
        Window->draw(VictoryRectangle);
        sf::Text text=CreateText(convert.str(),&font ,VictoryTextSize,
                                 sf::Color::Yellow, Vector2d(Resolution.x/2,Resolution.y/4));
        Window->draw(text);
    }

    if (Pause==1)
    {
        Vector2u Resolution=Window->getSize();
        PauseTextSize=Resolution.y/(12.f);
        sf::RectangleShape PauseRectangle=CreateRectangle(Vector2d(Resolution.x/2, PauseTextSize*3),
                                                    sf::Color::Blue,Vector2d(Resolution.x/2,Resolution.y/2));
        Window->draw(PauseRectangle);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(PauseTextSize);
        for (unsigned i=0; i<PauseOptions.size(); i++)
        {
            text.setString(PauseOptions[i].Item);
            if (Selection==i)
            {
                text.setColor(sf::Color::Yellow);
            }
            else{
                text.setColor(sf::Color::Red);
            }
            RepositionText(text,Vector2d(Resolution.x/2,Resolution.y/2+(i-0.5)*PauseTextSize));
            Window->draw(text);
        }
    }

    Window->display();
    Window->clear();
}

void Game::PrepareandScaleTriangle(sf::Vertex *tri, Vector2i TextPos, Vector2i Pos, unsigned Orientation){
    vector<sf::Vertex> quad(4);
    quad[0].position=Double2Float(Vector2d(Pos.x, Pos.y));
    quad[1].position=Double2Float(Vector2d(Pos.x, Pos.y+1));
    quad[2].position=Double2Float(Vector2d(Pos.x+1, Pos.y+1));
    quad[3].position=Double2Float(Vector2d(Pos.x+1, Pos.y));
    for (unsigned i=0; i<4;i++)
    {
        quad[i].position=ScalePosition(quad[i].position,Scaling,paddingdim,padding);
    }

    float TextureScaling=RenderSize;
    quad[0].texCoords=TextureScaling*Vector2f(TextPos.x, TextPos.y);
    quad[1].texCoords=TextureScaling*Vector2f(TextPos.x, TextPos.y+1);
    quad[2].texCoords=TextureScaling*Vector2f(TextPos.x+1, TextPos.y+1);
    quad[3].texCoords=TextureScaling*Vector2f(TextPos.x+1, TextPos.y);

    for (unsigned i=0; i<3; i++)
    {
        tri[i]=quad[(i+Orientation)%4];
    }
}

void Game::ActionUnpause(){
    Pause=0;
}

void Game::ActionExit(){
    GameActive=0;
}
