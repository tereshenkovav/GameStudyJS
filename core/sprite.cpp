#include "sprite.h"
#include "game.h"

void Sprite::fillEdgePoints(sf::Vector2f * p1, sf::Vector2f * p2, sf::Vector2f * p3,
                            sf::Vector2f * p4, sf::Vector2f *pstart) const
{
    *p1 = m_sprite.getTransform().transformPoint(0,0) ;
    *p2 = m_sprite.getTransform().transformPoint(0,sz.y()) ;
    *p3 = m_sprite.getTransform().transformPoint(sz.x(),sz.y()) ;
    *p4 = m_sprite.getTransform().transformPoint(sz.x(),0) ;
    *pstart = *p1 ;
}

Sprite::Sprite(Game *game, QObject *parent) : QObject(parent)
{
    this->game = game ;

    mirrhorz=false ;
    mirrvert=false ;
    fscalex=1 ;
    fscaley=1 ;
    fx=0 ;
    fy=0 ;    
    contactmodel = ModelBox ;
    showborder = false ;

    line.setPrimitiveType(sf::LineStrip) ;
    line.resize(5);
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
    line[2].color = sf::Color::White;
    line[3].color = sf::Color::White;
    line[4].color = sf::Color::White;
}

void Sprite::loadFromFile(const QString &filename)
{
    sf::Texture tex ;
    tex.loadFromFile(("sprites/"+filename).toStdString()) ;
    loadFromTexture(tex) ;    
}

void Sprite::loadFromTexture(const sf::Texture &tex)
{
    m_tex = sf::Texture(tex) ;
    m_tex.setSmooth(true);

    m_sprite.setTexture(m_tex);

    sz.setX(m_tex.getSize().x) ;
    sz.setY(m_tex.getSize().y) ;

    m_sprite.setOrigin(sz.x() / 2, sz.y() / 2);
}

void Sprite::update(double dt)
{
    Q_UNUSED(dt)
    // Empty
}

void Sprite::renderTo(int x, int y)
{
    m_sprite.setPosition(x,y);
    float mx = mirrhorz?-fscalex:fscalex ;
    float my = mirrvert?-fscaley:fscaley ;

    m_sprite.setScale(mx,my);
    game->RenderDrawable(m_sprite) ;

    if (showborder) {
        fillEdgePoints(&line[0].position,&line[1].position,&line[2].position,
                &line[3].position,&line[4].position) ;
        game->RenderVertex(line) ;
    }
}

void Sprite::render()
{
    renderTo(fx,fy) ;
}

void Sprite::setXY(int x, int y)
{
    fx = x ;
    fy = y ;
}

void Sprite::mirrorHorz(bool mirror) {
    mirrhorz=mirror ;
}

void Sprite::mirrorVert(bool mirror) {
    mirrvert=mirror ;
}

void Sprite::setBordered(bool bordered)
{
    showborder = bordered ;
}

void Sprite::setScale(int perc) {
    fscalex=((float)perc)/100.0f ;
    fscaley=((float)perc)/100.0f ;
}

void Sprite::setScaleX(int perc)
{
    fscalex=((float)perc)/100.0f ;
}

void Sprite::setScaleY(int perc)
{
    fscaley=((float)perc)/100.0f ;
}

void Sprite::setAngle(int a) {
    m_sprite.setRotation(a);
}

void Sprite::setAlpha(int alpha) {
    m_sprite.setColor(sf::Color(255,255,255,alpha));
}

void Sprite::setHotSpot(int x, int y)
{
    m_sprite.setOrigin(x,y);
}

void Sprite::setSmooth(bool smooth)
{
    m_tex.setSmooth(smooth) ;
}

void Sprite::setContactModelBox()
{
    contactmodel = ModelBox;
}

void Sprite::setContactModelCircle()
{
    contactmodel = ModelCircle;
}

bool isInterSect(float x1,float y1,float x2,float y2,
               float x3,float y3,float x4,float y4)
{
    const float EPS = 0.001 ;
  float k = ((x4-x3)*(y2-y1)-(y4-y3)*(x2-x1)) ;
  if (abs(k)>0.001)
  {
    float tb=((x1-x3)*(y2-y1)+(x2-x1)*(y3-y1))/k ;

    if (abs(y2-y1)>EPS)
    {
        float ta=((y4-y3)*tb+(y3-y1))/(y2-y1) ;
        return (0<=ta)and(ta<=1)and(0<=tb)and(tb<=1);
    }
    else
        return (0<=tb)and(tb<=1);
  }
  else
    return false ;
}

