#ifndef QGAMESYSTEM_H
#define QGAMESYSTEM_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QTimer>

class TimerEvent: public QObject
{
    Q_OBJECT
public:
    explicit TimerEvent(QScriptEngine * engine, QString code, bool isonce);

private:
    QString code ;
    QScriptEngine * engine ;
    bool isonce ;

public slots:
    void execCode() ;
};

class QGameSystem : public QObject
{
    Q_OBJECT 
public:
    explicit QGameSystem(QScriptEngine * engine);

    void clearTimers() ;

    static QString ScriptValue2String(const QScriptValue & val) ;

private:
    QScriptEngine * engine ;
    QList<QTimer*> timers ;

signals:
    void writeMessage(QString msg) ;
    void writePair(QString name, QVariant value) ;
    void showCursor(bool showed) ;

public slots:
    void saveObject(QString filename, QScriptValue obj) ;
    QScriptValue loadObject(QString filename) ;
    void saveObjectToAppData(QString filename, QScriptValue obj) ;
    QScriptValue loadObjectFromAppData(QString filename) ;
    void setTimeout(QString code, int ms) ;
    void setInterval(QString code, int ms) ;
};

#endif // QGAMESYSTEM_H
