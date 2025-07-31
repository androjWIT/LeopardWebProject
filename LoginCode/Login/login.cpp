#include "login.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QWidget(parent), mainWindow(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Username:"));
    usernameEdit = new QLineEdit(this);
    layout->addWidget(usernameEdit);

    layout->addWidget(new QLabel("Password:"));
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    loginButton = new QPushButton("Login", this);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &Login::handleLogin);
}

void Login::handleLogin()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username == "login" && password == "Password") {
        mainWindow = new MainWindow();
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}
