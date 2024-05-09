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

private:
    Ui::MainWindow *ui;
    QLineEdit* name;
    QLineEdit* surname;
    QLineEdit* phone;
    QVBoxLayout* mainLayout;
    QPushButton* submit;
    sqlite3* db;
};
#endif // MAINWINDOW_H
