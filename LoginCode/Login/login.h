#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "mainwindow.h"

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);

private slots:
    void handleLogin();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    MainWindow *mainWindow;
};

#endif // LOGIN_H
