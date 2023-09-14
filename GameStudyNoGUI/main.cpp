#include <QCoreApplication>
#include <QObject>
#include <QDebug>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <QtScript/QScriptEngine>
#include <QDir>
#include "extproc.h"
#include "game.h"
#include "qgamesystem.h"

Game * createGame(const QString & scriptname, ExtProc & extproc) {
    Game * game = new Game("scripts/"+scriptname+".js") ;

    QObject::connect(game->sys,SIGNAL(writeMessage(QString)),&extproc,SLOT(getMessage(QString))) ;
    QObject::connect(game->sys,SIGNAL(writePair(QString,QVariant)),&extproc,SLOT(getPair(QString,QVariant))) ;
    QObject::connect(game,SIGNAL(sendLog(QString)),&extproc,SLOT(getMessage(QString))) ;
    QObject::connect(game,SIGNAL(sendTitle(QString)),&extproc,SLOT(setTitle(QString))) ;
    QObject::connect(game->sys,SIGNAL(showCursor(bool)),&extproc,SLOT(switchCursor(bool))) ;

    return game ;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc>1) QDir::setCurrent(argv[1]) ;

    sf::RenderWindow window(sf::VideoMode(800, 600),"GameStudyJS") ;
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    ExtProc extproc(&window) ;

    Game * game = createGame("main",extproc);
    QScriptValue arrarg = game->engine.newObject() ;
    for (int i=0; i<argc; i++)
        arrarg.setProperty(i,game->engine.newVariant(argv[i])) ;
    arrarg.setProperty("length",argc);
    if (!game->Init(QGameSystem::ScriptValue2String(arrarg))) return 1 ;

    sf::Clock clock ;
    float lasttime = clock.getElapsedTime().asSeconds() ;

    while (window.isOpen())
    {
        // Это для работы сигналов
        a.processEvents() ;

        float newtime = clock.getElapsedTime().asSeconds() ;
        float dt = newtime-lasttime ;
        lasttime = newtime ;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close() ;
                break ;
            }            
            if (event.type == sf::Event::KeyPressed)
                game->setKey(event.key.code) ;
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button==sf::Mouse::Button::Left) game->setLeftButtonClicked() ;
                if (event.mouseButton.button==sf::Mouse::Button::Right) game->setRightButtonClicked() ;
            }
        }

        game->setMousePos(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) ;

	if (window.hasFocus()) {
        if (!game->Frame(dt)) {
            window.close() ;
            break ;
        }

        if (game->isNewScript()) {
            QString script = game->getNewScript() ;
            QString args  = QGameSystem::ScriptValue2String(game->getNewScriptArgs()) ;
            game->UnInit() ;
            delete game ;
            game = createGame(game->getNewScript(),extproc) ;
            if (!game->Init(args)) return 1 ;
            // Убираем слишком большую дельту, вызванную инициализацией новой игры
            lasttime = clock.getElapsedTime().asSeconds() ;
        }
        }

        window.clear();
        game->Render(&window) ;
        window.display();
    }

    delete game ;

    a.quit() ;
    return EXIT_SUCCESS ;
}
