#include "agenda.h"

Agenda::Agenda() : _valid(false) {}

Agenda::Agenda(QString &&name, QString &&url, size_t number_of_events)
    : _name(name), _url(url), _number_of_events(number_of_events), _valid(true) {}

Agenda::~Agenda() {}

Agenda::Agenda(QJsonObject &&json) : _valid(true) {
    _name = json.value("name").toString();
    _url = json.value("url").toString();
    _number_of_events = json.value("nb_of_events").toInt();
}

bool Agenda::isValid() const { return _valid; }

QString Agenda::toString() const {
    return QString("Calendar name: %1\n  Url: %2\n  Number of events: %3\n")
        .arg(_name, _url)
        .arg(_number_of_events);
}

size_t Agenda::updateEvents(QJsonArray &&json) {
    events.clear();
    events_map.clear();
    for (auto it = json.begin(); it != json.end(); ++it) {
        auto obj = it->toObject();
        events.emplace_back(QSharedPointer<Event>::create(std::move(obj)));
    }
    std::sort(events.begin(), events.end(),
              [](QSharedPointer<Event> a, QSharedPointer<Event> b) { return *a < *b; });
    for (auto it = events.begin(); it != events.end(); ++it) {
        events_map.insert((*it)->uid(), *it);
    }
    return events.size();
}

QVector<QSharedPointer<Event>> Agenda::getEvents(QDateTime start, QDateTime end) {
    if (start.isValid() && end.isValid() && end < start) {
        return QVector<QSharedPointer<Event>>();
    }
    QVector<QSharedPointer<Event>> res;
    if (!start.isValid() && !end.isValid()) {
        res = QVector<QSharedPointer<Event>>(events.begin(), events.end());
    } else {
        for (auto it = events.begin(); it != events.end(); ++it) {
            if ((*it)->startTime() >= start && (*it)->startTime() <= end) {
                res.push_back(*it);
            }
        }
    }
    // TODO add the recurrent events
    return res;
}

QDebug operator<<(QDebug debug, const Agenda &c) {
    QDebugStateSaver saver(debug);
    debug.nospace().noquote() << c.toString();

    return debug;
}