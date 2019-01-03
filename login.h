#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <qabstractsocket.h>
class QTcpSocket;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_acceptBTN_clicked();

    void on_exitBTN_clicked();

    void on_registerNewUser_clicked();

private:
    Ui::Login *ui;
    void initDate();

protected:
    void timerEvent(QTimerEvent * event);
    int begin;
    QTcpSocket * tcpSocket;
};

#endif // LOGIN_H
