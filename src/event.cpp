#include "event.h"

Event::Event(QString content, QString rrule, QDateTime startTime, QDateTime endTime,
             QDateTime lastModified, QDateTime creationDatetime, QString uid)
    : _content(content),
      _rrule(RRules::fromString(rrule)),
      _startTime(startTime),
      _endTime(endTime),
      _lastModified(lastModified),
      _creationDatetime(creationDatetime),
      _uid(uid) {}

Event::Event(QJsonObject &&json) {
    _content = json.value("content").toString();
    _rrule = RRules::fromString(json.value("rrule").toString());
    _startTime = parseDateTime(json.value("start").toString());
    _endTime = parseDateTime(json.value("end").toString());
    _lastModified = parseDateTime(json.value("last_modified").toString());
    _creationDatetime = parseDateTime(json.value("created").toString());
    _uid = json.value("uid").toString();
}

Event::~Event() {}

QString Event::toString() const {
    return QString(
               "Event content: %1\n  Created: %2\n  Start: %3\n  End: %4\n  Last modified: %5\n  "
               "uid: %6\n  rrule: %7\n")
        .arg(_content, _creationDatetime.toString(), _startTime.toString(), _endTime.toString(),
             _lastModified.toString(), _uid, _rrule.toString());
}

QDateTime Event::startTime() const { return _startTime; }
QDateTime Event::lastModified() const { return _lastModified; }
QString Event::uid() const { return _uid; }

QVector<Event> Event::getReccurentInstances(QDate _begin, QDate _end) {
    QDateTime begin(_begin, QTime(0, 0), _startTime.timeZone());
    QDateTime end(_end, QTime(23, 59), _startTime.timeZone());
    QVector<Event> res;
    if (!_rrule.isValid()) {
        qDebug() << "RRule invalid";
        return res;
    }
    if (_startTime > end) {
        qDebug() << "start of event after end of time window";
        return res;
    }
    if (_rrule.until() < begin && _rrule.until().isValid()) {
        qDebug() << "end of reccurent events before start of time window";
        return res;
    }
    auto dateLimit = _rrule.until().isValid() ? std::min<QDateTime>(_rrule.until(), end) : end;
    auto count = _rrule.count() != 0 ? _rrule.count() - 1 : _startTime.daysTo(end) + 1;
    switch (_rrule.freq()) {
        case RRules::Freq::kDaily: {
            auto currentDate = _startTime;
            while (currentDate <= dateLimit && count > 0) {
                currentDate = currentDate.addDays(_rrule.interval());
                if (currentDate <= dateLimit && currentDate >= begin) {
                    res.append(copyToDate(currentDate));
                }
                count--;
            }
            break;
        }
        case RRules::Freq::kWeekly: {
            int dayOfWeek = _startTime.date().dayOfWeek();
            auto currentDate = _startTime.addDays(-(dayOfWeek - 1));
            auto byDay = _rrule.byDay();

            while (currentDate <= dateLimit && count > 0) {
                if (byDay.size()) {
                    for (auto it = byDay.begin(); it != byDay.end(); ++it) {
                        auto dateToAdd =
                            currentDate.addDays(_rrule.interval() * 7 + RRules::mapDayNum[*it]);
                        if (dateToAdd <= dateLimit && dateToAdd >= begin) {
                            res.append(copyToDate(dateToAdd));
                        }
                        count--;
                    }
                } else {
                    auto dateToAdd = currentDate.addDays(_rrule.interval() * 7 + dayOfWeek - 1);
                    if (dateToAdd <= dateLimit && dateToAdd >= begin) {
                        res.append(copyToDate(dateToAdd));
                    }
                    count--;
                }
                currentDate = currentDate.addDays(_rrule.interval() * 7);
            }
            break;
        }
        case RRules::Freq::kMonthly: {
            int dayOfMonth = _startTime.date().day();
            auto currentDate = _startTime.addDays(-dayOfMonth + 1);
            auto byDay = _rrule.byDay();
            auto byMonthDay = _rrule.byMonthDay();
            auto rankInMonth = _rrule.dayRankInMonth();
            while (currentDate <= dateLimit && count > 0) {
                if (byDay.size()) {
                } else if (byMonthDay.size()) {
                } else {
                    auto dateToAdd = currentDate.addDays(_rrule.interval() * 7 + dayOfMonth - 1);
                    if (currentDate <= dateLimit && currentDate >= begin) {
                        res.append(copyToDate(currentDate));
                    }
                    count--;
                }
                currentDate = currentDate.addMonths(_rrule.interval());
            }
            break;
        }
        case RRules::Freq::kYearly: {
            auto currentDate = _startTime;
            while (currentDate <= dateLimit && count > 0) {
                currentDate = currentDate.addYears(_rrule.interval());
                if (currentDate <= dateLimit && currentDate >= begin) {
                    res.append(copyToDate(currentDate));
                }
                count--;
            }
            break;
        }

        default:
            break;
    }
    return res;
}

Event Event::copyToDate(QDateTime date) {
    Event res = *this;
    res._startTime = date;
    return res;
}

bool Event::operator<(const Event &other) const { return _startTime < other._startTime; }

QDebug operator<<(QDebug debug, const Event &c) {
    QDebugStateSaver saver(debug);
    debug.nospace().noquote() << c.toString();

    return debug;
}