#include "./ui_fulldayswidget.h"
#include "fulldayswidget.h"

FullDaysWidget::FullDaysWidget(int numberOfDays, QDate firstDayDate, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::FullDaysWidget),
      _numberOfDays(numberOfDays),
      _firstDay(firstDayDate) {
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setStyleSheet("background-color: transparent;");
    _daysWidth = QVector<int>(numberOfDays, 10);
}

FullDaysWidget::~FullDaysWidget() { delete ui; }

void FullDaysWidget::insertEvent(QSharedPointer<Event> newEvent) {
    if (!newEvent->isFullDays()) {
        return;
    }
    if (std::max(newEvent->startTime().date(), _firstDay) >
        std::min(newEvent->endTime().date(), _firstDay.addDays(_numberOfDays))) {
        return;
    }

    auto widget = new EventDayWidget(newEvent->content(), newEvent->startTime().time(),
                                     newEvent->endTime().time(), newEvent->isReccurent(),
                                     newEvent->uid(), newEvent->isFullDays());
    widget->setParent(ui->widget);
    _children.push_back(QPair(widget, newEvent));
}

void FullDaysWidget::resizeEvent(QResizeEvent *event) {
    auto height        = QFontMetrics(_font).height() * 1.2;
    auto widthDay      = this->width() / _numberOfDays;
    auto [poses, size] = getFullDayEventPos();
    for (auto elem : poses) {
        elem.event->setWidthInDay(
            sumDayWidthbetween(elem.dayOfWeek, elem.dayOfWeek + elem.duration));
        elem.event->setPeriodHeight(height);
        elem.event->move(sumDayWidthbefore(elem.dayOfWeek), elem.layer * height);
    }
    this->setFixedHeight(height * (size - 1));
    update();
}

void FullDaysWidget::setFont(QFont font) {
    _font = font;
    for (auto elem : _children) {
        elem.first->setFont(font);
    }
    update();
}

QSize FullDaysWidget::sizeHint() const {
    auto [poses, size] = getFullDayEventPos();
    return QSize(100, QFontMetrics(_font).height() * 1.2 * size);
}

void FullDaysWidget::setDaysWidth(QVector<int> widths) {
    if (widths.size() != _numberOfDays) return;
    _daysWidth = widths;
    update();
}

void FullDaysWidget::updateDayWidth(int width, int dayIndex) {
    if (dayIndex < 0 || dayIndex >= _numberOfDays) return;
    _daysWidth[dayIndex] = width;
    update();
}

std::tuple<QVector<FullDaysWidget::FullDayEventPos>, int> FullDaysWidget::getFullDayEventPos()
    const {
    QVector<FullDayEventPos> res;
    QVector<QVector<bool>>   occupancy{QVector<bool>(_numberOfDays, false)};

    for (auto eventPair : _children) {
        auto event    = eventPair.second;
        auto firstDay = std::max(event->startTime().date(), _firstDay);
        auto lastDay  = std::min(event->endTime().date(), _firstDay.addDays(_numberOfDays));

        // qDebug() << firstDay << lastDay;

        int  dayIndex = _firstDay.daysTo(firstDay);
        int  duration = firstDay.daysTo(lastDay);
        bool flag     = true;
        int  layer    = 0;
        do {
            flag = true;
            for (int i = 0; i < duration; ++i) {
                if (occupancy[layer][i + dayIndex]) {
                    layer++;
                    occupancy.push_back(QVector<bool>(_numberOfDays, false));
                    flag = false;
                    break;
                }
            }
        } while (!flag && layer < 100);
        if (flag) {
            // qDebug() << layer << dayIndex;
            for (int i = 0; i < duration; ++i) {
                occupancy[layer][i + dayIndex] = true;
            }
            res.push_back(FullDayEventPos{eventPair.first, dayIndex, layer, duration});
        }
    }
    /*for (int i = 0; i < occupancy.size(); ++i) {
        qDebug() << occupancy[i];
    }*/
    return std::make_tuple(res, occupancy.size());
}

int FullDaysWidget::sumDayWidthbefore(int day) {
    int res = 0;
    for (int i = 0; i < day; ++i) {
        res += _daysWidth[i];
    }
    return res;
}

int FullDaysWidget::sumDayWidthbetween(int firstDay, int lastDay) {
    int res = 0;
    for (int i = firstDay; i < lastDay; ++i) {
        res += _daysWidth[i];
    }
    return res;
}
