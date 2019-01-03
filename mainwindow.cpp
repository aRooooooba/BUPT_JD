#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "details.h"
#include "user_class.h"
#include "commodity_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"
#include "varieties.h"
#include <qlabel.h>
#include <QMessageBox>
#include <qtablewidget.h>
#include <qstring.h>
#include <qvector.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qsignalmapper.h>
#include <qinputdialog.h>
#include <QtNetwork>
#include <qdatastream.h>
#include <qtimer.h>

int colunmNUM = 1;
int thisAddress = -1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    checkOrders();
    initList();
    initAllList();
    initBookList();
    initClothList();
    initElectronicList();
    initTrolleyList();
    calculatePrice();
    initPersonalInfo();
    initOrderList();
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    timeFlag = true;
}

void MainWindow::checkOrders()
{
    QVector <user_CLASS>::iterator iterUser;
    for (iterUser = users.begin(); iterUser != users.end(); iterUser++)
    {
        for (int i = 0; i < iterUser->get_orderNUM(); i++)
        {
            if ((0 == iterUser->get_order(i)->get_state() || 1 == iterUser->get_order(i)->get_state())
                    && (date.get_year() > iterUser->get_order(i)->get_refreshDate().get_year()
                        || date.get_month() > iterUser->get_order(i)->get_refreshDate().get_month()
                        || date.get_day() > iterUser->get_order(i)->get_refreshDate().get_day() + 2))
            {
                book_CLASS * thatBook = NULL;
                cloth_CLASS * thatCloth = NULL;
                electronic_CLASS * thatElectronic = NULL;
                if (!findItem(iterUser->get_order(i)->get_id(), thatBook, thatCloth, thatElectronic))
                {
                    QMessageBox::critical(this, "Error", QString::fromLocal8Bit("文件出错！"), QMessageBox::Yes);
                    exit(0);
                }
                if (NULL != thatBook)
                    thatBook->reSetNUM(iterUser->get_order(i)->get_number());
                else if (NULL != thatCloth)
                    thatCloth->reSetNUM(iterUser->get_order(i)->get_number());
                else
                    thatElectronic->reSetNUM(iterUser->get_order(i)->get_number());
                if (thisUser == iterUser)
                    QMessageBox::information(this, "alert", QString::fromLocal8Bit("您编号为%1的订单\n由于未及时付款，已被删除。")
                                             .arg(iterUser->get_order(i)->get_id()), QMessageBox::Yes);
                iterUser->delete_order(i);
                i--;
            }
        }
    }
}

void MainWindow::initList()
{
    ui->itemList->setColumnWidth(0, 220);
    ui->itemList->setColumnWidth(1, 210);
    ui->itemList->setColumnWidth(2, 100);
    ui->itemList->setColumnWidth(3, 200);
    ui->itemList->setColumnWidth(4, 320);
    ui->bookList->setColumnWidth(0, 220);
    ui->bookList->setColumnWidth(1, 210);
    ui->bookList->setColumnWidth(2, 100);
    ui->bookList->setColumnWidth(3, 200);
    ui->bookList->setColumnWidth(4, 320);
    ui->clothList->setColumnWidth(0, 220);
    ui->clothList->setColumnWidth(1, 210);
    ui->clothList->setColumnWidth(2, 100);
    ui->clothList->setColumnWidth(3, 200);
    ui->clothList->setColumnWidth(4, 320);
    ui->electronicList->setColumnWidth(0, 220);
    ui->electronicList->setColumnWidth(1, 210);
    ui->electronicList->setColumnWidth(2, 100);
    ui->electronicList->setColumnWidth(3, 200);
    ui->electronicList->setColumnWidth(4, 320);
    ui->trolleyList->setColumnWidth(0, 60);
    ui->trolleyList->setColumnWidth(1, 100);
    ui->trolleyList->setColumnWidth(2, 150);
    ui->trolleyList->setColumnWidth(3, 150);
    ui->trolleyList->setColumnWidth(4, 100);
    ui->trolleyList->setColumnWidth(5, 50);
    ui->trolleyList->setColumnWidth(6, 150);
    ui->trolleyList->setColumnWidth(7, 100);
    ui->waitForPayingList->setColumnWidth(0, 100);
    ui->waitForPayingList->setColumnWidth(1, 150);
    ui->waitForPayingList->setColumnWidth(2, 100);
    ui->waitForPayingList->setColumnWidth(3, 100);
    ui->waitForPayingList->setColumnWidth(4, 150);
    ui->finishedList->setColumnWidth(0, 100);
    ui->finishedList->setColumnWidth(1, 200);
    ui->finishedList->setColumnWidth(2, 100);
    ui->finishedList->setColumnWidth(3, 100);
    ui->finishedList->setColumnWidth(4, 150);
}

void MainWindow::initAllList()
{
    disconnect(ui->itemList, SIGNAL(cellClicked(int,int)), this, SLOT(locateItem(int)));
    int i;
    QVector <book_CLASS *> pNULLBooks;
    QVector <cloth_CLASS *> pNULLClothes;
    QVector <electronic_CLASS *> pNULLElectronics;
    selectedBooks.swap(pNULLBooks);
    selectedClothes.swap(pNULLClothes);
    selectedElectronics.swap(pNULLElectronics);
    for (i = 0; i < books.size(); i++)
        selectedBooks.push_back(&books[i]);
    for (i = 0; i < clothes.size(); i++)
        selectedClothes.push_back(&clothes[i]);
    for (i = 0; i < electronics.size(); i++)
        selectedElectronics.push_back(&electronics[i]);
    ui->itemList->setRowCount(books.size() + clothes.size() + electronics.size());
    i = 0;
    initBooks(i, ui->itemList, selectedBooks);
    initClothes(i, ui->itemList, selectedClothes);
    initElectronics(i, ui->itemList, selectedElectronics);
    connect(ui->itemList, SIGNAL(cellClicked(int,int)), this, SLOT(locateItem(int)));
}

