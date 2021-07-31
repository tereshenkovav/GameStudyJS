#include "game.h"
#include <QFile>
#include "animation.h"
#include "pcxtexloader.h"
#include <QDebug>

const char * ScriptConsts =
#ifdef KEYCODE_FROM_SFML
        "const KEY_ESCAPE    = 36;"
        "const KEY_TAB       = 60;"
        "const KEY_ENTER     = 58;"
        "const KEY_LEFT      = 71;"
        "const KEY_UP        = 73;"
        "const KEY_RIGHT     = 72;"
        "const KEY_DOWN      = 74;"
        "const KEY_SHIFT     = 38;"
        "const KEY_CONTROL   = 37;"
        "const KEY_ALT       = 39;"
        "const KEY_F1        = 85;"
        "const KEY_F2        = 86;"
        "const KEY_F3        = 87;"
        "const KEY_F4        = 88;"
        "const KEY_F5        = 89;"
        "const KEY_F6        = 90;"
        "const KEY_F7        = 91;"
        "const KEY_F8        = 92;"
        "const KEY_F9        = 93;"
        "const KEY_F10       = 94;"
        "const KEY_F11       = 95;"
        "const KEY_F12       = 96;"
        "const KEY_SPACE     = 57;"
        "const KEY_PLUS      = 67;"
        "const KEY_MINUS     = 68;"
        "const KEY_0         = 26;"
        "const KEY_1         = 27;"
        "const KEY_2         = 28;"
        "const KEY_3         = 29;"
        "const KEY_4         = 30;"
        "const KEY_5         = 31;"
        "const KEY_6         = 32;"
        "const KEY_7         = 33;"
        "const KEY_8         = 34;"
        "const KEY_9         = 35;"
        "const KEY_A         = 0;"
        "const KEY_B         = 1;"
        "const KEY_C         = 2;"
        "const KEY_D         = 3;"
        "const KEY_E         = 4;"
        "const KEY_F         = 5;"
        "const KEY_G         = 6;"
        "const KEY_H         = 7;"
        "const KEY_I         = 8;"
        "const KEY_J         = 9;"
        "const KEY_K         = 10;"
        "const KEY_L         = 11;"
        "const KEY_M         = 12;"
        "const KEY_N         = 13;"
        "const KEY_O         = 14;"
        "const KEY_P         = 15;"
        "const KEY_Q         = 16;"
        "const KEY_R         = 17;"
        "const KEY_S         = 18;"
        "const KEY_T         = 19;"
        "const KEY_U         = 20;"
        "const KEY_V         = 21;"
        "const KEY_W         = 22;"
        "const KEY_X         = 23;"
        "const KEY_Y         = 24;"
        "const KEY_Z         = 25;";
#else
        "const KEY_ESCAPE=0x01000000; "
        "const KEY_TAB=0x01000001; "
        "const KEY_ENTER=0x01000005; "
        "const KEY_LEFT=0x01000012; "
        "const KEY_UP=0x01000013; "
        "const KEY_RIGHT=0x01000014; "
        "const KEY_DOWN=0x01000015; "
        "const KEY_SHIFT=0x01000020; "
        "const KEY_CONTROL=0x01000021; "
        "const KEY_ALT=0x01000023; "
        "const KEY_F1=0x01000030; "
        "const KEY_F2=0x01000031; "
        "const KEY_F3=0x01000032; "
        "const KEY_F4=0x01000033; "
        "const KEY_F5=0x01000034; "
        "const KEY_F6=0x01000035; "
        "const KEY_F7=0x01000036; "
        "const KEY_F8=0x01000037; "
        "const KEY_F9=0x01000038; "
        "const KEY_F10=0x01000039; "
        "const KEY_F11=0x0100003a; "
        "const KEY_F12=0x0100003b; "
        "const KEY_SPACE=0x20; "
        "const KEY_PLUS=0x2b; "
        "const KEY_MINUS=0x2d; "
        "const KEY_0=0x30; "
        "const KEY_1=0x31; "
        "const KEY_2=0x32; "
        "const KEY_3=0x33; "
        "const KEY_4=0x34; "
        "const KEY_5=0x35; "
        "const KEY_6=0x36; "
        "const KEY_7=0x37; "
        "const KEY_8=0x38; "
        "const KEY_9=0x39; "
        "const KEY_A=0x41; "
        "const KEY_B=0x42; "
        "const KEY_C=0x43; "
        "const KEY_D=0x44; "
        "const KEY_E=0x45; "
        "const KEY_F=0x46; "
        "const KEY_G=0x47; "
        "const KEY_H=0x48; "
        "const KEY_I=0x49; "
        "const KEY_J=0x4a; "
        "const KEY_K=0x4b; "
        "const KEY_L=0x4c; "
        "const KEY_M=0x4d; "
        "const KEY_N=0x4e; "
        "const KEY_O=0x4f; "
        "const KEY_P=0x50; "
        "const KEY_Q=0x51; "
        "const KEY_R=0x52; "
        "const KEY_S=0x53; "
        "const KEY_T=0x54; "
        "const KEY_U=0x55; "
        "const KEY_V=0x56; "
        "const KEY_W=0x57; "
        "const KEY_X=0x58; "
        "const KEY_Y=0x59; "
        "const KEY_Z=0x5a; " ;
