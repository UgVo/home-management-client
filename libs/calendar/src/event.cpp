#include "event.h"

Event::Event(QString content, QString rrule, QDateTime startTime, QDateTime endTime,
             QDateTime lastModified, QDateTime creationDatetime, QString uid) {
    if (startTime > endTime) {
        _valid = false;
        return;
    }
    _content          = content;
    _rrule            = RRules::fromString(rrule);
    _startTime        = startTime;
    _endTime          = endTime;
    _lastModified     = lastModified;
    _creationDatetime = creationDatetime;
    _uid              = uid;
    _valid            = true;
}

Event::Event(QJsonObject &&json) {
    auto startTime = parseDateTime(json.value("start").toString());
    auto endTime   = parseDateTime(json.value("end").toString());
    if (startTime > endTime) {
        _valid = false;
        return;
    }

    _content          = json.value("content").toString();
    _rrule            = RRules::fromString(json.value("rrule").toString());
    _startTime        = startTime;
    _endTime          = endTime;
    _lastModified     = parseDateTime(json.value("last_modified").toString());
    _creationDatetime = parseDateTime(json.value("created").toString());
    _uid              = json.value("uid").toString();
}

Event::Event(const Event &other) {
    if (other._startTime > other._endTime) {
        _valid = false;
        return;
    }

    _content          = other._content;
    _rrule            = other._rrule;
    _startTime        = other._startTime;
    _endTime          = other._endTime;
    _lastModified     = other._lastModified;
    _creationDatetime = other._creationDatetime;
    _uid              = QUuid::createUuid().toString();
}

Event::~Event() {}

bool Event::isValid() const { return _valid; }

QString Event::toString() const {
    return QString(
               "Event content: %1\n  Created: %2\n  Start: %3\n  End: %4\n  "
               "Last modified: %5\n  "
               "uid: %6\n  rrule: %7\n")
        .arg(_content, _creationDatetime.toString(), _startTime.toString(), _endTime.toString(),
             _lastModified.toString(), _uid, _rrule.toString());
}

QDateTime Event::startTime() const { return _startTime; }

QDateTime Event::endTime() const { return _endTime; }

QDateTime Event::lastModified() const { return _lastModified; }

QString Event::uid() const { return _uid; }

QString Event::content() const { return _content; }

bool Event::isReccurent() const { return _rrule.isValid(); }

