#include "qsfmlcanvas.h"

#include <string>
#include <QDebug>
#include <SFML/Audio.hpp>

QSFMLCanvas::QSFMLCanvas(QWidget *parent, Game * game) :
    QWidget(parent),    
    iscursorvisible(true),
    m_initialized(false)
{
    this->game = game ;

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setVerticalSyncEnabled(true) ;
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

void QSFMLCanvas::setGame(Game *game)
{
    this->game = game ;
}

void QSFMLCanvas::doRepaint()
{
    repaint() ;
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return nullptr;
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!m_initialized)
    {
        RenderWindow::create(sf::WindowHandle(winId()),sf::ContextSettings(24));

        m_initialized = true;
    }
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{    
        /* Render to file/sprite example
        sf::RenderTexture texture ;
        texture.create(800, 600) ;
        texture.clear(sf::Color(0,0,0));
        game->Render(&texture) ;
        texture.display();
        texture.getTexture().copyToImage().saveToFile(
                QString("P:\\video\\frame%1.png").arg(framen, 8, 'g', -1, '0').toStdString()) ;
        */

    game->Render(this) ;
    display();
}

void QSFMLCanvas::keyPressEvent(QKeyEvent *event)
{
    game->setKey(event->key()) ;
    if (event->key()==Qt::Key_Escape) {
        RenderWindow::setMouseCursorVisible(true) ;
        emit sigPauseOrResume();
    }
    if (event->key()==Qt::Key_F5) {
        emit sigRun();
    }
}

void QSFMLCanvas::mouseMoveEvent(QMouseEvent *event)
{
    game->setMousePos(event->x(),event->y());
}

void QSFMLCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) game->setLeftButtonClicked();
    if (event->button()==Qt::RightButton) game->setRightButtonClicked();
}
