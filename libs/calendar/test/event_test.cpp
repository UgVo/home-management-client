#include <gtest/gtest.h>

#include <QJsonDocument>
#include <QTimeZone>

#include "event.h"

TEST(Event, Constructor) {
    Event   event("Test Event", "", QDateTime(QDate(2024, 10, 26), QTime(11, 3), QTimeZone(3600)),
                  QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    QString json_str =
        "{\"content\":\"test\",\"created\":\"2024-02-20T23:02:19+01:00\","
        "\"start\":\"2024-02-06T15:"
        "00:00+01:00\",\"end\":\"2024-02-06T16:00:00+01:00\",\"last_modified\":"
        "\"2024-02-21T11:40:"
        "43+01:00\",\"uid\":\"4b5d0414-96c7-4a42-9ce9-f13c3e38e393\",\"rrule\":"
        "\"\"}";
    Event eventJson(QJsonDocument::fromJson(json_str.toLocal8Bit()).object());
    ASSERT_EQ(QDateTime(QDate(2024, 10, 26), QTime(11, 3), QTimeZone(3600)), event.startTime());
    ASSERT_EQ(eventJson.startTime(), QDateTime(QDate(2024, 02, 06), QTime(15, 0), QTimeZone(3600)));
}

TEST(Event, OperatorInf) {
    Event event1("Test Event", "", QDateTime(QDate(2024, 10, 26), QTime(11, 3), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    Event event2("Test Event", "", QDateTime(QDate(2024, 11, 26), QTime(11, 3), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    Event event3("Test Event", "", QDateTime(QDate(2024, 11, 27), QTime(11, 3), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    Event event4("Test Event", "", QDateTime(QDate(2024, 11, 27), QTime(12, 3), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    Event event5("Test Event", "", QDateTime(QDate(2024, 11, 27), QTime(12, 4), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    ASSERT_LT(event1, event2);
    ASSERT_LT(event2, event3);
    ASSERT_LT(event3, event4);
    ASSERT_LT(event4, event5);
}

TEST(Event, RRulesDaily) {
    Event event1("Test Event", "FREQ=DAILY",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    auto  events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 2);
    events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 30));
    ASSERT_EQ(events.size(), 4);
    events = event1.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);

    Event event2("Test Event", "FREQ=DAILY;COUNT=5",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 2);
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 29));
    ASSERT_EQ(events.size(), 3);
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 4);
    events = event2.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event2.getReccurentInstances(QDate(2024, 10, 31), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 0);

    Event event3("Test Event", "FREQ=DAILY;COUNT=5;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event3.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 1);
    events = event3.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 30));
    ASSERT_EQ(events.size(), 2);
    events = event3.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 4);
    events = event3.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event3.getReccurentInstances(QDate(2024, 11, 4), QDate(2024, 11, 5));
    ASSERT_EQ(events.size(), 0);

    Event event4("Test Event", "FREQ=DAILY;UNTIL=20241031T000000Z;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 1);
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 30));
    ASSERT_EQ(events.size(), 2);
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 2);
    events = event4.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event4.getReccurentInstances(QDate(2024, 11, 4), QDate(2024, 11, 5));
    ASSERT_EQ(events.size(), 0);
}

TEST(Event, RRulesWeekly) {
    Event event1("Test Event", "FREQ=WEEKLY",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    auto  events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 3));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));
    events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 30));
    ASSERT_EQ(events.size(), 0);
    events = event1.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 17));
    ASSERT_EQ(events.size(), 3);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 16), QTime(0, 0), QTimeZone(3600)));

    Event event2("Test Event", "FREQ=WEEKLY;BYDAY=FR,SA",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 3));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 1), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 30));
    ASSERT_EQ(events.size(), 0);
    events = event2.getReccurentInstances(QDate(2024, 9, 28), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 17));
    ASSERT_EQ(events.size(), 6);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 1), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[3].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[4].startTime(), QDateTime(QDate(2024, 11, 15), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[5].startTime(), QDateTime(QDate(2024, 11, 16), QTime(0, 0), QTimeZone(3600)));

    Event event3("Test Event", "FREQ=WEEKLY;BYDAY=FR,SA;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event3.getReccurentInstances(QDate(2024, 10, 14), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 4);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 22), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[3].startTime(), QDateTime(QDate(2024, 11, 23), QTime(0, 0), QTimeZone(3600)));

    Event event4("Test Event", "FREQ=WEEKLY;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 23), QTime(0, 0), QTimeZone(3600)));

    Event event5("Test Event", "FREQ=WEEKLY;UNTIL=20241107T000000Z",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event5.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 0);
    events = event5.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));

    Event event6("Test Event", "FREQ=WEEKLY;COUNT=5",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event6.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 0);
    events = event6.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 4);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 2), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 16), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[3].startTime(), QDateTime(QDate(2024, 11, 23), QTime(0, 0), QTimeZone(3600)));

    Event event7("Test Event", "FREQ=WEEKLY;BYDAY=FR;COUNT=5",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event7.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 0);
    events = event7.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 5);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 1), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 15), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[3].startTime(), QDateTime(QDate(2024, 11, 22), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[4].startTime(), QDateTime(QDate(2024, 11, 29), QTime(0, 0), QTimeZone(3600)));
}

