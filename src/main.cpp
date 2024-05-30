#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/NotoColorEmoji.ttf"));

    MainWindow w;

    w.show();
    return a.exec();
}