void MainWindow::initBookList()
{
    disconnect(ui->bookList, SIGNAL(cellClicked(int,int)), this, SLOT(locateBook(int)));
    int i;
    QVector <book_CLASS *> pNULLBooks;
    selectedBooks_2.swap(pNULLBooks);
    for (i = 0; i < books.size(); i++)
        selectedBooks_2.push_back(&books[i]);
    ui->bookList->setRowCount(books.size());
    i = 0;
    initBooks(i, ui->bookList, selectedBooks_2);
    connect(ui->bookList, SIGNAL(cellClicked(int,int)), this, SLOT(locateBook(int)));
}

void MainWindow::initClothList()
{
    disconnect(ui->clothList, SIGNAL(cellClicked(int,int)), this, SLOT(locateCloth(int)));
    int i;
    QVector <cloth_CLASS *> pNULLClothes;
    selectedClothes_2.swap(pNULLClothes);
    for (i = 0; i < clothes.size(); i++)
        selectedClothes_2.push_back(&clothes[i]);
    ui->clothList->setRowCount(clothes.size());
    i = 0;
    initClothes(i, ui->clothList, selectedClothes_2);
    connect(ui->clothList, SIGNAL(cellClicked(int,int)), this, SLOT(locateCloth(int)));
}

void MainWindow::initElectronicList()
{
    disconnect(ui->electronicList, SIGNAL(cellClicked(int,int)), this, SLOT(locateElectronic(int)));
    int i;
    QVector <electronic_CLASS *> pNULLElectronics;
    selectedElectronics_2.swap(pNULLElectronics);
    for (i = 0; i < electronics.size(); i++)
        selectedElectronics_2.push_back(&electronics[i]);
    ui->electronicList->setRowCount(electronics.size());
    i = 0;
    initElectronics(i, ui->electronicList, selectedElectronics_2);
    connect(ui->electronicList, SIGNAL(cellClicked(int,int)), this, SLOT(locateElectronic(int)));
}

