#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "daywidget.h"
#include "fillerhourcolumn.h"
#include "hourslateraldaywidget.h"

namespace Ui {
class WeekView;
}

class WeekView : public QWidget {
    Q_OBJECT

    static const int widthBrush = 1;

   public:
    explicit WeekView(QWidget *parent = nullptr, int weekNumber = QDate::currentDate().weekNumber(),
                      int numberOfDays = 7);
    ~WeekView();

    void setFont(const QFont &newFont);

    bool insertEvent(QSharedPointer<Event> newEvent);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

   private:
    QDate getFirstMondayOfYear(int year);

    Ui::WeekView          *ui;
    int                    _numberOfDays;
    QVector<DayWidget *>   _days;
    QVector<QLabel *>      _headers;
    HoursLateralDayWidget *_hoursLateral;
    QDate                  _firstDay;
    QDate                  _lastDay;
    QFont                  _font;

    FillerHourColumn *_fillerHourColumn;

    QVector<QWidget *>                 _fullDayHolder;
    QVector<QVector<EventDayWidget *>> _fullDayEvents;
};

#endif  // WEEKVIEW_H
