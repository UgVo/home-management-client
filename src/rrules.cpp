#include "rrules.h"

RRules RRules::fromString(const QString rrules) {
    RRules res;
    if (rrules.isEmpty()) return res;
    QStringList subStrings = rrules.split(';');
    for (auto it = subStrings.begin(); it != subStrings.end(); ++it) {
        QStringList rule = it->split('=');
        if (rule[0] == "FREQ") {
            res.setFreq(RRules::mapFreq[rule[1]]);
        } else if (rule[0] == "COUNT") {
            res.setCount(rule[1].toInt());
        } else if (rule[0] == "INTERVAL") {
            res.setInterval(rule[1].toInt());
        } else if (rule[0] == "UNTIL") {
            res.setUntil(parseDateTime(rule[1]));
        } else if (rule[0] == "BYDAY") {
            QStringList days = rule[1].split(',');
            for (auto it = days.begin(); it != days.end(); ++it) {
                if (it->size() > 2) {
                    QString day = it->sliced(it->size() - 2);
                    QString rank = it->chopped(2);
                    res.insertDay(RRules::mapDay[day]);
                    res.setDayRankInMonth(rank.toInt());
                } else {
                    res.insertDay(RRules::mapDay[*it]);
                }
            }
        } else if (rule[0] == "BYMONTHDAY") {
            QStringList days = rule[1].split(',');
            for (auto it = days.begin(); it != days.end(); ++it) {
                res.insertMonthDay(it->toInt());
            }
        } else if (rule[0] == "BYMONTH") {
            res.setByMonth(rule[1].toInt());
        }
    }
    res._str = rrules;
    return res;
}

RRules::Freq RRules::freq() const { return _freq; }

void RRules::setFreq(const Freq freq) {
    _freq = freq;
    _valid = true;
}

int RRules::count() const { return _count; }

void RRules::setCount(const int count) { _count = std::clamp(count, 0, 999); }

int RRules::interval() const { return _interval; }

void RRules::setInterval(const int interval) { _interval = std::clamp(interval, 1, 999); }

QDateTime RRules::until() const { return _until; }

void RRules::setUntil(const QDateTime &&until) { _until = until; }

QSet<RRules::Day> RRules::byDay() const { return _byDay; }

void RRules::setByDay(const QSet<Day> &&byDay) { _byDay += byDay; }

void RRules::insertDay(const Day day) {
    if (day == Day::kNone) return;
    _byDay.insert(day);
}

int RRules::dayRankInMonth() const { return _dayRankInMonth; }

void RRules::setDayRankInMonth(const int dayRank) {
    if (dayRank > 5 || dayRank < -1) {
        _dayRankInMonth = 0;
        return;
    }
    _dayRankInMonth = dayRank;
}

QSet<int> RRules::byMonthDay() const { return _byMonthDay; }

void RRules::setByMonthDay(const QSet<int> byMonthDay) { _byMonthDay += byMonthDay; }

void RRules::insertMonthDay(const int monthDay) {
    if (monthDay > 31 || monthDay < -1 || monthDay == 0) {
        return;
    }
    _byMonthDay.insert(monthDay);
}

int RRules::byMonth() const { return _byMonth; }

void RRules::setByMonth(const int byMonth) {
    if (byMonth > 12 || byMonth <= 0) {
        _byMonth = 0;
        return;
    }
    _byMonth = byMonth;
}

bool RRules::isValid() const { return _valid; }

QString RRules::toString() const { return _str; }
