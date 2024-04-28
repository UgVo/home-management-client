#include <gtest/gtest.h>

#include <QJsonDocument>
#include <QTimeZone>

#include "agenda.h"

TEST(Agenda, Constructor) {
    Agenda agenda_invalid;
    Agenda agenda_first_ctor(QString("Agenda test"), QString("https://caldav.server.com"), 10);
    QString json_str = QString(
        "{\"name\":\"commun-test\",\"url\":\"https://baikal.mycozycloud.ovh/dav.php/"
        "calendars/dev/commun_test/\",\"nb_of_events\":6}");
    Agenda agenda_second_ctor(QJsonDocument::fromJson(json_str.toLocal8Bit()).object());
    ASSERT_FALSE(agenda_invalid.isValid());
    ASSERT_TRUE(agenda_first_ctor.isValid());
    ASSERT_TRUE(agenda_second_ctor.isValid());
}

TEST(Agenda, UpdateEvents) {
    Agenda agenda(QString("Agenda test"), QString("https://caldav.server.com"), 10);
    QString json_str = QString(
        "[{\"content\":\"test\",\"created\":\"2024-02-20T23:02:19+01:00\",\"start\":\"2024-02-"
        "06T15:"
        "00:00+01:00\",\"end\":\"2024-02-06T16:00:00+01:00\",\"last_modified\":\"2024-02-21T11:40:"
        "43+01:00\",\"uid\":\"4b5d0414-96c7-4a42-9ce9-f13c3e38e393\",\"rrule\":\"\"},{\"content\":"
        "\"test_recur\",\"created\":\"2024-02-22T11:42:29+01:00\",\"start\":\"2024-02-07T13:00:00+"
        "01:00\",\"end\":\"2024-02-07T14:00:00+01:00\",\"last_modified\":\"2024-02-22T11:42:59+01:"
        "00\",\"uid\":\"641123e0-f2a6-4d2c-bc55-368edce07248\",\"rrule\":\"FREQ=MONTHLY;BYMONTHDAY="
        "7,9\"}]");
    auto size = agenda.updateEvents(QJsonDocument::fromJson(json_str.toLocal8Bit()).array());
    ASSERT_EQ(size, size_t(2));
}

TEST(Agenda, GetEventsWhole) {
    Agenda agenda(QString("Agenda test"), QString("https://caldav.server.com"), 10);
    QString json_str = QString(
        "[{\"content\":\"test\",\"created\":\"2024-02-20T23:02:19+01:00\",\"start\":\"2024-02-"
        "06T15:"
        "00:00+01:00\",\"end\":\"2024-02-06T16:00:00+01:00\",\"last_modified\":\"2024-02-21T11:40:"
        "43+01:00\",\"uid\":\"4b5d0414-96c7-4a42-9ce9-f13c3e38e393\",\"rrule\":\"\"},{\"content\":"
        "\"test_recur\",\"created\":\"2024-02-22T11:42:29+01:00\",\"start\":\"2024-02-07T13:00:00+"
        "01:00\",\"end\":\"2024-02-07T14:00:00+01:00\",\"last_modified\":\"2024-02-22T11:42:59+01:"
        "00\",\"uid\":\"641123e0-f2a6-4d2c-bc55-368edce07248\",\"rrule\":\"FREQ=MONTHLY;BYMONTHDAY="
        "7,9\"}]");
    agenda.updateEvents(QJsonDocument::fromJson(json_str.toLocal8Bit()).array());
    auto events = agenda.getEvents();
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0]->uid(), QString("4b5d0414-96c7-4a42-9ce9-f13c3e38e393"));
    ASSERT_EQ(events[1]->uid(), QString("641123e0-f2a6-4d2c-bc55-368edce07248"));
    ASSERT_EQ(events[0]->lastModified(),
              QDateTime::fromString("2024-02-21T11:40:43+01:00", Qt::ISODate));
    ASSERT_EQ(events[1]->lastModified(),
              QDateTime::fromString("2024-02-22T11:42:59+01:00", Qt::ISODate));
}

TEST(Agenda, GetEventsPartial) {
    Agenda agenda(QString("Agenda test"), QString("https://caldav.server.com"), 10);
    QString json_str = QString(
        "[{\"content\":\"test\",\"created\":\"2024-02-20T23:02:19+01:00\",\"start\":\"2024-02-"
        "06T15:"
        "00:00+01:00\",\"end\":\"2024-02-06T16:00:00+01:00\",\"last_modified\":\"2024-02-21T11:40:"
        "43+01:00\",\"uid\":\"4b5d0414-96c7-4a42-9ce9-f13c3e38e393\",\"rrule\":\"\"},{\"content\":"
        "\"test_recur\",\"created\":\"2024-02-22T11:42:29+01:00\",\"start\":\"2024-02-07T13:00:00+"
        "01:00\",\"end\":\"2024-02-07T14:00:00+01:00\",\"last_modified\":\"2024-02-22T11:42:59+01:"
        "00\",\"uid\":\"641123e0-f2a6-4d2c-bc55-368edce07248\",\"rrule\":\"FREQ=MONTHLY;BYMONTHDAY="
        "7,9\"}]");
    agenda.updateEvents(QJsonDocument::fromJson(json_str.toLocal8Bit()).array());
    auto events = agenda.getEvents(QDateTime(QDate(2024, 02, 06), QTime(14, 0), QTimeZone(3600)),
                                   QDateTime(QDate(2024, 02, 07), QTime(12, 0), QTimeZone(3600)));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0]->uid(), QString("4b5d0414-96c7-4a42-9ce9-f13c3e38e393"));
    ASSERT_EQ(events[0]->lastModified(),
              QDateTime::fromString("2024-02-21T11:40:43+01:00", Qt::ISODate));

    events = agenda.getEvents(QDateTime(QDate(2024, 02, 06), QTime(16, 0), QTimeZone(3600)),
                              QDateTime(QDate(2024, 02, 07), QTime(14, 0), QTimeZone(3600)));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0]->uid(), QString("641123e0-f2a6-4d2c-bc55-368edce07248"));
    ASSERT_EQ(events[0]->lastModified(),
              QDateTime::fromString("2024-02-22T11:42:59+01:00", Qt::ISODate));

    events = agenda.getEvents(QDateTime(QDate(2024, 02, 07), QTime(16, 0), QTimeZone(3600)),
                              QDateTime(QDate(2024, 02, 07), QTime(14, 0), QTimeZone(3600)));
    ASSERT_EQ(events.size(), 0);
}