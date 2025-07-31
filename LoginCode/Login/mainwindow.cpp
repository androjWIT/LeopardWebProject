#include "mainwindow.h"
#include "login.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    logoutButton = new QPushButton("Logout", this);
    layout->addWidget(logoutButton);

    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);
}

void MainWindow::handleLogout()
{
    Login *loginScreen = new Login();
    loginScreen->show();
    this->close();
}
