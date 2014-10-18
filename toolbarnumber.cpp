#include "toolbaritem.h"


ToolbarNumber::ToolbarNumber( vector<string> NewTitle,Vector2d NewPosition, unsigned NewWidth
                             , unsigned &NewTarget, unsigned NewMin, unsigned NewMax)
                  : ToolbarItem(NewTitle,NewPosition,NewWidth), Target(&NewTarget), Min(NewMin), Max(NewMax)
{
    LeftButtonBox=CreateLineBox(Vector2d(Position.x-2*TextSize,Position.y+TextSize),Width,sf::Color::Red);
    RightButtonBox=CreateLineBox(Vector2d(Position.x+2*TextSize,Position.y+TextSize),Width,sf::Color::Red);

    Vector2f BaseLeft=Vector2f(Position.x-2*TextSize,Position.y+TextSize);
    Vector2f BaseRight=Vector2f(Position.x+2*TextSize,Position.y+TextSize);
    Arrows.resize(6);
    for (unsigned i=0; i<6;i++)
    {
        Arrows[i].color=sf::Color::Red;
    }
    Arrows.setPrimitiveType(sf::Triangles);
    float Quarter=Width/4.0;
    Arrows[0].position=BaseLeft+Vector2f(Quarter,Quarter);
    Arrows[1].position=BaseLeft+Vector2f(Quarter,-Quarter);
    Arrows[2].position=BaseLeft+Vector2f(-Quarter,0);
    Arrows[3].position=BaseRight+Vector2f(-Quarter,-Quarter);
    Arrows[4].position=BaseRight+Vector2f(-Quarter,Quarter);
    Arrows[5].position=BaseRight+Vector2f(Quarter,0);
}

void ToolbarNumber::Update(Editor &editor){
    int Half=Width/2.0;
    Vector2i MouseLocation=sf::Mouse::getPosition(*Window);
    vector<Vector2i> RegionLeft={Vector2i(Position.x-Half-2*TextSize,Position.x-2*TextSize+Half),
                                Vector2i(Position.y-Half+TextSize,Position.y+Width+Half+TextSize)};

    if (MouseLocation.x>RegionLeft[0].x && MouseLocation.x<RegionLeft[0].y
        &&  MouseLocation.y>RegionLeft[1].x && MouseLocation.y<RegionLeft[1].y)
        {
            if (*Target>Min)
                (*Target)--;
            else
                *Target=Max;
        }

    vector<Vector2i> RegionRight={Vector2i(Position.x-Half+2*TextSize,Position.x+2*TextSize+Half),
                                Vector2i(Position.y-Half+TextSize,Position.y+Width+Half+TextSize)};

    if (MouseLocation.x>RegionRight[0].x && MouseLocation.x<RegionRight[0].y
        &&  MouseLocation.y>RegionRight[1].x && MouseLocation.y<RegionRight[1].y)
        {
            if (*Target<Max)
                (*Target)++;
            else
                *Target=Min;
        }
}

void ToolbarNumber::Render(){
    ToolbarItem::Render();
    ostringstream Convert;
    Convert<<*Target;
    sf::Text text=CreateText(Convert.str(),ToolbarItem::Font,1.5*TextSize,sf::Color::Red,Vector2d(Position.x,Position.y+TextSize));
    Window->draw(text);
//    Window->draw(LeftButtonBox);
//    Window->draw(RightButtonBox);
    Window->draw(Arrows);
}

