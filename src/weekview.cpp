#include "./ui_weekview.h"
#include "weekview.h"

WeekView::WeekView(QWidget *parent, int weekNumber, int numberOfDays)
    : QWidget(parent), ui(new Ui::WeekView), _numberOfDays(numberOfDays) {
    ui->setupUi(this);
    _hoursLateral = new HoursLateralDayWidget();
    QLocale      locale(QLocale("fr_FR"));
    QHBoxLayout *contentLayout =
        dynamic_cast<QHBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    QGridLayout *headerLayout = dynamic_cast<QGridLayout *>(ui->headers->layout());

    _fullDayEvents = QVector<QVector<EventDayWidget *>>(_numberOfDays);

    _firstDay =
        getFirstMondayOfYear(QDate::currentDate().year()).addDays(_numberOfDays * (weekNumber - 1));
    _lastDay          = _firstDay.addDays(6);
    _fillerHourColumn = new QWidget();
    headerLayout->addWidget(_fillerHourColumn, 0, 0);
    headerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 1);
    contentLayout->addWidget(_hoursLateral);

    QDate current = _firstDay;
    for (int i = 0; i < _numberOfDays; ++i) {
        _headers.emplace_back(new QLabel(locale.toString(current.addDays(i), "ddd dd MMMM")));
        _headers.last()->setAlignment(Qt::AlignCenter);
        _days.emplace_back(new DayWidget());
        _fullDayLayouts.emplace_back(new QVBoxLayout());
        _fullDayLayouts.last()->setContentsMargins(0, 0, 0, 0);

        headerLayout->addWidget(_headers.last(), 0, 2 * i + 1);
        headerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 2 * i);
        headerLayout->addLayout(_fullDayLayouts.last(), 1, 2 * i + 1);
        contentLayout->addWidget(_days.last());
    }
    headerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, _numberOfDays * 2);

    QObject::connect(_hoursLateral, &HoursLateralDayWidget::widthChanged,
                     [this](int newWidth) { _fillerHourColumn->setFixedWidth(newWidth); });
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

    for (auto current = first.date(); current <= last.date(); current = current.addDays(1)) {
        QTime startTime = current == first.date() ? first.time() : QTime();
        QTime endTime   = current == last.date() ? last.time() : QTime();

        auto widget =
            new EventDayWidget(newEvent->content(), startTime, endTime, newEvent->isReccurent(),
                               newEvent->uid(), newEvent->isFullDays());
        if (newEvent->isFullDays()) {
            _fullDayEvents[current.dayOfWeek() - 1].push_back(widget);
            _fullDayLayouts[current.dayOfWeek() - 1]->addWidget(widget);
            _fullDayLayouts[current.dayOfWeek() - 1]->setAlignment(widget, Qt::AlignCenter);
        } else {
            _days[current.dayOfWeek() - 1]->addEventDayWidget(widget);
        }
    }
    return true;
}

void WeekView::resizeEvent(QResizeEvent *event) {
    auto width = ui->headers->width() - _hoursLateral->width();
    for (auto elem : _headers) {
        elem->setFixedWidth(width / _numberOfDays);
    }

    auto height = QFontMetrics(_font).height();
    for (int i = 0; i < _fullDayEvents.size(); ++i) {
        for (auto elem : _fullDayEvents[i]) {
            elem->setFixedHeight(height * 1.2);
            elem->setFixedWidth(_days[i]->getFullWidth());
        }
    }
}

QDate WeekView::getFirstMondayOfYear(int year) {
    QDate dt(year, 1, 1);

    while (dt.dayOfWeek() != Qt::Monday) {
        dt = dt.addDays(1);
    }
    return dt;
}