#include <QDateTime>
#include <QString>
#include <QTimeZone>

#include "utils.h"

QDateTime parseDateTime(const QString datetime) {
    if (datetime.contains('-') &&
        (datetime.size() == 20 ||
         datetime.size() == 25)) {  // ISO8601 with '-' and ':' (ex: 2024-02-09T21:00:00Z or
                                    // 2024-02-20T23:02:19+01:00)
        return QDateTime::fromString(datetime, Qt::ISODate);
    } else if (datetime.size() == 16) {  // ISO8601 without '-' and ':' (ex: 20240209T210000Z)
        QString datetimeFormated = datetime;
        datetimeFormated.insert(4, "-").insert(7, '-').insert(13, ':').insert(16, ':');
        return QDateTime::fromString(datetimeFormated, Qt::ISODate);
    }
    return QDateTime();
}

QVector<QDateTime> getNextDaysOfWeekInMonth(QDateTime begin, int rankInWeek, int rankInMonth) {
    QVector<QDateTime> res;
    if (rankInWeek > 7 || rankInWeek < 0) return res;
    if (rankInMonth > 5 || rankInMonth < -1) return res;
    auto month = begin.date().month();
    auto cal   = QCalendar();
    auto end =
        QDateTime(QDate(begin.date().year(), month, cal.daysInMonth(month, begin.date().year())),
                  begin.time(), begin.timeZone());
    auto currentDateTime = begin;
    while (currentDateTime <= end) {
        if (cal.dayOfWeek(currentDateTime.date()) == rankInWeek) {
            if (rankInMonth != 0) {
                if (ceil(currentDateTime.date().day() / 7.0) == rankInMonth) {
                    res.append(currentDateTime);
                } else if (rankInMonth == -1 &&
                           currentDateTime.date().day() >=
                               cal.daysInMonth(month, begin.date().year()) - 7) {
                    res.append(currentDateTime);
                }
            } else {
                res.append(currentDateTime);
            }
        }
        currentDateTime = currentDateTime.addDays(1);
    }

    return res;
}
