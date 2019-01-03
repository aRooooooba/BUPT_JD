#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qvector.h>
#include <qtablewidget.h>
#include <qsignalmapper.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qabstractsocket.h>
#include "commodity_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"
#include "varieties.h"
class QTcpSocket;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *);

    void on_logout_clicked();

    void locateItem(int i);

    void locateBook(int i);

    void locateCloth(int i);

    void locateElectronic(int i);

    void locateTrolley(int trolley, int j);

    void locateWaitingOrders(int waiting);

    void locateFinishedOrders(int finished);

    void on_allItems_triggered();

    void on_books_triggered();

    void on_clothes_triggered();

    void on_electronics_triggered();

    void on_itemSearchKey_editingFinished();

    void on_bookSearchKey_editingFinished();

    void on_clothSearchKey_editingFinished();

    void on_electronicSearchKey_editingFinished();

    void on_trolley_triggered();

    void calculatePrice();

    void deleteOrder(int i);

    void on_allCheck_clicked();

    void on_myInfo_triggered();

    void on_editPassword_clicked();

    void locateThisAddress(int i);

    void on_deleteBankAccount_clicked();

    void on_deleteAddress_clicked();

    void on_saveAddress_clicked();

    void on_orders_triggered();

    void on_pay_clicked();

    void on_deleteAllPayingOrders_clicked();

    void on_clearAllFinishedOrders_clicked();

    void readMessage();

    void on_addBankAccount_clicked();

    void on_payNow_clicked();

private:
    Ui::MainWindow * ui;
    float totalPrice;
    QTcpSocket * tcpSocket;
    QString TCPusername;
    quint16 blockSize;
    quint16 optionType;
    QSignalMapper * deleteMapper;
    QVector <book_CLASS *> selectedBooks;
    QVector <cloth_CLASS *> selectedClothes;
    QVector <electronic_CLASS *> selectedElectronics;
    QVector <book_CLASS *> selectedBooks_2;
    QVector <cloth_CLASS *> selectedClothes_2;
    QVector <electronic_CLASS *> selectedElectronics_2;
    QVector <QCheckBox *> check;
    QVector <QPushButton *> deleteBtn;
    QVector <QSpinBox *> number;
    void checkOrders();
    void initList();
    void initAllList();
    void initBookList();
    void initClothList();
    void initElectronicList();
    void initTrolleyList();
    void initOrderList();
    void initPersonalInfo();
    void initAddressTable();
    void clearAddressGroup();
    void waitSec(int s);
    void initAddressGroup(address_CLASS * a);
    void selectBooks(QString s, QVector <book_CLASS *> &select);
    void selectClothes(QString s, QVector <cloth_CLASS *> &select);
    void selectElectronics(QString s, QVector <electronic_CLASS *> &select);
    void initBooks(int &i, QTableWidget * list, QVector <book_CLASS *> select);
    void initClothes(int &i, QTableWidget * list, QVector <cloth_CLASS *> select);
    void initElectronics(int &i, QTableWidget * list, QVector <electronic_CLASS *> select);
};

#endif // MAINWINDOW_H