void MainWindow::initTrolleyList()
{
    for (int j = 0; j < check.size(); j++)
    {
        disconnect(check[j], SIGNAL(clicked(bool)), this, SLOT(calculatePrice()));
        disconnect(number[j], SIGNAL(valueChanged(int)), this, SLOT(calculatePrice()));
        disconnect(deleteBtn[j], SIGNAL(clicked(bool)), deleteMapper, SLOT(map()));
    }
    disconnect(ui->trolleyList, SIGNAL(cellClicked(int,int)), this, SLOT(locateTrolley(int, int)));
    QVector <QCheckBox *> nullCheck;
    QVector <QPushButton *> nullDeleteBtn;
    QVector <QSpinBox *> nullNumber;
    check.swap(nullCheck);
    deleteBtn.swap(nullDeleteBtn);
    number.swap(nullNumber);
    book_CLASS * thatBook = NULL;
    cloth_CLASS * thatCloth = NULL;
    electronic_CLASS * thatElectronic = NULL;
    int i = 0;
    for (int j = 0; j < thisUser->get_orderNUM(); j++)
        if (0 == thisUser->get_order(j)->get_state())
            i++;
    ui->trolleyList->setRowCount(i);
    i = 0;
    deleteMapper = new QSignalMapper(this);
    for (int j = 0; j < thisUser->get_orderNUM(); j++)
    {
        if (0 != thisUser->get_order(j)->get_state())
            continue;
        findItem(thisUser->get_order(j)->get_id(), thatBook, thatCloth, thatElectronic);
        QCheckBox * anotherCheck = new QCheckBox();
        anotherCheck->setText("");
        QPushButton * anotherDeleteBtn = new QPushButton();
        anotherDeleteBtn->setStyleSheet("QPushButton" + deleteBtnURL);
        QSpinBox * anotherNumber = new QSpinBox();
        anotherNumber->setMinimum(1);
        anotherNumber->setValue(thisUser->get_order(j)->get_number());
        check.push_back(anotherCheck);
        deleteBtn.push_back(anotherDeleteBtn);
        number.push_back(anotherNumber);
        QLabel * picture = new QLabel();
        ui->trolleyList->setRowHeight(i, 100);
        ui->trolleyList->setCellWidget(i, 0, check[i]);
        ui->trolleyList->setCellWidget(i, 1, picture);
        ui->trolleyList->setCellWidget(i, 5, number[i]);
        ui->trolleyList->setItem(i, 6, new QTableWidgetItem(QString("%1.%2.%3")
                                                            .arg(thisUser->get_order(j)->get_refreshDate().get_year())
                                                            .arg(thisUser->get_order(j)->get_refreshDate().get_month())
                                                            .arg(thisUser->get_order(j)->get_refreshDate().get_day())));
        ui->trolleyList->setCellWidget(i, 7, deleteBtn[i]);
        connect(check[i], SIGNAL(clicked(bool)), this, SLOT(calculatePrice()));
        connect(number[i], SIGNAL(valueChanged(int)), this, SLOT(calculatePrice()));
        connect(deleteBtn[i], SIGNAL(clicked(bool)), deleteMapper, SLOT(map()));
        deleteMapper->setMapping(deleteBtn[i], i);
        if (NULL != thatBook)
        {
            picture->setStyleSheet("QLabel" + thatBook->get_URL());
            ui->trolleyList->setItem(i, 2, new QTableWidgetItem(thatBook->get_title()));
            ui->trolleyList->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(thatBook->get_price())));
            if (0 == thatBook->get_discount().get_discountType())
                ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("无")));
            else if (1 == thatBook->get_discount().get_discountType())
            {
                if (0 == thatBook->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                     .arg(thatBook->get_discount().get_multiple() * 10)));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                     .arg(thatBook->get_discount().get_limit())
                                                                     .arg(thatBook->get_discount().get_multiple() * 10)));
            }
            else
            {
                if (0 == thatBook->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                     .arg(thatBook->get_discount().get_minus())));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                     .arg(thatBook->get_discount().get_limit())
                                                                     .arg(thatBook->get_discount().get_minus())));
            }
            number[i]->setMaximum(thatBook->get_number());
        }
        else if (NULL != thatCloth)
        {
            picture->setStyleSheet("QLabel" + thatCloth->get_URL());
            ui->trolleyList->setItem(i, 2, new QTableWidgetItem(thatCloth->get_title()));
            ui->trolleyList->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(thatCloth->get_price())));
            if (0 == thatCloth->get_discount().get_discountType())
                ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("无")));
            else if (1 == thatCloth->get_discount().get_discountType())
            {
                if (0 == thatCloth->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                     .arg(thatCloth->get_discount().get_multiple() * 10)));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                     .arg(thatCloth->get_discount().get_limit())
                                                                     .arg(thatCloth->get_discount().get_multiple() * 10)));
            }
            else
            {
                if (0 == thatCloth->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                     .arg(thatCloth->get_discount().get_minus())));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                     .arg(thatCloth->get_discount().get_limit())
                                                                     .arg(thatCloth->get_discount().get_minus())));
            }
            number[i]->setMaximum(thatCloth->get_number());
        }
        else
        {
            picture->setStyleSheet("QLabel" + thatElectronic->get_URL());
            ui->trolleyList->setItem(i, 2, new QTableWidgetItem(thatElectronic->get_title()));
            ui->trolleyList->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(thatElectronic->get_price())));
            if (0 == thatElectronic->get_discount().get_discountType())
                ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("无")));
            else if (1 == thatElectronic->get_discount().get_discountType())
            {
                if (0 == thatElectronic->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                     .arg(thatElectronic->get_discount().get_multiple() * 10)));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                     .arg(thatElectronic->get_discount().get_limit())
                                                                     .arg(thatElectronic->get_discount().get_multiple() * 10)));
            }
            else
            {
                if (0 == thatElectronic->get_discount().get_limit())
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                     .arg(thatElectronic->get_discount().get_minus())));
                else
                    ui->trolleyList->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                     .arg(thatElectronic->get_discount().get_limit())
                                                                     .arg(thatElectronic->get_discount().get_minus())));
            }
            number[i]->setMaximum(thatElectronic->get_number());
        }
        i++;
    }
    connect(ui->trolleyList, SIGNAL(cellClicked(int,int)), this, SLOT(locateTrolley(int, int)));
    connect(deleteMapper, SIGNAL(mapped(int)), this, SLOT(deleteOrder(int)));
}