#endif

const char * EmptyScript =
        "function Init(params) { return true ; } "
        "function Render() { return true ; } "
        "function Frame(dt) { return true ; } " ;

bool Game::safeExecBoolScript(QString command)
{
    QScriptValue v = engine.evaluate(command) ;
    if (v.isError()) {
        emit sendLog("ОШИБКА функции "+command+": "+v.toString());
        return false ;
    }

    if (v.isBool()) {
        if (!v.toBool()) emit sendLog("Игра остановлена");
        return v.toBool() ;
    }

    return true ;
}

Game::Game(QString scriptfile, QObject *parent) : QObject(parent)
{
    QFile file(scriptfile);
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        QString str = file.readAll() ;

        QRegExp rx("\\$include<(.*)>") ;
        QStringList lines = str.split("\n") ;
        for (int i=0; i<lines.count(); i++)
            if (rx.indexIn(lines[i])!=-1) {
                QFile incfile("scripts/"+rx.capturedTexts()[1]);
                if ((incfile.exists())&&(incfile.open(QIODevice::ReadOnly)))
                {
                    lines[i] = incfile.readAll() ;
                    incfile.close();
                }
            }
        str=lines.join("\n") ;
        str=ScriptConsts+str ;
        engine.evaluate(str);
        file.close();
    }

    mousepos = engine.newObject() ;
    mousepos.setProperty("x",0);
    mousepos.setProperty("y",0);
    mbleftpressed = false ;
    mbrightpressed = false ;
    bgcolor = sf::Color(0, 0, 0) ;
    resetTotalTime();
    timefps = 0 ;
    lastfps = 0 ;
    calcfps = 0 ;
    newscript = "" ;

    sys = new QGameSystem(&engine) ;
    addObjectToEngine("game",this);
    addObjectToEngine("system",sys);

}

Game::Game(QObject *parent)
{
    sys = new QGameSystem(&engine) ;
    addObjectToEngine("game",this);
    addObjectToEngine("system",sys);
    Q_UNUSED(parent) ;
}

void Game::addObjectToEngine(QString varname, QObject * obj)
{
    engine.globalObject().setProperty(varname,engine.newQObject(obj));
}

void Game::RenderDrawable(sf::Drawable & drawable)
{
    if (render==nullptr) return ;
    render->draw(drawable);
}

void Game::RenderVertex(sf::VertexArray & vertex)
{
    if (render==nullptr) return ;
    render->draw(vertex);
}

void Game::setKey(int key)
{
    if (!keys.contains(key)) keys.insert(key) ;
}

void Game::setMousePos(int x, int y)
{
    mousepos.setProperty("x",x);
    mousepos.setProperty("y",y);
}

void Game::setLeftButtonClicked()
{
    mbleftpressed = true ;
}

void Game::setRightButtonClicked()
{
    mbrightpressed = true ;
}

bool Game::Render(sf::RenderTarget *render)
{
    this->render = render ;
    render->clear(bgcolor);
    return safeExecBoolScript("Render()") ;
}

bool Game::Frame(double dt)
{
    for (auto spr: sprites)
        spr->update(dt);

    totaltime+=dt ;
    timefps+=dt ;
    calcfps++ ;
    if (timefps>=1.0) {
        lastfps=calcfps ;
        calcfps=0 ;
        timefps=0 ;
    }
    bool r = safeExecBoolScript("Frame("+QString::number(dt)+")") ;
    keys.clear();
    return r ;
}

bool Game::Init(const QString & args)
{
    return safeExecBoolScript("Init("+args+")") ;
}

bool Game::UnInit()
{
    sys->clearTimers() ;
    for (int i=0; i<sounds.count(); i++)
        delete sounds[i] ;
    for (int i=0; i<texts.count(); i++)
        delete texts[i] ;
    for (int i=0; i<lines.count(); i++)
        delete lines[i] ;
    for (int i=0; i<rects.count(); i++)
        delete rects[i] ;
    for (int i=0; i<sprites.count(); i++)
        delete sprites[i] ;
    return true ;
}

bool Game::isNewScript() const
{
    return newscript.length()>0 ;
}

QString Game::getNewScript() const
{
    return newscript ;
}

