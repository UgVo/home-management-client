#ifndef HOME_MANAGEMENT_CLIENT_RRULES_H
#define HOME_MANAGEMENT_CLIENT_RRULES_H

#include <QDateTime>
#include <QSet>
#include <QString>
#include <algorithm>
#include <limits>

#include "utils.h"

class RRules {
   public:
    enum class Freq {
        kNone,
        kDaily,
        kWeekly,
        kMonthly,
        kYearly,
    };
    inline static QMap<QString, Freq> mapFreq = {{"DAILY", Freq::kDaily},
                                                 {"WEEKLY", Freq::kWeekly},
                                                 {"MONTHLY", Freq::kMonthly},
                                                 {"YEARLY", Freq::kYearly}};
    enum class Day { kNone, kMonday, kTuesday, kWednesday, kThursday, kFriday, kSaturday, kSunday };
    inline static QMap<QString, Day> mapDay = {{"MO", Day::kMonday},    {"TU", Day::kTuesday},
                                               {"WE", Day::kWednesday}, {"TH", Day::kThursday},
                                               {"FR", Day::kFriday},    {"SA", Day::kSaturday},
                                               {"SU", Day::kSunday}};
    inline static QMap<Day, int> mapDayNum = {
        {Day::kMonday, 0}, {Day::kTuesday, 1},  {Day::kWednesday, 2}, {Day::kThursday, 3},
        {Day::kFriday, 4}, {Day::kSaturday, 5}, {Day::kSunday, 6}};

    static RRules fromString(const QString rrules);

    Freq freq() const;
    void setFreq(const Freq freq);
    int count() const;
    void setCount(const int count);
    int interval() const;
    void setInterval(const int interval);
    QDateTime until() const;
    void setUntil(const QDateTime &&until);
    QSet<Day> byDay() const;
    void setByDay(const QSet<Day> &&byDay);
    void insertDay(const Day day);
    int dayRankInMonth() const;
    void setDayRankInMonth(const int dayRank);
    QSet<int> byMonthDay() const;
    void setByMonthDay(const QSet<int> byMonthDay);
    void insertMonthDay(const int monthDay);
    int byMonth() const;
    void setByMonth(const int byMonth);
    bool isValid() const;
    QString toString() const;

   private:
    Freq _freq = Freq::kNone;
    int _count = 0;
    int _interval = 1;
    QDateTime _until = QDateTime();
    QSet<Day> _byDay;         // Only when freq = kDaily or freq = kWeekly
    int _dayRankInMonth = 0;  // Only for freq = monthly, and a specific day of the week is
                              // chosen (ex : first tuesday of the month)
    QSet<int> _byMonthDay;    // Only when freq = monthly || yearly, value between -1 and 31, -1
                              // being the last day of the month
    int _byMonth = 0;         // Only when freq = kYearly in combinaison with byMonthDay
                              // (with size = 1)
    bool _valid = false;
    QString _str;
};

#endif  // HOME_MANAGEMENT_CLIENT_RRULES_H