#include "library.h"
#include "book.h"
#include "user.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

/**
 * @brief Конструктор по умолчанию класса Library
 */
Library::Library() {}

/**
 * @brief Подключается к базе данных PostgreSQL
 * @param host Хост базы данных
 * @param db_name Имя базы данных
 * @param user Имя пользователя БД
 * @param password Пароль пользователя БД
 * @return true при успешном подключении, false при ошибке
 * @note Использует драйвер QPSQL (PostgreSQL)
 */
bool Library::connect_to_database(const QString &host, const QString &db_name,
                                  const QString &user, const QString &password)
{
    db_ = QSqlDatabase::addDatabase("QPSQL");
    db_.setHostName(host);
    db_.setDatabaseName(db_name);
    db_.setUserName(user);
    db_.setPassword(password);

    if (!db_.open()) {
        qWarning() << "Ошибка подключения к базе данных: " << db_.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Выдает книгу пользователю
 * @param book Книга для выдачи (объект класса Book)
 * @param card_number Номер библиотечной карты пользователя
 * @return true при успешной выдаче, false при ошибке
 * @note Выполняет следующие действия:
 * 1. Проверяет доступность книги
 * 2. Находит ID пользователя
 * 3. Создает запись о выдаче
 * 4. Уменьшает счетчик доступных книг
 */
bool Library::issue_book(const Book &book, const QString &card_number) {
    // Проверка доступности книги
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT book_id, availability_count FROM books WHERE title = :title AND author_id = "
                       "(SELECT author_id FROM authors WHERE first_name || ' ' || last_name = :author)");
    checkQuery.bindValue(":title", book.title());
    checkQuery.bindValue(":author", book.author());

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value("availability_count").toInt() <= 0) {
        qWarning() << "Ошибка выдачи книги: книга недоступна или не найдена.";
        return false;
    }
    int bookId = checkQuery.value("book_id").toInt();

    // Поиск пользователя
    QSqlQuery userQuery;
    userQuery.prepare("SELECT user_id FROM users WHERE library_card_number = :libraryCardNumber");
    userQuery.bindValue(":libraryCardNumber", card_number);
    if (!userQuery.exec() || !userQuery.next()) {
        qWarning() << "Ошибка получения пользователя: пользователь не найден.";
        return false;
    }
    int userId = userQuery.value("user_id").toInt();

    // Создание записи о выдаче
    QSqlQuery issueQuery;
    issueQuery.prepare("INSERT INTO book_loans (book_id, user_id, loan_date) VALUES (:bookId, :userId, CURRENT_DATE)");
    issueQuery.bindValue(":bookId", bookId);
    issueQuery.bindValue(":userId", userId);
    if (!issueQuery.exec()) {
        qWarning() << "Ошибка выдачи книги:" << issueQuery.lastError().text();
        return false;
    }

    // Обновление счетчика книг
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET availability_count = availability_count - 1 WHERE book_id = :bookId");
    updateQuery.bindValue(":bookId", bookId);
    if (!updateQuery.exec()) {
        qWarning() << "Ошибка обновления количества книг:" << updateQuery.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Возвращает книгу в библиотеку
 * @param card_number Номер библиотечной карты пользователя
 * @param book Книга для возврата (объект класса Book)
 * @return true при успешном возврате, false при ошибке
 * @note Выполняет следующие действия:
 * 1. Проверяет существование пользователя
 * 2. Находит ID книги
 * 3. Проверяет, что книга была выдана
 * 4. Отмечает возврат в БД
 * 5. Увеличивает счетчик доступных книг
 */
bool Library::return_book(const QString &card_number, const Book &book) {
    // Поиск пользователя
    QSqlQuery userQuery;
    userQuery.prepare("SELECT user_id FROM users WHERE library_card_number = :libraryCardNumber");
    userQuery.bindValue(":libraryCardNumber", card_number);
    if (!userQuery.exec() || !userQuery.next()) {
        qWarning() << "Ошибка: пользователь не найден по номеру библиотечного билета.";
        return false;
    }
    int userId = userQuery.value("user_id").toInt();

    // Поиск книги
    QSqlQuery bookQuery;
    bookQuery.prepare("SELECT book_id FROM books WHERE title = :title AND author_id = "
                      "(SELECT author_id FROM authors WHERE first_name || ' ' || last_name = :author)");
    bookQuery.bindValue(":title", book.title());
    bookQuery.bindValue(":author", book.author());
    if (!bookQuery.exec() || !bookQuery.next()) {
        qWarning() << "Ошибка возврата книги: книга не найдена.";
        return false;
    }
    int bookId = bookQuery.value("book_id").toInt();

    // Проверка выдачи книги
    QSqlQuery loanQuery;
    loanQuery.prepare("SELECT loan_id FROM book_loans WHERE book_id = :bookId AND user_id = :userId AND return_date IS NULL");
    loanQuery.bindValue(":bookId", bookId);
    loanQuery.bindValue(":userId", userId);
    if (!loanQuery.exec() || !loanQuery.next()) {
        qWarning() << "Ошибка: книга не была выдана этому пользователю или уже возвращена.";
        return false;
    }
    int loanId = loanQuery.value("loan_id").toInt();

    // Отметка о возврате
    QSqlQuery returnQuery;
    returnQuery.prepare("UPDATE book_loans SET return_date = CURRENT_DATE WHERE loan_id = :loanId");
    returnQuery.bindValue(":loanId", loanId);
    if (!returnQuery.exec()) {
        qWarning() << "Ошибка возврата книги:" << returnQuery.lastError().text();
        return false;
    }

    // Обновление счетчика книг
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET availability_count = availability_count + 1 WHERE book_id = :bookId");
    updateQuery.bindValue(":bookId", bookId);
    if (!updateQuery.exec()) {
        qWarning() << "Ошибка обновления количества книг:" << updateQuery.lastError().text();
        return false;
    }
    return true;
}