QScriptValue Game::getNewScriptArgs() const
{
    return newscriptargs ;
}

void Game::soundsPauseOrResume()
{
    for (int i=0; i<sounds.count(); i++)
        sounds[i]->pauseOrResume() ;
}

Game *Game::createNoGame()
{
    Game * res = new Game() ;
    res->engine.evaluate(EmptyScript);
    return res ;
}

QScriptValue Game::loadSprite(QString filename)
{
    Sprite * spr = new Sprite(this);
    spr->loadFromFile(filename) ;
    sprites.append(spr);

    return engine.newQObject(spr) ;
}

QScriptValue Game::loadSpritePCX8bit(QString filename, bool usetransp)
{
    Sprite * spr = new Sprite(this);

    sf::Texture tex ;
    PcxTexLoader loader ;
    if (loader.loadFile("sprites/"+filename,usetransp)) {
        tex.create(loader.getWidth(),loader.getHeight()) ;
        tex.update(loader.getData()) ;
    }
    spr->loadFromTexture(tex) ;
    sprites.append(spr);

    return engine.newQObject(spr) ;
}

QScriptValue Game::loadSound(QString filename)
{
    Sound * snd = new Sound(filename,this);
    sounds.append(snd);

    return engine.newQObject(snd) ;

}

QScriptValue Game::loadText(QString fontname, QString text, int size)
{
    Text * stext = new Text(fontname,this);
    stext->setText(text);
    stext->setSize(size);
    texts.append(stext);

    return engine.newQObject(stext) ;
}

QScriptValue Game::loadAnimationFromFiles(QScriptValue arr, int fps)
{
    if (arr.isArray()) {
        QStringList list ;
        for (int i=0; i<arr.property("length").toInt32(); i++)
           list.append(arr.property(i).toString()) ;

        Sprite * ani = new Animation(list,fps,this);
        sprites.append(ani);

        return engine.newQObject(ani) ;
    }
    else
        return "none" ;
}

QScriptValue Game::loadAnimation(QString filename, int w, int h, int framecount, int fps)
{
    Sprite * ani = new Animation(filename,w,h,framecount,fps,true,this);
    sprites.append(ani);

    return engine.newQObject(ani) ;
}

QScriptValue Game::loadAnimation(QString filename, int framecount, int fps)
{
    Sprite * ani = new Animation(filename,-1,-1,framecount,fps,true,this);
    sprites.append(ani);

    return engine.newQObject(ani) ;
}

QScriptValue Game::loadAnimationPCX8bit(QString filename, int w, int h, int framecount, int fps, int usetransp)
{
    Sprite * ani = new Animation(filename,w,h,framecount,fps,usetransp,this);
    sprites.append(ani);

    return engine.newQObject(ani) ;
}

QScriptValue Game::loadAnimationPCX8bit(QString filename, int framecount, int fps, int usetransp)
{
    Sprite * ani = new Animation(filename,-1,-1,framecount,fps,usetransp,this);
    sprites.append(ani);

    return engine.newQObject(ani) ;
}

QScriptValue Game::createLine(int r, int g, int b)
{
    Line * sline = new Line(this);
    sline->setColor(r,g,b);
    lines.append(sline);
    return engine.newQObject(sline) ;
}

QScriptValue Game::createRect(int r, int g, int b)
{
    Rect * srect = new Rect(this);
    srect->setFillColor(r,g,b);
    srect->setBorderColor(r,g,b);
    rects.append(srect);
    return engine.newQObject(srect) ;
}

bool Game::isKeyDown(int keycode)
{
    return keys.contains(keycode) ;
}

bool Game::isOneOfKeysDown(QScriptValue arr)
{
    if (arr.isArray())
        for (int i=0; i<arr.property("length").toInt32(); i++)
           if (keys.contains(arr.property(i).toInt32())) return true ;
    return false ;
}

QScriptValue Game::getMousePos() const
{
    return mousepos ;
}

bool Game::isLeftButtonClicked()
{
    bool r = mbleftpressed ;
    mbleftpressed = false ;
    return r ;
}

bool Game::isRightButtonClicked()
{
    bool r = mbrightpressed ;
    mbrightpressed = false ;
    return r ;
}

void Game::setGameTitle(QString title)
{
    emit sendTitle(title) ;
}

int Game::getFPS() const
{
    return lastfps ;
}

double Game::getTotalTime() const
{
    return totaltime ;
}

void Game::resetTotalTime()
{
    totaltime=0.0f ;
}

void Game::setBackgroundColor(int r, int g, int b)
{
    bgcolor = sf::Color(r, g, b) ;
}

void Game::goToScript(QString scriptname, QScriptValue params)
{
    newscript = scriptname ;
    newscriptargs = params ;
}
