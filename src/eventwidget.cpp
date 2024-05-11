#include "./ui_eventwidget.h"
#include "eventwidget.h"

EventWidget::EventWidget(QSharedPointer<Event> event, QWidget *parent)
    : QWidget(parent), ui(new Ui::EventWidget) {
    ui->setupUi(this);
    _event = event;

    ui->reccurentIcon->setScaledContents(true);
    ui->content->setText(_event->content());
    ui->content->setWordWrap(true);
    ui->time->setText(formatDates(_event->startTime(), _event->endTime()));
    ui->reccurentIcon->setVisible(event->isReccurent());
}

EventWidget::~EventWidget() { delete ui; }

void EventWidget::setFont(const QFont &newFont) {
    auto bold = newFont;
    bold.setBold(true);
    ui->content->setFont(bold);

    ui->time->setFont(newFont);
    auto heigth = QFontMetrics(newFont).height();
    ui->reccurentIcon->setMinimumSize(heigth, heigth);
    ui->reccurentIcon->setMaximumSize(heigth, heigth);
}

QString EventWidget::formatDates(const QDateTime &start, const QDateTime &end) const {
    QString res;
    res += start.time().toString("HH:mm");
    res += "-";
    res += end.time().toString("HH:mm");
    return res;
}
