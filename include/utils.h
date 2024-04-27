#ifndef HOME_MANAGEMENT_CLIENT_UTILS_H
#define HOME_MANAGEMENT_CLIENT_UTILS_H

#include <QDateTime>
#include <QString>

/// @brief Generate QDateTime object from ISO8601 date strings
/// @param datetime as an ISO8601 string
/// @return QDateTime corresponding if the format is correct, an invalid QDateTime is returned
/// otherwise.
QDateTime parseDateTime(const QString datetime);

#endif  // HOME_MANAGEMENT_CLIENT_UTILS_H