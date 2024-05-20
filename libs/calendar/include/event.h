#ifndef HOME_MANAGEMENT_CLIENT_EVENT_H
#define HOME_MANAGEMENT_CLIENT_EVENT_H

#include <QCalendar>
#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTimeZone>
#include <QUuid>
#include <limits>

#include "rrules.h"
#include "utils.h"

class Event : public QObject {
    Q_OBJECT

   public:
    Event(QString content, QString rrule, QDateTime startTime, QDateTime _endTime,
          QDateTime lastModified     = QDateTime::currentDateTime(),
          QDateTime creationDatetime = QDateTime::currentDateTime(),
          QString   uid              = QUuid::createUuid().toString());
    Event(QJsonObject &&json);
    Event(const Event &other);
    ~Event();

    bool      isValid() const;
    QString   toString() const;
    QDateTime startTime() const;
    QDateTime endTime() const;
    QDateTime lastModified() const;
    QString   uid() const;
    QString   content() const;
    bool      isReccurent() const;
    bool      isFullDays() const;

    void updateContent(const QString newContent);
    void updateStartTime(const QDateTime newStart);
    void updateEndTime(const QDateTime newEndTime);
    void updateReccurenceRules(const RRules newRules);

    QSharedPointer<Event>          copyToDate(const QDateTime date) const;
    QVector<QSharedPointer<Event>> getReccurentInstances(QDate begin, QDate end) const;

    bool operator<(const Event &other) const;

   signals:
    void contentChanged(QString uid, QString content);
    void startTimeChanged(QString uid, QDateTime start);
    void endTimeChanged(QString uid, QDateTime end);
    void reccurentStateChanged(QString uid, bool reccurent);

   private:
    bool fitCriteriaMonth() const;

    QString   _content;
    QDateTime _creationDatetime;
    QDateTime _startTime;
    QDateTime _endTime;
    QDateTime _lastModified;
    QString   _uid;
    RRules    _rrule;
    bool      _valid;

    QString _mainEventUID;
};

QDebug operator<<(QDebug debug, const Event &c);

#endif  // HOME_MANAGEMENT_CLIENT_EVENT_H