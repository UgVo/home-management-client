#include "./ui_eventdaywidget.h"
#include "eventdaywidget.h"

EventDayWidget::EventDayWidget(QString content, QTime start, QTime end, bool isReccurent,
                               QString uidEvent, bool fullDay, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::EventDayWidget),
      _start(start),
      _end(end),
      _eventUID(uidEvent),
      _fullDay(fullDay) {
    ui->setupUi(this);

    _ratioWidth = 0;
    _order      = 0;
    _content    = content;

    ui->reccurentIcon->setScaledContents(true);
    ui->content->setText(content);
    ui->content->setWordWrap(true);
    ui->time->setText(formatDates(start, end));
    ui->timeAlt->setText(formatDates(start, end));
    ui->reccurentIcon->setVisible(isReccurent);

    this->setStyleSheet(
        "QWidget#widget{background-color : #c3c3c3;  border-radius :3px; border: 1px solid "
        "black;}");

    resizeToDuration();
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
}

void EventDayWidget::setPeriodHeight(const int height) {
    _periodHeight = height;
    resizeToDuration();
}

void EventDayWidget::setWidthInDay(const int dayWidth, const int ratio) {
    if (ratio > _ratioWidth) {
        _ratioWidth = ratio;
    }
    setFixedWidth(dayWidth / _ratioWidth);
}

void EventDayWidget::slideLateral(int dayWidth, int shift, int ratio, int order) {
    if (order > _order) {
        _order = order;
    }
    this->move(shift + _order * (dayWidth / _ratioWidth), this->pos().ry());
}

QString EventDayWidget::uid() const { return _eventUID; }
QTime   EventDayWidget::start() const { return _start; }
QTime   EventDayWidget::end() const { return _end; }
QString EventDayWidget::content() const { return _content; };

QSize EventDayWidget::sizeHint() const { return (QSize(100, _periodHeight)); }

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

void EventDayWidget::resizeToDuration() {
    if (_fullDay) {
        setFixedHeight(_periodHeight - 1);
        return;
    }
    QTime startTime = _start.isValid() ? _start : QTime(0, 0);
    QTime endTime   = _end.isValid() ? _end : QTime(23, 59);

    float lenght = (endTime.msecsSinceStartOfDay() - startTime.msecsSinceStartOfDay()) /
                   float(QTime(1, 0).msecsSinceStartOfDay());

    setFixedHeight(_periodHeight * std::ceil(lenght) - 1);
}

void EventDayWidget::resizeEvent(QResizeEvent *event) {
    auto height = QFontMetrics(ui->content->font()).height();
    if (_fullDay) {
        ui->time->hide();
        ui->timeAlt->hide();
        return;
    }

    if (height * 2 + ui->widget->layout()->contentsMargins().bottom() +
            ui->widget->layout()->contentsMargins().top() + ui->widget->layout()->spacing() >
        this->height()) {
        ui->time->hide();
        ui->timeAlt->show();
    } else {
        ui->time->show();
        ui->timeAlt->hide();
    }
}
