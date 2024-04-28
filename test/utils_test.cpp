#include <gtest/gtest.h>

#include <QTimeZone>

#include "utils.h"

TEST(Event, getNextDaysOfWeekInMonth) {
    QVector<QDateTime> res = getNextDaysOfWeekInMonth(
        QDateTime(QDate(2024, 10, 1), QTime(0, 0), QTimeZone(3600)), 1);
    ASSERT_EQ(res.size(), 4);
    ASSERT_EQ(res[0],
              QDateTime(QDate(2024, 10, 7), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(res[1],
              QDateTime(QDate(2024, 10, 14), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(res[2],
              QDateTime(QDate(2024, 10, 21), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(res[3],
              QDateTime(QDate(2024, 10, 28), QTime(0, 0), QTimeZone(3600)));

    res = getNextDaysOfWeekInMonth(
        QDateTime(QDate(2024, 10, 1), QTime(0, 0), QTimeZone(3600)), 5, 3);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0],
              QDateTime(QDate(2024, 10, 18), QTime(0, 0), QTimeZone(3600)));

    res = getNextDaysOfWeekInMonth(
        QDateTime(QDate(2024, 10, 1), QTime(0, 0), QTimeZone(3600)), 2, -1);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0],
              QDateTime(QDate(2024, 10, 29), QTime(0, 0), QTimeZone(3600)));
}