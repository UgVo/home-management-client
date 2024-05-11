#include <gtest/gtest.h>

#include <QJsonDocument>
#include <QTimeZone>

#include "rrules.h"

TEST(RRules, RRulesInterpretationDaily) {
    RRules dailyInvalid = RRules::fromString("");
    ASSERT_FALSE(dailyInvalid.isValid());

    RRules daily = RRules::fromString("FREQ=DAILY");
    ASSERT_TRUE(daily.isValid());
    ASSERT_EQ(daily.freq(), RRules::Freq::kDaily);
    ASSERT_EQ(daily.interval(), 1);
    ASSERT_EQ(daily.byDay().size(), 0);
    ASSERT_EQ(daily.count(), 0);
    ASSERT_FALSE(daily.until().isValid());

    RRules daily2 = RRules::fromString("FREQ=DAILY;INTERVAL=5");
    ASSERT_TRUE(daily2.isValid());
    ASSERT_EQ(daily2.freq(), RRules::Freq::kDaily);
    ASSERT_EQ(daily2.interval(), 5);
    ASSERT_EQ(daily2.byDay().size(), 0);
    ASSERT_EQ(daily2.count(), 0);
    ASSERT_FALSE(daily2.until().isValid());

    RRules daily3 = RRules::fromString("FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR");
    ASSERT_TRUE(daily3.isValid());
    ASSERT_EQ(daily3.freq(), RRules::Freq::kDaily);
    ASSERT_EQ(daily3.interval(), 1);
    ASSERT_EQ(daily3.byDay().size(), 5);
    ASSERT_EQ(daily3.count(), 0);
    ASSERT_FALSE(daily3.until().isValid());

    RRules daily4 = RRules::fromString("FREQ=DAILY;COUNT=2");
    ASSERT_TRUE(daily4.isValid());
    ASSERT_EQ(daily4.freq(), RRules::Freq::kDaily);
    ASSERT_EQ(daily4.interval(), 1);
    ASSERT_EQ(daily4.byDay().size(), 0);
    ASSERT_EQ(daily4.count(), 2);
    ASSERT_FALSE(daily4.until().isValid());

    QDateTime expected(QDate(2024, 2, 9), QTime(21, 0, 0), QTimeZone::utc());
    RRules    daily5 = RRules::fromString("FREQ=DAILY;UNTIL=20240209T210000Z");
    ASSERT_TRUE(daily5.isValid());
    ASSERT_EQ(daily5.freq(), RRules::Freq::kDaily);
    ASSERT_EQ(daily5.interval(), 1);
    ASSERT_EQ(daily5.byDay().size(), 0);
    ASSERT_EQ(daily5.count(), 0);
    ASSERT_EQ(daily5.until(), expected);
}

TEST(RRules, RRulesInterpretationWeekly) {
    RRules rule = RRules::fromString("FREQ=WEEKLY");
    ASSERT_TRUE(rule.isValid());
    ASSERT_EQ(rule.freq(), RRules::Freq::kWeekly);
    ASSERT_EQ(rule.interval(), 1);
    ASSERT_EQ(rule.byDay().size(), 0);
    ASSERT_EQ(rule.count(), 0);
    ASSERT_FALSE(rule.until().isValid());

    RRules rule2 = RRules::fromString("FREQ=WEEKLY;INTERVAL=5");
    ASSERT_TRUE(rule2.isValid());
    ASSERT_EQ(rule2.freq(), RRules::Freq::kWeekly);
    ASSERT_EQ(rule2.interval(), 5);
    ASSERT_EQ(rule2.byDay().size(), 0);
    ASSERT_EQ(rule2.count(), 0);
    ASSERT_FALSE(rule2.until().isValid());

    RRules rule3 = RRules::fromString("FREQ=WEEKLY;BYDAY=MO,TU,FR");
    ASSERT_TRUE(rule3.isValid());
    ASSERT_EQ(rule3.freq(), RRules::Freq::kWeekly);
    ASSERT_EQ(rule3.interval(), 1);
    ASSERT_EQ(rule3.byDay().size(), 3);
    ASSERT_EQ(rule3.count(), 0);
    ASSERT_FALSE(rule3.until().isValid());

    RRules rule4 = RRules::fromString("FREQ=WEEKLY;COUNT=2");
    ASSERT_TRUE(rule4.isValid());
    ASSERT_EQ(rule4.freq(), RRules::Freq::kWeekly);
    ASSERT_EQ(rule4.interval(), 1);
    ASSERT_EQ(rule4.byDay().size(), 0);
    ASSERT_EQ(rule4.count(), 2);
    ASSERT_FALSE(rule4.until().isValid());

    QDateTime expected(QDate(2024, 2, 9), QTime(21, 10, 0), QTimeZone::utc());
    RRules    rule5 = RRules::fromString("FREQ=WEEKLY;UNTIL=20240209T211000Z");
    ASSERT_TRUE(rule5.isValid());
    ASSERT_EQ(rule5.freq(), RRules::Freq::kWeekly);
    ASSERT_EQ(rule5.interval(), 1);
    ASSERT_EQ(rule5.byDay().size(), 0);
    ASSERT_EQ(rule5.count(), 0);
    ASSERT_EQ(rule5.until(), expected);
}

