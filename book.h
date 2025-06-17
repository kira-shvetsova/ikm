#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QSqlDatabase>

/**
 * @brief Класс, представляющий книгу в библиотечной системе
 *
 * Класс инкапсулирует данные о книге и предоставляет интерфейс для работы с ними.
 * Содержит методы для получения информации о книге и базовых операций с базой данных.
 */
class Book {
public:
    /**
     * @brief Конструктор книги
     * @param title Название книги (не должно быть пустым)
     * @param author Автор в формате "Фамилия Имя"
     * @param category Категория книги (например, "Фантастика")
     * @param availability_count Количество доступных экземпляров (≥ 0)
     * @throws std::invalid_argument Если availability_count отрицательный
     */
    Book(const QString& title, const QString& author,
         const QString& category, int availability_count);

    /// @name Геттеры
    /// @{
    /**
     * @brief Получить название книги
     * @return Константная ссылка на название книги
     */
    const QString& title() const;

    /**
     * @brief Получить автора книги
     * @return Константная ссылка на имя автора
     */
    const QString& author() const;

    /**
     * @brief Получить категорию книги
     * @return Константная ссылка на категорию
     */
    const QString& category() const;

    /**
     * @brief Получить количество доступных экземпляров
     * @return Количество доступных книг (≥ 0)
     */
    int availability_count() const;
    /// @}

    /// @name Сеттеры и методы работы с БД
    /// @{
    /**
     * @brief Установить количество доступных экземпляров
     * @param count Новое количество (≥ 0)
     * @throws std::invalid_argument Если count отрицательный
     */
    void set_availability_count(int count);

    /**
     * @brief Найти ID автора в базе данных
     * @param author Имя автора для поиска
     * @return ID автора или -1 если не найден
     */
    int get_author_id(const QString& author);

    /**
     * @brief Найти ID категории в базе данных
     * @param category Название категории для поиска
     * @return ID категории или -1 если не найдена
     */
    int get_category_id(const QString& category);

    /**
     * @brief Добавить книгу в базу данных
     * @param book Книга для добавления
     * @return true если операция успешна
     * @throws std::runtime_error При ошибке SQL-запроса
     */
    bool add_book(const Book& book);

    /**
     * @brief Удалить книгу из базы данных
     * @param book Книга для удаления
     * @return true если операция успешна
     * @throws std::runtime_error При ошибке SQL-запроса
     */
    bool delete_book(const Book& book);
    /// @}

private:
    QString title_;              ///< Название книги
    QString author_;             ///< Автор книги
    QString category_;           ///< Категория книги
    int availability_count_;     ///< Количество доступных экземпляров
};

#endif // BOOK_H
