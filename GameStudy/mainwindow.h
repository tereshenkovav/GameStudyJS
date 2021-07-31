#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qsfmlcanvas.h"
#include <QGridLayout>
#include <QPlainTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ButStopStart_clicked();
    void on_ButRun_clicked();
    void onTimeout();

public slots:
    void getMessage(QString msg) ;
    void getPair(QString name, QVariant value) ;
    void setTitle(QString title) ;
    void switchCursor(bool visible) ;

private:
    QMap<QString,QVariant> pairs ;
    Ui::MainWindow *ui;
    QSFMLCanvas * wsfml ;
    QPlainTextEdit * log ;
    QPlainTextEdit * edpairs ;
    QTimer timer;
    sf::Clock clock ;
    double lastsec ;
    Game * game  ;

    void Start() ;
    void Pause() ;
    void Stop() ;

    virtual void keyPressEvent(QKeyEvent *event) override ;
};

#endif // MAINWINDOW_H
