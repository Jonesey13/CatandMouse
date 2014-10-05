#ifndef CLICKABLE_H
#define CLICKABLE_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;


template <typename T>
class Clickable
{
public:
    Clickable(T *NewTarget=nullptr, vector<Vector2i> TexturePositions=vector<Vector2i>(0,Vector2i(0,0)), double NewWidth=0
                , Vector2f NewPosition=Vector2f(0,0), vector<string> Message=vector<string>{""}, float TextSize=0,
                 unsigned NewOffset=0);
    static sf::Font font;
    static bool FontLoaded;
    vector<sf::VertexArray> Graphics;
    vector<Vector2f> Region={Vector2f(),Vector2f()};
    void IncrementTarget();
    void CheckAndUpdate(Vector2i MouseLocation);
    void Render(sf::RenderWindow *Window, sf::Texture *RenderTexture);
    double Width;
    T *Target;
    unsigned Offset;
    unsigned RenderSize=32;
    vector<sf::Text> Explanation;

};


template <typename T>
sf::Font Clickable<T>::font=sf::Font();

template <typename T>
bool Clickable<T>::FontLoaded=0;

template <typename T>
Clickable<T>::Clickable(T *NewTarget, vector<Vector2i> TexturePositions, double NewWidth
                     , Vector2f Position, vector<string> Message, float TextSize, unsigned NewOffset)
{
    Width=NewWidth;
    Offset=NewOffset;
    if (!FontLoaded)
    {
        font.loadFromFile("Aller.ttf");
        FontLoaded=1;
    }

    Explanation.resize(Message.size());
    for(unsigned i=0; i< Explanation.size(); i++)
    {
        Explanation[i].setFont(font);
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
        float RenderSizef=RenderSize;
        vector<sf::Vertex> quad(4);
        quad[0].position=Position+Vector2f(0, 0);
        quad[1].position=Position+Vector2f(0, Width);
        quad[2].position=Position+Vector2f(Width,Width);
        quad[3].position=Position+Vector2f(Width,0);

        Vector2i TextPos=TexturePositions[i];
        quad[0].texCoords=RenderSizef*Vector2f(TextPos.x, TextPos.y);
        quad[1].texCoords=RenderSizef*Vector2f(TextPos.x, TextPos.y+1);
        quad[2].texCoords=RenderSizef*Vector2f(TextPos.x+1, TextPos.y+1);
        quad[3].texCoords=RenderSizef*Vector2f(TextPos.x+1, TextPos.y);
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
