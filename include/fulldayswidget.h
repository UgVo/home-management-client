#ifndef FULLDAYSWIDGET_H
#define FULLDAYSWIDGET_H

#include <QVector>
#include <QWidget>

#include "eventdaywidget.h"

namespace Ui {
class FullDaysWidget;
}

class FullDaysWidget : public QWidget {
    Q_OBJECT

    static const int widthBrush = 1;

    struct FullDayEventPos {
        EventDayWidget *event     = nullptr;
        int             dayOfWeek = 0;
        int             layer     = 0;
        int             duration  = 0;
    };

   public:
    explicit FullDaysWidget(int numberOfDays, QDate firstDayDate, QWidget *parent = nullptr);
    ~FullDaysWidget();

    void  insertEvent(QSharedPointer<Event> newEvent);
    void  resizeEvent(QResizeEvent *event) override;
    void  setFont(QFont font);
    QSize sizeHint() const override;
    void  setDaysWidth(QVector<int> widths);

   public slots:
    void updateDayWidth(int width, int dayIndex);

   private:
    std::tuple<QVector<FullDayEventPos>, int> getFullDayEventPos() const;

    int sumDayWidthbefore(int day);
    int sumDayWidthbetween(int firstDay, int lastDay);

    Ui::FullDaysWidget *ui;
    int                 _numberOfDays;
    QDate               _firstDay;
    QFont               _font;
    QVector<int>        _daysWidth;

    QVector<QPair<EventDayWidget *, QSharedPointer<Event>>> _children;
};

#endif  // FULLDAYSWIDGET_H
