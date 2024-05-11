#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QCursor>
#include <QFontMetrics>
#include <QPlainTextDocumentLayout>
#include <QTextEdit>
#include <QWidget>

#include "event.h"

namespace Ui {
class EventWidget;
}

class EventWidget : public QWidget {
    Q_OBJECT

   public:
    explicit EventWidget(QSharedPointer<Event> event, QWidget *parent = nullptr);
    ~EventWidget();
    void setFont(const QFont &newFont);

   private:
    QString formatDates(const QDateTime &start, const QDateTime &end) const;

    Ui::EventWidget      *ui;
    QSharedPointer<Event> _event;
};

#endif  // EVENTWIDGET_H
