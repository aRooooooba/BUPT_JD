#include "details.h"
#include "ui_details.h"
#include "commodity_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"
#include "order_class.h"
#include "user_class.h"
#include "varieties.h"
#include <qlabel.h>
#include <qmessagebox.h>

details::details(QWidget * parent,
                 book_CLASS * _thisBook,
                 cloth_CLASS * _thisCloth,
                 electronic_CLASS * _thisElectronic) :
    QDialog(parent),
    ui(new Ui::details)
{
    thisBook = _thisBook;
    thisCloth = _thisCloth;
    thisElectronic = _thisElectronic;
    ui->setupUi(this);
    ui->author->setVisible(false);
    ui->publisher->setVisible(false);
    ui->material->setVisible(false);
    ui->brand->setVisible(false);
    ui->clothType->setVisible(false);
    ui->voltage->setVisible(false);
    ui->power->setVisible(false);
    ui->originalPlace->setVisible(false);
    ui->discountLabel->setVisible(true);
    ui->discount->setVisible(true);
    if (NULL != thisBook)
        initBookUI();
    else if (NULL != thisCloth)
        initClothUI();
    else
        initElectronicUI();
}

details::~details()
{
    delete ui;
}

void details::initBookUI()
{
    ui->author->setVisible(true);
    ui->publisher->setVisible(true);
    ui->title->setText(thisBook->get_title());
    ui->id->setText(thisBook->get_id());
    ui->description->setPlainText(thisBook->get_description());
    ui->price->setText(QString("%1").arg(thisBook->get_price()));
    if (0 == thisBook->get_discount().get_discountType())
    {
        ui->discountLabel->setVisible(false);
        ui->discount->setVisible(false);
    }
    else if (1 == thisBook->get_discount().get_discountType())
    {
        if (0 == thisBook->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场%1折！")
                                  .arg(thisBook->get_discount().get_multiple() * 10));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，打%2折！")
                                  .arg(thisBook->get_discount().get_limit())
                                  .arg(thisBook->get_discount().get_multiple() * 10));
    }
    else
    {
        if (0 == thisBook->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场直降%1！")
                                  .arg(thisBook->get_discount().get_minus()));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，直降%2！")
                                  .arg(thisBook->get_discount().get_limit())
                                  .arg(thisBook->get_discount().get_minus()));
    }
    ui->number->setText(QString::number(thisBook->get_number(), 10));
    ui->place->setText(thisBook->get_place());
    ui->picture->setStyleSheet("QLabel" + thisBook->get_URL());
    ui->author->setText(QString::fromLocal8Bit("%1 著").arg(thisBook->get_author()));
    ui->publisher->setText(QString::fromLocal8Bit("%1 出版").arg(thisBook->get_publisher()));
}

void details::initClothUI()
{
    ui->material->setVisible(true);
    ui->brand->setVisible(true);
    ui->clothType->setVisible(true);
    ui->title->setText(thisCloth->get_title());
    ui->id->setText(thisCloth->get_id());
    ui->description->setPlainText(thisCloth->get_description());
    ui->price->setText(QString("%1").arg(thisCloth->get_price()));
    if (0 == thisCloth->get_discount().get_discountType())
    {
        ui->discountLabel->setVisible(false);
        ui->discount->setVisible(false);
    }
    else if (1 == thisCloth->get_discount().get_discountType())
    {
        if (0 == thisCloth->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场%1折！")
                                  .arg(thisCloth->get_discount().get_multiple() * 10));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，打%2折！")
                                  .arg(thisCloth->get_discount().get_limit())
                                  .arg(thisCloth->get_discount().get_multiple() * 10));
    }
    else
    {
        if (0 == thisCloth->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场直降%1！")
                                  .arg(thisCloth->get_discount().get_minus()));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，直降%2！")
                                  .arg(thisCloth->get_discount().get_limit())
                                  .arg(thisCloth->get_discount().get_minus()));
    }
    ui->number->setText(QString::number(thisCloth->get_number(), 10));
    ui->place->setText(thisCloth->get_place());
    ui->picture->setStyleSheet("QLabel" + thisCloth->get_URL());
    ui->material->setText(QString::fromLocal8Bit("%1 制").arg(thisCloth->get_material()));
    ui->brand->setText(QString::fromLocal8Bit("品牌：%1").arg(thisCloth->get_brand()));
    ui->clothType->setText(QString::fromLocal8Bit("衣服类型：%1").arg(thisCloth->get_clothType()));
}

