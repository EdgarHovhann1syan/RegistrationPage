#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(700, 700);
    this->name = new QLineEdit(this);
    this->name->setPlaceholderText("Name");
    this->surname = new QLineEdit(this);
    this->surname->setPlaceholderText("Surname");
    this->phone = new QLineEdit(this);
    this->phone->setPlaceholderText("Phone Number");
    this->mainLayout = new QVBoxLayout(this);
    this->submit = new QPushButton(this);
    this->submit->setText("Sign Up");

    this->mainLayout->addStretch();

    this->mainLayout->addWidget(this->name, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->surname, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->phone, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->submit, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->submit, 0, Qt::AlignHCenter);

    this->mainLayout->addStretch();
    \
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);

    this->name->setFixedSize(500, 50);
    this->surname->setFixedSize(500, 50);
    this->phone->setFixedSize(500, 50);
    this->submit->setFixedSize(150, 60);

    this->name->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->surname->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->phone->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->submit->setStyleSheet("border-radius: 20px; border: 1px solid orange;");


    //SQL connection

    int rc = sqlite3_open("/home/edgar/Desktop/QtTasks/RegistrationForm/users.db",&this->db);
    if(rc != SQLITE_OK)
    {
        qDebug() << "ERROR";
    } else
    {
        qDebug() << "Success";
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS user(id INTEGER PRIMARY KEY, name TEXT, surname TEXT, phone TEXT)";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if(rc != SQLITE_OK)
    {
        qDebug() << "Failed to create table";
    }

    //connections
    connect(this->submit, &QPushButton::clicked, this, &MainWindow::submitHandler);



}


void MainWindow::submitHandler()
{
    if(this->name->text().isEmpty() || this->surname->text().isEmpty() || this->phone->text().isEmpty())
    {
        QMessageBox::critical(this, "Incorrect", "Some fields are empty. Please correct.");
    } else
    {
        QString sql = QString("INSERT INTO user(name, surname, phone) VALUES ('%1', '%2', '%3')")
        .arg(this->name->text())
        .arg(this->surname->text())
        .arg(this->phone->text());

        int rc = sqlite3_exec(this->db, sql.toUtf8().constData(), nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK)
        {
            QMessageBox::critical(this, "Error", "Something Wrong: Try Again");
        }
        this->name->clear();
        this->surname->clear();
        this->phone->clear();

        QMessageBox::information(this, "Success", "Registration Success");
    }
}

MainWindow::~MainWindow()
{
    sqlite3_close(this->db);
    delete ui;
}