void MainWindow::initOrderList()
{
    disconnect(ui->waitForPayingList, SIGNAL(cellClicked(int,int)), this, SLOT(locateWaitingOrders(int)));
    disconnect(ui->finishedList, SIGNAL(cellClicked(int,int)), this, SLOT(locateFinishedOrders(int)));
    totalPrice = 0;
    book_CLASS * thatBook = NULL;
    cloth_CLASS * thatCloth = NULL;
    electronic_CLASS * thatElectronic = NULL;
    int wait = 0, finished = 0;
    for (int i = 0; i < thisUser->get_orderNUM(); i++)
    {
        if (1 == thisUser->get_order(i)->get_state())
            wait++;
        else if (2 == thisUser->get_order(i)->get_state())
            finished++;
    }
    ui->waitForPayingList->setRowCount(wait);
    ui->finishedList->setRowCount(finished);
    wait = 0;
    finished = 0;
    float thisPrice;
    for (int i = 0; i < thisUser->get_orderNUM(); i++)
    {
        thisPrice = 0;
        QLabel * picture = new QLabel();
        findItem(thisUser->get_order(i)->get_id(), thatBook, thatCloth, thatElectronic);
        if (1 == thisUser->get_order(i)->get_state())    //未付款
        {
            ui->waitForPayingList->setRowHeight(wait, 100);
            ui->waitForPayingList->setItem(wait, 2, new QTableWidgetItem(QString("%1")
                                                                         .arg(thisUser->get_order(i)->get_number())));
            ui->waitForPayingList->setItem(wait, 4, new QTableWidgetItem(QString("%1.%2.%3")
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_year())
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_month())
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_day())));
            if (NULL != thatBook)
            {
                picture->setStyleSheet("QLabel" + thatBook->get_URL());
                ui->waitForPayingList->setCellWidget(wait, 0, picture);
                ui->waitForPayingList->setItem(wait, 1, new QTableWidgetItem(thatBook->get_title()));
                thisPrice = thatBook->calcPrice(thatBook->get_price() * thisUser->get_order(i)->get_number());
                totalPrice += thisPrice;
            }
            else if (NULL != thatCloth)
            {
                picture->setStyleSheet("QLabel" + thatCloth->get_URL());
                ui->waitForPayingList->setCellWidget(wait, 0, picture);
                ui->waitForPayingList->setItem(wait, 1, new QTableWidgetItem(thatCloth->get_title()));
                thisPrice = thatCloth->calcPrice(thatCloth->get_price() * thisUser->get_order(i)->get_number());
                totalPrice += thisPrice;
            }
            else
            {
                picture->setStyleSheet("QLabel" + thatElectronic->get_URL());
                ui->waitForPayingList->setCellWidget(wait, 0, picture);
                ui->waitForPayingList->setItem(wait, 1, new QTableWidgetItem(thatElectronic->get_title()));
                thisPrice = thatElectronic->calcPrice(thatElectronic->get_price() * thisUser->get_order(i)->get_number());
                totalPrice += thisPrice;
            }
            ui->waitForPayingList->setItem(wait, 3, new QTableWidgetItem(QString("%1").arg(thisPrice)));
            wait++;
        }
        else if (2 == thisUser->get_order(i)->get_state())
        {
            ui->finishedList->setRowHeight(finished, 100);
            ui->finishedList->setCellWidget(finished, 0, picture);
            ui->finishedList->setItem(finished, 2, new QTableWidgetItem(QString("%1")
                                                                         .arg(thisUser->get_order(i)->get_number())));
            ui->finishedList->setItem(finished, 4, new QTableWidgetItem(QString("%1.%2.%3")
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_year())
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_month())
                                                                         .arg(thisUser->get_order(i)->get_refreshDate().get_day())));
            if (NULL != thatBook)
            {
                picture->setStyleSheet("QLabel" + thatBook->get_URL());
                ui->finishedList->setItem(finished, 1, new QTableWidgetItem(thatBook->get_title()));
                thisPrice = thatBook->calcPrice(thatBook->get_price() * thisUser->get_order(i)->get_number());
            }
            else if (NULL != thatCloth)
            {
                picture->setStyleSheet("QLabel" + thatCloth->get_URL());
                ui->finishedList->setItem(finished, 1, new QTableWidgetItem(thatCloth->get_title()));
                thisPrice = thatCloth->calcPrice(thatCloth->get_price() * thisUser->get_order(i)->get_number());
            }
            else
            {
                picture->setStyleSheet("QLabel" + thatElectronic->get_URL());
                ui->finishedList->setItem(finished, 1, new QTableWidgetItem(thatElectronic->get_title()));
                thisPrice = thatElectronic->calcPrice(thatElectronic->get_price() * thisUser->get_order(i)->get_number());
            }
            ui->finishedList->setItem(finished, 3, new QTableWidgetItem(QString("%1").arg(thisPrice)));
            finished++;
        }
    }
    if (0 == wait)
    {
        ui->totalPrice->setText("");
        ui->deleteAllPayingOrders->setEnabled(false);
    }
    else
    {
        ui->totalPrice->setText(QString("%1").arg(totalPrice));
        ui->deleteAllPayingOrders->setEnabled(true);
    }
    if (0 == finished)
        ui->clearAllFinishedOrders->setEnabled(false);
    else
        ui->clearAllFinishedOrders->setEnabled(true);
    connect(ui->waitForPayingList, SIGNAL(cellClicked(int,int)), this, SLOT(locateWaitingOrders(int)));
    connect(ui->finishedList, SIGNAL(cellClicked(int,int)), this, SLOT(locateFinishedOrders(int)));
}

void MainWindow::initPersonalInfo()
{
    ui->username->setText(thisUser->get_username());
    if ("-1" == thisUser->get_bankAccount())
    {
        ui->bankAccount->setText(QString::fromLocal8Bit("无"));
        ui->deleteBankAccount->setEnabled(false);
        ui->addBankAccount->setEnabled(true);
    }
    else
    {
        ui->bankAccount->setText(thisUser->get_bankAccount());
        ui->deleteBankAccount->setEnabled(true);
        ui->addBankAccount->setEnabled(false);
    }
    ui->addressTable->setColumnWidth(0, 60);
    ui->addressTable->setColumnWidth(1, 100);
    ui->addressTable->setColumnWidth(2, 150);
    ui->addressTable->setColumnWidth(3, 300);
    initAddressTable();
    clearAddressGroup();
}

void MainWindow::initBooks(int &i, QTableWidget * list, QVector <book_CLASS *> select)
{
    for (int j = 0; j < select.size(); j++, i++)
    {
        list->setRowHeight(i, 220);
        QLabel * picture = new QLabel();
        picture->setStyleSheet("QLabel" + select[j]->get_URL());
        list->setCellWidget(i, 0, picture);
        list->setItem(i, 1, new QTableWidgetItem(select[j]->get_title()));
        list->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(select[j]->get_price())));
        if (0 == select[j]->get_discount().get_discountType())
            list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("无")));
        else if (1 == select[j]->get_discount().get_discountType())
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
        }
        else
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                 .arg(select[j]->get_discount().get_minus())));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_minus())));
        }
        list->setItem(i, 4, new QTableWidgetItem(select[j]->get_description()));
    }
}

void MainWindow::initClothes(int &i, QTableWidget * list, QVector <cloth_CLASS *> select)
{
    for (int j = 0; j < select.size(); j++, i++)
    {
        list->setRowHeight(i, 220);
        QLabel * picture = new QLabel();
        picture->setStyleSheet("QLabel" + select[j]->get_URL());
        list->setCellWidget(i, 0, picture);
        list->setItem(i, 1, new QTableWidgetItem(select[j]->get_title()));
        list->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(select[j]->get_price())));
        if (0 == select[j]->get_discount().get_discountType())
            list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("无")));
        else if (1 == select[j]->get_discount().get_discountType())
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
        }
        else
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                 .arg(select[j]->get_discount().get_minus())));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_minus())));
        }
        list->setItem(i, 4, new QTableWidgetItem(select[j]->get_description()));
    }
}

