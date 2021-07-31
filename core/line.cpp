#include "line.h"
#include "game.h"

Line::Line(Game * game, QObject *parent) : QObject(parent)
{
    this->game = game ;

    line.setPrimitiveType(sf::Lines) ;
    line.resize(2);
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
}

void Line::drawTo(int x1, int y1, int x2, int y2)
{
    line[0].position = sf::Vector2f(x1,y1);
    line[1].position = sf::Vector2f(x2,y2);
    game->RenderVertex(line) ;
}

void Line::setColor(int r, int g, int b)
{
    line[0].color = sf::Color(r,g,b) ;
    line[1].color = sf::Color(r,g,b) ;
}
