#ifndef POLL_H
#define POLL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>
#include <QSet>

class MessageProcessor;
class Database;
class NameDatabase;
class Subscribe;
class Permission;

class PollData
{
public:
    typedef QSet<qint64> Users;
    typedef QPair<QString, Users> Option;
    typedef QList<Option> Options;

private:
    QString titl;
    Options optns;
    bool multi;

public:
    PollData() {}
    PollData(const QString &tit, bool mc) : titl(tit), multi(mc) {}
    PollData(const QString &tit, bool mc, const Options &opts) : titl(tit), optns(opts), multi(mc) {}

    const QString &title() const {return titl;}
    QString &title() {return titl;}
    bool multiChoice() const {return multi;}
    Options &options() {return optns;}
};

class Poll : public QObject
{
    Q_OBJECT
private:
    static const int MaxPollsPerGroup;
    static const int MaxOptionsPerPoll;

    Database *database;
    NameDatabase *nameDatabase;
    MessageProcessor *messageProcessor;
    Permission *permission;
    Subscribe *subscribe;

    typedef QMap<qint64, PollData> Polls;
    typedef QMap<qint64, Polls> Groups;

    Groups data;

    void loadData();

    qint64 createPoll(qint64 gid, const QString &title, bool multi);
    void addOption(qint64 gid, qint64 id, const QString &opt);
    void delOption(qint64 gid, qint64 id, int idx);
    void terminatePoll(qint64 gid, qint64 id);
    void changeTitle(qint64 gid, qint64 pid, const QString &str);

public:
    explicit Poll(Database *db, NameDatabase *namedb, MessageProcessor *msgproc, Permission *perm,
                  Subscribe *sub, QObject *parent = 0);
    void input(const QString &gid, const QString &uid, const QString &str, bool inpm, bool isAdmin);
    void saveData();
    void groupDeleted(qint64 gid);

public slots:

};

#endif // POLL_H