TEST(RRules, RRulesInterpretationMonthly) {
    RRules rule = RRules::fromString("FREQ=MONTHLY");
    ASSERT_TRUE(rule.isValid());
    ASSERT_EQ(rule.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule.interval(), 1);
    ASSERT_EQ(rule.byDay().size(), 0);
    ASSERT_EQ(rule.count(), 0);
    ASSERT_EQ(rule.dayRankInMonth(), 0);
    ASSERT_EQ(rule.byMonthDay().size(), 0);
    ASSERT_FALSE(rule.until().isValid());

    RRules rule2 = RRules::fromString("FREQ=MONTHLY;INTERVAL=5");
    ASSERT_TRUE(rule2.isValid());
    ASSERT_EQ(rule2.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule2.interval(), 5);
    ASSERT_EQ(rule2.byDay().size(), 0);
    ASSERT_EQ(rule2.count(), 0);
    ASSERT_EQ(rule2.dayRankInMonth(), 0);
    ASSERT_EQ(rule2.byMonthDay().size(), 0);
    ASSERT_FALSE(rule2.until().isValid());

    RRules rule3 = RRules::fromString("FREQ=MONTHLY;BYDAY=1MO");
    ASSERT_TRUE(rule3.isValid());
    ASSERT_EQ(rule3.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule3.interval(), 1);
    ASSERT_EQ(rule3.byDay().size(), 1);
    ASSERT_EQ(rule3.count(), 0);
    ASSERT_EQ(rule3.dayRankInMonth(), 1);
    ASSERT_EQ(rule3.byMonthDay().size(), 0);
    ASSERT_FALSE(rule3.until().isValid());

    RRules rule4 = RRules::fromString("FREQ=MONTHLY;BYMONTHDAY=-1,1,8,9");
    ASSERT_TRUE(rule4.isValid());
    ASSERT_EQ(rule4.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule4.interval(), 1);
    ASSERT_EQ(rule4.byDay().size(), 0);
    ASSERT_EQ(rule4.count(), 0);
    ASSERT_EQ(rule4.dayRankInMonth(), 0);
    ASSERT_EQ(rule4.byMonthDay().size(), 4);
    ASSERT_FALSE(rule4.until().isValid());

    RRules rule5 = RRules::fromString("FREQ=MONTHLY;COUNT=2");
    ASSERT_TRUE(rule5.isValid());
    ASSERT_EQ(rule5.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule5.interval(), 1);
    ASSERT_EQ(rule5.byDay().size(), 0);
    ASSERT_EQ(rule5.count(), 2);
    ASSERT_EQ(rule5.dayRankInMonth(), 0);
    ASSERT_EQ(rule5.byMonthDay().size(), 0);
    ASSERT_FALSE(rule5.until().isValid());

    QDateTime expected(QDate(2024, 2, 9), QTime(21, 10, 0), QTimeZone::utc());
    RRules    rule6 = RRules::fromString("FREQ=MONTHLY;UNTIL=20240209T211000Z");
    ASSERT_TRUE(rule6.isValid());
    ASSERT_EQ(rule6.freq(), RRules::Freq::kMonthly);
    ASSERT_EQ(rule6.interval(), 1);
    ASSERT_EQ(rule6.byDay().size(), 0);
    ASSERT_EQ(rule6.count(), 0);
    ASSERT_EQ(rule6.dayRankInMonth(), 0);
    ASSERT_EQ(rule6.byMonthDay().size(), 0);
    ASSERT_EQ(rule6.until(), expected);
}