QVector<QSharedPointer<Event>> Event::getReccurentInstances(QDate _begin, QDate _end) const {
    QDateTime begin(_begin, QTime(0, 0), _startTime.timeZone());
    QDateTime end(_end, QTime(23, 59), _startTime.timeZone());
    QDateTime lowerBound(std::max(begin.date(), _startTime.date()), begin.time(), begin.timeZone());
    QVector<QSharedPointer<Event>> res;
    if (!_rrule.isValid()) {
        qInfo() << "RRule invalid";
        return res;
    }
    if (_startTime > end) {
        qInfo() << "start of event after end of time window";
        return res;
    }
    if (_rrule.until() < begin && _rrule.until().isValid()) {
        qInfo() << "end of reccurent events before start of time window";
        return res;
    }
    auto dateLimit = _rrule.until().isValid() ? std::min<QDateTime>(_rrule.until(), end) : end;
    auto count     = _rrule.count() != 0 ? _rrule.count() : _startTime.daysTo(end) + 1;

    switch (_rrule.freq()) {
        case RRules::Freq::kDaily: {
            count -= 1;  // Event day does fit the criterium of the reccurent
                         // rule, as such it count in the total of events
            auto currentDate = _startTime;
            while (currentDate <= dateLimit && count > 0) {
                currentDate = currentDate.addDays(_rrule.interval());
                if (currentDate <= dateLimit && currentDate > lowerBound) {
                    res.append(copyToDate(currentDate));
                }
                count--;
            }
            break;
        }
        case RRules::Freq::kWeekly: {
            int  dayOfWeek   = _startTime.date().dayOfWeek();
            auto currentDate = _startTime.addDays(-(dayOfWeek - 1));
            auto byDay       = _rrule.byDay();

            // Conditions to remove main event date from the count
            if (byDay.empty() || byDay.contains(RRules::mapNumDay[_startTime.date().dayOfWeek()])) {
                count -= 1;
            }
            while (currentDate <= dateLimit && count > 0) {
                if (byDay.size()) {
                    for (auto it = byDay.begin(); it != byDay.end(); ++it) {
                        auto dateToAdd =
                            currentDate.addDays(_rrule.interval() * 7 + RRules::mapDayNum[*it] - 1);
                        if (dateToAdd <= dateLimit && dateToAdd > lowerBound) {
                            res.append(copyToDate(dateToAdd));
                        }
                        count--;
                    }
                } else {
                    auto dateToAdd = currentDate.addDays(_rrule.interval() * 7 + dayOfWeek - 1);
                    if (dateToAdd <= dateLimit && dateToAdd > lowerBound) {
                        res.append(copyToDate(dateToAdd));
                    }
                    count--;
                }
                currentDate = currentDate.addDays(_rrule.interval() * 7);
            }
            break;
        }
        case RRules::Freq::kMonthly: {
            int  dayOfMonth  = _startTime.date().day();
            auto currentDate = _startTime.addDays(-dayOfMonth + 1);
            auto byDay       = _rrule.byDay();
            auto byMonthDay  = _rrule.byMonthDay();
            auto rankInMonth = _rrule.dayRankInMonth();
            if (byDay.size() > 1) break;

            // Conditions to remove main event date from the count
            if (fitCriteriaMonth()) {
                count -= 1;
            }
            while (currentDate <= dateLimit && count > 0) {
                if (byDay.size() == 1) {
                    QVector<QDateTime> dates;
                    for (auto it = byDay.begin(); it != byDay.end(); ++it) {
                        dates.append(getNextDaysOfWeekInMonth(
                            currentDate, RRules::mapDayNum.value(*it), rankInMonth));
                    }
                    for (auto it = dates.begin(); it != dates.end(); ++it) {
                        if (lowerBound < *it && *it <= dateLimit && count > 0) {
                            res.append(copyToDate(*it));
                            count--;
                        }
                    }
                } else if (byMonthDay.size()) {
                    for (auto it = byMonthDay.begin(); it != byMonthDay.end(); ++it) {
                        auto dateToAdd = currentDate.addDays(*it - 1);
                        if (lowerBound < dateToAdd && dateToAdd <= dateLimit && count > 0) {
                            res.append(copyToDate(dateToAdd));
                            count--;
                        } else if (_startTime < dateToAdd && dateToAdd <= dateLimit) {
                            count--;
                        }
                    }
                }
                currentDate = currentDate.addMonths(_rrule.interval());
            }
            break;
        }
        case RRules::Freq::kYearly: {
            auto currentDate = _startTime;
            auto byMonth     = _rrule.byMonth();
            auto byMonthDay  = _rrule.byMonthDay();
            auto byDay       = _rrule.byDay();
            auto rankInMonth = _rrule.dayRankInMonth();
            if (byMonthDay.size() > 1 || byDay.size() > 1 || byMonth == 0) break;

            if (byMonth == _startTime.date().month() && fitCriteriaMonth()) {
                count -= 1;
            }
            while (currentDate <= dateLimit && count > 0) {
                if (byMonthDay.size() == 1) {
                    auto dateToAdd =
                        QDateTime(QDate(currentDate.date().year(), byMonth, *byMonthDay.begin()),
                                  currentDate.time(), currentDate.timeZone());
                    if (lowerBound < dateToAdd && dateToAdd <= dateLimit && count > 0) {
                        res.append(copyToDate(dateToAdd));
                        count--;
                    } else if (_startTime < dateToAdd && dateToAdd <= dateLimit) {
                        count--;
                    }
                } else if (rankInMonth != 0 && byDay.size() == 1) {
                    auto dates = getNextDaysOfWeekInMonth(
                        QDateTime(QDate(currentDate.date().year(), byMonth, 1), QTime(0, 0),
                                  currentDate.timeZone()),
                        RRules::mapDayNum.value(*byDay.begin()), rankInMonth);
                    for (auto it = dates.begin(); it != dates.end(); ++it) {
                        if (lowerBound < *it && *it <= dateLimit && count > 0) {
                            res.append(copyToDate(*it));
                            count--;
                        }
                    }
                }
                currentDate = currentDate.addYears(_rrule.interval());
            }
            break;
        }

        default:
            break;
    }
    return res;
}

void Event::updateContent(const QString newContent) {
    _content = newContent;
    emit contentChanged(_uid, _content);
}

void Event::updateStartTime(const QDateTime newStart) {
    _startTime = newStart;
    emit startTimeChanged(_uid, _startTime);
}

void Event::updateEndTime(const QDateTime newEndTime) {
    _endTime = newEndTime;
    emit endTimeChanged(_uid, _endTime);
}

void Event::updateReccurenceRules(const RRules newRules) {
    _rrule = newRules;
    emit reccurentStateChanged(_uid, _rrule.isValid());
}

QSharedPointer<Event> Event::copyToDate(const QDateTime date) const {
    QSharedPointer<Event> res = QSharedPointer<Event>::create(*this);
    res->_startTime           = date;
    res->_endTime             = _endTime - _startTime + date;
    res->_mainEventUID        = _uid;
    return res;
}

bool Event::operator<(const Event &other) const { return _startTime < other._startTime; }

QDebug operator<<(QDebug debug, const Event &c) {
    QDebugStateSaver saver(debug);
    debug.nospace().noquote() << c.toString();

    return debug;
}

bool Event::fitCriteriaMonth() const {
    auto rules = _rrule;

    bool fitDayOfWeek = rules.byDay().contains(RRules::mapNumDay[_startTime.date().dayOfWeek()]);
    int  rankEventInMonth = ceil(_startTime.date().day() / 7.0);
    bool fitDayOfMonth    = rules.byMonthDay().contains(_startTime.date().day());

    return fitDayOfMonth || (fitDayOfWeek && (rankEventInMonth == rules.dayRankInMonth()));
}