#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include "qgamesystem.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800,830);

    log = new QPlainTextEdit(this) ;
    log->setGeometry(0,662,600,140);
    log->setReadOnly(true);

    edpairs = new QPlainTextEdit(this) ;
    edpairs->setGeometry(600,662,200,140);
    edpairs->setReadOnly(true);

    // Пустая игра по умолчанию
    game = Game::createNoGame() ;

    wsfml = new QSFMLCanvas(this,game);
    wsfml->setGeometry(0,60,800,600);

    this->layout()->addWidget(wsfml);

    connect(wsfml,SIGNAL(sigPauseOrResume()),this,SLOT(on_ButStopStart_clicked())) ;
    connect(wsfml,SIGNAL(sigRun()),this,SLOT(on_ButRun_clicked())) ;

    timer.setInterval(1);
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void MainWindow::getMessage(QString msg) {
    log->appendPlainText(msg);
}

void MainWindow::getPair(QString name, QVariant value)
{
    pairs[name]=value ;
    edpairs->clear() ;
    foreach (QString name, pairs.keys())
        edpairs->appendPlainText(name+" : "+pairs[name].toString()) ;
}

void MainWindow::setTitle(QString title)
{
    this->setWindowTitle(title);
}

void MainWindow::switchCursor(bool visible)
{
    wsfml->iscursorvisible = visible ;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_F5) emit on_ButRun_clicked();
    if (event->key()==Qt::Key_Escape) emit on_ButStopStart_clicked();
}

MainWindow::~MainWindow()
{
    delete wsfml ;
    delete ui;
}

void MainWindow::on_ButStopStart_clicked()
{
    if (timer.isActive()) {
        ui->ButStopStart->setText("Продолжить (Esc)");
        Pause();
    }
    else {
        ui->ButStopStart->setText("Пауза (Esc)");
        Start();
        wsfml->setFocus();
    }
}

void MainWindow::on_ButRun_clicked()
{
    Stop() ;
    game = new Game("scripts/main.js") ;

    connect(game->sys,SIGNAL(writeMessage(QString)),this,SLOT(getMessage(QString))) ;
    connect(game->sys,SIGNAL(writePair(QString,QVariant)),this,SLOT(getPair(QString,QVariant))) ;
    connect(game,SIGNAL(sendLog(QString)),this,SLOT(getMessage(QString))) ;
    connect(game,SIGNAL(sendTitle(QString)),this,SLOT(setTitle(QString))) ;
    connect(game->sys,SIGNAL(showCursor(bool)),this,SLOT(switchCursor(bool))) ;

    log->clear() ;
    edpairs->clear() ;

    if (!game->Init(ui->lineParam->text().trimmed())) return ;

    wsfml->setGame(game) ;
    Start() ;
    wsfml->setFocus();

    ui->ButStopStart->setText("Пауза (Esc)");
}

void MainWindow::onTimeout()
{
    timer.stop();
    if (wsfml->iscursorvisible)
        wsfml->RenderWindow::setMouseCursorVisible(true) ;
    else
        wsfml->RenderWindow::setMouseCursorVisible(!underMouse()) ;
    wsfml->repaint() ;
    double teksec = clock.getElapsedTime().asSeconds() ;
    double dt = teksec-lastsec ;
    lastsec = teksec ;

    if (game->Frame(dt)) timer.start();

    if (game->isNewScript()) {
        QString script = game->getNewScript() ;
        QString args  = QGameSystem::ScriptValue2String(game->getNewScriptArgs()) ;
        game->UnInit() ;
        delete game ;                
        game = new Game("scripts/"+game->getNewScript()+".js") ;

        connect(game->sys,SIGNAL(writeMessage(QString)),this,SLOT(getMessage(QString))) ;
        connect(game->sys,SIGNAL(writePair(QString,QVariant)),this,SLOT(getPair(QString,QVariant))) ;
        connect(game,SIGNAL(sendLog(QString)),this,SLOT(getMessage(QString))) ;
        connect(game,SIGNAL(sendTitle(QString)),this,SLOT(setTitle(QString))) ;
        connect(game->sys,SIGNAL(showCursor(bool)),this,SLOT(switchCursor(bool))) ;

        wsfml->setGame(game) ;

        if (!game->Init(args)) timer.stop() ;
    }    
}

void MainWindow::Start()
{
    lastsec = clock.getElapsedTime().asSeconds() ;
    timer.start();
    game->soundsPauseOrResume() ;
}

void MainWindow::Pause()
{
    timer.stop();
    game->soundsPauseOrResume() ;
}

void MainWindow::Stop()
{
    timer.stop();
    game->UnInit() ;
}

