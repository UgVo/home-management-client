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