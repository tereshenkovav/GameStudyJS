#include "qgamesystem.h"
#include <QScriptValueIterator>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>

QJsonValue script2json(const QScriptValue & val) {
    return QJsonValue::fromVariant(val.toVariant()) ;
}

QScriptValue json2script(QScriptEngine *engine, const QJsonValue & val) {
    return engine->newVariant(val.toVariant()) ;
}

QGameSystem::QGameSystem(QScriptEngine *engine)
{
    this->engine = engine ;    
}

void QGameSystem::clearTimers()
{
    foreach(auto timer, timers)
        delete timer ;
    timers.clear() ;
}

QString QGameSystem::ScriptValue2String(const QScriptValue &obj)
{
    if (obj.isObject()||obj.isArray()) {
        QJsonDocument doc ;
        if (obj.isObject()) {
            QJsonObject json ;
            QScriptValueIterator it(obj);
            while(it.hasNext()) {
                it.next();
                json.insert(it.name(),script2json(it.value())) ;
            }
            doc.setObject(json);
        }
        if (obj.isArray()) {
            QJsonDocument doc ;
            QJsonArray json ;
            for (int i=0; i<obj.property("length").toInt32(); i++)
                json.append(script2json(obj.property(i)));
            doc.setArray(json);
        }
        return doc.toJson() ;
    }
    if (obj.isString()) return "\""+obj.toString()+"\"" ;
    if (obj.isNumber()) return QString::number(obj.toNumber()) ;
    if (obj.isBool()) return obj.toBool()?"true":"false" ;
    if (obj.isNull()) return "null" ;
    return "" ;
}

void QGameSystem::saveObjectToAppData(QString filename, QScriptValue obj)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) ;
    QDir dir(path) ;
    if (!dir.exists()) dir.mkpath(path) ;
    saveObject(path+"/"+filename,obj) ;
}

void QGameSystem::saveObject(QString filename, QScriptValue obj)
{
    QFile file(filename) ;
    file.open(QIODevice::WriteOnly);
    file.write(ScriptValue2String(obj).toUtf8()) ;
    file.close() ;
}

QScriptValue QGameSystem::loadObjectFromAppData(QString filename)
{
    return loadObject(QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/"+filename) ;
}

QScriptValue QGameSystem::loadObject(QString filename)
{
    QFile file(filename);
    if (file.exists()&&file.open(QIODevice::ReadOnly)) {
        QString cmd = "var v = "+file.readAll()+"; v ;" ;
        QScriptValue v = engine->evaluate(cmd) ;
        //qDebug()<<v.toString() ;
        return v ;

        /*
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll()) ;
        file.close() ;

        QScriptValue val = engine->newObject();

        if (doc.isObject()) {
            foreach (QString key, doc.object().keys())
                val.setProperty(key,json2script(engine,doc.object().value(key)));
        }
        if (doc.isArray()) {
            for (int i=0; i<doc.array().count(); i++)
                val.setProperty(i,json2script(engine,doc.array().at(i))) ;
            val.setProperty("length",doc.array().count());
        }
        return val ;
        */

    }
    else
        return QScriptValue::NullValue ;
}

void QGameSystem::setTimeout(QString code, int ms)
{
    //QTimer::singleShot(ms,new TimerEvent(engine,code,true),SLOT(execCode())) ; old release
    QTimer * timer = new QTimer(this) ;
    timer->setSingleShot(true) ;
    connect(timer,SIGNAL(timeout()),new TimerEvent(engine,code,true),SLOT(execCode())) ;
    timer->start(ms) ;
    timers.append(timer) ;
}

void QGameSystem::setInterval(QString code, int ms)
{
    QTimer * timer = new QTimer(this) ;
    timer->setSingleShot(false) ;
    connect(timer,SIGNAL(timeout()),new TimerEvent(engine,code,false),SLOT(execCode())) ;
    timer->start(ms) ;
    timers.append(timer) ;
}

TimerEvent::TimerEvent(QScriptEngine *engine, QString code, bool isonce)
{
    this->engine = engine ;
    this->code = code ;
    this->isonce = isonce ;
}

void TimerEvent::execCode()
{
    engine->evaluate(code) ;
    if (isonce) deleteLater() ;
}
