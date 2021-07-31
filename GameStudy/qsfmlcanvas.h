#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <SFML/Graphics.hpp>
#include "game.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit QSFMLCanvas(QWidget* parent = nullptr, Game * game = nullptr);
    virtual ~QSFMLCanvas() {}

    void setGame(Game * game) ;
    void doRepaint() ;

    bool iscursorvisible ;

private :
    virtual QPaintEngine* paintEngine() const override;
    virtual void showEvent(QShowEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;
    virtual void keyPressEvent(QKeyEvent *event) override ;
    virtual void mouseMoveEvent(QMouseEvent *event) override ;
    virtual void mousePressEvent(QMouseEvent * event) override ;

private:
    bool m_initialized;
    Game * game ;

signals:
    void sigPauseOrResume() ;
    void sigRun() ;
};

#endif // QSFMLCANVAS_H
