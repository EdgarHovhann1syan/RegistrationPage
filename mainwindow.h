#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <sqlite3.h>
#include <QDebug>
#include <QString>
#include <QListWidget>
#include <sstream>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void submitHandler();
    void handleShowUsers();
    void deleteHandler();

private:
    Ui::MainWindow *ui;
    QLineEdit* name;
    QLineEdit* surname;
    QLineEdit* phone;
    QVBoxLayout* mainLayout;
    QPushButton* submit;
    sqlite3* db;
    QListWidget* users;
    QLabel* countOfUsers;
    QPushButton* deleteButton;

};
#endif // MAINWINDOW_H
