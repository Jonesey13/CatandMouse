#include "renderfunctions.h"

sf::Text CreateText(string Message, sf::Font *Font ,double CharacterSize, sf::Color Color, Vector2d Position){

    sf::Text Text;
    Text.setFont(*Font);
    Text.setString(Message);
    Text.setCharacterSize(CharacterSize);
    Text.setColor(Color);
    sf::FloatRect Rect = Text.getLocalBounds();
    Text.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Text.setPosition(sf::Vector2f(Position.x,Position.y));
    return Text;
}


sf::RectangleShape CreateRectangle(Vector2d RectangleSize,sf::Color Color, Vector2d Position){
    sf::RectangleShape Rectangle(Vector2f(RectangleSize.x,RectangleSize.y));
    Rectangle.setFillColor(Color);
    sf::FloatRect Rect = Rectangle.getLocalBounds();
    Rectangle.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Rectangle.setPosition(sf::Vector2f(Position.x,Position.y));
    return Rectangle;

}

sf::CircleShape CreateCircle(double Radius,sf::Color Color, Vector2d Position){
    sf::CircleShape Circle(Radius);
    Circle.setFillColor(Color);
    sf::FloatRect Rect = Circle.getLocalBounds();
    Circle.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Circle.setPosition(sf::Vector2f(Position.x,Position.y));
    return Circle;

}

sf::VertexArray CreateLineBox(Vector2d Position, double Width, sf::Color Color)
{
    sf::VertexArray Lines;
    Lines.resize(8);
    Lines.setPrimitiveType(sf::Lines);
    for (unsigned i=0; i<8;i++)
    {
        Lines[i].color=Color;
    }
    Vector2f Positionf=Vector2f(Position.x,Position.y);
    float Half=Width/2.0;
    Lines[0].position=Positionf+Vector2f(-Half, -Half);
    Lines[1].position=Positionf+Vector2f(-Half, Half);
    Lines[2].position=Positionf+Vector2f(-Half, Half);
    Lines[3].position=Positionf+Vector2f(Half,Half);
    Lines[4].position=Positionf+Vector2f(Half,Half);
    Lines[5].position=Positionf+Vector2f(Half,-Half);
    Lines[6].position=Positionf+Vector2f(Half,-Half);
    Lines[7].position=Positionf+Vector2f(-Half,-Half);

    return Lines;
}

sf::VertexArray CreateTexturedBox(Vector2d Position,double Width, Vector2u TexturePosition, float RenderSize)
{
    Vector2f Positionf=Vector2f(Position.x,Position.y);
    float Half=Width/2.0;
    sf::VertexArray BasicQuad;
    BasicQuad.resize(4);
    BasicQuad.setPrimitiveType(sf::Quads);
    float RenderSizef=RenderSize;
    vector<sf::Vertex> quad(4);
    quad[0].position=Positionf+Vector2f(-Half, -Half);
    quad[1].position=Positionf+Vector2f(-Half, Half);
    quad[2].position=Positionf+Vector2f(Half,Half);
    quad[3].position=Positionf+Vector2f(Half,-Half);

    Vector2u TextPos=TexturePosition;
    quad[0].texCoords=RenderSizef*Vector2f(TextPos.x, TextPos.y);
    quad[1].texCoords=RenderSizef*Vector2f(TextPos.x, TextPos.y+1);
    quad[2].texCoords=RenderSizef*Vector2f(TextPos.x+1, TextPos.y+1);
    quad[3].texCoords=RenderSizef*Vector2f(TextPos.x+1, TextPos.y);
    for (unsigned j=0; j<4;++j)
    {
        BasicQuad[j]=quad[j];
    }
    return BasicQuad;
}


sf::Text & AdjustText(sf::Text& Text,string Message, sf::Font *Font ,double CharacterSize, sf::Color Color, Vector2d Position){
    Text.setFont(*Font);
    Text.setString(Message);
    Text.setCharacterSize(CharacterSize);
    Text.setColor(Color);
    sf::FloatRect Rect = Text.getLocalBounds();
    Text.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Text.setPosition(sf::Vector2f(Position.x,Position.y));
    return Text;
}

sf::Text & RepositionText(sf::Text& Text, Vector2d Position){
    sf::FloatRect Rect = Text.getLocalBounds();
    Text.setOrigin(Rect.left + Rect.width/2.0f,Rect.top  + Rect.height/2.0f);
    Text.setPosition(sf::Vector2f(Position.x,Position.y));
    return Text;
}

Vector2d ScalePosition(Vector2d position, double Scaling, bool paddingdim, unsigned padding){
    position=Scaling*position;
    if (paddingdim==0)
    {
        position =position+Vector2d(padding,0);
    }
    else{
        position =position+Vector2d(0,padding);
    }
    return position;
}

Vector2f ScalePosition(Vector2f position, double Scaling, bool paddingdim, unsigned padding){
    position=static_cast<float>(Scaling)*position;
    if (paddingdim==0)
    {
        position =position+Vector2f(padding,0);
    }
    else{
        position =position+Vector2f(0,padding);
    }
    return position;
}


