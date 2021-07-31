#include "animation.h"
#include <QDebug>
#include "pcxtexloader.h"

Animation::Animation(const QStringList & files, int fps, Game *game, QObject *parent):
    Sprite(game,parent)
{
    // Здесь обязательно передать нулевой файл в Sprite - он там используется
    loadFromFile(files[0]) ;

    count = files.count() ;
    this->fps = fps ;
    tekt = 0 ;
    onceflag = false ;

    frames = new sf::Texture[count] ;
    for (int i=0; i<count; i++) {
        frames[i].loadFromFile(("sprites/"+files[i]).toStdString()) ;
        frames[i].setSmooth(true);
    }

    isplayed = true ;
    update(0) ;
    isplayed = false ;
}

Animation::Animation(const QString & filename, int w, int h, int framecount, int fps, bool usetransp, Game * game, QObject *parent) :
    Sprite(game,parent)
{
    this->count = framecount ;

    this->fps = fps ;
    tekt = 0 ;
    onceflag = false ;

    frames = new sf::Texture[count] ;
    sf::Image source ;
    if (filename.endsWith(".pcx")) {
        PcxTexLoader loader ;
        if (loader.loadFile("sprites/"+filename,usetransp))
            source.create(loader.getWidth(),loader.getHeight(),loader.getData()) ;
    }
    else
        source.loadFromFile(("sprites/"+filename).toStdString()) ;    

    sf::IntRect rect ;
    if ((w<0)||(h<0)) {
        w = source.getSize().x/count ;
        h = source.getSize().y ;
    }
    rect.left = 0 ;
    rect.top = 0 ;
    rect.width = w ;
    rect.height = h ;

    for (int i=0; i<count; i++) {
        frames[i].loadFromImage(source,rect) ;
        frames[i].setSmooth(true);
        rect.left+=w ;
        if (rect.left>=(int)source.getSize().x) {
            rect.left=0 ;
            rect.top+=h ;
        }
    }

    // Здесь обязательно передать нулевой файл в Sprite - он там используется
    loadFromTexture(frames[0]) ;

    isplayed = true ;
    update(0) ;
    isplayed = false ;
}

void Animation::update(double dt)
{
    if (!isplayed) return ;

    float sec_per_frame = 1.0f/fps ;

    tekt+=dt ;
    int tekframe = (int)(tekt/sec_per_frame) ;
    if (tekframe>=count) {
        if (onceflag) {
            tekframe = count-1 ;
            isplayed = false ;
        }
        else {
            tekframe = 0 ;
            tekt-=count*sec_per_frame ;
        }
    }
    m_sprite.setTexture(frames[tekframe]) ;
}

void Animation::play()
{
    isplayed = true ;
    onceflag = false ;
}

void Animation::stop()
{
    isplayed = false ;
}

bool Animation::isPlayed()
{
    return isplayed ;
}

void Animation::playOneTime()
{
    isplayed = true ;
    onceflag = true ;
    tekt=0 ;
    update(0) ;
}

void Animation::setSmooth(bool smooth)
{
    for (int i=0; i<count; i++)
        frames[i].setSmooth(smooth);
}

void Animation::convertPixels(const QString &convertFunc)
{
    for (int i=0; i<count; i++)
        convertPixelsInTexture(frames+i,convertFunc) ;
}
