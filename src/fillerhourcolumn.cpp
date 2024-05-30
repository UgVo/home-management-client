#include "./ui_fillerhourcolumn.h"
#include "fillerhourcolumn.h"

FillerHourColumn::FillerHourColumn(QWidget *parent)
    : QWidget(parent), ui(new Ui::FillerHourColumn) {
    ui->setupUi(this);
    _vshit     = 0;
    _hideWords = true;
}

FillerHourColumn::~FillerHourColumn() { delete ui; }

void FillerHourColumn::setFont(QFont newfont) {
    _font = newfont;
    setFixedWidth(QFontMetrics(_font).horizontalAdvance(tr("journée")) + bulletRadius * 2);
    update();
}

void FillerHourColumn::setVerticalShift(int vshift) {
    _vshit = vshift;
    update();
}

void FillerHourColumn::hideWords(bool flag) { _hideWords = flag; }

void FillerHourColumn::paintEvent(QPaintEvent *event) {
    if (_hideWords) return;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen penText(QColor("#AEAEAE"), widthBrush);
    int  heightText = QFontMetrics(_font).height() * 2 + QFontMetrics(_font).lineSpacing();
    int  widthText  = QFontMetrics(_font).horizontalAdvance(tr("Journée"));
    painter.setPen(penText);
    painter.setFont(_font);

    painter.drawText(QRect(0, _vshit, widthText, heightText), Qt::AlignRight,
                     tr("Journée\nentière"));

    QRect rect = QRect(0, 0, 100, 50);
}