#ifndef ANIMATION_H
#define ANIMATION_H

#include "sprite.h"
#include <QObject>
#include <QString>
#include <SFML/Graphics.hpp>

class Game ;

class Animation : public Sprite
{
    Q_OBJECT
private:
    sf::Texture * frames ;
    int count ;
    float fps ;
    float tekt ;
    bool isplayed ;
    bool onceflag ;

public:
    explicit Animation(const QStringList & files, int fps, Game * game, QObject *parent = nullptr) ;
    explicit Animation(const QString & filename, int w, int h, int framecount, int fps, bool usetransp, Game * game, QObject *parent = nullptr) ;
    virtual void update(double dt) ;

public slots:
    virtual void setSmooth(bool smooth) override ;
    virtual void convertPixels(const QString &convertFunc) override ;

    void play() ;
    void playOneTime() ;
    void stop() ;
    bool isPlayed() ;
};

#endif // ANIMATION_H
