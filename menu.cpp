#include "menu.h"


Menu::Menu(sf::RenderWindow &NewWindow)
{
    Window=&NewWindow;
    font.loadFromFile("Aller.ttf");

    Resolution=Window->getSize();
    TextSize=Resolution.y/(12.f);

    MenuTitles.push_back("Cat & Mouse");
    MenuTitles.push_back("Track Editor");
    MenuTitles.push_back("Track Editor");
    MenuTitles.push_back("Options");
    MenuTitles.push_back("Controller Settings");

    vector<MenuItem> ItemVector;
    for (unsigned i=0; i<10; i++)
    {
        Items.push_back(ItemVector);
    }
    MenuItem Item=MenuItem("Start",ActionStartGame);
    Items[0].push_back(Item);


    Item=MenuItem("Track Number",Config.TrackNumber,1,100,NoAction);
    Items[0].push_back(Item);

    Item=MenuItem("Number Of Players",Config.NumberOfPlayers,1,8,NoAction);
    Items[0].push_back(Item);

    Item=MenuItem("Options",ActionOptionsMenu);
    Items[0].push_back(Item);

    Item=MenuItem("Track Editor",ActionEditorMenu1);
    Items[0].push_back(Item);

    Item=MenuItem("Track Number",Config.TrackNumber,NoAction);
    Items[1].push_back(Item);

    Item=MenuItem("Use Existing Track","No Track Recorded",ActionEditorRunCurrent);
    Items[1].push_back(Item);

    Item=MenuItem("Start A New Track",ActionEditorMenu2);
    Items[1].push_back(Item);

    Item=MenuItem("Return",ActionReturn);
    Items[1].push_back(Item);

    Item=MenuItem("Set Track Width",EditOptions.NewTrackDim.x,1,100,NoAction);
    Items[2].push_back(Item);

    Item=MenuItem("Set Track Height",EditOptions.NewTrackDim.y,1,100,NoAction);
    Items[2].push_back(Item);

    Item=MenuItem("Begin",ActionEditorRunNew);
    Items[2].push_back(Item);

    Item=MenuItem("Return",ActionReturn);
    Items[2].push_back(Item);

    Item=MenuItem("Number of Laps",Config.NumberOfLaps,NoAction);
    Items[3].push_back(Item);

    Item=MenuItem("Configure Controllers",ActionControllerMenu);
    Items[3].push_back(Item);

    Item=MenuItem("Save Settings","Done",ActionSaveOptions);
    Items[3].push_back(Item);

    Item=MenuItem("Load Settings","Done",ActionLoadOptions);
    Items[3].push_back(Item);

    Item=MenuItem("Return",ActionReturn);
    Items[3].push_back(Item);

    Item=MenuItem("Player Number",CurrentPlayer,1,8,NoAction);
    Items[4].push_back(Item);

    Item=MenuItem("Choose Joystick For Player","Press Any Button For Player",CurrentPlayer,ActionJoyConfig);
    Items[4].push_back(Item);

    Item=MenuItem("Choose Accelerate Button","Press Button Now",ActionAccelerateSet);
    Items[4].push_back(Item);

    Item=MenuItem("Choose Brake Button","Press Button Now",ActionBrakeSet);
    Items[4].push_back(Item);

    Item=MenuItem("Return to Options",ActionOptionsMenu);
    Items[4].push_back(Item);

}



void Menu::ProcessEvents(sf::Event &Event){
    while (Window->pollEvent(Event))
    {
        switch (Event.type)
        {
        case sf::Event::Closed:
            {
            Window->close();
            break;
            }
        case sf::Event::KeyPressed:
            switch (Event.key.code)
            {
            case sf::Keyboard::Escape:
                {
                    Window->close();
                }
                break;
            case sf::Keyboard::Return:
                {
                Selected=1;
                }
                break;
            case sf::Keyboard::Down:
                {
                if (Selection < Items[Choice].size()-1)
                    {
                        Items[Choice][Selection].Flag=0;
                        ClearVariables();
                        ++Selection;
                        Selected=0;
                    }
                }
                break;
            case sf::Keyboard::Up:
                {
                if (Selection > 0)
                    {
                        Items[Choice][Selection].Flag=0;
                        ClearVariables();
                        --Selection;
                        Selected=0;
                    }
                }
                break;
            case sf::Keyboard::Left:
                {
                Change=-1;
                }
                break;
            case sf::Keyboard::Right:
                {
                Change=1;
                }
                break;
            default:
                break;
            }
            break;
        case sf::Event::JoystickButtonPressed:
        {
            unsigned ActiveJoy=Event.joystickButton.joystickId;
            if (JoyConfig==1)
            {
                for( unsigned index=0; index<Config.JoyId.size();index++)
                {
                    if (Config.JoyId[index]==static_cast<int>(CurrentPlayer-1))
                    {
                        Config.JoyId[index]=-1;
                    }
                }
                Config.JoyId[ActiveJoy]=CurrentPlayer-1;
                JoyConfig=0;
                Items[Choice][Selection].Flag=0;
            }
            if(Config.JoyId[ActiveJoy]==static_cast<int>(CurrentPlayer-1) )
            {
                unsigned CurrentButton=Event.joystickButton.button;
                if (ButtonConfig==1 && CurrentAction==ACCELERATE)
                {
                    for (auto index: Config.ButtonsToActions[CurrentPlayer-1])
                    {
                        if (index.second==ACCELERATE){
                            Config.ButtonsToActions[CurrentPlayer-1].erase(index.first);
                            break;
                        }
                    }
                    Config.ButtonsToActions[CurrentPlayer-1][CurrentButton]=ACCELERATE;
                    ButtonConfig=0;
                    Items[Choice][Selection].Flag=0;
                }
                if (ButtonConfig==1 && CurrentAction==BRAKE)
                {
                    for (auto index: Config.ButtonsToActions[CurrentPlayer-1])
                    {
                        if (index.second==BRAKE){
                            Config.ButtonsToActions[CurrentPlayer-1].erase(index.first);
                            break;
                        }
                    }
                    Config.ButtonsToActions[CurrentPlayer-1][CurrentButton]=BRAKE;
                    ButtonConfig=0;
                    Items[Choice][Selection].Flag=0;
                }
            }
        }
            break;
        default:
            break;
        }
    }
}


