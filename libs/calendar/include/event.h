#ifndef HOME_MANAGEMENT_CLIENT_EVENT_H
#define HOME_MANAGEMENT_CLIENT_EVENT_H

#include <QCalendar>
#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QTimeZone>
#include <QUuid>
#include <limits>

#include "rrules.h"
#include "utils.h"

class Event {
   public:
    Event(QString content, QString rrule, QDateTime startTime, QDateTime _endTime,
          QDateTime lastModified     = QDateTime::currentDateTime(),
          QDateTime creationDatetime = QDateTime::currentDateTime(),
          QString   uid              = QUuid::createUuid().toString());
    Event(QJsonObject &&json);
    ~Event();

    QString   toString() const;
    QDateTime startTime() const;
    QDateTime endTime() const;
    QDateTime lastModified() const;
    QString   uid() const;
    QString   content() const;
    bool      isReccurent() const;

    QSharedPointer<Event>          copyToDate(QDateTime date) const;
    QVector<QSharedPointer<Event>> getReccurentInstances(QDate begin, QDate end) const;

    bool operator<(const Event &other) const;

   private:
    bool fitCriteriaMonth() const;

    QString   _content;
    QDateTime _creationDatetime;
    QDateTime _startTime;
    QDateTime _endTime;
    QDateTime _lastModified;
    QString   _uid;
    RRules    _rrule;
};

QDebug operator<<(QDebug debug, const Event &c);

#endif  // HOME_MANAGEMENT_CLIENT_EVENT_H