#include "./ui_weekview.h"
#include "weekview.h"

WeekView::WeekView(QWidget *parent, int weekNumber, int numberOfDays)
    : QWidget(parent), ui(new Ui::WeekView), _numberOfDays(numberOfDays) {
    ui->setupUi(this);
    _hoursLateral = new HoursLateralDayWidget();
    QLocale      locale(QLocale("fr_FR"));
    QHBoxLayout *contentLayout =
        dynamic_cast<QHBoxLayout *>(ui->scrollAreaWidgetContents->layout());

    _fullDayEvents    = QVector<QVector<EventDayWidget *>>(_numberOfDays);
    _fillerHourColumn = new FillerHourColumn();

    _firstDay = getFirstMondayOfYear(QDate::currentDate().year()).addDays(7 * (weekNumber - 1));
    _lastDay  = _firstDay.addDays(6);

    _fulldayswidget = new FullDaysWidget(numberOfDays, _firstDay);

    _dayRatios = QVector<float>(_numberOfDays, 1.0f);

    contentLayout->addWidget(_hoursLateral);
    ui->layoutwidgetTop->insertWidget(0, _fillerHourColumn);
    ui->topLayout->addWidget(_fulldayswidget);

    QDate current = _firstDay;
    for (int i = 0; i < _numberOfDays; ++i) {
        _headers.emplace_back(new QLabel(locale.toString(current.addDays(i), "ddd dd MMMM")));
        _headers.last()->setAlignment(Qt::AlignCenter);

        _days.emplace_back(new DayWidget(i));
        _fullDayHolder.emplace_back(new QWidget());
        _fullDayHolder.last()->setLayout(new QVBoxLayout());
        _fullDayHolder.last()->layout()->setContentsMargins(0, 0, 0, 0);
        _fullDayHolder.last()->layout()->setSpacing(1);

        ui->headerLayout->addWidget(_headers.last());
        // ui->fullDayLayout->addWidget(_fullDayHolder.last());
        contentLayout->addWidget(_days.last());
        QObject::connect(_days.last(), &DayWidget::widthChanged, _fulldayswidget,
                         &FullDaysWidget::updateDayWidth);
    }

    QObject::connect(_hoursLateral, &HoursLateralDayWidget::widthChanged,
                     [this](int newWidth) { _fillerHourColumn->setFixedWidth(newWidth); });

    ui->scrollArea->setStyleSheet("background-color: transparent;");
}

WeekView::~WeekView() { delete ui; }

void WeekView::setFont(const QFont &newFont) {
    _font = newFont;

    for (auto elem : _headers) {
        elem->setFont(_font);
    }

    for (auto elem : _days) {
        elem->setFont(_font);
    }

    for (auto day : _fullDayEvents) {
        for (auto elem : day) {
            elem->setFont(_font);
        }
    }

    _hoursLateral->setFont(_font);
    _fillerHourColumn->setFont(_font);
    _fulldayswidget->setFont(_font);
    update();
}

bool WeekView::insertEvent(QSharedPointer<Event> newEvent) {
    if (!newEvent->isValid()) {
        qWarning() << "Attempt to add an invalid event: \n" << *newEvent.data();
        return false;
    }
    auto _firstDayTime = QDateTime(_firstDay, QTime(0, 0), QTimeZone(3600));
    auto _lastDayTime  = QDateTime(_lastDay, QTime(23, 59), QTimeZone(3600));

    auto first = std::max(newEvent->startTime(), _firstDayTime);
    auto last  = std::min(newEvent->endTime(), _lastDayTime);
    if (last < first) {
        qWarning() << "Attempt to add event out of bounds: \n" << *newEvent.data();
        return false;
    }

    if (newEvent->isFullDays()) {
        _fulldayswidget->insertEvent(newEvent);
        _fillerHourColumn->hideWords(false);
        return true;
    }

    for (auto current = first.date(); current <= last.date(); current = current.addDays(1)) {
        QTime startTime = current == first.date() ? first.time() : QTime();
        QTime endTime   = current == last.date() ? last.time() : QTime();

        auto widget =
            new EventDayWidget(newEvent->content(), startTime, endTime, newEvent->isReccurent(),
                               newEvent->uid(), newEvent->isFullDays());
        _days[current.dayOfWeek() - 1]->addEventDayWidget(widget);
    }
    return true;
}

void WeekView::resizeEvent(QResizeEvent *event) {
    QVector<int> daysWidths(_numberOfDays, 0);

    if (_fullDayEvents.empty()) {
        _fulldayswidget->hide();
        _fillerHourColumn->hideWords(true);
    } else {
        _fulldayswidget->show();
        auto height = QFontMetrics(_font).height();
        for (int i = 0; i < _fullDayEvents.size(); ++i) {
            for (auto elem : _fullDayEvents[i]) {
                elem->setFixedHeight(height * 1.2);
                elem->setFixedWidth(_days[i]->getFullWidth() - DayWidget::lateralMargin);
            }
        }
    }

    if (!_dayRatios.empty()) {
        auto width = ui->scrollAreaWidgetContents->width() - _hoursLateral->width() -
                     ui->scrollAreaWidgetContents->layout()->spacing();
        float max = std::accumulate(_dayRatios.begin(), _dayRatios.end(), 0.0f);
        for (int i = 0; i < _numberOfDays; ++i) {
            if (_dayRatios[i] != 1) {
                int widthDay  = width * (_dayRatios[i] / max);
                daysWidths[i] = widthDay;
                _days[i]->setFixedWidth(widthDay);
                _headers[i]->setFixedWidth(widthDay);
            }
        }
    }

    _fillerHourColumn->setVerticalShift(ui->headerWidget->height());
    ui->scrollArea->ensureVisible(0, _hoursLateral->getCurrentTimePosition(), 0, 100);
}

void WeekView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen     pen(QColor("#AEAEAE"), widthBrush);

    int left, top;

    this->layout()->getContentsMargins(&left, &top, nullptr, nullptr);
    int hShift = _hoursLateral->sizeHint().width() + left;
    int height = this->height();
    int width  = ui->topWidget->width() - _hoursLateral->sizeHint().width();

    painter.setPen(pen);

    for (int i = 1; i < _numberOfDays; ++i) {
        hShift += _days[i - 1]->width();
        painter.drawLine(hShift, top, hShift, height);
    }

    painter.drawLine(_hoursLateral->sizeHint().width() + left, top + ui->topWidget->height(),
                     this->width(), top + ui->topWidget->height());
}

void WeekView::setRatioDays(QVector<float> ratios) {
    if (ratios.size() != _numberOfDays) return;
    for (auto ratio : ratios) {
        if (ratio < 1) {
            return;
        }
    }
    _dayRatios = ratios;
}

void WeekView::updateView() {
    // TODO update _dayRatios
    int todayWeekIndex = QDate::currentDate().dayOfWeek() - 1;
    qDebug() << todayWeekIndex << _numberOfDays << _dayRatios.size();
    for (int i = 0; i < _dayRatios.size(); ++i) {
        if (i < todayWeekIndex) {
            _dayRatios[i] = 1.0f;
        } else if (i == todayWeekIndex) {
            _dayRatios[i] = 2.0f;
        } else if (i <= todayWeekIndex + 2) {
            _dayRatios[i] = 1.5f;
        } else {
            _dayRatios[i] = 1.0f;
        }
    }
    update();
}

QDate WeekView::getFirstMondayOfYear(int year) {
    QDate dt(year, 1, 1);

    while (dt.dayOfWeek() != Qt::Monday) {
        dt = dt.addDays(1);
    }
    return dt;
}