void Menu::Render(){
    sf::Text text=CreateText(MenuTitles[Choice],&font ,TextSize, sf::Color::Red, Vector2d(Resolution.x/2,TextSize));
    Window->draw(text);
    for (unsigned i=0; i< Items[Choice].size(); i++)
    {
        if(Items[Choice][i].HasNumber==1)
        {
            ostringstream convert;
            convert<<Items[Choice][i].Item<<": "<<*Items[Choice][i].NumberPointer;
            text.setString(convert.str());
        }
        else
        {
            text.setString(Items[Choice][i].Item);
        }
        if (Selection==i)
        {
            text.setColor(sf::Color::Yellow);
        }
        else{
            text.setColor(sf::Color::Red);
        }
        RepositionText(text,Vector2d(Resolution.x/2,(4+i)*TextSize) );
        Window->draw(text);
        if (Items[Choice][i].HasFlag==1)
        {
            if(Items[Choice][i].HasFlagNumber==1)
            {
                ostringstream convert;
                convert<<Items[Choice][i].FlagMessage<<": "<<*Items[Choice][i].FlagPointer;
                text.setString(convert.str());
            }
            else{
                text.setString(Items[Choice][i].FlagMessage);
            }
            text.setColor(sf::Color::Yellow);
            RepositionText(text,Vector2d(Resolution.x/2,3*TextSize));
            if (Items[Choice][i].Flag==1)
            {
             Window->draw(text);
            }
        }
    }
    Window->display();
    Window->clear();
}


void Menu::Update(){
    if (Selected==1)
    {
        (this->*Items[Choice][Selection].ActionFunction)();
        Selected=0;
    }
    if (Items[Choice][Selection].HasNumber==1)
    {
        MenuItem Current=Items[Choice][Selection];
        if( *Current.NumberPointer+Change>=Current.Minimum &&  *Current.NumberPointer+Change<=Current.Maximum)
            *Current.NumberPointer+=Change;
    }
    Change=0;
}


void Menu::ActionStartGame(){
    StartGame=1;
}

void Menu::ActionReturn(){
    Choice=0;
    Selection=0;
}

void Menu::ActionEditorMenu1(){
    Choice=1;
    Selection=0;
}

void Menu::ActionEditorMenu2(){
    Choice=2;
    Selection=0;
}

void Menu::ActionOptionsMenu(){
    Choice=3;
    Selection=0;
}

void Menu::ActionControllerMenu(){
    Choice=4;
    Selection=0;
}

void Menu::ActionSaveOptions(){
    Config.Save();
    Items[Choice][Selection].Flag=1;
}

void Menu::ActionLoadOptions(){
    Config.Load();
    Items[Choice][Selection].Flag=1;
}

void Menu::ActionAccelerateSet(){
    ButtonConfig=1;
    CurrentAction=ACCELERATE;
    Items[Choice][Selection].Flag=1;
}

void Menu::ActionBrakeSet(){
    ButtonConfig=1;
    CurrentAction=BRAKE;
    Items[Choice][Selection].Flag=1;
}


void Menu::ActionJoyConfig(){
    JoyConfig=1;
    Items[Choice][Selection].Flag=1;
}

void Menu::ActionEditorRunCurrent(){
    Track RaceTrack;
    RaceTrack.ReadTrack(Config.TrackNumber);
    if (RaceTrack.TrackReady==1)
    {
        StartEditor=1;
        EditOptions.BlankTrack=0;
        EditOptions.TrackNumber=Config.TrackNumber;
    }
    else
    {
        Items[Choice][Selection].Flag=1;
    }
}

void Menu::ActionEditorRunNew(){
    EditOptions.BlankTrack=1;
    EditOptions.TrackNumber=Config.TrackNumber;
    StartEditor=1;
}
