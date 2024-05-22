#ifndef FILLERHOURCOLUMN_H
#define FILLERHOURCOLUMN_H

#include <QFont>
#include <QPainter>
#include <QWidget>

namespace Ui {
class FillerHourColumn;
}

class FillerHourColumn : public QWidget {
    Q_OBJECT

    static const int bulletRadius = 4;
    static const int widthBrush   = 1;

   public:
    explicit FillerHourColumn(QWidget *parent = nullptr);
    ~FillerHourColumn();
    void setFont(QFont newfont);
    void setVerticalShift(int vshift);

   protected:
    void paintEvent(QPaintEvent *event) override;

   private:
    Ui::FillerHourColumn *ui;

    QFont _font;
    int   _vshit;
};

#endif  // FILLERHOURCOLUMN_H
