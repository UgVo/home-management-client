#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QLabel>
#include <QWidget>

#include "daywidget.h"
#include "hourslateraldaywidget.h"

namespace Ui {
class WeekView;
}

class WeekView : public QWidget {
    Q_OBJECT

   public:
    explicit WeekView(QWidget *parent = nullptr, int weekNumber = QDate::currentDate().weekNumber(),
                      int numberOfDays = 7);
    ~WeekView();

    void setFont(const QFont &newFont);

    bool insertEvent(QSharedPointer<Event> newEvent);
    void resizeEvent(QResizeEvent *event) override;

   private:
    QDate getFirstMondayOfYear(int year);

    Ui::WeekView          *ui;
    int                    _numberOfDays;
    QVector<DayWidget *>   _days;
    QVector<QLabel *>      _headers;
    QWidget               *_fillerHourColumn;
    HoursLateralDayWidget *_hoursLateral;
    QDate                  _firstDay;
    QDate                  _lastDay;
    QFont                  _font;
};

#endif  // WEEKVIEW_H
