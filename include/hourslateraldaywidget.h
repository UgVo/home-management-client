#ifndef HOURSLATERALDAYWIDGET_H
#define HOURSLATERALDAYWIDGET_H

#include <QFont>
#include <QPainter>
#include <QTime>
#include <QWidget>

namespace Ui {
class HoursLateralDayWidget;
}

class HoursLateralDayWidget : public QWidget {
    Q_OBJECT

    static const int widthBrush   = 1;
    static const int bulletRadius = 4;

   public:
    explicit HoursLateralDayWidget(QWidget *parent = nullptr);
    ~HoursLateralDayWidget();
    QSize sizeHint() const override;
    void  setFont(const QFont &newFont);
    int   getCurrentTimePosition() const;

   signals:
    void widthChanged(int newWidth);

   protected:
    void paintEvent(QPaintEvent *event) override;
    int  getInterval() const;

   private:
    Ui::HoursLateralDayWidget *ui;

    QFont _font;
};

#endif  // HOURSLATERALDAYWIDGET_H