void details::initElectronicUI()
{
    ui->voltage->setVisible(true);
    ui->power->setVisible(true);
    ui->brand->setVisible(true);
    ui->originalPlace->setVisible(true);
    ui->title->setText(thisElectronic->get_title());
    ui->id->setText(thisElectronic->get_id());
    ui->description->setPlainText(thisElectronic->get_description());
    ui->price->setText(QString("%1").arg(thisElectronic->get_price()));
    if (0 == thisElectronic->get_discount().get_discountType())
    {
        ui->discountLabel->setVisible(false);
        ui->discount->setVisible(false);
    }
    else if (1 == thisElectronic->get_discount().get_discountType())
    {
        if (0 == thisElectronic->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场%1折！")
                                  .arg(thisElectronic->get_discount().get_multiple() * 10));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，打%2折！")
                                  .arg(thisElectronic->get_discount().get_limit())
                                  .arg(thisElectronic->get_discount().get_multiple() * 10));
    }
    else
    {
        if (0 == thisElectronic->get_discount().get_limit())
            ui->discount->setText(QString::fromLocal8Bit("全场直降%1！")
                                  .arg(thisElectronic->get_discount().get_minus()));
        else
            ui->discount->setText(QString::fromLocal8Bit("满%1，直降%2！")
                                  .arg(thisElectronic->get_discount().get_limit())
                                  .arg(thisElectronic->get_discount().get_minus()));
    }
    ui->number->setText(QString::number(thisElectronic->get_number(), 10));
    ui->place->setText(thisElectronic->get_place());
    ui->picture->setStyleSheet("QLabel" + thisElectronic->get_URL());
    ui->voltage->setText(QString::fromLocal8Bit("额定电压：%1").arg(thisElectronic->get_voltage()));
    ui->power->setText(QString::fromLocal8Bit("额定功率：%1").arg(thisElectronic->get_power()));
    ui->brand->setText(QString::fromLocal8Bit("品牌：%1").arg(thisElectronic->get_brand()));
    ui->originalPlace->setText(QString::fromLocal8Bit("生产地：%1").arg(thisElectronic->get_originalPlace()));
}

void details::on_addToTrolley_clicked()
{
    QString id;
    int number = ui->itemNUM->value();
    if (NULL != thisBook)
    {
        if (number <= thisBook->get_number())
        {
            thisBook->soldSome(number);
            id = thisBook->get_id();
            int i;
            for (i = 0; i < thisUser->get_orderNUM(); i++)
                if (id == thisUser->get_order(i)->get_id() && thisUser->get_order(i)->get_state() != 2)
                    break;
            if (i != thisUser->get_orderNUM())
                QMessageBox::critical(this, "Error", QString::fromLocal8Bit("订单中已有该物品！\n请先付款！"), QMessageBox::Yes);
            else
            {
                order_CLASS anotherOrder = order_CLASS(0, id, number, date);
                thisUser->add_order(anotherOrder);
                QMessageBox::information(this, "success", QString::fromLocal8Bit("加入购物车成功！\n请在3天之内结账！"), QMessageBox::Yes);
            }
        }
        else
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("加入购物车失败！\n物品数量不足！"), QMessageBox::Yes);
    }
    else if (NULL != thisCloth)
    {
        if (number <= thisCloth->get_number())
        {
            thisCloth->soldSome(number);
            id = thisCloth->get_id();
            int i;
            for (i = 0; i < thisUser->get_orderNUM(); i++)
                if (id == thisUser->get_order(i)->get_id() && thisUser->get_order(i)->get_state() != 2)
                    break;
            if (i != thisUser->get_orderNUM())
                QMessageBox::critical(this, "Error", QString::fromLocal8Bit("订单中已有该物品！\n请先付款！"), QMessageBox::Yes);
            else
            {
                order_CLASS anotherOrder = order_CLASS(0, id, number, date);
                thisUser->add_order(anotherOrder);
                saveUsers();
                QMessageBox::information(this, "success", QString::fromLocal8Bit("加入购物车成功！\n请在3天之内结账！"), QMessageBox::Yes);
            }
        }
        else
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("加入购物车失败！\n物品数量不足！"), QMessageBox::Yes);
    }
    else
    {
        if (number <= thisElectronic->get_number())
        {
            thisElectronic->soldSome(number);
            id = thisElectronic->get_id();
            int i;
            for (i = 0; i < thisUser->get_orderNUM(); i++)
                if (id == thisUser->get_order(i)->get_id() && thisUser->get_order(i)->get_state() != 2)
                    break;
            if (i != thisUser->get_orderNUM())
                QMessageBox::critical(this, "Error", QString::fromLocal8Bit("订单中已有该物品！\n请先付款！"), QMessageBox::Yes);
            else
            {
                order_CLASS anotherOrder = order_CLASS(0, id, number, date);
                thisUser->add_order(anotherOrder);
                QMessageBox::information(this, "success", QString::fromLocal8Bit("加入购物车成功！\n请在3天之内结账！"), QMessageBox::Yes);
            }
        }
        else
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("加入购物车失败！\n物品数量不足！"), QMessageBox::Yes);
    }
    accept();
}
