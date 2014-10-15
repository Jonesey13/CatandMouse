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
