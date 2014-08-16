#include "header.h"


Menu::Menu(sf::RenderWindow& NewWindow)
{
    Window=&NewWindow;
    font.loadFromFile("Aller.ttf");

    sf::Vector2u Resolution=Window->getSize();
    TextSize=Resolution.y/(12.f);

    ostringstream convert;

    vector<MenuItem> ItemVector;
    TextBlock.push_back(ItemVector);
    TextBlock.push_back(ItemVector);
    TextBlock.push_back(ItemVector);

    MenuItem text;
    text.setFont(font); // font is a sf::Font
    text.setString("Cat & Mouse");
    text.setCharacterSize(TextSize);
    text.setColor(sf::Color::Red);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,2*TextSize));
    text.SelectionIndex=-1;
    TextBlock[0].push_back(text);



    text.setString("Track Editor");
    text.setCharacterSize(TextSize);
    text.setColor(sf::Color::Red);
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,2*TextSize));
    text.SelectionIndex=-1;
    TextBlock[1].push_back(text);
    TextBlock[2].push_back(text);



    text.setString("Start");
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,5*TextSize));
    text.SelectionIndex=0;
    TextBlock[0].push_back(text);



    convert<<"Track Number:"<<Config.TrackNumber;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,6*TextSize));
    text.SelectionIndex=1;
    TextBlock[0].push_back(text);

    convert.str("");
    convert<<"Number Of Players:"<<Config.NumberOfPlayers;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,7*TextSize));
    text.SelectionIndex=2;
    TextBlock[0].push_back(text);

    convert.str("");
    convert<<"Configure Joysticks";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,8*TextSize));
    text.SelectionIndex=3;
    TextBlock[0].push_back(text);


    convert.str("");
    convert<<"Press a Button On Joystick "<<CurrentJoy+1;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setColor(sf::Color::Yellow);
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,9*TextSize));
    text.SelectionIndex=3;
    text.flag=1;
    TextBlock[0].push_back(text);
    text.flag=0;

    text.setString("Track Editor Mode");
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,10*TextSize));
    text.SelectionIndex=4;
    TextBlock[0].push_back(text);

    convert.str("");
    convert<<"Track Number:"<<Config.TrackNumber;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,5*TextSize));
    text.SelectionIndex=0;
    TextBlock[1].push_back(text);

    convert.str("");
    convert<<"Use Existing Track";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,7*TextSize));
    text.SelectionIndex=1;
    TextBlock[1].push_back(text);

    convert.str("");
    convert<<"No Track Recorded";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,8*TextSize));
    text.SelectionIndex=1;
    text.flag=1;
    TextBlock[1].push_back(text);
    text.flag=0;


    convert.str("");
    convert<<"Start a New Track";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,9*TextSize));
    text.SelectionIndex=2;
    TextBlock[1].push_back(text);

    convert.str("");
    convert<<"Return";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,10*TextSize));
    text.SelectionIndex=3;
    TextBlock[1].push_back(text);


    convert.str("");
    convert<<"Track Width:"<<Config.NewTrackDim.x;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,5*TextSize));
    text.SelectionIndex=0;
    TextBlock[2].push_back(text);

    convert.str("");
    convert<<"Track Height:"<<Config.NewTrackDim.y;
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,6*TextSize));
    text.SelectionIndex=1;
    TextBlock[2].push_back(text);


    convert.str("");
    convert<<"Begin";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,7*TextSize));
    text.SelectionIndex=2;
    TextBlock[2].push_back(text);


    convert.str("");
    convert<<"Return";
    text.setString(convert.str());
    textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(Resolution.x/2,8*TextSize));
    text.SelectionIndex=3;
    TextBlock[2].push_back(text);
}



