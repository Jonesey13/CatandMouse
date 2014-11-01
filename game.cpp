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
    Resolution=Window->getSize();
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


    PauseItem Option=PauseItem("Resume",&Game::ActionUnpause);
    PauseOptions.push_back(Option);
    Option=PauseItem("Exit",&Game::ActionExit);
    PauseOptions.push_back(Option);


    TrackTiles.setPrimitiveType(sf::Triangles);
    Vector2u TrackDim=track->getDim();
    TrackTiles.resize((TrackDim.x*TrackDim.y)*6);
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

    RefreshRendering();

    Clock.restart();
}


void Game::ProcessEvents(sf::Event &Event){
//    while (Window->pollEvent(Event))
//    {
//        switch (Event.type)
//        {
//        case sf::Event::Closed:
//            {
//            Window->close();
//            break;
//            }
//        default:
//            break;
//        }
//    }
//        // Keyboard Input (1 Player Only)
//        case sf::Event::KeyPressed:
//            switch (Event.key.code)
//            {
//            case sf::Keyboard::Escape:
//                {
//                Pause=1;
//                break;
//                }
//            case sf::Keyboard::W:
//                Player[0]->VelocitySwitch=1;
//                break;
//            case sf::Keyboard::S:
//                Player[0]->BrakeSwitch=1;
//                break;
//            case sf::Keyboard::A:
//                Player[0]->RMinusSwitch=1;
//                break;
//            case sf::Keyboard::D:
//                Player[0]->RPlusSwitch=1;
//                break;
//            case sf::Keyboard::Up:
//                if(Pause==1)
//                {
//                    if(Selection>0)
//                    {
//                        Selection--;
//                    }
//                }
//                break;
//            case sf::Keyboard::Down:
//                if(Pause==1)
//                {
//                    if(Selection<PauseOptions.size()-1)
//                    {
//                        Selection++;
//                    }
//                }
//                break;
//            case sf::Keyboard::Return:
//                if(Pause==1)
//                {
//                    Selected=1;
//                }
//                break;
//            default:
//                break;
//            }
//            break;
//        case sf::Event::KeyReleased:
//            {
//            switch (Event.key.code)
//            {
//            case sf::Keyboard::W:
//                Player[0]->VelocitySwitch=0;
//                break;
//            case sf::Keyboard::S:
//                Player[0]->BrakeSwitch=0;
//                break;
//            case sf::Keyboard::A:
//                Player[0]->RMinusSwitch=0;
//                break;
//            case sf::Keyboard::D:
//                Player[0]->RPlusSwitch=0;
//                break;
//            default:
//                break;
//            }
//            break;
//            }
//        case sf::Event::JoystickButtonPressed:
//            {
//                unsigned CurrentJoy=Event.joystickButton.joystickId;
//                unsigned CurrentButton=Event.joystickButton.button;
//                int CurrentPlayer=Config.JoyId[CurrentJoy];
//                int NumberOfPlayers=Config.NumberOfPlayers;
//                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
//                {
//                    map<unsigned, Action>* ButtonsToActions=&Config.ButtonsToActions[CurrentPlayer];
//                    if (ButtonsToActions->find(CurrentButton)!=ButtonsToActions->end())
//                    {
//                        Action action = (*ButtonsToActions)[CurrentButton];
//                        if (action==ACCELERATE)
//                            Player[CurrentPlayer]->VelocitySwitch=1;
//                        if (action==BRAKE)
//                            Player[CurrentPlayer]->BrakeSwitch=1;
//                    }
//                }
//
//            }
//            break;
//        case sf::Event::JoystickButtonReleased:
//            {
//                unsigned CurrentJoy=Event.joystickButton.joystickId;
//                unsigned CurrentButton=Event.joystickButton.button;
//                int CurrentPlayer=Config.JoyId[CurrentJoy];
//                int NumberOfPlayers=Config.NumberOfPlayers;
//                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
//                {
//                    map<unsigned, Action>* ButtonsToActions=&Config.ButtonsToActions[CurrentPlayer];
//                    if (ButtonsToActions->find(CurrentButton)!=ButtonsToActions->end())
//                    {
//                        Action action = (*ButtonsToActions)[CurrentButton];
//                        if (action==ACCELERATE)
//                            Player[CurrentPlayer]->VelocitySwitch=0;
//                        if (action==BRAKE)
//                            Player[CurrentPlayer]->BrakeSwitch=0;
//                    }
//                }
//            }
//            break;
//        case sf::Event::JoystickMoved:
//            {
//                unsigned currentjoy=Event.joystickMove.joystickId;
//                int CurrentPlayer=Config.JoyId[currentjoy];
//                int NumberOfPlayers=Config.NumberOfPlayers;
//                if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
//                {
//                    Car* ThisPlayer=Player[Config.JoyId[currentjoy]];
//                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)<-50
//                       && ThisPlayer->RMinusSwitch==0)
//                    {
//                        ThisPlayer->RMinusSwitch=1;
//                    }
//                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)>50
//                       && ThisPlayer->RPlusSwitch==0)
//                    {
//                        ThisPlayer->RPlusSwitch=1;
//                    }
//                    if(sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)>-50
//                       && sf::Joystick::getAxisPosition(currentjoy,sf::Joystick::X)<50)
//                    {
//                        ThisPlayer->RMinusSwitch=0;
//                        ThisPlayer->RPlusSwitch=0;
//                    }
//                }
//            }
//            break;
//        default:
//            break;
//        }
//    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        Pause=1;
    }

    Player[0]->VelocitySwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    Player[0]->BrakeSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    Player[0]->RMinusSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    Player[0]->RPlusSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    if(Config.NumberOfPlayers>=2)
    {
        Player[1]->VelocitySwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        Player[1]->BrakeSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        Player[1]->RMinusSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        Player[1]->RPlusSwitch=sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    }
    if (Pause==1)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && Selection<PauseOptions.size()-1)
            Selection++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && Selection>0)
            Selection--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) )
            Selected=1;
    }


    sf::Joystick::update();


    for (unsigned CurrentJoy=0; CurrentJoy<sf::Joystick::Count; CurrentJoy++)
    {
        int CurrentPlayer=Config.JoyId[CurrentJoy];
        int NumberOfPlayers=Config.NumberOfPlayers;
        if (CurrentPlayer!=-1 && CurrentPlayer<NumberOfPlayers)
        {
            for(unsigned CurrentButton=0; CurrentButton<sf::Joystick::ButtonCount; CurrentButton++)
            {
                bool OnOff=sf::Joystick::isButtonPressed(CurrentJoy, CurrentButton);
                map<unsigned, Action>* ButtonsToActions=&Config.ButtonsToActions[CurrentPlayer];

                if (ButtonsToActions->find(CurrentButton)!=ButtonsToActions->end())
                {
                    Action action = (*ButtonsToActions)[CurrentButton];
                    if (action==ACCELERATE)
                        {
                            Player[CurrentPlayer]->VelocitySwitch=OnOff;
                        }
                    if (action==BRAKE)
                        Player[CurrentPlayer]->BrakeSwitch=OnOff;
                }
            }
            for(unsigned CurrentAxis=0; CurrentAxis<sf::Joystick::AxisCount; CurrentAxis++)
            {
                if(sf::Joystick::getAxisPosition(CurrentJoy,sf::Joystick::X)<-50
                   && Player[CurrentPlayer]->RMinusSwitch==0)
                {
                    Player[CurrentPlayer]->RMinusSwitch=1;
                }
                if(sf::Joystick::getAxisPosition(CurrentJoy,sf::Joystick::X)>50
                   && Player[CurrentPlayer]->RPlusSwitch==0)
                {
                    Player[CurrentPlayer]->RPlusSwitch=1;
                }
                if(sf::Joystick::getAxisPosition(CurrentJoy,sf::Joystick::X)>-50
                   && sf::Joystick::getAxisPosition(CurrentJoy,sf::Joystick::X)<50)
                {
                    Player[CurrentPlayer]->RMinusSwitch=0;
                    Player[CurrentPlayer]->RPlusSwitch=0;
                }
            }
        }
    }