bool isInTriangle(float x1,float y1,float x2,float y2,
                  float x3,float y3,float xt,float yt) {
    float v1 = (x1 - xt) * (y2 - y1) - (x2 - x1) * (y1 - yt) ;
    float v2 = (x2 - xt) * (y3 - y2) - (x3 - x2) * (y2 - yt) ;
    float v3 = (x3 - xt) * (y1 - y3) - (x1 - x3) * (y3 - yt) ;
    if ((v1<0)&&(v2<0)&&(v3<0)) return true ;
    if ((v1>0)&&(v2>0)&&(v3>0)) return true ;
    return false ;
}

bool Sprite::isContactWith(const QScriptValue &obj2) const
{
    if (!obj2.isQObject()) return false ;

    if (!obj2.toQObject()->metaObject()->inherits(Sprite::metaObject())) return false ;

    Sprite * sprite2 =  (Sprite*)obj2.toQObject() ;

    sf::Vector2f p[5] ;
    fillEdgePoints(p,p+1,p+2,p+3,p+4);
    sf::Vector2f pe[5] ;
    sprite2->fillEdgePoints(pe,pe+1,pe+2,pe+3,pe+4);

    if (contactmodel==ModelBox) {
        for (int i=0; i<4; i++) {
            if (isInTriangle(p[0].x,p[0].y,p[1].x,p[1].y,p[2].x,p[2].y,pe[i].x,pe[i].y))
                return true ;
            if (isInTriangle(p[2].x,p[2].y,p[3].x,p[3].y,p[0].x,p[0].y,pe[i].x,pe[i].y))
                return true ;
            }
        return false ;
    }
    else
    if (contactmodel==ModelCircle) {
        float dist = sz.x()+sz.y() + sprite2->sz.x()+sprite2->sz.y() ;
        float dx = 0 ; float dy = 0 ;
        for (int i=0; i<4; i++) {
            dx+=(p[i].x-pe[i].x) ;
            dy+=(p[i].y-pe[i].y) ;
        }
        return dx*dx+dy*dy<dist*dist ;
    }
    else
        return false ;
}

bool Sprite::isPointIn(int x, int y) const
{
    sf::Vector2f p[5] ;
    fillEdgePoints(p,p+1,p+2,p+3,p+4);

    if (isInTriangle(p[0].x,p[0].y,p[1].x,p[1].y,p[2].x,p[2].y,x,y))
        return true ;
    if (isInTriangle(p[2].x,p[2].y,p[3].x,p[3].y,p[0].x,p[0].y,x,y))
        return true ;
    return false ;
}

void Sprite::setTag(const QString &tagname, const QScriptValue &tagvalue)
{
    if (tags.contains(tagname)) tags.remove(tagname) ;
    tags.insert(tagname,tagvalue) ;
}

void Sprite::convertPixelsInTexture(sf::Texture * tex, const QString &convertFunc) {
    sf::Image img =  tex->copyToImage() ;
    for (int i=0; i<img.getSize().x; i++)
        for (int j=0; j<img.getSize().y; j++) {
            sf::Color c = img.getPixel(i,j) ;
            QScriptValue newc = game->engine.evaluate(
                        QString(convertFunc+"({r:%1,g:%2,b:%3,a:%4})").arg(c.r).arg(c.g).arg(c.b).arg(c.a)) ;
            img.setPixel(i,j,sf::Color(newc.property("r").toInt32(),
                                       newc.property("g").toInt32(),
                                       newc.property("b").toInt32(),
                                       newc.property("a").toInt32())) ;
        }
    tex->loadFromImage(img) ;
}

void Sprite::convertPixels(const QString &convertFunc)
{
    convertPixelsInTexture(&m_tex,convertFunc) ;
}

QScriptValue Sprite::getTag(const QString &tagname) const
{
    if (tags.contains(tagname)) return tags[tagname] ; else return QScriptValue() ;
}

int Sprite::getWidth() const
{
    return sz.x() ;
}

int Sprite::getHeight() const
{
    return sz.y() ;
}
