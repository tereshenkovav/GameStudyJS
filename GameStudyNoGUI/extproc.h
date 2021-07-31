#ifndef EXTPROC_H
#define EXTPROC_H

#include <QObject>
#include <QVariant>
#include <SFML/Graphics.hpp>

class ExtProc: public QObject
{
    Q_OBJECT
public:
    ExtProc(sf::RenderWindow * renderwindow) ;
private:
    sf::RenderWindow * renderwindow ;

public slots:
    void getMessage(QString msg) ;
    void getPair(QString name,QVariant value) ;
    void setTitle(QString caption) ;
    void switchCursor(bool visible) ;

signals:
};


#endif // EXTPROC_H