TEST(Event, RRulesMonthly) {
    Event event1("Test Event", "FREQ=MONTHLY;BYDAY=WE",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    auto  events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 3));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 10, 30), QTime(0, 0), QTimeZone(3600)));
    events = event1.getReccurentInstances(QDate(2024, 9, 8), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 17));
    ASSERT_EQ(events.size(), 3);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 10, 30), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 6), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 13), QTime(0, 0), QTimeZone(3600)));

    Event event2("Test Event", "FREQ=MONTHLY;BYDAY=2FR",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 9));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 9, 1), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 12, 17));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 12, 13), QTime(0, 0), QTimeZone(3600)));

    Event event3("Test Event", "FREQ=MONTHLY;BYDAY=FR,SA;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event3.getReccurentInstances(QDate(2024, 10, 14), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 0);

    Event event4("Test Event", "FREQ=MONTHLY;BYDAY=FR;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 12, 14));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 12, 6), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 12, 13), QTime(0, 0), QTimeZone(3600)));

    Event event5("Test Event", "FREQ=MONTHLY;BYDAY=FR;UNTIL=20241107T000000Z",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event5.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 1), QTime(0, 0), QTimeZone(3600)));

    Event event6("Test Event", "FREQ=MONTHLY;BYDAY=FR;COUNT=5",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event6.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 0);
    events = event6.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 5);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 1), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 8), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 15), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[3].startTime(), QDateTime(QDate(2024, 11, 22), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[4].startTime(), QDateTime(QDate(2024, 11, 29), QTime(0, 0), QTimeZone(3600)));

    Event event7("Test Event", "FREQ=MONTHLY;BYMONTHDAY=6,9,15;COUNT=5",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event7.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 10, 28));
    ASSERT_EQ(events.size(), 0);
    events = event7.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 3);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 6), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 11, 9), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[2].startTime(), QDateTime(QDate(2024, 11, 15), QTime(0, 0), QTimeZone(3600)));

    events = event7.getReccurentInstances(QDate(2024, 11, 30), QDate(2024, 12, 30));
    std::sort(events.begin(), events.end());
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 12, 6), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 12, 9), QTime(0, 0), QTimeZone(3600)));

    Event event8("Test Event", "FREQ=MONTHLY;BYDAY=4SA",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event8.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 11, 9));
    ASSERT_EQ(events.size(), 0);
    events = event8.getReccurentInstances(QDate(2024, 9, 1), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event8.getReccurentInstances(QDate(2024, 10, 23), QDate(2024, 12, 30));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2024, 11, 23), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2024, 12, 28), QTime(0, 0), QTimeZone(3600)));
}

TEST(Event, RRulesYearly) {
    Event event1("Test Event", "FREQ=YEARLY;BYMONTH=10;BYMONTHDAY=26",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    auto  events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2025, 11, 3));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 26), QTime(0, 0), QTimeZone(3600)));

    events = event1.getReccurentInstances(QDate(2023, 9, 8), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event1.getReccurentInstances(QDate(2024, 10, 23), QDate(2026, 11, 17));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 26), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2026, 10, 26), QTime(0, 0), QTimeZone(3600)));

    Event event2("Test Event", "FREQ=YEARLY;BYMONTH=10;BYDAY=2TU",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2025, 11, 9));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 14), QTime(0, 0), QTimeZone(3600)));

    events = event2.getReccurentInstances(QDate(2024, 9, 1), QDate(2024, 9, 30));
    ASSERT_EQ(events.size(), 0);
    events = event2.getReccurentInstances(QDate(2024, 10, 23), QDate(2026, 12, 17));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 14), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2026, 10, 13), QTime(0, 0), QTimeZone(3600)));

    Event event3("Test Event", "FREQ=YEARLY;BYDAY=FR,SA",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event3.getReccurentInstances(QDate(2024, 10, 14), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 0);

    Event event3_2("Test Event", "FREQ=YEARLY;BYMONTH=10,11",
                   QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                   QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event3_2.getReccurentInstances(QDate(2024, 10, 14), QDate(2024, 11, 30));
    ASSERT_EQ(events.size(), 0);

    qDebug() << "-------------------------------------";
    Event event4("Test Event", "FREQ=YEARLY;BYMONTH=10;BYDAY=2TU;INTERVAL=2",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2026, 11, 9));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2026, 10, 13), QTime(0, 0), QTimeZone(3600)));

    events = event4.getReccurentInstances(QDate(2024, 10, 23), QDate(2028, 12, 17));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2026, 10, 13), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2028, 10, 10), QTime(0, 0), QTimeZone(3600)));

    Event event4_3("Test Event", "FREQ=YEARLY;BYMONTH=10;BYDAY=4SA;COUNT=2",
                   QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                   QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event4_3.getReccurentInstances(QDate(2024, 10, 23), QDate(2026, 11, 9));
    ASSERT_EQ(events.size(), 1);
    qDebug() << events[0].startTime();
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 25), QTime(0, 0), QTimeZone(3600)));

    events = event4_3.getReccurentInstances(QDate(2024, 10, 23), QDate(2028, 12, 17));
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 25), QTime(0, 0), QTimeZone(3600)));

    Event event5("Test Event", "FREQ=YEARLY;BYMONTH=10;BYDAY=4SA;UNTIL=20261107T000000Z",
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)),
                 QDateTime(QDate(2024, 10, 26), QTime(0, 0), QTimeZone(3600)));
    events = event5.getReccurentInstances(QDate(2024, 10, 23), QDate(2027, 12, 17));
    ASSERT_EQ(events.size(), 2);
    ASSERT_EQ(events[0].startTime(), QDateTime(QDate(2025, 10, 25), QTime(0, 0), QTimeZone(3600)));
    ASSERT_EQ(events[1].startTime(), QDateTime(QDate(2026, 10, 24), QTime(0, 0), QTimeZone(3600)));
}
