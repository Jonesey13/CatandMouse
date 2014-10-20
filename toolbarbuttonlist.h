#ifndef TOOLBARBUTTONLIST_H
#define TOOLBARBUTTONLIST_H



class ToolbarButtonList: public ToolbarItem{
private:
    vector<sf::VertexArray> Graphics;
    vector<sf::RectangleShape> Backgrounds;
    vector<sf::VertexArray> BorderLines;
public:
    typedef void (Editor::*EditorVoidptr)(unsigned &Index);
    ~ToolbarButtonList()=default;
    ToolbarButtonList()=default;
    ToolbarButtonList(vector<string> NewTitle,Vector2d NewPosition,unsigned NewWidth,EditorVoidptr NewAction, sf::Texture& NewTexture
                  , vector<Vector2u> NewTexturePositions, unsigned StartingIndex=0, unsigned *BindingTarget=nullptr);
    void Update(Editor &editor) override;
    void Render() override;

    EditorVoidptr Action;
    sf::Texture* Texture;
    vector<Vector2u> TexturePositions;
    unsigned Total;
    double RenderHalf;
    unsigned RenderIndex=0;
    unsigned RenderSize=32;
    unsigned *RenderBind=nullptr;

};



#endif //TOOLBARBUTTONLIST_H


