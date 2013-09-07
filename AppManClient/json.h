#ifndef JSON_H
#define JSON_H

#include <QObject>
#include <QDebug>
#include <QVariantMap>
#include <QVariant>
#include <QScriptValue>
#include <QScriptEngine>

struct JSONData;


class JSON : public QObject
{
    Q_OBJECT

public:
    static JSON& instance();
    ~JSON();

    QVariantMap parse(const QString& string) const;
    QString serialize(const QVariant& value) const;

protected:
    JSON();

private:
    //QScriptValue CreateValue(const QVariant& value, QScriptEngine& engine);

private:
    JSONData* d;

};

struct JSONData
{
    QScriptEngine engine;
    QScriptValue parseFn;
    QScriptValue serializeFn;
};

#endif // JSON_H
