#include "apiconnector.h"

ApiConnector::ApiConnector(QString _url) : url(_url) {}

ApiConnector::~ApiConnector() {}

void ApiConnector::getCalendarsInfos(Reply_handler f) {
    QNetworkRequest request(QUrl(url + "/v1/calendars/infos"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Type", "text/plain");
    auto reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, f]() {
        qDebug() << "CalendarsInfos";
        f(reply);
    });
}

void ApiConnector::getCalendarEvents(QString &&calendarName, Reply_handler f) {
    QNetworkRequest request(QUrl(url + "/v1/calendars/" + calendarName + "/events"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Type", "text/plain");
    auto reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, f, calendarName]() {
        qDebug() << "CalendarsEvents : " << calendarName;
        f(reply);
    });
}

void ApiConnector::getCalendarUpdate(QString &&calendarName, Reply_handler f) {
    QNetworkRequest request(QUrl(url + "/v1/calendars/" + calendarName + "/update"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Type", "text/plain");
    auto reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, f, calendarName]() {
        qDebug() << "CalendarsUpdate : " << calendarName;
        f(reply);
    });
}
