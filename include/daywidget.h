#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QPainter>
#include <QSizePolicy>
#include <QWidget>
#include <algorithm>

#include "eventdaywidget.h"

namespace Ui {
class DayWidget;
}

class DayWidget : public QWidget {
    Q_OBJECT

   public:
    static const int lateralMargin = 6;
    static const int widthBrush    = 1;

    explicit DayWidget(QWidget *parent = nullptr);
    ~DayWidget();
    void  addEventDayWidget(EventDayWidget *eventDayWidget);
    QSize sizeHint() const override;
    void  setFont(const QFont &newFont);
    int   getFullWidth() const;

   protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    int  getInterval() const;

    QVector<QStringList> computeWidthEvents();

   private:
    Ui::DayWidget *ui;

    QFont _font;
    int   _fullwidth;

    QMap<QString, EventDayWidget *> _childrenMap;
};

#endif  // DAYWIDGET_H
