#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QSqlQuery>
#include <QMessageBox>

/**
 * @brief Конструктор главного окна
 * @param lib Указатель на объект Library для работы с библиотекой
 * @param parent Родительский виджет (по умолчанию nullptr)
 */
MainWindow::MainWindow(Library *lib, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), library(lib)
{
    ui->setupUi(this);
}

/**
 * @brief Деструктор главного окна
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Слот для отображения списка книг
 * @details Заполняет таблицу данными о книгах из базы данных,
 * включая название, автора, категорию и количество доступных экземпляров
 */
void MainWindow::on_ViewBooks_clicked()
{
    QSqlQuery query("SELECT b.title, a.first_name || ' ' || a.last_name AS author, "
                    "c.name AS category, b.availability_count FROM books b "
                    "JOIN authors a ON b.author_id = a.author_id "
                    "JOIN categories c ON b.category_id = c.category_id");

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Название", "Автор", "Категория", "Доступно"});

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 4; ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}

/**
 * @brief Слот для отображения списка пользователей
 * @details Заполняет таблицу данными о пользователях из базы данных,
 * включая номер библиотечной карточки и ФИО пользователя
 */
void MainWindow::on_ViewUsers_clicked()
{
    QSqlQuery query("SELECT u.library_card_number, u.first_name || ' ' || u.last_name AS user FROM users u");

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Номер карточки", "Пользователь"});

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ++row;
    }
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 250);
}

/**
 * @brief Слот для добавления новой книги
 * @details Запрашивает у пользователя данные о книге через диалоговые окна
 * и добавляет новую книгу в базу данных
 */
void MainWindow::on_AddBook_clicked()
{
    QString title = QInputDialog::getText(this, "Добавить книгу", "Введите название книги:");
    if (title.isEmpty()) return;

    QString author = QInputDialog::getText(this, "Добавить книгу", "Введите автора:");
    QString category = QInputDialog::getText(this, "Добавить книгу", "Введите жанр книги:");
    int availabilityCount = QInputDialog::getInt(this, "Добавить книгу", "Введите количество доступных книг:");

    Book book(title, author, category, availabilityCount);
    if (book.add_book(book)) {
        QMessageBox::information(this, "Успех", "Книга успешно добавлена.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить книгу.");
    }
}

/**
 * @brief Слот для удаления книги
 * @details Запрашивает название и автора книги для удаления из базы данных
 */
void MainWindow::on_DeleteBook_clicked()
{
    QString title = QInputDialog::getText(this, "Удалить книгу", "Введите название книги:");
    QString author = QInputDialog::getText(this, "Удалить книгу", "Введите автора книги:");
    Book book(title, author, "", 0);
    if (book.delete_book(book)) {
        QMessageBox::information(this, "Успех", "Книга успешно удалена.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить книгу.");
    }
}

/**
 * @brief Слот для выдачи книги пользователю
 * @details Запрашивает данные о книге и номере карточки пользователя,
 * затем выполняет операцию выдачи через объект Library
 */
void MainWindow::on_GiveBook_clicked()
{
    QString title = QInputDialog::getText(this, "Выдать книгу", "Введите название книги:");
    QString author = QInputDialog::getText(this, "Выдать книгу", "Введите автора книги:");
    QString cardNumber = QInputDialog::getText(this, "Выдать книгу", "Введите номер карточки пользователя:");
    Book book(title, author, "", 0);

    if (library->issue_book(book, cardNumber)) {
        QMessageBox::information(this, "Успех", "Книга успешно выдана.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось выдать книгу.");
    }
}

/**
 * @brief Слот для возврата книги в библиотеку
 * @details Запрашивает данные о книге и номере карточки пользователя,
 * затем выполняет операцию возврата через объект Library
 */
void MainWindow::on_ReturnBook_clicked()
{
    QString libraryCardNumber = QInputDialog::getText(this, "Вернуть книгу", "Введите номер карточки пользователя:");
    QString title = QInputDialog::getText(this, "Вернуть книгу", "Введите название книги:");
    QString author = QInputDialog::getText(this, "Вернуть книгу", "Введите автора книги:");
    Book book(title, author, "", 0);

    if (library->return_book(libraryCardNumber, book)) {
        QMessageBox::information(this, "Успех", "Книга успешно возвращена.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось вернуть книгу.");
    }
}

/**
 * @brief Слот для добавления нового пользователя
 * @details Запрашивает данные о пользователе через диалоговые окна
 * и добавляет нового пользователя в базу данных
 */
void MainWindow::on_AddUser_clicked()
{
    QString firstName = QInputDialog::getText(this, "Добавить пользователя", "Введите имя:");
    QString lastName = QInputDialog::getText(this, "Добавить пользователя", "Введите фамилию:");
    QString cardNumber = QInputDialog::getText(this, "Добавить пользователя", "Введите номер карточки:");

    User user(firstName, lastName, cardNumber);
    if (user.addUser(user)) {
        QMessageBox::information(this, "Успех", "Пользователь успешно добавлен.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить пользователя.");
    }
}

/**
 * @brief Слот для удаления пользователя
 * @details Запрашивает номер карточки пользователя для удаления из базы данных
 */
void MainWindow::on_DeleteUser_clicked()
{
    QString cardNumber = QInputDialog::getText(this, "Удалить пользователя", "Введите номер карточки пользователя для удаления:");
    User user("", "", cardNumber);
    if (user.deleteUser(cardNumber)) {
        QMessageBox::information(this, "Успех", "Пользователь успешно удален.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить пользователя.");
    }
}

/**
 * @brief Слот для отображения учета книг
 * @details Показывает информацию о всех выданных книгах,
 * включая данные о пользователе, книге и датах выдачи/возврата
 */
void MainWindow::on_Accouting_clicked()
{
    QSqlQuery query("SELECT u.library_card_number, u.first_name || ' ' || u.last_name AS user, "
                    "b.title, a.first_name || ' ' || a.last_name AS author, "
                    "bl.loan_date, bl.return_date FROM book_loans bl "
                    "JOIN books b ON bl.book_id = b.book_id "
                    "JOIN authors a ON b.author_id = a.author_id "
                    "JOIN users u ON bl.user_id = u.user_id");

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"Номер карточки", "Пользователь", "Книга", "Автор", "Дата выдачи", "Дата возврата"});

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}
