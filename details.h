#ifndef DETAILS_H
#define DETAILS_H

#include <QDialog>
#include "commodity_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"

namespace Ui {
class details;
}

class details : public QDialog
{
    Q_OBJECT

public:
    explicit details(QWidget * parent = 0,
                     book_CLASS * _thisBook = NULL,
                     cloth_CLASS * _thisCloth = NULL,
                     electronic_CLASS * _thisElectronic = NULL);
    ~details();

private slots:
    void on_addToTrolley_clicked();

private:
    book_CLASS * thisBook;
    cloth_CLASS * thisCloth;
    electronic_CLASS * thisElectronic;
    Ui::details *ui;
    void initBookUI();
    void initClothUI();
    void initElectronicUI();
};

#endif // DETAILS_H
