#ifndef USER_H
#define USER_H

#include <QString>
#include <QSqlDatabase>

/**
 * @brief Класс, представляющий пользователя библиотеки
 *
 * Инкапсулирует данные о пользователе библиотечной системы:
 * - Имя и фамилию
 * - Номер читательского билета
 * - Методы работы с базой данных
 */
class User {
public:
    /**
     * @brief Конструктор пользователя
     * @param firstName Имя пользователя (не может быть пустым)
     * @param lastName Фамилия пользователя (не может быть пустой)
     * @param cardNumber Номер читательского билета (уникальный)
     * @throws std::invalid_argument Если имя, фамилия или номер билета пустые
     */
    User(const QString& firstName, const QString& lastName, const QString& cardNumber);

    /// @name Геттеры
    /// @{
    /**
     * @brief Получить имя пользователя
     * @return Имя пользователя
     */
    QString getFirstName() const;

    /**
     * @brief Получить фамилию пользователя
     * @return Фамилия пользователя
     */
    QString getLastName() const;

    /**
     * @brief Получить номер читательского билета
     * @return Номер билета
     */
    QString getCardNumber() const;
    /// @}

    /// @name Методы работы с базой данных
    /// @{
    /**
     * @brief Добавить пользователя в базу данных
     * @param user Объект пользователя для добавления
     * @return true если операция успешна
     * @throws std::runtime_error При ошибке SQL-запроса
     */
    bool addUser(const User& user);

    /**
     * @brief Удалить пользователя из базы данных
     * @param cardNumber Номер читательского билета
     * @return true если операция успешна
     * @throws std::runtime_error При ошибке SQL-запроса
     */
    bool deleteUser(const QString& cardNumber);
    /// @}

private:
    QString u_firstName;    ///< Имя пользователя
    QString u_lastName;     ///< Фамилия пользователя
    QString u_cardNumber;   ///< Номер читательского билета
};

#endif // USER_H
