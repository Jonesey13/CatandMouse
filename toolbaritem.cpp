#include "toolbaritem.h"

sf::RenderWindow* ToolbarItem::Window=nullptr;
sf::Font* ToolbarItem::Font=nullptr;
unsigned ToolbarItem::TextSize=0;


ToolbarItem::ToolbarItem( vector<string> NewTitle,Vector2d NewPosition , unsigned NewWidth ):
    Title(NewTitle),Position(NewPosition), Width(NewWidth)
{
    TitleText.resize(Title.size());
    for(unsigned i=0; i< Title.size(); i++)
        TitleText[i]=CreateText(Title[i],Font,TextSize,sf::Color::Red,
                             Vector2d(Position.x,Position.y-(TitleText.size()-i)*TextSize));
};


void ToolbarItem::Render(){
    for(unsigned i=0; i< Title.size(); i++)
        Window->draw(TitleText[i]);
}