void MainWindow::initElectronics(int &i, QTableWidget * list, QVector <electronic_CLASS *> select)
{
    for (int j = 0; j < select.size(); j++, i++)
    {
        list->setRowHeight(i, 220);
        QLabel * picture = new QLabel();
        picture->setStyleSheet("QLabel" + select[j]->get_URL());
        list->setCellWidget(i, 0, picture);
        list->setItem(i, 1, new QTableWidgetItem(select[j]->get_title()));
        list->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(select[j]->get_price())));
        if (0 == select[j]->get_discount().get_discountType())
            list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("无")));
        else if (1 == select[j]->get_discount().get_discountType())
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场%1折！")
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1，打%2折！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_multiple() * 10)));
        }
        else
        {
            if (0 == select[j]->get_discount().get_limit())
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("全场直降%1！")
                                                                 .arg(select[j]->get_discount().get_minus())));
            else
                list->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("满%1减%2！")
                                                                 .arg(select[j]->get_discount().get_limit())
                                                                 .arg(select[j]->get_discount().get_minus())));
        }
        list->setItem(i, 4, new QTableWidgetItem(select[j]->get_description()));
    }
}

void MainWindow::locateItem(int i)
{
    if (i < selectedBooks.size())
    {
        details d(0, selectedBooks[i], NULL, NULL);
        if (QDialog::Accepted == d.exec())
            initTrolleyList();
    }
    else if (i < selectedBooks.size() + selectedClothes.size())
    {
        details d(0, NULL, selectedClothes[i - selectedBooks.size()], NULL);
        if (QDialog::Accepted == d.exec())
            initTrolleyList();
    }
    else
    {
        details d(0, NULL, NULL, selectedElectronics[i - selectedBooks.size() - selectedClothes.size()]);
        if (QDialog::Accepted == d.exec())
            initTrolleyList();
    }
}

void MainWindow::locateBook(int i)
{
    details d(0, selectedBooks_2[i], NULL, NULL);
    if (QDialog::Accepted == d.exec())
        initTrolleyList();
}

void MainWindow::locateCloth(int i)
{
    details d(0, NULL, selectedClothes_2[i], NULL);
    if (QDialog::Accepted == d.exec())
        initTrolleyList();
}

void MainWindow::locateElectronic(int i)
{
    details d(0, NULL, NULL, selectedElectronics_2[i]);
    if (QDialog::Accepted == d.exec())
        initTrolleyList();
}

void MainWindow::locateTrolley(int trolley, int j)
{
    if (j == 1 || j == 2 || j == 3 || j == 4)
    {
        book_CLASS * thatBook = NULL;
        cloth_CLASS * thatCloth = NULL;
        electronic_CLASS * thatElectronic = NULL;
        int i, k;
        for (i = 0, k = -1; i <= trolley;)
        {
            k++;
            if (0 == thisUser->get_order(k)->get_state())
                i++;
        }
        findItem(thisUser->get_order(k)->get_id(), thatBook, thatCloth, thatElectronic);
        details d(0, thatBook, thatCloth, thatElectronic);
        d.exec();
    }
}

void MainWindow::locateWaitingOrders(int waiting)
{
    book_CLASS * thatBook = NULL;
    cloth_CLASS * thatCloth = NULL;
    electronic_CLASS * thatElectronic = NULL;
    int i, k;
    for (i = 0, k = -1; i <= waiting;)
    {
        k++;
        if (1 == thisUser->get_order(k)->get_state())
            i++;
    }
    findItem(thisUser->get_order(k)->get_id(), thatBook, thatCloth, thatElectronic);
    details d(0, thatBook, thatCloth, thatElectronic);
    d.exec();
}

void MainWindow::locateFinishedOrders(int finished)
{
    book_CLASS * thatBook = NULL;
    cloth_CLASS * thatCloth = NULL;
    electronic_CLASS * thatElectronic = NULL;
    int i, k;
    for (i = 0, k = -1; i <= finished;)
    {
        k++;
        if (2 == thisUser->get_order(k)->get_state())
            i++;
    }
    findItem(thisUser->get_order(k)->get_id(), thatBook, thatCloth, thatElectronic);
    details d(0, thatBook, thatCloth, thatElectronic);
    d.exec();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    on_logout_clicked();
}

void MainWindow::on_logout_clicked()
{
    if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("确定注销吗？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        tcpSocket->abort();
        tcpSocket->connectToHost("127.0.0.1",6666);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << (quint16) 0;
        out << (quint16) 1; //注销
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        tcpSocket->write(block);
        waitSec(1);
        thisUser = NULL;
        saveUsers();
        close();
    }
}

void MainWindow::on_allItems_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_books_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_clothes_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_electronics_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_trolley_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_itemSearchKey_editingFinished()
{
    disconnect(ui->itemList, SIGNAL(cellClicked(int,int)), this, SLOT(locateItem(int)));
    selectBooks(ui->itemSearchKey->text(), selectedBooks);
    selectClothes(ui->itemSearchKey->text(), selectedClothes);
    selectElectronics(ui->itemSearchKey->text(), selectedElectronics);
    ui->itemList->setRowCount(selectedBooks.size() + selectedClothes.size() + selectedElectronics.size());
    int i = 0;
    initBooks(i, ui->itemList, selectedBooks);
    initClothes(i, ui->itemList, selectedClothes);
    initElectronics(i, ui->itemList, selectedElectronics);
    connect(ui->itemList, SIGNAL(cellClicked(int,int)), this, SLOT(locateItem(int)));
}

