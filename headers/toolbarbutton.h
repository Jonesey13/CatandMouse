#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H





class ToolbarButton: public ToolbarItem{
private:
    vector<sf::VertexArray> Graphics;
    sf::RectangleShape Background;
    sf::VertexArray BorderLines;
public:
    typedef void (Editor::*EditorVoidptr)(unsigned &Index);
    ~ToolbarButton()=default;
    ToolbarButton()=default;
    ToolbarButton(vector<string> NewTitle,Vector2d NewPosition,unsigned NewWidth,EditorVoidptr NewAction, sf::Texture& NewTexture
                  , vector<Vector2u> NewTexturePositions, unsigned StartingIndex=0);
    void Update(Editor &editor) override;
    void Render() override;

    EditorVoidptr Action;
    sf::Texture* Texture=nullptr;
    vector<Vector2u> TexturePositions;
    unsigned RenderIndex=0;
    unsigned RenderSize=32;

};



#endif //TOOLBARBUTTON_H

