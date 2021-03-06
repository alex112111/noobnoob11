#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QMap>
#include <QSet>

class UserStatsData
{
private:
    qint64 cnt;
    qint64 len;
    qint64 onl;

public:
    UserStatsData() : cnt(0), len(0), onl(0) {}
    UserStatsData(qint64 _cnt, qint64 _len, qint64 _onl)
        : cnt(_cnt), len(_len), onl(_onl) {}

    const qint64 &count() const {return cnt;}
    const qint64 &length() const {return len;}
    const qint64 &online() const {return onl;}
    qint64 &count() {return cnt;}
    qint64 &length() {return len;}
    qint64 &online() {return onl;}
};

class Database;
class NameDatabase;
class MessageProcessor;
class Permission;
class QTimer;

class Statistics : public QObject
{
    Q_OBJECT
private:
    typedef QMap<qint64, UserStatsData> Users;
    typedef QMap<qint64, Users> Groups;
    typedef QMap<qint64, Groups> Dates;

    typedef QPair<UserStatsData, qint64> DataPair;

    typedef QPair<qint64, QByteArray> DateAndSendee;
    typedef QPair<qint64, DateAndSendee> QueueData;

    QMap<qint64, qint64> onlineTime;

    static const int GraphDelay;

    Dates data;

    QList<DataPair> tempList;
    QList<QueueData> activityQueue;

    Database *database;
    NameDatabase *nameDatabase;
    MessageProcessor *messageProcessor;
    Permission *permission;

    QTimer *activityQueueTimer;

    void loadData();

    static bool compareByCount(const DataPair &a, const DataPair &b)
    {
        return a.first.count() > b.first.count();
    }

    static bool compareByLength(const DataPair &a, const DataPair &b)
    {
        return a.first.length() > b.first.length();
    }

    static bool compareByDensity(const DataPair &a, const DataPair &b)
    {
        return (static_cast<qreal>(a.first.length()) / a.first.count()) >
               (static_cast<qreal>(b.first.length()) / b.first.count());
    }

    static bool compareByOnline(const DataPair &a, const DataPair &b)
    {
        return a.first.online() > b.first.online();
    }

    void finalizeOnlineTimes();

public:
    explicit Statistics(Database *db, NameDatabase *namedb, MessageProcessor *msgproc, Permission *perm,
                        QObject *parent = 0);
    void rawInput(const QString &str);
    void input1(const QString &gid, const QString &uid, const QString &str);
    void input2(const QString &gid, const QString &uid, const QString &str, bool inpm, bool isAdmin);
    void giveStat(qint64 gid, const QString &date, const QString &factor, const QString &limit = QString(),
                  qint64 uid = -1, bool inpm = false, bool isAdmin = true);
    void cleanUpBefore(qint64 date);
    void saveData();
    void groupDeleted(qint64 gid);

public slots:
    void processGraph();
    void processActivityQueue();
};

#endif // STATISTICS_H
