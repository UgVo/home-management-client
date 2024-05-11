#include <QApplication>

#include "apiconnector.h"
#include "eventwidget.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/NotoColorEmoji.ttf"));

    MainWindow w;

    QFont serifFont("Sans Serif", 12);

    QSharedPointer<Event> event = QSharedPointer<Event>::create(
        "🥳 test", "", QDateTime(QDate(2024, 10, 26), QTime(11, 3), QTimeZone(3600)),
        QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    EventWidget testEventWidget(event);
    testEventWidget.setFont(serifFont);

    QSharedPointer<Event> event2 = QSharedPointer<Event>::create(
        "Test Event", "FREQ=MONTHLY;BYDAY=WE",
        QDateTime(QDate(2024, 10, 26), QTime(11, 3), QTimeZone(3600)),
        QDateTime(QDate(2024, 10, 26), QTime(11, 6), QTimeZone(3600)));
    EventWidget testEventWidgetReccu(event2);
    testEventWidgetReccu.setFont(serifFont);

    w.show();
    testEventWidget.show();
    testEventWidgetReccu.show();
    return a.exec();
}