void Menu::ProcessEvents(sf::Event& Event){
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
                if (Selection < TextBlock[Choice].size())
                {
                    ++Selection;
                }
                }
                break;
            case sf::Keyboard::Up:
                {
                if (Selection > 0)
                {
                    --Selection;
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
        case sf::Event::JoystickButtonPressed:
        {
        unsigned int ActiveJoy=Event.joystickButton.joystickId;
        if (JoyConfig==1 && none_of(RegJoy.begin(),RegJoy.end(),
                                      [&](unsigned int k){return k==ActiveJoy;}))
        {
            Config.JoyId[ActiveJoy]=CurrentJoy;
            RegJoy.push_back(ActiveJoy);
            if(CurrentJoy+1<Config.NumberOfPlayers)
            {
               ++CurrentJoy;
            }
            else{
                JoyConfig=0;
            }
        }
        break;
        }
        default:
            break;
        }
    }
}


void Menu::Render(){
    for (unsigned int i=0; i< TextBlock[Choice].size(); i++)
    {
        if ((int)Selection==TextBlock[Choice][i].SelectionIndex)
        {
            TextBlock[Choice][i].setColor(sf::Color::Yellow);
        }
        else{
            TextBlock[Choice][i].setColor(sf::Color::Red);
        }
        if(TextBlock[Choice][i].flag==0 || JoyConfig==1 || FlagOn==1)
        {
        Window->draw(TextBlock[Choice][i]);
        }
    }
    Window->display();
    Window->clear();
}


void Menu::Update(){
    if(Choice==0)
    {
        if(Selection==0 && Selected==1)
        {
            StartGame=1;
        }

        if(Selection==1)
        {
            Config.TrackNumber=Config.TrackNumber+Change;
            ostringstream convert;
            convert<<"Track Number:"<<Config.TrackNumber;
            TextBlock[0][2].setString(convert.str());
            Change=0;
        }

        if(Selection==2)
        {
            Config.NumberOfPlayers=Config.NumberOfPlayers+Change;
            ostringstream convert;
            convert<<"Number Of Players:"<<Config.NumberOfPlayers;
            TextBlock[0][3].setString(convert.str());
            Change=0;
        }

        if(Selection==3)
        {
            if(Selected==1)
            {
                JoyConfig=1;
            }
            if(JoyConfig==1)
            {
                ostringstream convert;
                convert.str("");
                convert<<"Press a Button On Joystick "<<CurrentJoy+1;
                TextBlock[0][5].setString(convert.str());
            }
        }
        if(Selection==4 && Selected==1)
        {
            Choice=1;
            Selection=0;
        }
    }
    if(Choice==1)
    {
        if(Selection==0)
        {
            Config.TrackNumber=Config.TrackNumber+Change;
            ostringstream convert;
            convert<<"Track Number:"<<Config.TrackNumber;
            TextBlock[1][1].setString(convert.str());
            Change=0;
            FlagOn=0;
        }
        if(Selection==1 && Selected==1)
        {
            Track RaceTrack;
            RaceTrack.ReadTrack(Config.TrackNumber);
            if (RaceTrack.TrackReady==1)
            {
                StartEditor=1;
            }
            else
            {
                FlagOn=1;
            }

        }
        if(Selection==2 && Selected==1)
        {
            Choice=2;
            Selection=0;
        }
        if(Selection==3 && Selected==1)
        {
            Choice=0;
            Selection=0;
        }
    }
    if(Choice==2)
    {
        if(Selection==0)
        {
            Config.NewTrackDim.x=Config.NewTrackDim.x+Change;
            ostringstream convert;
            convert<<"Track Width:"<<Config.NewTrackDim.x;
            TextBlock[2][1].setString(convert.str());
            Change=0;
        }
        if(Selection==1)
        {
            Config.NewTrackDim.y=Config.NewTrackDim.y+Change;
            ostringstream convert;
            convert<<"Track Height:"<<Config.NewTrackDim.y;
            TextBlock[2][2].setString(convert.str());
            Change=0;
        }
        if(Selection==2 && Selected==1)
        {
            Track RaceTrack;
            RaceTrack.FlushTrack(Config.TrackNumber);
            RaceTrack.SetBlank(Config.NewTrackDim);
            RaceTrack.WriteTrack(Config.TrackNumber);
            StartEditor=1;
        }
        if(Selection==3 && Selected==1)
        {
            Choice=0;
            Selection=0;
        }
    }


    Selected=0;
}
