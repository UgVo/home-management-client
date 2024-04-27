#ifndef HOME_MANAGEMENT_CLIENT_APICONNECTOR_H
#define HOME_MANAGEMENT_CLIENT_APICONNECTOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class ApiConnector {
   public:
    ApiConnector(QString url = "http://localhost:3030");
    ~ApiConnector();

   public slots:
    void getCalendarsInfos(std::function<void(QNetworkReply *)> f);
    void getCalendarEvents(QString &&calendarName, std::function<void(QNetworkReply *)> f);
    void getCalendarUpdate(QString &&calendarName, std::function<void(QNetworkReply *)> f);

   private:
    QString url;
    QNetworkAccessManager manager;
};
#endif  // HOME_MANAGEMENT_CLIENT_APICONNECTOR_H
