#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include <QString>
#include <SFML/Graphics.hpp>

class Game ;

class Text : public QObject
{
    Q_OBJECT
private:
   sf::Font m_font ;
   sf::Text m_text ;
   Game * game ;
   bool iscentered ;

public:
    explicit Text(QString fontname, Game * game, QObject *parent = 0);

signals:

public slots:
   void printTo(int x, int y) ;
   void setAlignCenter() ;
   void setText(QString str) ;
   void setSize(int size) ;
   void setColor(int r, int g, int b) ;

};

#endif // TEXT_H
