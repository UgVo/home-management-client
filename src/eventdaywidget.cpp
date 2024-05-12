#include "./ui_eventdaywidget.h"
#include "eventdaywidget.h"

EventDayWidget::EventDayWidget(QString content, QTime start, QTime end, bool isReccurent,
                               QString uidEvent, QWidget *parent)
    : QWidget(parent), ui(new Ui::EventDayWidget) {
    ui->setupUi(this);

    ui->reccurentIcon->setScaledContents(true);
    ui->content->setText(content);
    ui->content->setWordWrap(true);
    ui->time->setText(formatDates(start, end));
    ui->timeAlt->setText(formatDates(start, end));
    ui->reccurentIcon->setVisible(isReccurent);

    resizeToDuration(start, end);
}

EventDayWidget::~EventDayWidget() { delete ui; }

void EventDayWidget::setFont(const QFont &newFont) {
    auto bold = newFont;
    bold.setBold(true);
    ui->content->setFont(bold);

    ui->time->setFont(newFont);
    ui->timeAlt->setFont(newFont);
    auto height = QFontMetrics(newFont).height();
    ui->reccurentIcon->setMinimumSize(height, height);
    ui->reccurentIcon->setMaximumSize(height, height);

    if (height * 2 + layout()->contentsMargins().bottom() + layout()->contentsMargins().top() +
            layout()->spacing() >
        this->height()) {
        ui->time->hide();
        ui->timeAlt->show();
    } else {
        ui->time->show();
        ui->timeAlt->hide();
    }
}

void EventDayWidget::setPeriodHeight(const int height) {}

QString EventDayWidget::formatDates(const QTime &start, const QTime &end) const {
    QString res;
    if (!start.isValid() && !end.isValid()) return res;
    if (start.isValid()) {
        res += start.toString("HH:mm");
    } else {
        res += "...";
    }
    res += " → ";
    if (end.isValid()) {
        res += end.toString("HH:mm");
    } else {
        res += "...";
    }
    return res;
}

void EventDayWidget::resizeToDuration(QTime start, QTime end) {
    QTime startTime = start.isValid() ? start : QTime(0, 0);
    QTime endTime   = end.isValid() ? end : QTime(23, 29);

    int lenght      = ceil((endTime.msecsSinceStartOfDay() - startTime.msecsSinceStartOfDay()) /
                           float(QTime(0, 15).msecsSinceStartOfDay()));

    qDebug() << lenght;
    setFixedHeight(_periodHeight * lenght);
}
