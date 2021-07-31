#include "extproc.h"
#include <QDebug>

ExtProc::ExtProc(sf::RenderWindow * renderwindow):QObject() {
    this->renderwindow = renderwindow ;
}

void ExtProc::getMessage(QString msg) {
    qDebug()<<msg ;
}

void ExtProc::getPair(QString name,QVariant value) {
    qDebug()<<name<<value ;
}

void ExtProc::setTitle(QString caption) {
    QByteArray bb = caption.toUtf8();
    renderwindow->setTitle(sf::String::fromUtf8(bb.constBegin(),bb.constEnd())) ;
}

void ExtProc::switchCursor(bool visible) {
    renderwindow->setMouseCursorVisible(visible) ;
}
