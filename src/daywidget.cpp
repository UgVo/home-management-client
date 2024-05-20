#include "daywidget.h"
#include "ui_daywidget.h"

DayWidget::DayWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DayWidget) {
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    _fullwidth = 100;
}

DayWidget::~DayWidget() { delete ui; }

void DayWidget::addEventDayWidget(EventDayWidget *eventDayWidget) {
    _childrenMap.insert(eventDayWidget->uid(), eventDayWidget);
    eventDayWidget->setParent(this);
    eventDayWidget->setPeriodHeight(getInterval() / 4);
    eventDayWidget->show();
}

QSize DayWidget::sizeHint() const {
    auto height = QFontMetrics(_font).height();
    return QSize(100, 24 * 4 * height * 1.2);
}

void DayWidget::setFont(const QFont &newFont) {
    _font = newFont;
    for (auto elem : _childrenMap) {
        elem->setFont(newFont);
    }
    update();
}

int DayWidget::getFullWidth() const { return _fullwidth; }

void DayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QTime    current = QTime::currentTime();

    int height   = this->height();
    int width    = this->width();
    int interval = getInterval();

    float currentTimeY = current.hour() * interval + (interval / 60.0 * current.minute());

    QPen   pen(QColor("#AEAEAE"), widthBrush);
    QPen   penBullet(QColor("#4277FF"), widthBrush * 2);
    QBrush brushBullet(QColor("#4277FF"));
    painter.setPen(pen);

    for (int i = 0; i < 24; ++i) {
        painter.drawLine(0, i * interval, width, i * interval);
    }
    painter.drawLine(0, height - 1, width, height - 1);

    painter.setPen(penBullet);
    painter.setBrush(brushBullet);
    painter.drawLine(0, currentTimeY, width, currentTimeY);
}

void DayWidget::resizeEvent(QResizeEvent *event) {
    auto quatersSections = computeWidthEvents();
    _fullwidth           = (this->width() - lateralMargin * 2);

    for (auto it = quatersSections.begin(); it != quatersSections.end(); ++it) {
        if (it->empty()) continue;
        int size = it->size();
        for (int i = 0; i < size; ++i) {
            auto widget = _childrenMap.value(it->value(i), nullptr);
            if (widget != nullptr) {
                auto start = widget->start().msecsSinceStartOfDay();
                auto shift =
                    getInterval() * (start / QTime(1, 0).msecsSinceStartOfDay()) + widthBrush;

                widget->setPeriodHeight(getInterval());
                widget->setWidthInDay(_fullwidth, size);
                widget->move(0, shift);
                widget->slideLateral(_fullwidth, lateralMargin, size, i);
            }
        }
    }
}

int DayWidget::getInterval() { return int(std::round(this->height() / 24.0)); }

QVector<QStringList> DayWidget::computeWidthEvents() {
    QVector<QStringList> res = QVector<QStringList>(24 * 4);

    int duration15min = QTime(0, 15).msecsSinceStartOfDay();
    for (auto widget : _childrenMap) {
        auto start = widget->start().isValid() ? widget->start().msecsSinceStartOfDay()
                                               : QTime(0, 0).msecsSinceStartOfDay();
        auto end   = widget->end().isValid() ? widget->end().msecsSinceStartOfDay()
                                             : QTime(23, 59).msecsSinceStartOfDay();
        for (int i = (start / duration15min); i < (end / duration15min); ++i) {
            res[i].push_back(widget->uid());
        }
    }
    return res;
}