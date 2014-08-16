#include "header.h"



sf::Vector2f RotateVector(sf::Vector2f input, float rotation)
{
    sf::Vector2f output;
    output.x=input.x*cos(2*M_PI*rotation)-input.y*sin(2*M_PI*rotation);
    output.y=input.y*cos(2*M_PI*rotation)+input.x*sin(2*M_PI*rotation);
    return output;
}

float DotProduct(sf::Vector2f vector1, sf::Vector2f vector2)
{
    float product= vector1.x*vector2.x+vector1.y*vector2.y;
    return product;
}
