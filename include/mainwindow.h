#ifndef HOME_MANAGEMENT_CLIENT_MAINWINDOW_H
#define HOME_MANAGEMENT_CLIENT_MAINWINDOW_H

#include <QMainWindow>

#include "apiconnector.h"
#include "storage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    Ui::MainWindow *ui;
    ApiConnector api_connector;
    Storage storage;
};
#endif  // HOME_MANAGEMENT_CLIENT_MAINWINDOW_H
