#ifndef EVENTDAYWIDGET_H
#define EVENTDAYWIDGET_H

#include <QCursor>
#include <QFontMetrics>
#include <QPlainTextDocumentLayout>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <limits>

#include "event.h"

namespace Ui {
class EventDayWidget;
}

class EventDayWidget : public QWidget {
    Q_OBJECT

   public:
    explicit EventDayWidget(QString content, QTime start, QTime end, bool isReccurent,
                            QString uidEvent, bool fullDay = false, QWidget *parent = nullptr);
    ~EventDayWidget();
    void setFont(const QFont &newFont);

    /// @brief Set the height equivalent to a period of time (15 minutes)
    /// @param height height in pixel
    void setPeriodHeight(const int height);

    void setWidthInDay(const int dayWidth, const int ratio = 1);
    void slideLateral(int dayWidth, int shift, int ratio, int order);

    QString uid() const;
    QTime   start() const;
    QTime   end() const;
    QString content() const;

    QSize sizeHint() const override;

   protected:
    QString formatDates(const QTime &start, const QTime &end) const;
    void    resizeToDuration();
    void    resizeEvent(QResizeEvent *event) override;

    Ui::EventDayWidget *ui;
    QString             _eventUID;
    int                 _periodHeight = 40;
    QTime               _start;
    QTime               _end;
    int                 _ratioWidth;
    int                 _order;
    QString             _content;
    bool                _fullDay;
};

#endif  // EVENTDAYWIDGET_H
