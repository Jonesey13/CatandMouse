#include "header.h"



Vector2d RotateVector(Vector2d input, double rotation)
{
    Vector2d output;
    output.x=input.x*cos(2*M_PI*rotation)-input.y*sin(2*M_PI*rotation);
    output.y=input.y*cos(2*M_PI*rotation)+input.x*sin(2*M_PI*rotation);
    return output;
}

double DotProduct(Vector2d vector1, Vector2d vector2)
{
    double product= vector1.x*vector2.x+vector1.y*vector2.y;
    return product;
}

Vector2f Double2Float(Vector2d input)
{
    return Vector2f(input.x, input.y);
}
