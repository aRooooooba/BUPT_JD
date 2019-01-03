#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T18:22:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BUPT_JD + tcpClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    user_class.cpp \
    varities.cpp \
    date_class.cpp \
    registernewuser.cpp \
    address_class.cpp \
    commodity_class.cpp \
    discount_class.cpp \
    book_class.cpp \
    cloth_class.cpp \
    electronic_class.cpp \
    details.cpp \
    order_class.cpp

HEADERS  += mainwindow.h \
    login.h \
    user_class.h \
    varieties.h \
    date_class.h \
    registernewuser.h \
    address_class.h \
    commodity_class.h \
    discount_class.h \
    book_class.h \
    cloth_class.h \
    electronic_class.h \
    details.h \
    order_class.h

FORMS    += mainwindow.ui \
    login.ui \
    registernewuser.ui \
    details.ui

RESOURCES += \
    pics/background/background.qrc \
    pics/buttonIcon/buttonicon.qrc \
    pics/labelIcon/labelicon.qrc
