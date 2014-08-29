#include "header.h"

Vector2i Resolution=Vector2i(800,600);
double CyclesPerSecond=100;
double CycleTime=1/CyclesPerSecond;
double FramesPerSecond=60;
double FrameTime=1/FramesPerSecond;
sf::Clock GameClock;
unsigned cycleaccumulator=0;
unsigned frameaccumulator=0;

int main()
{
    //timeBeginPeriod(1);
    sf::RenderWindow Window(sf::VideoMode(Resolution.x, Resolution.y), "Cat & Mouse");
    Track RaceTrack;
    Car::init();
    sf::Event Event;
    Menu menu(Window);
    Game game;
    Editor editor;
    while (Window.isOpen())
    {

        if (game.GameActive==1)
        {
            game.ProcessEvents(Event);
            if (game.getTime()-cycleaccumulator*CycleTime>CycleTime)
            {
                game.Update(CycleTime);
                cycleaccumulator++;
            }
            if (game.getTime()-frameaccumulator*FrameTime>FrameTime)
            {
                game.Render();
                frameaccumulator++;
            }
        }

        if (editor.EditorActive==1)
        {
            sf::sleep(sf::milliseconds(1));
            editor.ProcessEvents(Event);
            if (editor.getTime()-cycleaccumulator*CycleTime>CycleTime)
            {
                editor.Update(CycleTime);
                cycleaccumulator++;
            }
            if (editor.getTime()-frameaccumulator*FrameTime>FrameTime)
            {
                editor.Render();
                frameaccumulator++;
            }
        }

        if (game.GameActive==0 && editor.EditorActive==0)
        {
            sf::sleep(sf::milliseconds(1));
            menu.ProcessEvents(Event);
            menu.Update();
            menu.Render();
            if (menu.StartGame==1)
            {
                RaceTrack.init(menu.Config.TrackNumber);
                game.init(menu.Config,RaceTrack,Window);
                cycleaccumulator=0;
                frameaccumulator=0;
                menu.StartGame=0;
            }
            if (menu.StartEditor==1)
            {
                RaceTrack.init(menu.Config.TrackNumber);
                editor= Editor();
                editor.init(menu.Config,RaceTrack,Window);
                cycleaccumulator=0;
                frameaccumulator=0;
                menu.StartEditor=0;
            }
        }

    }
    //timeEndPeriod(1);
    return 0;
}
