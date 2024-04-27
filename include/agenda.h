#ifndef HOME_MANAGEMENT_CLIENT_CALENDAR_H
#define HOME_MANAGEMENT_CLIENT_CALENDAR_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QString>

#include "event.h"

class Agenda {
   public:
    Agenda();
    Agenda(QString &&name, QString &&url, size_t number_of_events = 0);
    Agenda(QJsonObject &&json);
    ~Agenda();

    bool isValid() const;
    QString toString() const;
    size_t updateEvents(QJsonArray &&json);

    // TODO take in account the reccurent events, they may not be taken in account if the main event
    // in not in the interval, more over they may account for more than one occurrence (should
    // probably make a second iteration over the data to get the recurrents events)
    QVector<QSharedPointer<Event>> getEvents(QDateTime start = QDateTime(),
                                             QDateTime end = QDateTime());

   private:
    bool _valid;
    QString _name;
    QString _url;
    size_t _number_of_events;
    QVector<QSharedPointer<Event>> events;
    QMap<QString, QSharedPointer<Event>> events_map;
};

QDebug operator<<(QDebug debug, const Agenda &c);

#endif  // HOME_MANAGEMENT_CLIENT_CALENDAR_H