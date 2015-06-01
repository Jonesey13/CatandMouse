#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <math.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#define M_PI 3.14159265358979323846

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;


Vector2d RotateVector(Vector2d input, double rotation);
double DotProduct(Vector2d vector1, Vector2d vector2);
Vector2f Double2Float(Vector2d input);


bool InPolygon(vector<Vector2d> Polygon1, vector<Vector2d> Polygon2,
               bool &CornerPoly, Vector2u &CornerSide, Vector2d &Direction, double &Overlap, bool &DoubleFlag);
bool InPolygon(vector<Vector2d> Polygon1, vector<Vector2d> Polygon2);
bool InPolygon(Vector2d Point, vector<Vector2d> Polygon);
bool getTriangleHalf(Vector2d Position, bool Orientation);



#endif //FUNCTIONS_H
