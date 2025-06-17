#ifndef LIBRARY_H
#define LIBRARY_H

#include <QString>
#include <QSqlDatabase>
#include "book.h"
#include "user.h"

/**
 * @brief Класс для работы с библиотечной базой данных
 *
 * Предоставляет функционал для подключения к БД и управления книгами:
 * - Подключение к PostgreSQL
 * - Выдача книг читателям
 * - Прием возвращенных книг
 */
class Library {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Library();

    /**
     * @brief Подключается к PostgreSQL базе данных
     * @param host Адрес сервера БД
     * @param db_name Имя базы данных
     * @param user Имя пользователя БД
     * @param password Пароль пользователя
     * @return true при успешном подключении
     */
    bool connect_to_database(const QString& host, const QString& db_name,
                             const QString& user, const QString& password);

    /**
     * @brief Выдает книгу читателю
     * @param book Книга для выдачи
     * @param card_number Номер читательского билета
     * @return true если операция успешна
     */
    bool issue_book(const Book& book, const QString& card_number);

    /**
     * @brief Принимает возвращенную книгу
     * @param card_number Номер читательского билета
     * @param book Возвращаемая книга
     * @return true если операция успешна
     */
    bool return_book(const QString& card_number, const Book& book);

private:
    QSqlDatabase db_; ///< Подключение к базе данных
};

#endif // LIBRARY_H
