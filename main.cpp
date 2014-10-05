#include "game.h"
#include "track.h"
#include "editor.h"
#include "menu.h"

Vector2i Resolution=Vector2i(800,600);
double CyclesPerSecond=100;
double CycleTime=1/CyclesPerSecond;
double FramesPerSecond=60;
double FrameTime=1/FramesPerSecond;
sf::Clock GameClock;
unsigned cycles=0;
unsigned frames=0;

int main()
{
    sf::RenderWindow Window(sf::VideoMode(Resolution.x, Resolution.y), "Cat & Mouse");
    sf::Event Event;
    Menu menu(Window);
    Game game;
    Editor editor;
    while (Window.isOpen())
    {
        if (game.GameActive==1)
        {
            game.ProcessEvents(Event);
            if (game.getTime()-cycles*CycleTime>CycleTime)
            {
                game.Update(CycleTime);
                cycles++;
            }
            if (game.getTime()-frames*FrameTime>FrameTime)
            {
                game.Render();
                frames++;
            }
        }

        if (editor.EditorActive==1)
        {
            sf::sleep(sf::milliseconds(1));
            editor.ProcessEvents(Event);
            if (editor.getTime()-cycles*CycleTime>CycleTime)
            {
                editor.Update(CycleTime);
                cycles++;
            }
            if (editor.getTime()-frames*FrameTime>FrameTime)
            {
                editor.Render();
                frames++;
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
                game=Game();
                game.init(menu.Config,Window,cycles,frames);
                menu.StartGame=0;
            }
            if (menu.StartEditor==1)
            {
                editor=Editor();
                editor.init(menu.EditOptions,Window,cycles,frames);
                menu.StartEditor=0;
            }
        }

    }
    return 0;
}
