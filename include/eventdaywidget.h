#ifndef EVENTDAYWIDGET_H
#define EVENTDAYWIDGET_H

#include <QCursor>
#include <QFontMetrics>
#include <QPlainTextDocumentLayout>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "event.h"

namespace Ui {
class EventDayWidget;
}

class EventDayWidget : public QWidget {
    Q_OBJECT

   public:
    explicit EventDayWidget(QString content, QTime start, QTime end, bool isReccurent,
                            QString uidEvent, QWidget *parent = nullptr);
    ~EventDayWidget();
    void setFont(const QFont &newFont);

    /// @brief Set the height equivalent to a period of time (15 minutes)
    /// @param height height in pixel
    void setPeriodHeight(const int height);

   private:
    QString formatDates(const QTime &start, const QTime &end) const;
    void    resizeToDuration(const QTime start, const QTime end);

    Ui::EventDayWidget *ui;
    QString             _eventUID;
    int                 _periodHeight = 40;
};

#endif  // EVENTDAYWIDGET_H
