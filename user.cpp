#include "user.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

/**
 * @brief Конструктор класса User
 * @param firstName Имя пользователя
 * @param lastName Фамилия пользователя
 * @param cardNumber Номер библиотечной карты
 */
User::User(const QString &firstName, const QString &lastName, const QString &cardNumber)
    : u_firstName(firstName), u_lastName(lastName), u_cardNumber(cardNumber)
{
}

/**
 * @brief Возвращает имя пользователя
 * @return Имя пользователя
 */
QString User::getFirstName() const {
    return u_firstName;
}

/**
 * @brief Возвращает фамилию пользователя
 * @return Фамилия пользователя
 */
QString User::getLastName() const {
    return u_lastName;
}

/**
 * @brief Возвращает номер библиотечной карты
 * @return Номер библиотечной карты
 */
QString User::getCardNumber() const {
    return u_cardNumber;
}

/**
 * @brief Добавляет пользователя в базу данных
 * @param user Объект пользователя для добавления
 * @return true при успешном добавлении, false при ошибке
 * @note В случае ошибки выводит сообщение в консоль отладки
 */
bool User::addUser(const User &user) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (first_name, last_name, library_card_number) "
                  "VALUES (:firstName, :lastName, :cardNumber)");
    query.bindValue(":firstName", user.getFirstName());
    query.bindValue(":lastName", user.getLastName());
    query.bindValue(":cardNumber", user.getCardNumber());
    if (!query.exec()) {
        qWarning() << "Ошибка добавления пользователя:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Удаляет пользователя из базы данных
 * @param cardNumber Номер библиотечной карты пользователя для удаления
 * @return true при успешном удалении, false при ошибке
 * @note В случае ошибки выводит сообщение в консоль отладки
 */
bool User::deleteUser(const QString &cardNumber) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE library_card_number = :cardNumber");
    query.bindValue(":cardNumber", cardNumber);
    if (!query.exec()) {
        qWarning() << "Ошибка удаления пользователя:" << query.lastError().text();
        return false;
    }
    return true;
}
