#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , countOfUsers(0)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 1000);
    this->name = new QLineEdit(this);
    this->name->setPlaceholderText("Name");
    this->surname = new QLineEdit(this);
    this->surname->setPlaceholderText("Surname");
    this->phone = new QLineEdit(this);
    this->phone->setPlaceholderText("Phone Number");
    this->mainLayout = new QVBoxLayout(this);
    this->submit = new QPushButton(this);
    this->submit->setText("Sign Up");
    this->users = new QListWidget(this);
    this->users->setSelectionMode(QAbstractItemView::SingleSelection); // Set selection mode
    this->users->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->users->setGeometry(0, 700, 1000, 300);
    this->users->setStyleSheet("color: black;");
    this->countOfUsers = new QLabel(this);
    this->countOfUsers->setGeometry(50, 50, 50, 50);
    this->countOfUsers->setStyleSheet("border: 1px solid orange; border-radius: 10px;");
    this->countOfUsers->setText("0");
    this->deleteButton = new QPushButton(this);
    this->deleteButton->setText("Delete");

    this->mainLayout->addWidget(this->name, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->surname, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->phone, 0, Qt::AlignHCenter);
    this->mainLayout->addWidget(this->submit, 0, Qt::AlignHCenter); // Add submit button aligned to the left
    this->mainLayout->addWidget(this->deleteButton, 0, Qt::AlignHCenter);
    this->mainLayout->addStretch(0);
    \
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);


    this->name->setFixedSize(500, 50);
    this->surname->setFixedSize(500, 50);
    this->phone->setFixedSize(500, 50);
    this->submit->setFixedSize(150, 60);
    this->deleteButton->setFixedSize(150, 60);

    this->name->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->surname->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->phone->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->submit->setStyleSheet("border-radius: 20px; border: 1px solid orange;");
    this->deleteButton->setStyleSheet("border-radius: 20px; border: 1px solid orange;");



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
    } else
    {
        rc = sqlite3_exec(this->db, "DELETE FROM user;", nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK)
        {
            qDebug() << "Clear Error";
        }
    }

    //connections
    connect(this->submit, &QPushButton::clicked, this, &MainWindow::submitHandler);
    connect(this->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteHandler);

    handleShowUsers();



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
        qint64 count = this->countOfUsers->text().toInt();
        ++count;
        this->countOfUsers->setText(QString::number(count));
        QMessageBox::information(this, "Success", "Registration Success");
    }
    this->handleShowUsers();
}
void MainWindow::handleShowUsers()
{
    this->users->clear(); // Clear existing items

    const char* sql = "SELECT name, surname, phone FROM user";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK) {
        qDebug() << "Failed to prepare statement";
        return;
    }

    int row = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        QString name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        QString surname = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        QString phone = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

        QString userData = QString("%1 %2 %3").arg(name).arg(surname).arg(phone);
        this->users->insertItem(row++, userData);
    }

    sqlite3_finalize(stmt);

    this->countOfUsers->setText(QString::number(row));
}

void MainWindow::deleteHandler()
{
    QListWidgetItem* user = this->users->currentItem(); // Get the selected item
    if(user)
    {
        QString sql = QString("DELETE FROM user WHERE name = ('%1') AND surname = ('%2') AND phone = ('%3')")
                          .arg(user->text().section(' ', 0, 0))
                          .arg(user->text().section(' ', 1, 1))
                          .arg(user->text().section(' ', 2, 2));

        int rc = sqlite3_exec(this->db, sql.toUtf8().constData(), nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK)
        {
            qDebug() << "Deletion error:" << sqlite3_errmsg(this->db);
        }
        else
        {
            qDebug() << "Deletion successful";
            this->handleShowUsers();
        }
    }
}

MainWindow::~MainWindow()
{
    sqlite3_close(this->db);
    delete ui;
}