void MainWindow::on_bookSearchKey_editingFinished()
{
    disconnect(ui->bookList, SIGNAL(cellClicked(int,int)), this, SLOT(locateBook(int)));
    selectBooks(ui->bookSearchKey->text(), selectedBooks_2);
    ui->bookList->setRowCount(selectedBooks_2.size());
    int i = 0;
    initBooks(i, ui->bookList, selectedBooks_2);
    connect(ui->bookList, SIGNAL(cellClicked(int,int)), this, SLOT(locateBook(int)));
}

void MainWindow::on_clothSearchKey_editingFinished()
{
    disconnect(ui->clothList, SIGNAL(cellClicked(int,int)), this, SLOT(locateCloth(int)));
    selectClothes(ui->clothSearchKey->text(), selectedClothes_2);
    ui->clothList->setRowCount(selectedClothes_2.size());
    int i = 0;
    initClothes(i, ui->clothList, selectedClothes_2);
    connect(ui->clothList, SIGNAL(cellClicked(int,int)), this, SLOT(locateCloth(int)));
}

void MainWindow::on_electronicSearchKey_editingFinished()
{
    disconnect(ui->electronicList, SIGNAL(cellClicked(int,int)), this, SLOT(locateElectronic(int)));
    selectElectronics(ui->electronicSearchKey->text(), selectedElectronics_2);
    ui->electronicList->setRowCount(selectedElectronics_2.size());
    int i = 0;
    initElectronics(i, ui->electronicList, selectedElectronics_2);
    connect(ui->electronicList, SIGNAL(cellClicked(int,int)), this, SLOT(locateElectronic(int)));
}

void MainWindow::selectBooks(QString s, QVector <book_CLASS *> &select)
{
    QVector <book_CLASS *> pNULLBooks;
    select.swap(pNULLBooks);
    for (int i = 0; i < books.size(); i++)
        if (books[i].get_title().contains(s) ||
                books[i].get_description().contains(s) ||
                books[i].get_place().contains(s) ||
                books[i].get_author().contains(s) ||
                books[i].get_publisher().contains(s))
            select.push_back(&books[i]);
}

void MainWindow::selectClothes(QString s, QVector <cloth_CLASS *> &select)
{
    QVector <cloth_CLASS *> pNULLClothes;
    select.swap(pNULLClothes);
    for (int i = 0; i < clothes.size(); i++)
        if (clothes[i].get_title().contains(s) ||
                clothes[i].get_description().contains(s) ||
                clothes[i].get_place().contains(s) ||
                clothes[i].get_material().contains(s) ||
                clothes[i].get_brand().contains(s) ||
                clothes[i].get_clothType().contains(s))
            select.push_back(&clothes[i]);
}

void MainWindow::selectElectronics(QString s, QVector <electronic_CLASS *> &select)
{
    QVector <electronic_CLASS *> pNULLElectronics;
    select.swap(pNULLElectronics);
    for (int i = 0; i < electronics.size(); i++)
        if (electronics[i].get_title().contains(s) ||
                electronics[i].get_description().contains(s) ||
                electronics[i].get_place().contains(s) ||
                electronics[i].get_voltage().contains(s) ||
                electronics[i].get_power().contains(s) ||
                electronics[i].get_brand().contains(s) ||
                electronics[i].get_originalPlace().contains(s))
            select.push_back(&electronics[i]);
}

void MainWindow::calculatePrice()
{
    float originalPrice = 0, currentPrice = 0, thisPrice;
    int i = 0;
    book_CLASS * thatBook = NULL;
    cloth_CLASS * thatCloth = NULL;
    electronic_CLASS * thatElectronic = NULL;
    for (int k = 0; k < thisUser->get_orderNUM(); k++)
    {
        if (0 != thisUser->get_order(k)->get_state())
            continue;
        thisPrice = 0;
        if (check[i]->isChecked())
        {
            findItem(thisUser->get_order(k)->get_id(), thatBook, thatCloth, thatElectronic);
            if (NULL != thatBook)
            {
                thisPrice = thatBook->get_price() * number[i]->value();
                originalPrice += thisPrice;
                currentPrice += thatBook->calcPrice(thisPrice);
            }
            else if (NULL != thatCloth)
            {
                thisPrice = thatCloth->get_price() * number[i]->value();
                originalPrice += thisPrice;
                currentPrice += thatCloth->calcPrice(thisPrice);
            }
            else
            {
                thisPrice = thatElectronic->get_price() * number[i]->value();
                originalPrice += thisPrice;
                currentPrice += thatElectronic->calcPrice(thisPrice);
            }
        }
        i++;
    }
    ui->originalPrice->setText(QString("%1").arg(originalPrice));
    ui->currentPrice->setText(QString("%1").arg(currentPrice));
    ui->discount->setText(QString("%1").arg(originalPrice - currentPrice));
}

