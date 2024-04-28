#ifndef HOME_MANAGEMENT_CLIENT_UTILS_H
#define HOME_MANAGEMENT_CLIENT_UTILS_H

#include <QDateTime>
#include <QString>
#include <QTimeZone>

/// @brief Generate QDateTime object from ISO8601 date strings
/// @param datetime as an ISO8601 string
/// @return QDateTime corresponding if the format is correct, an invalid
/// QDateTime is returned otherwise.
QDateTime parseDateTime(const QString datetime);

/// @brief Compute the dates following the `begin` date in the same month and fitting the contraints
/// of rank in week and rank in month
/// @param begin Date after which looking for dates fitting the constraints
/// @param rankInWeek Value between 1 and 7 corresponding to the day of the week
/// @param rankInMonth Value between 1 and 5 corresponding to the "rank" of a day in the month (for
/// example : 3 for the third monday of the month)
/// @return A vector of the founded date fitting the constraints
QVector<QDateTime> getNextDaysOfWeekInMonth(QDateTime begin, int rankInWeek, int rankInMonth = 0);

#endif  // HOME_MANAGEMENT_CLIENT_UTILS_H