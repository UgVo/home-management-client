#include "./ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QObject::connect(ui->action_GetCalendarsInfos, &QAction::triggered, [this]() {
        api_connector.getCalendarsInfos([this](QNetworkReply* reply) {
            qDebug() << "Number of calendar found: " << storage.setupCalendars(reply->readAll());
            qDebug() << storage;
        });
    });
    QObject::connect(ui->action_GetCalendarEvents, &QAction::triggered, [this]() {
        api_connector.getCalendarEvents("commun-test", [this](QNetworkReply* reply) {
            auto buff = reply->readAll();
            qDebug() << buff;
            qDebug() << "Number of event added: "
                     << storage.setupCalendarEvents("commun-test", buff);
        });
    });
    QObject::connect(ui->action_UpdateCalendarEvents, &QAction::triggered, [this]() {
        api_connector.getCalendarUpdate("commun-test",
                                        [](QNetworkReply* reply) { qDebug() << reply->readAll(); });
    });
}

MainWindow::~MainWindow() { delete ui; }
