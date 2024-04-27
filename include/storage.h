#ifndef HOME_MANAGEMENT_CLIENT_STORAGE_H
#define HOME_MANAGEMENT_CLIENT_STORAGE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

#include "agenda.h"

class Storage {
   public:
    Storage(/* args */);
    ~Storage();

    size_t setupCalendars(QString &&json);
    size_t setupCalendarEvents(QString &&calendarName, QString &&json);

    QVector<Agenda> calendars() const;

   private:
    QMap<QString, Agenda> _calendars;
};

QDebug operator<<(QDebug debug, const Storage &c);

#endif  // HOME_MANAGEMENT_CLIENT_STORAGE_H