TEST(RRules, RRulesInterpretationYearly) {
    RRules rule = RRules::fromString("FREQ=YEARLY");
    ASSERT_TRUE(rule.isValid());
    ASSERT_EQ(rule.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule.interval(), 1);
    ASSERT_EQ(rule.byDay().size(), 0);
    ASSERT_EQ(rule.count(), 0);
    ASSERT_EQ(rule.dayRankInMonth(), 0);
    ASSERT_EQ(rule.byMonthDay().size(), 0);
    ASSERT_FALSE(rule.until().isValid());

    RRules rule2 = RRules::fromString("FREQ=YEARLY;INTERVAL=5");
    ASSERT_TRUE(rule2.isValid());
    ASSERT_EQ(rule2.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule2.interval(), 5);
    ASSERT_EQ(rule2.byDay().size(), 0);
    ASSERT_EQ(rule2.count(), 0);
    ASSERT_EQ(rule2.dayRankInMonth(), 0);
    ASSERT_EQ(rule2.byMonthDay().size(), 0);
    ASSERT_FALSE(rule2.until().isValid());

    RRules rule3 = RRules::fromString("FREQ=YEARLY;BYMONTH=3;BYMONTHDAY=13");
    ASSERT_TRUE(rule3.isValid());
    ASSERT_EQ(rule3.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule3.interval(), 1);
    ASSERT_EQ(rule3.byDay().size(), 0);
    ASSERT_EQ(rule3.count(), 0);
    ASSERT_EQ(rule3.dayRankInMonth(), 0);
    ASSERT_EQ(rule3.byMonthDay().size(), 1);
    ASSERT_EQ(rule3.byMonth(), 3);
    ASSERT_FALSE(rule3.until().isValid());

    RRules rule4 = RRules::fromString("FREQ=YEARLY;BYMONTH=3;BYDAY=2TU");
    ASSERT_TRUE(rule4.isValid());
    ASSERT_EQ(rule4.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule4.interval(), 1);
    ASSERT_EQ(rule4.byDay().size(), 1);
    ASSERT_EQ(rule4.count(), 0);
    ASSERT_EQ(rule4.dayRankInMonth(), 2);
    ASSERT_EQ(rule4.byMonthDay().size(), 0);
    ASSERT_EQ(rule4.byMonth(), 3);
    ASSERT_FALSE(rule4.until().isValid());

    RRules rule5 = RRules::fromString("FREQ=YEARLY;COUNT=2");
    ASSERT_TRUE(rule5.isValid());
    ASSERT_EQ(rule5.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule5.interval(), 1);
    ASSERT_EQ(rule5.byDay().size(), 0);
    ASSERT_EQ(rule5.count(), 2);
    ASSERT_EQ(rule5.dayRankInMonth(), 0);
    ASSERT_EQ(rule5.byMonthDay().size(), 0);
    ASSERT_EQ(rule5.byMonth(), 0);
    ASSERT_FALSE(rule5.until().isValid());

    QDateTime expected(QDate(2024, 2, 9), QTime(21, 10, 0), QTimeZone::utc());
    RRules    rule6 = RRules::fromString("FREQ=YEARLY;UNTIL=20240209T211000Z");
    ASSERT_TRUE(rule6.isValid());
    ASSERT_EQ(rule6.freq(), RRules::Freq::kYearly);
    ASSERT_EQ(rule6.interval(), 1);
    ASSERT_EQ(rule6.byDay().size(), 0);
    ASSERT_EQ(rule6.count(), 0);
    ASSERT_EQ(rule6.dayRankInMonth(), 0);
    ASSERT_EQ(rule6.byMonthDay().size(), 0);
    ASSERT_EQ(rule6.byMonth(), 0);
    ASSERT_EQ(rule6.until(), expected);
}

TEST(RRules, RRulesInterpretationFreq) {
    RRules rule = RRules::fromString("FREQ=DAILY");
    ASSERT_EQ(rule.freq(), RRules::Freq::kDaily);

    rule = RRules::fromString("FREQ=WEEKLY");
    ASSERT_EQ(rule.freq(), RRules::Freq::kWeekly);

    rule = RRules::fromString("FREQ=MONTHLY");
    ASSERT_EQ(rule.freq(), RRules::Freq::kMonthly);

    rule = RRules::fromString("FREQ=YEARLY");
    ASSERT_EQ(rule.freq(), RRules::Freq::kYearly);

    rule = RRules::fromString("FREQ=");
    ASSERT_EQ(rule.freq(), RRules::Freq::kNone);

    rule = RRules::fromString("FREQ=avegredg");
    ASSERT_EQ(rule.freq(), RRules::Freq::kNone);
}

