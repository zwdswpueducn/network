QT += core gui
greaterThan(QT_MAJOR_VERSION,4): QT += widgets printsupport
TEMPLATE      = app
HEADERS       = cell.h \
                finddialog.h \
                gotocelldialog.h \
                mainwindow.h \
                sortdialog.h \
                spreadsheet.h \
    qcustomplot.h
SOURCES       = cell.cpp \
                finddialog.cpp \
                gotocelldialog.cpp \
                main.cpp \
                mainwindow.cpp \
                sortdialog.cpp \
                spreadsheet.cpp \
    qcustomplot.cpp
FORMS         = gotocelldialog.ui \
                sortdialog.ui
RESOURCES     = spreadsheet.qrc
