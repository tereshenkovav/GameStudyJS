#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QString>
#include <SFML/Graphics.hpp>
#include <QScriptValue>
#include <QPoint>
#include <QMap>

class Game ;

class Sprite : public QObject
{
    Q_OBJECT

protected:
    Game * game ;

    sf::Texture m_tex ;
    sf::Sprite   m_sprite;
    sf::VertexArray line;

    bool mirrhorz ;
    bool mirrvert ;
    bool showborder ;
    float fscalex ;
    float fscaley ;
    int fx ;
    int fy ;

    QMap<QString,QScriptValue> tags ;

    enum ContactModel { ModelBox, ModelCircle } ;
    ContactModel contactmodel ;

    QPoint sz ;
    void fillEdgePoints(sf::Vector2f * p1,sf::Vector2f * p2,sf::Vector2f * p3,sf::Vector2f * p4, sf::Vector2f * pstart) const ;

    void convertPixelsInTexture(sf::Texture * tex, const QString &convertFunc) ;
public:
    explicit Sprite(Game * game, QObject *parent = nullptr);
    void loadFromFile(const QString & filename) ;
    void loadFromTexture(const sf::Texture &tex) ;

    virtual void update(double dt) ;    
signals:

public slots:
    void renderTo(int x, int y) ;
    void render() ;
    void setXY(int x, int y) ;
    void mirrorHorz(bool mirror) ;
    void mirrorVert(bool mirror) ;
    void setBordered(bool bordered) ;
    void setScale(int perc) ;
    void setScaleX(int perc) ;
    void setScaleY(int perc) ;
    void setAngle(int a) ;
    void setAlpha(int alpha) ;
    void setHotSpot(int x,int y) ;
    virtual void setSmooth(bool smooth) ;
    void setContactModelBox() ;
    void setContactModelCircle() ;
    void setTag(const QString & tagname, const QScriptValue & tagvalue) ;
    virtual void convertPixels(const QString &convertFunc) ;

    bool isContactWith(const QScriptValue & obj2) const ;
    bool isPointIn(int x, int y) const ;
    QScriptValue getTag(const QString & tagname) const ;
    int getWidth() const ;
    int getHeight() const ;
};

#endif // SPRITE_H
