#include "mainwindow.h"
#include "library.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

/**
 * @brief Точка входа в приложение
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код возврата приложения
 *
 * @note Выполняет следующие действия:
 * 1. Создает объект QApplication
 * 2. Загружает и применяет стили из файла style.qss
 * 3. Создает и инициализирует объект Library
 * 4. Подключается к базе данных PostgreSQL
 * 5. Создает и отображает главное окно приложения
 * 6. Запускает главный цикл обработки событий
 *
 * @warning Путь к файлу стилей жестко закодирован, может потребоваться изменение
 * при переносе приложения на другую систему
 */
int main(int argc, char *argv[])
{
    // Инициализация Qt-приложения
    QApplication a(argc, argv);

    // Загрузка и применение стилей из QSS-файла
    QFile styleFile("C:/Users/yaros/Desktop/newnew_build/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QTextStream ts(&styleFile);
        QString style = ts.readAll();
        a.setStyleSheet(style);
    }

    // Инициализация библиотечной системы
    Library library;
    // Подключение к базе данных PostgreSQL
    if (!library.connect_to_database("localhost", "post", "postgres", "Kirakira8922310")) {
        return -1; // Возврат с кодом ошибки при неудачном подключении
    }

    // Создание и отображение главного окна
    MainWindow w(&library);
    w.show();

    // Запуск главного цикла обработки событий
    return a.exec();
}
