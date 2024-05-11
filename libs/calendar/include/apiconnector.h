#ifndef HOME_MANAGEMENT_CLIENT_APICONNECTOR_H
#define HOME_MANAGEMENT_CLIENT_APICONNECTOR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

using Reply_handler = std::function<void(QNetworkReply *)>;

class ApiConnector {
   public:
    ApiConnector(QString url = "http://localhost:3030");
    ~ApiConnector();

   public slots:
    void getCalendarsInfos(Reply_handler f);
    void getCalendarEvents(QString &&calendarName, Reply_handler f);
    void getCalendarUpdate(QString &&calendarName, Reply_handler f);

   private:
    QString               url;
    QNetworkAccessManager manager;
};
#endif  // HOME_MANAGEMENT_CLIENT_APICONNECTOR_H
