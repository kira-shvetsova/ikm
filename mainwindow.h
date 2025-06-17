#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "book.h"
#include "user.h"
#include "library.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Класс главного окна приложения библиотеки
 *
 * Обеспечивает графический интерфейс для работы с библиотечной системой:
 * - Просмотр книг и пользователей
 * - Добавление/удаление книг
 * - Выдача/возврат книг
 * - Учет операций
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор главного окна
     * @param lib Указатель на объект работы с библиотекой (не может быть nullptr)
     * @param parent Родительский виджет
     */
    explicit MainWindow(Library *lib, QWidget *parent = nullptr);

    /**
     * @brief Деструктор
     */
    ~MainWindow();

private slots:
    /// @name Слоты для обработки действий пользователя
    /// @{
    /**
     * @brief Обновляет таблицу с книгами
     */
    void on_ViewBooks_clicked();

    /**
     * @brief Обновляет таблицу с пользователями
     */
    void on_ViewUsers_clicked();

    /**
     * @brief Открывает диалог добавления новой книги
     */
    void on_AddBook_clicked();

    /**
     * @brief Удаляет выбранную книгу
     */
    void on_DeleteBook_clicked();

    /**
     * @brief Выдает книгу читателю
     */
    void on_GiveBook_clicked();

    /**
     * @brief Принимает книгу от читателя
     */
    void on_ReturnBook_clicked();

    /**
     * @brief Открывает диалог добавления нового пользователя
     */
    void on_AddUser_clicked();

    /**
     * @brief Удаляет выбранного пользователя
     */
    void on_DeleteUser_clicked();

    /**
     * @brief Открывает отчет по учету операций
     */
    void on_Accouting_clicked();
    /// @}

private:
    Ui::MainWindow *ui; ///< Указатель на сгенерированный UI-класс
    Library *library;   ///< Указатель на объект работы с библиотекой
};

#endif // MAINWINDOW_H
