#include "toolbaritem.h"


ToolbarButton::ToolbarButton(vector<string> NewTitle,Vector2d NewPosition,unsigned NewWidth,EditorVoidptr NewAction,
                             sf::Texture& NewTexture, vector<Vector2u> NewTexturePositions, unsigned StartingIndex)
            : ToolbarItem(NewTitle,NewPosition,NewWidth),  Action(NewAction), Texture(&NewTexture),
            TexturePositions(NewTexturePositions), RenderIndex(StartingIndex)
{
    Background=CreateRectangle(Vector2d(Width,Width),sf::Color::Black,Position+Vector2d(0,TextSize));

    BorderLines=CreateLineBox(Vector2d(Position.x,Position.y+TextSize),Width,sf::Color::Red);

    Graphics.resize(TexturePositions.size());
    for (unsigned i=0; i<Graphics.size(); i++)
    {
        Graphics[i]=CreateTexturedBox(Vector2d(Position.x,Position.y+TextSize), Width,TexturePositions[i],RenderSize);
    }
}

void ToolbarButton::Update(Editor &editor){
    int Half=Width/2;
    Vector2i MouseLocation=sf::Mouse::getPosition(*Window);
    vector<Vector2i> Region={Vector2i(Position.x-Half,Position.x+Half),Vector2i(Position.y-Half+TextSize,Position.y+Width+Half+TextSize)};
    if (MouseLocation.x>Region[0].x && MouseLocation.x<Region[0].y
        &&  MouseLocation.y>Region[1].x && MouseLocation.y<Region[1].y)
        (editor.*Action)(RenderIndex);
}

void ToolbarButton::Render(){
    ToolbarItem::Render();
    sf::VertexArray RenderTarget=Graphics[RenderIndex];
    Window->draw(Background);
    Window->draw(RenderTarget,Texture);
    Window->draw(BorderLines);
}