void MainWindow::deleteOrder(int i)
{
    if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("确认删除该项吗？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        book_CLASS * thisBook = NULL;
        cloth_CLASS * thisCloth = NULL;
        electronic_CLASS * thisElectronic = NULL;
        int k, l;
        for (l = 0, k = -1; l <= i;)
        {
            k++;
            if (0 == thisUser->get_order(k)->get_state())
                l++;
        }
        findItem(thisUser->get_order(k)->get_id(), thisBook, thisCloth, thisElectronic);
        if (NULL != thisBook)
            thisBook->reSetNUM(thisUser->get_order(k)->get_number());
        else if (NULL != thisCloth)
            thisCloth->reSetNUM(thisUser->get_order(k)->get_number());
        else
            thisElectronic->reSetNUM(thisUser->get_order(k)->get_number());
        thisUser->delete_order(k);
        initTrolleyList();
        calculatePrice();
    }
}

void MainWindow::on_allCheck_clicked()
{
    if (ui->allCheck->isChecked())
    {
        for (int i = 0; i < check.size(); i++)
            check[i]->setChecked(true);
        calculatePrice();
    }
    else
    {
        for (int i = 0; i < check.size(); i++)
            check[i]->setChecked(false);
        calculatePrice();
    }
}

void MainWindow::on_myInfo_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::initAddressTable()
{
    disconnect(ui->addressTable, SIGNAL(cellClicked(int,int)), this, SLOT(locateThisAddress(int)));
    ui->deleteAddress->setEnabled(false);
    ui->addressTable->setRowCount(thisUser->get_addressNUM());
    for (int i = 0; i < thisUser->get_addressNUM(); i++)
    {
        ui->addressTable->setItem(i, 0, new QTableWidgetItem(thisUser->get_address(i)->get_city()));
        ui->addressTable->setItem(i, 1, new QTableWidgetItem(thisUser->get_address(i)->get_receiverName()));
        ui->addressTable->setItem(i, 2, new QTableWidgetItem(thisUser->get_address(i)->get_receiverPhone()));
        ui->addressTable->setItem(i, 3, new QTableWidgetItem(thisUser->get_address(i)->get_detailedAddress()));
    }
    connect(ui->addressTable, SIGNAL(cellClicked(int,int)), this, SLOT(locateThisAddress(int)));
}

void MainWindow::clearAddressGroup()
{
    ui->selectCity->setCurrentIndex(-1);
    ui->receiverName->clear();
    ui->receiverPhone->clear();
    ui->detailedAddress->clear();
}

void MainWindow::initAddressGroup(address_CLASS * a)
{
    ui->selectCity->setCurrentText(a->get_city());
    ui->receiverName->setText(a->get_receiverName());
    ui->receiverPhone->setText(a->get_receiverPhone());
    ui->detailedAddress->setText(a->get_detailedAddress());
}

void MainWindow::locateThisAddress(int i)
{
    thisAddress = i;
    ui->deleteAddress->setEnabled(true);
    initAddressGroup(thisUser->get_address(thisAddress));
}

void MainWindow::on_editPassword_clicked()
{
    bool ok = false;
    QString oldPassword = QInputDialog::getText(this, "step1", QString::fromLocal8Bit("请输入原始密码："), QLineEdit::Password, QString::null, &ok);
    if (ok)
    {
        if (oldPassword == thisUser->get_password())
        {
            ok = false;
            while (!ok)
            {
                QString newPassword = QInputDialog::getText(this, "step2", QString::fromLocal8Bit("请输入新密码："), QLineEdit::Password, QString::null, &ok);
                if (ok)
                {
                    if (newPassword.size() < 6)
                    {
                        QMessageBox::critical(this,"Error",QString::fromLocal8Bit("密码过短！\n请重新输入！"),QMessageBox::Yes);
                        ok = false;
                    }
                    else
                    {
                        ok = false;
                        QString newPassword2 = QInputDialog::getText(this, "step3", QString::fromLocal8Bit("请重复新密码："), QLineEdit::Password, QString::null, &ok);
                        if (ok)
                        {
                            if (newPassword == newPassword2)
                            {
                                thisUser->editPassword(newPassword);
                                QMessageBox::information(this,"ok",QString::fromLocal8Bit("修改成功，请牢记密码！"),QMessageBox::Yes);
                            }
                            else
                            {
                                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("两次密码不一致！\n请重新输入！"),QMessageBox::Yes);
                                ok = false;
                            }
                        }
                        else
                            break;
                    }
                }
                else
                    break;
            }
        }
        else
            QMessageBox::critical(this,"Error",QString::fromLocal8Bit("原始密码错误！"),QMessageBox::Yes);
    }
}

void MainWindow::on_deleteBankAccount_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确认删除吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        tcpSocket->abort();
        tcpSocket->connectToHost("127.0.0.1", 6666);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << (quint16) 0;
        out << (quint16) 0; //解绑账户的操作
        out.device()->seek(0);
        out << (quint16) (block.size() - sizeof(quint16));
        tcpSocket->write(block);
        thisUser->set_bankAccount("-1");
        initPersonalInfo();
    }
}

void MainWindow::on_deleteAddress_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确认删除吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        thisUser->delete_address(thisAddress);
        thisAddress = -1;
        initAddressTable();
    }
}

void MainWindow::on_saveAddress_clicked()
{
    if (ui->receiverName->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入收件人姓名！"), QMessageBox::Yes);
        ui->receiverName->setFocus();
    }
    else if (ui->receiverPhone->text().isEmpty() || ui->receiverPhone->text().size() != 11)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的收件人手机号！"), QMessageBox::Yes);
        ui->receiverPhone->setFocus();
    }
    else if (ui->detailedAddress->toPlainText().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的收件人地址！"), QMessageBox::Yes);
        ui->detailedAddress->setFocus();
    }
    else if (ui->detailedAddress->toPlainText().contains(' '))
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("地址中不能含有空格！"), QMessageBox::Yes);
        ui->detailedAddress->setFocus();
    }
    else
    {
        address_CLASS anotherAddress = address_CLASS(ui->selectCity->currentText(),
                                                     ui->receiverName->text(),
                                                     ui->receiverPhone->text(),
                                                     ui->detailedAddress->toPlainText());
        thisUser->add_address(anotherAddress);
        QMessageBox::information(this, "success", QString::fromLocal8Bit("添加地址成功！"), QMessageBox::Yes);
        initAddressTable();
        clearAddressGroup();
        thisAddress = -1;
    }
}

