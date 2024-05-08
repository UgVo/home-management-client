#include "storage.h"

Storage::Storage(/* args */) {}

Storage::~Storage() {}

size_t Storage::setupCalendars(QString &&json_str) {
    auto json = QJsonDocument::fromJson(json_str.toUtf8());
    auto jsonArray = json.array();

    for (auto it = jsonArray.begin(); it < jsonArray.end(); it++) {
        auto obj = it->toObject();
        auto name = obj.value("name").toString();
        _calendars.insert(name, Agenda(std::move(obj)));
    }

    return _calendars.size();
}

size_t Storage::setupCalendarEvents(QString &&calendarName, QString &&json_str) {
    auto calendar = _calendars.value(calendarName);
    if (calendar.isValid()) {
        auto json = QJsonDocument::fromJson(json_str.toUtf8());
        return calendar.updateEvents(json.array());
    }
    return size_t();
}

QVector<Agenda> Storage::calendars() const { return _calendars.values(); }

QDebug operator<<(QDebug debug, const Storage &c) {
    QDebugStateSaver saver(debug);
    auto calendars = c.calendars();
    for (auto it = calendars.begin(); it != calendars.end(); ++it) {
        debug.nospace() << *it;
    }

    return debug;
}