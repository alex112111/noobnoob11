#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <QObject>
#include <QTextStream>
#include <QMap>

class QProcess;
class QTimer;
class Calculator;
class NameDatabase;
class Statistics;
class Database;
class Help;
class Sup;

class MessageProcessor : public QObject
{
    Q_OBJECT
private:
    static const qint64 keepAliveInterval;

    QTextStream output;
    QProcess *proc;

    QTimer *keepAliveTimer;

    Calculator *calculator;
    Database *database;
    NameDatabase *nameDatabase;
    Statistics *stats;
    Help *help;
    Sup *sup;

    qint64 endDayCron;
    qint64 hourlyCron;

public:
    explicit MessageProcessor(QObject *parent = 0);
    ~MessageProcessor();
    void sendCommand(const QByteArray &str);

    static qint64 processDate(const QString &str);

public slots:
    void readData();
    void keepAlive();
};

#endif // MESSAGEPROCESSOR_H