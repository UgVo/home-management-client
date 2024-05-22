#include "hourslateraldaywidget.h"
#include "ui_hourslateraldaywidget.h"

HoursLateralDayWidget::HoursLateralDayWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::HoursLateralDayWidget) {
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
}

HoursLateralDayWidget::~HoursLateralDayWidget() { delete ui; }

QSize HoursLateralDayWidget::sizeHint() const {
    auto height = QFontMetrics(_font).height();
    auto width  = QFontMetrics(_font).horizontalAdvance(tr("journée")) + bulletRadius * 2;
    return QSize(width, 24 * 4 * height * 1.2);
}

void HoursLateralDayWidget::setFont(const QFont &newFont) {
    _font = newFont;
    setFixedWidth(QFontMetrics(_font).horizontalAdvance(tr("journée")) + bulletRadius * 2);
    update();
}

int HoursLateralDayWidget::getCurrentTimePosition() const {
    QTime current  = QTime::currentTime();
    int   interval = getInterval();

    return current.hour() * interval + (interval / 60.0 * current.minute());
}

void HoursLateralDayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect    = QRect(0, 0, 100, 50);
    QTime current = QTime::currentTime();

    int height     = this->height();
    int heightText = QFontMetrics(_font).height();
    int widthText  = QFontMetrics(_font).horizontalAdvance(tr("journée"));
    int interval   = getInterval();

    float bulletCenter = current.hour() * interval + (interval / 60.0 * current.minute());

    QPen   penText(QColor("#AEAEAE"), widthBrush);
    QPen   penBullet(QColor("#4277FF"), widthBrush);
    QBrush brushBullet(QColor("#4277FF"));
    painter.setPen(penText);
    painter.setFont(_font);

    auto shiftV = heightText / 2;

    for (int i = 1; i < 24; ++i) {
        painter.drawText(0, i * interval - shiftV, widthText, heightText, Qt::AlignRight,
                         QString("%1:00").arg(i));
    }
    painter.drawText(0, 0, widthText, heightText, Qt::AlignRight, QString("00:00"));

    painter.setPen(penBullet);
    painter.setBrush(brushBullet);
    painter.drawEllipse(QPointF(this->width() - bulletRadius, bulletCenter), bulletRadius,
                        bulletRadius);

    emit widthChanged(this->width());
}

int HoursLateralDayWidget::getInterval() const { return int(std::floor(this->height() / 24.0)); }
