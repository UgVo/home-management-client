#include <QApplication>

#include "apiconnector.h"
#include "daywidget.h"
#include "eventdaywidget.h"
#include "mainwindow.h"
#include "weekview.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/NotoColorEmoji.ttf"));

    MainWindow w;

    QFont serifFont("Sans Serif", 11);

    QSharedPointer<Event> event = QSharedPointer<Event>::create(
        "🥳 test 1", "", QDateTime(QDate(2024, 05, 18), QTime(11, 0), QTimeZone(3600)),
        QDateTime(QDate(2024, 05, 18), QTime(13, 0), QTimeZone(3600)));

    QSharedPointer<Event> event2 = QSharedPointer<Event>::create(
        "🥳 test 2", "", QDateTime(QDate(2024, 05, 18), QTime(16, 0), QTimeZone(3600)),
        QDateTime(QDate(2024, 05, 19), QTime(13, 0), QTimeZone(3600)));

    WeekView weekview;

    weekview.insertEvent(event);
    weekview.insertEvent(event2);

    weekview.setFont(serifFont);

    w.show();
    weekview.show();
    return a.exec();
}
