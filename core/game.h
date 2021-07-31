#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QSet>
#include <QtScript/QScriptEngine>
#include <SFML/Graphics.hpp>
#include "sprite.h"
#include "text.h"
#include "line.h"
#include "rect.h"
#include "sound.h"
#include <QPoint>
#include "qgamesystem.h"

class Game : public QObject
{
    Q_OBJECT

private:

    QList<Sprite*> sprites ;
    QList<Text*> texts ;
    QList<Line*> lines ;
    QList<Rect*> rects ;
    QList<Sound*> sounds ;
    sf::RenderTarget * render ;
    QSet<int> keys ;
    QScriptValue mousepos ;
    bool mbleftpressed ;
    bool mbrightpressed ;
    double totaltime ;
    int lastfps ;
    int calcfps ;
    double timefps ;
    sf::Color bgcolor ;
    QString newscript ;
    QScriptValue newscriptargs ;

    bool safeExecBoolScript(QString command) ;
public:
    explicit Game(QString scriptfile, QObject *parent = nullptr);    
    explicit Game(QObject *parent = nullptr);

    QScriptEngine engine ;
    QGameSystem * sys ;

    void addObjectToEngine(QString varname, QObject *obj) ;

    void RenderDrawable(sf::Drawable & drawable) ;
    void RenderVertex(sf::VertexArray & vertex) ;
    void setKey(int key) ;
    void setMousePos(int x, int y) ;
    void setLeftButtonClicked() ;
    void setRightButtonClicked() ;

    bool Render(sf::RenderTarget * render) ;
    bool Frame(double dt) ;
    bool Init(const QString &args) ;
    bool UnInit() ;

    bool isNewScript() const ;
    QString getNewScript() const ;
    QScriptValue getNewScriptArgs() const ;

    void soundsPauseOrResume() ;

    static Game * createNoGame() ;

signals:
    void sendLog(QString msg) ;
    void sendTitle(QString title) ;

public slots:
    QScriptValue loadSprite(QString filename) ;    
    QScriptValue loadSpritePCX8bit(QString filename, bool usetransp=false) ;
    QScriptValue loadSound(QString filename) ;
    QScriptValue loadText(QString fontname, QString text, int size) ;
    QScriptValue loadAnimationFromFiles(QScriptValue arr, int fps) ;
    QScriptValue loadAnimation(QString filename, int w, int h, int framecount, int fps) ;
    QScriptValue loadAnimation(QString filename, int framecount, int fps) ;
    QScriptValue loadAnimationPCX8bit(QString filename, int w, int h, int framecount, int fps, int usetransp=false) ;
    QScriptValue loadAnimationPCX8bit(QString filename, int framecount, int fps, int usetransp=false) ;
    QScriptValue createLine(int r, int g, int b) ;
    QScriptValue createRect(int r, int g, int b) ;
    bool isKeyDown(int keycode) ;
    bool isOneOfKeysDown(QScriptValue arr) ;
    QScriptValue getMousePos() const ;
    bool isLeftButtonClicked() ;
    bool isRightButtonClicked() ;
    void setGameTitle(QString title) ;
    double getTotalTime() const ;
    int getFPS() const ;
    void resetTotalTime() ;
    void setBackgroundColor(int r, int g, int b) ;
    void goToScript(QString scriptname, QScriptValue params) ;
};

#endif // GAME_H