void MainWindow::on_orders_triggered()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->orderTab->setCurrentIndex(0);
}

void MainWindow::on_pay_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确认付款吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        for (int i = 0, j = 0; i < thisUser->get_orderNUM(); i++)
        {
            if (0 == thisUser->get_order(i)->get_state())
            {
                if (check[j]->isChecked())
                    thisUser->get_order(i)->refreshState(1);
                j++;
            }
        }
        initTrolleyList();
        calculatePrice();
        initOrderList();
        ui->stackedWidget->setCurrentIndex(6);
        ui->orderTab->setCurrentIndex(0);
    }
}

void MainWindow::on_deleteAllPayingOrders_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确定要清除未付款订单吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        book_CLASS * thatBook = NULL;
        cloth_CLASS * thatCloth = NULL;
        electronic_CLASS * thatElectronic = NULL;
        for (int i = 0; i < thisUser->get_orderNUM(); i++)
        {
            if (1 != thisUser->get_order(i)->get_state())
                continue;
            findItem(thisUser->get_order(i)->get_id(), thatBook, thatCloth, thatElectronic);
            if (NULL != thatBook)
                thatBook->reSetNUM(thisUser->get_order(i)->get_number());
            else if (NULL != thatCloth)
                thatCloth->reSetNUM(thisUser->get_order(i)->get_number());
            else
                thatElectronic->reSetNUM(thisUser->get_order(i)->get_number());
            thisUser->delete_order(i);
            i--;
        }
        initOrderList();
    }
}

void MainWindow::on_clearAllFinishedOrders_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确定要清除历史订单吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        for (int i = 0; i < thisUser->get_orderNUM(); i++)
        {
            if (2 != thisUser->get_order(i)->get_state())
                continue;
            thisUser->delete_order(i);
            i--;
        }
        initOrderList();
    }
}

void MainWindow::on_addBankAccount_clicked()
{
    bool ok;
    TCPusername = QInputDialog::getText(this, "step1", QString::fromLocal8Bit("请输入银行账户用户名："), QLineEdit::Normal, QString::null, &ok);
    QString password = QInputDialog::getText(this, "step2", QString::fromLocal8Bit("请输入银行账户密码："), QLineEdit::Password, QString::null, &ok);
    tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost("127.0.0.1",6665);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint16) 0;
    out << (quint16) 0; //绑定账户的操作
    out << TCPusername;
    out << password;
    out.device()->seek(0);
    out << (quint16) (block.size() - sizeof(quint16));
    tcpSocket->write(block);
}

void MainWindow::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_5); // 设置数据流版本，这里要和服务器端相同
    blockSize = 0;
    if (tcpSocket->bytesAvailable() < (int) (2 * sizeof(quint16)))
        return;
    in >> blockSize;
    if (tcpSocket->bytesAvailable() < blockSize)
        return;
    in >> optionType;
    if (0 == optionType)
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("用户名或密码错误！\n绑定失败！"), QMessageBox::Yes);
    else if (1 == optionType)
    {
        QMessageBox::information(this, "success", QString::fromLocal8Bit("绑定成功！"), QMessageBox::Yes);
        thisUser->set_bankAccount(TCPusername);
        initPersonalInfo();
    }
    else if (2 == optionType)
    {
        if (tcpSocket->bytesAvailable() == 0)
        {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("没有可用的银行卡！"), QMessageBox::Yes);
            return;
        }
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << (quint16) 0;
        out << (quint16) 3; //支付密码
        QString cardID, payCode;
        QStringList idList;
        while (!in.atEnd())
        {
            in >> cardID;
            idList << cardID;
        }
        cardID = QInputDialog::getItem(this, QString::fromLocal8Bit("可用的银行卡如下："),
                                       QString::fromLocal8Bit("请选择一张银行卡"),
                                       idList, false);
        payCode = QInputDialog::getText(this, "step2", QString::fromLocal8Bit("请输入支付密码："), QLineEdit::Password, QString::null);
        out << cardID << payCode;
        out.device()->seek(0);
        out << (quint16) (block.size() - sizeof(quint16));
        tcpSocket->write(block);
    }
    else if (3 == optionType)
    {
        QMessageBox::information(this, "success", QString::fromLocal8Bit("支付成功！"), QMessageBox::Yes);
        for (int i = 0; i < thisUser->get_orderNUM(); i++)
            if (1 == thisUser->get_order(i)->get_state())
                thisUser->get_order(i)->refreshState(2);
        initOrderList();
    }
    else if (4 == optionType)
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("密码错误！"), QMessageBox::Yes);
}

void MainWindow::waitSec(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<100*s)
        QCoreApplication::processEvents();
}

void MainWindow::on_payNow_clicked()
{
    tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost("127.0.0.1",6666);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint16) 0;
    out << (quint16) 2; //支付操作
    out << QString("%1").arg(totalPrice);
    out.device()->seek(0);
    out << (quint16) (block.size() - sizeof(quint16));
    tcpSocket->write(block);
}
