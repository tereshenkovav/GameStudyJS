#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <SFML/Audio.hpp>

class Game ;

class Sound : public QObject
{
    Q_OBJECT
private:
   Game * game ;
   sf::SoundBuffer buf ;
   sf::Sound snd ;
public:
   explicit Sound(QString filename, Game * game, QObject *parent = 0);
   void pauseOrResume() ;

signals:

public slots:
   void play() ;
   void stop() ;
   void setLoop(bool isloop) ;
   bool isPlayed() const ;
};

#endif // SOUND_H
