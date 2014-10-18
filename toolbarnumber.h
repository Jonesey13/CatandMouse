#ifndef TOOLBARNUMBER_H
#define TOOLBARNUMBER_H

class Editor;


class ToolbarNumber: public ToolbarItem{
private:
    sf::VertexArray LeftButtonBox;
    sf::VertexArray Arrows;
    sf::VertexArray RightButtonBox;
    sf::Text NumberText;
public:
    ~ToolbarNumber()=default;
    ToolbarNumber()=default;
    ToolbarNumber( vector<string> NewTitle,Vector2d NewPosition,
                  unsigned NewWidth , unsigned &NewTarget, unsigned NewMin, unsigned NewMax);
    void Update(Editor &editor);
    void Render();
    unsigned *Target;
    unsigned Min;
    unsigned Max;
};

#endif //TOOLBARNUMBER_H