//    while (Window->pollEvent(Event)){
//    }


    if (Selection==1 && Selected==1)
    {
        while (Window->pollEvent(Event)){};
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
    unsigned current=0;
    RefreshRendering();
    Vector2u TrackDim=track->getDim();
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


    for (unsigned i=0; i<track->Traps.size();i++)
    {
        for (unsigned j=0; j<track->Traps[i].SwitchPositions.size();j++)
        {
            Window->draw(SwitchCircles[i][j]);
        }
        for (unsigned j=0; j<track->Traps[i].trapData.size();j++)
        {
            Window->draw(TrapBoxes[i][j]);
        }
        for (unsigned j=0; j<track->Traps[i].Wiring.size();j++)
        {
            Window->draw(WiringLines[i][j]);
        }
    }

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

void Game::PrepareandScaleTriangle(sf::Vertex *tri, const Vector2i& TextPos, const Vector2i& Pos, const unsigned& Orientation){
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

void Game::RefreshRendering(){

    SwitchCircles.clear();
    TrapBoxes.clear();
    WiringLines.clear();
    SwitchCircles.resize(track->Traps.size());
    TrapBoxes.resize(track->Traps.size());
    WiringLines.resize(track->Traps.size());
    for (unsigned i=0; i<track->Traps.size(); i++)
    {
        Trap& trap=track->Traps[i];
        SwitchCircles[i].resize(trap.SwitchPositions.size());
        TrapBoxes[i].resize(trap.trapData.size());
        WiringLines[i].resize(trap.Wiring.size());
        bool MainSwitch=trap.MainSwitch;
        for (unsigned j=0; j<trap.SwitchPositions.size(); j++)
        {
            Vector2d SwitchPosition=ScalePosition(Vector2d(trap.SwitchPositions[j].x+0.5, trap.SwitchPositions[j].y+0.5)
                                                  , Scaling, paddingdim, padding);
            if (MainSwitch==0)
                SwitchCircles[i][j]=CreateCircle(0.3*Scaling,sf::Color::Yellow,SwitchPosition);
            else
                SwitchCircles[i][j]=CreateCircle(0.3*Scaling,sf::Color::Green,SwitchPosition);
        }
        for (unsigned j=0; j<trap.trapData.size(); j++)
        {
            Vector2u TrapPosUnsigned=trap.trapData[j].Square;
            Tile* CurrentTile=track->getTile(TrapPosUnsigned.x,TrapPosUnsigned.y);
            bool isSquare=CurrentTile->isSquare;
            unsigned Orientation=CurrentTile->Orientation;
            bool Half=trap.trapData[j].Half;
            Vector2d TrapPosition=ScalePosition(Vector2d(TrapPosUnsigned.x+0.5, TrapPosUnsigned.y+0.5)
                                                  , Scaling, paddingdim, padding);
            if (trap.MainSwitch==1 && isSquare)
                TrapBoxes[i][j]=CreateLineBox(TrapPosition, Scaling, sf::Color::Yellow);
            if (trap.MainSwitch==0 && isSquare)
                TrapBoxes[i][j]=CreateLineBox(TrapPosition, Scaling, sf::Color::Red);
            if (trap.MainSwitch==1 && !isSquare)
                TrapBoxes[i][j]=CreateLineTriangle(TrapPosition,Orientation,Half, Scaling, sf::Color::Yellow);
            if (trap.MainSwitch==0 && !isSquare)
                TrapBoxes[i][j]=CreateLineTriangle(TrapPosition,Orientation,Half, Scaling, sf::Color::Red);
        }
        for (unsigned j=0; j<trap.Wiring.size(); j++)
        {
            Vector2d StartPos=trap.Wiring[j].first;
            Vector2d EndPos=trap.Wiring[j].second;
            StartPos=ScalePosition(StartPos, Scaling, paddingdim, padding);
            EndPos=ScalePosition(EndPos, Scaling, paddingdim, padding);
            if(trap.MainSwitch==0)
                WiringLines[i][j]=CreateLine(StartPos,EndPos,sf::Color::Red);
            else
                WiringLines[i][j]=CreateLine(StartPos,EndPos,sf::Color::Yellow);
        }
    }
}
