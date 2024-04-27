#ifndef HOME_MANAGEMENT_CLIENT_EVENT_H
#define HOME_MANAGEMENT_CLIENT_EVENT_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QTimeZone>
#include <QUuid>
#include <limits>

#include "rrules.h"

class Event {
   public:
    Event(QString content, QString rrule, QDateTime startTime, QDateTime _endTime,
          QDateTime lastModified = QDateTime::currentDateTime(),
          QDateTime creationDatetime = QDateTime::currentDateTime(),
          QString uid = QUuid::createUuid().toString());
    Event(QJsonObject &&json);
    ~Event();

    QString toString() const;
    QDateTime startTime() const;
    QDateTime lastModified() const;
    QString uid() const;
    QVector<Event> getReccurentInstances(QDate begin, QDate end);
    Event copyToDate(QDateTime date);

    bool operator<(const Event &other) const;

   private:
    QString _content;
    QDateTime _creationDatetime;
    QDateTime _startTime;
    QDateTime _endTime;
    QDateTime _lastModified;
    QString _uid;
    RRules _rrule;
};

QDebug operator<<(QDebug debug, const Event &c);

#endif  // HOME_MANAGEMENT_CLIENT_EVENT_H