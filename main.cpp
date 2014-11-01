#include "game.h"
#include "editor.h"
#include "menu.h"


int main()
{
    Vector2i Resolution=Vector2i(800,600);
    double CyclesPerSecond=120;
    double CycleTime=1/CyclesPerSecond;
    double FramesPerSecond=120;
    double FrameTime=1/FramesPerSecond;
    sf::Clock GameClock;
    unsigned cycles=0;
    unsigned frames=0;
    unsigned fails=0;

    sf::RenderWindow Window(sf::VideoMode(Resolution.x, Resolution.y), "Cat & Mouse");
    sf::Event Event;
    Menu menu(Window);
    Game game;
    Editor editor;
    while (Window.isOpen())
    {
        if (game.GameActive==1)
        {
            if (game.getTime()-cycles*CycleTime>CycleTime)
            {
                game.Update(CycleTime);
                game.ProcessEvents(Event);
                if (game.getTime()-cycles*CycleTime>2*CycleTime)
                {
                    fails++;
                    cerr<<fails<<endl;
                }
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
            if (editor.getTime()-cycles*CycleTime>CycleTime)
            {
                editor.ProcessEvents(Event);
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