TEST(RRules, RRulesInterpretationCount) {
    RRules rule = RRules::fromString("COUNT=-1");
    ASSERT_EQ(rule.count(), 0);

    rule = RRules::fromString("COUNT=20");
    ASSERT_EQ(rule.count(), 20);

    rule = RRules::fromString("COUNT=1000");
    ASSERT_EQ(rule.count(), 999);

    rule = RRules::fromString("COUNT=");
    ASSERT_EQ(rule.count(), 0);

    rule = RRules::fromString("COUNT=aaafff");
    ASSERT_EQ(rule.count(), 0);
}

TEST(RRules, RRulesInterpretationInterval) {
    RRules rule = RRules::fromString("INTERVAL=-1");
    ASSERT_EQ(rule.interval(), 1);

    rule = RRules::fromString("INTERVAL=20");
    ASSERT_EQ(rule.interval(), 20);

    rule = RRules::fromString("INTERVAL=1000");
    ASSERT_EQ(rule.interval(), 999);

    rule = RRules::fromString("INTERVAL=");
    ASSERT_EQ(rule.interval(), 1);

    rule = RRules::fromString("INTERVAL=adrfg");
    ASSERT_EQ(rule.interval(), 1);
}

TEST(RRules, RRulesInterpretationUntil) {
    RRules rule = RRules::fromString("UNTIL=20240209T211000Z");
    ASSERT_EQ(rule.until(), QDateTime(QDate(2024, 2, 9), QTime(21, 10, 00), QTimeZone::utc()));

    rule = RRules::fromString("UNTIL=");
    ASSERT_EQ(rule.until(), QDateTime());

    rule = RRules::fromString("UNTIL=5362488g555522233a3");
    ASSERT_EQ(rule.until(), QDateTime());

    rule = RRules::fromString("UNTIL=20240209T2110");
    ASSERT_EQ(rule.until(), QDateTime());
}

TEST(RRules, RRulesInterpretationByDay) {
    RRules rule = RRules::fromString("BYDAY=MO,TU,FR");
    auto   days = rule.byDay();
    ASSERT_EQ(days.size(), 3);
    ASSERT_TRUE(days.contains(RRules::Day::kMonday));
    ASSERT_TRUE(days.contains(RRules::Day::kTuesday));
    ASSERT_TRUE(days.contains(RRules::Day::kFriday));

    rule = RRules::fromString("BYDAY=");
    days = rule.byDay();
    ASSERT_EQ(days.size(), 0);

    rule = RRules::fromString("BYDAY=hbajc");
    days = rule.byDay();
    ASSERT_EQ(days.size(), 0);

    rule = RRules::fromString("BYDAY=2MO");
    days = rule.byDay();
    ASSERT_EQ(days.size(), 1);
    ASSERT_TRUE(days.contains(RRules::Day::kMonday));
    ASSERT_EQ(rule.dayRankInMonth(), 2);

    rule = RRules::fromString("BYDAY=7MO");
    days = rule.byDay();
    ASSERT_EQ(days.size(), 1);
    ASSERT_TRUE(days.contains(RRules::Day::kMonday));
    ASSERT_EQ(rule.dayRankInMonth(), 0);
}

TEST(RRules, RRulesInterpretationByMonthDay) {
    RRules rule      = RRules::fromString("BYMONTHDAY=2,5,32");
    auto   monthDays = rule.byMonthDay();
    ASSERT_EQ(monthDays.size(), 2);
    ASSERT_TRUE(monthDays.contains(2));
    ASSERT_TRUE(monthDays.contains(5));

    rule      = RRules::fromString("BYMONTHDAY=");
    monthDays = rule.byMonthDay();
    ASSERT_EQ(monthDays.size(), 0);

    rule      = RRules::fromString("BYMONTHDAY=blabla");
    monthDays = rule.byMonthDay();
    ASSERT_EQ(monthDays.size(), 0);
}

TEST(RRules, RRulesInterpretationByMonth) {
    RRules rule = RRules::fromString("BYMONTH=2");
    ASSERT_EQ(rule.byMonth(), 2);

    rule = RRules::fromString("BYMONTH=");
    ASSERT_EQ(rule.byMonth(), 0);

    rule = RRules::fromString("BYMONTH=13");
    ASSERT_EQ(rule.byMonth(), 0);

    rule = RRules::fromString("BYMONTH=blabla");
    ASSERT_EQ(rule.byMonth(), 0);
}
