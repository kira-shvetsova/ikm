QT += core gui widgets sql


# Настраиваем компилятор и стандарт C++
CONFIG += c++17
CONFIG -= app_bundle
CONFIG += console

# Путь к исходным файлам проекта
SOURCES += \
    book.cpp \
    library.cpp \
    main.cpp \
    mainwindow.cpp \
    user.cpp

HEADERS += \
    book.h \
    library.h \
    mainwindow.h \
    user.h

FORMS += \
    mainwindow.ui
RESOURCES += resources.qrc
