#include "toolbaritem.h"


ToolbarButtonList::ToolbarButtonList(vector<string> NewTitle,Vector2d NewPosition,unsigned NewWidth,EditorVoidptr NewAction,
                             sf::Texture& NewTexture, vector<Vector2u> NewTexturePositions, unsigned StartingIndex , unsigned *BindingTarget)
            : ToolbarItem(NewTitle,NewPosition,NewWidth),  Action(NewAction), Texture(&NewTexture),
            TexturePositions(NewTexturePositions), RenderIndex(StartingIndex) , RenderBind(BindingTarget)
{
    Total=TexturePositions.size();
    RenderHalf=(Total-1)*Width/2.0;
    for(unsigned i=0; i<Total; i++)
        Backgrounds.push_back(CreateRectangle(Vector2d(Width,Width),sf::Color::Black,Position+Vector2d(-RenderHalf+Width*i,TextSize)));

    BorderLines.resize(Total);


    Graphics.resize(TexturePositions.size());
    for (unsigned i=0; i<Graphics.size(); i++)
    {
        Graphics[i]=CreateTexturedBox(Vector2d(Position.x-RenderHalf+Width*i,Position.y+TextSize), Width,TexturePositions[i],RenderSize);
    }
}

void ToolbarButtonList::Update(Editor &editor){
    int Half=Width/2;
    Vector2i MouseLocation=sf::Mouse::getPosition(*Window);
    for(unsigned i=0; i<Total; i++)
    {
        vector<Vector2i> Region={Vector2i(Position.x-Half-RenderHalf+Width*i,Position.x+Half-RenderHalf+Width*i),
                                Vector2i(Position.y-Half+TextSize,Position.y+Width+Half+TextSize)};
        if (MouseLocation.x>Region[0].x && MouseLocation.x<Region[0].y
            &&  MouseLocation.y>Region[1].x && MouseLocation.y<Region[1].y)
            {
                RenderIndex=i;
                (editor.*Action)(RenderIndex);
            }
    }

}

void ToolbarButtonList::Render(){
    ToolbarItem::Render();
    if(RenderBind)
        RenderIndex=*RenderBind;

    for(unsigned i=0; i<Total; i++)
        Window->draw(Backgrounds[i]);

    for(unsigned i=0; i<Total; i++)
        Window->draw(Graphics[i],Texture);

    for(unsigned i=0; i<Total; i++)
    {
        if (RenderIndex!=i)
        {
            BorderLines[i]=CreateLineBox(Vector2d(Position.x-RenderHalf+Width*i,Position.y+TextSize),Width,sf::Color::Red);
            Window->draw(BorderLines[i]);
        }
    }
    BorderLines[RenderIndex]=CreateLineBox(Vector2d(Position.x-RenderHalf+Width*RenderIndex,Position.y+TextSize),Width,sf::Color::Yellow);
    Window->draw(BorderLines[RenderIndex]);

}

