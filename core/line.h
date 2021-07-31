#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <SFML/Graphics.hpp>

class Game ;

class Line : public QObject
{
    Q_OBJECT
private:
   sf::VertexArray line;
   Game * game ;

public:
    explicit Line(Game * game, QObject *parent = 0);

signals:

public slots:
   void drawTo(int x1, int y1, int x2, int y2) ;
   void setColor(int r, int g, int b) ;
} ;

#endif // LINE_H
