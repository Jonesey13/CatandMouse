#ifndef CLICKABLE_H
#define CLICKABLE_H


template <typename T>
class Clickable
{
public:
    Clickable(T *NewTarget=nullptr, vector<Vector2i> TexturePositions=vector<Vector2i>(0,Vector2i(0,0)), double NewWidth=0
                , Vector2f NewPosition=Vector2f(0,0),sf::Font *FontPointer=nullptr, vector<string> Message=vector<string>{""}, float TextSize=0,
                float TextureSize=0, unsigned NewOffset=0);
    vector<sf::VertexArray> Graphics;
    vector<Vector2f> Region={Vector2f(),Vector2f()};
    void IncrementTarget();
    void CheckAndUpdate(Vector2i MouseLocation);
    void Render(sf::RenderWindow *Window, sf::Texture *RenderTexture);
    double Width;
    T *Target;
    unsigned Offset;
    vector<sf::Text> Explanation;

};

template <typename T>
Clickable<T>::Clickable(T *NewTarget, vector<Vector2i> TexturePositions, double NewWidth
                     , Vector2f Position,sf::Font *FontPointer, vector<string> Message, float TextSize, float TextureSize, unsigned NewOffset)
{
    Width=NewWidth;
    Offset=NewOffset;
    Explanation.resize(Message.size());
    for(unsigned i=0; i< Explanation.size(); i++)
    {
        Explanation[i].setFont(*FontPointer);
        Explanation[i].setString(Message[i]);
        Explanation[i].setCharacterSize(TextSize);
        Explanation[i].setColor(sf::Color::Red);
        sf::FloatRect TextRect = Explanation[i].getLocalBounds();
        Explanation[i].setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top  + TextRect.height/2.0f);
        Explanation[i].setPosition(Vector2f(Position.x+Width/2.0f,Position.y-(Explanation.size()-i)*TextSize));
    }



    vector<sf::VertexArray> NewGraphics(TexturePositions.size());
    Graphics=NewGraphics;
    Region[0]=Vector2f(Position.x,Position.x+Width);
    Region[1]=Vector2f(Position.y,Position.y+Width);

    for (unsigned i=0; i<Graphics.size(); i++)
    {
        sf::VertexArray BasicQuad;
        BasicQuad.resize(4);
        BasicQuad.setPrimitiveType(sf::Quads);
        vector<sf::Vertex> quad(4);
        quad[0].position=Position+Vector2f(0, 0);
        quad[1].position=Position+Vector2f(0, Width);
        quad[2].position=Position+Vector2f(Width,Width);
        quad[3].position=Position+Vector2f(Width,0);

        Vector2i TextPos=TexturePositions[i];
        quad[0].texCoords=TextureSize*Vector2f(TextPos.x, TextPos.y);
        quad[1].texCoords=TextureSize*Vector2f(TextPos.x, TextPos.y+1);
        quad[2].texCoords=TextureSize*Vector2f(TextPos.x+1, TextPos.y+1);
        quad[3].texCoords=TextureSize*Vector2f(TextPos.x+1, TextPos.y);
        for (unsigned j=0; j<4;++j)
        {
            BasicQuad[j]=quad[j];
        }
        Graphics[i]=BasicQuad;
    }
    Target=NewTarget;
}


template <typename T>
void Clickable<T>::IncrementTarget(){
    unsigned largest=Graphics.size();
    if (*Target-Offset<largest-1)
    {
        *Target=*Target+1;
    }
    else{
        *Target=Offset;
    }
}

template <typename T>
void Clickable<T>::CheckAndUpdate(Vector2i MouseLocation){
    if (MouseLocation.x>Region[0].x && MouseLocation.x<Region[0].y
        &&  MouseLocation.y>Region[1].x && MouseLocation.y<Region[1].y)
            IncrementTarget();
}

template <typename T>
void Clickable<T>::Render(sf::RenderWindow *Window, sf::Texture *RenderTexture){
    sf::VertexArray RenderTarget=Graphics[*Target-Offset];
    Window->draw(RenderTarget,RenderTexture);
    for (unsigned i=0; i<Explanation.size();i++)
    {
        Window->draw(Explanation[i]);
    }
}









#endif // CLICKABLE_H
