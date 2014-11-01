#ifndef EDITORCLICKABLE_H
#define EDITORCLICKABLE_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace std;
using Vector2d=sf::Vector2<double>;
using Vector2u=sf::Vector2u;
using Vector2i=sf::Vector2i;
using Vector2f=sf::Vector2f;

class Editor;


class EditorClickable{
public:
    typedef void (Editor::*voidptr)();
    voidptr ActionFunction;

    vector<sf::VertexArray> Graphics;
    vector<Vector2f> Region={Vector2f(),Vector2f()};
    double Width;
    string Explanation;
};

#endif // EDITORCLICKABLE_H
