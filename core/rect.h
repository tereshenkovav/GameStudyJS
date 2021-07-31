#ifndef RECT_H
#define RECT_H

#include <QObject>
#include <SFML/Graphics.hpp>

class Game ;

class Rect : public QObject
{
    Q_OBJECT
private:
   sf::RectangleShape rect;
   Game * game ;

public:
    explicit Rect(Game * game, QObject *parent = 0);

signals:

public slots:
   void drawTo(int x, int y, int w, int h) ;
   void setFillColor(int r, int g, int b) ;
   void setBorderColor(int r, int g, int b) ;
   void setLineWidth(int w) ;
} ;

#endif // RECT_H
