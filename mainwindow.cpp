/**
 * @file       mainwindow.cpp
 * @brief      学生通讯录管理系统主窗口的实现
 * @copyright  Copyright (c) 2025
 * @license    MIT
 * @author     lzq
 * @version    1.1 (重构以使用Qt Designer UI)
 * @date       2025-11-13
 *
 * @par        版本历史:
 *             V1.0: [lzq] [2025-11-13] [创建文件并实现基本功能]
 *             V1.1: [lzq] [2025-11-13] [重构UI到.ui文件，仅保留业务逻辑]
 *
 * @par        大数据处理说明:
 *             (保留为空)
 */

#include "mainwindow.h"
#include "ui_StudentMessageManagementSystem.h" // UI文件生成的头文件

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtConcurrent>
#include <QThread>
#include <QMetaObject>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StudentMessageManagementSystemClass) // 正确初始化ui指针
{
    // 第一行必须调用Qt Designer生成的UI
    ui->setupUi(this);

    // Test Chinese message box display (commented out)
    // QMessageBox::information(this, "编码测试", "你好，世界！");

    // 初始化数据库
    initDatabase();

    // 创建表（如果不存在）
    createTable();

    // 初始化分页变量
    currentPage = 0;
    totalPages = 0;
    totalCount = 0;

    // 设置初始状态信息
    updateStatus("Ready");

    // 将菜单操作与相应的槽函数关联
    // 第一行之后可以添加create...Menu()函数

    // 文件菜单
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewContactList);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onLoadFromFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveToFile);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close); // 直接连接到窗口的close()

    // 编辑菜单
    connect(ui->actionInsert, &QAction::triggered, this, &MainWindow::onInsertStudent);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteStudent);

    // 查询菜单
    connect(ui->actionQueryByID, &QAction::triggered, this, &MainWindow::onQueryByID);

    // 使用 lambda 表达式确保 resetPage 参数为 true
    connect(ui->actionQueryByName, &QAction::triggered, this, [this](){
        onQueryByName(true);
    });
    connect(ui->actionQueryYoungest, &QAction::triggered, this, &MainWindow::onQueryYoungestStudent);
    connect(ui->actionQueryByCoordX, &QAction::triggered, this, [this](){
        onQueryByAddressCoordX(true);
    });

    // 显示菜单
    // 同样修复所有带 bool 参数的槽的连接
    connect(ui->actionDisplayPreorder, &QAction::triggered, this, [this](){
        onDisplaySortByName(true);
    });
    connect(ui->actionDisplayInorder, &QAction::triggered, this, [this](){
        onDisplaySortByID_ASC(true);
    });
    connect(ui->actionDisplayPostorder, &QAction::triggered, this, [this](){
        onDisplaySortByID_DESC(true);
    });

    // 帮助菜单
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    // 分页按钮连接
    connect(ui->prevPageButton, &QPushButton::clicked, this, &MainWindow::onPrevPage);
    connect(ui->nextPageButton, &QPushButton::clicked, this, &MainWindow::onNextPage);

    // 更新分页控件初始状态
    updatePageControls();

    // 更新菜单项文本
    ui->actionDisplayPreorder->setText(QString::fromUtf8("按姓名排序"));
    ui->actionDisplayInorder->setText(QString::fromUtf8("按ID升序"));
    ui->actionDisplayPostorder->setText(QString::fromUtf8("按ID降序"));

    // 显示欢迎信息
    displayOutput(
        QString::fromUtf8("欢迎使用学生通讯录管理系统\n\n") +
        QString::fromUtf8("主要功能:\n") +
        QString::fromUtf8("* 文件: 新建, 打开, 保存, 退出\n") +
        QString::fromUtf8("* 编辑: 添加和删除学生记录\n") +
        QString::fromUtf8("* 查询: 按学号、姓名查询，最小年龄、按地址坐标查询\n") +
        QString::fromUtf8("* 显示: 按姓名排序、按ID升序、按ID降序\n") +
        QString::fromUtf8("* 帮助: 关于软件\n"));
}

MainWindow::~MainWindow()
{
    // 释放ui指针，避免内存泄漏
    delete ui;
}

// ==================== SQLite Database Implementation ====================

void MainWindow::initDatabase()
{
    // 初始化数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");

    // 打开数据库
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error",
                             "Failed to open database: " + db.lastError().text());
        return;
    }

    updateStatus("Database initialized successfully");
}

void MainWindow::createTable()
{
    QSqlQuery query(db);

    // 创建学生表
    bool success = query.exec("CREATE TABLE IF NOT EXISTS students ("
                            "studentID TEXT PRIMARY KEY NOT NULL,"
                            "name TEXT NOT NULL,"
                            "birthDate TEXT NOT NULL,"
                            "gender TEXT NOT NULL,"
                            "addressName TEXT NOT NULL,"
                            "addressCoordX INTEGER NOT NULL,"
                            "addressCoordY INTEGER NOT NULL"
                            ");");

    if (!success) {
        qDebug() << "Failed to create students table:" << query.lastError().text();
        return;
    }

    // 为name字段创建索引，优化查询性能
    success = query.exec("CREATE INDEX IF NOT EXISTS idx_students_name ON students(name);");
    if (!success) {
        qDebug() << "Failed to create index on name:" << query.lastError().text();
        return;
    }

    // 为addressCoordX字段创建索引，优化查询性能
    success = query.exec("CREATE INDEX IF NOT EXISTS idx_students_addressCoordX ON students(addressCoordX);");
    if (!success) {
        qDebug() << "Failed to create index on addressCoordX:" << query.lastError().text();
        return;
    }

    updateStatus("Database tables and indexes created successfully");
}

// ==================== 辅助函数 (Helper Functions) ====================
// create...Menu() 函数已被移除

QString MainWindow::formatStudentInfo(const Student &student) const
{
    return QString("ID: %1\n"
                   "Name: %2\n"
                   "Birth Date: %3\n"
                   "Gender: %4\n"
                   "Address: %5\n"
                   "Coordinates: (%6, %7)\n")
        .arg(student.studentID)
        .arg(student.name)
        .arg(student.birthDate.toString("yyyy-MM-dd"))
        .arg(student.gender)
        .arg(student.addressName)
        .arg(student.addressCoordX)
        .arg(student.addressCoordY);
}

QString MainWindow::formatMultipleStudents(const QVector<Student> &students) const
{
    if (students.isEmpty())
    {
        return "No matching students found";
    }

    QString result;
    for (int i = 0; i < students.size(); ++i)
    {
        result += QString("===== Student %1 =====\n").arg(i + 1);
        result += formatStudentInfo(students[i]);
        result += "\n";
    }
    return result;
}

void MainWindow::updateStatus(const QString &message)
{
    // statusBar() 是QMainWindow的内置函数，可以直接使用
    statusBar()->showMessage(message);
}

void MainWindow::displayOutput(const QString &text)
{
    // 通过ui指针访问.ui文件中定义的displayArea控件
    ui->displayArea->setText(text);
}

void MainWindow::appendOutput(const QString &text)
{
    // 通过ui指针访问displayArea
    ui->displayArea->append(text);
}

// ==================== File Menu Implementation ====================

void MainWindow::onNewContactList()
{
    int result = QMessageBox::question(this,
                                       "Confirmation",
                                       "Clear all data?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes)
    {
        QSqlQuery query(db);
        bool success = query.exec("DELETE FROM students;");

        if (success) {
            displayOutput("Contact list cleared");
            updateStatus("Contact list cleared");
        } else {
            QMessageBox::warning(this, "Error",
                                "Failed to clear contact list: " + query.lastError().text());
            updateStatus("Failed to clear contact list");
        }
    }
}

/**
 * @brief 从文件加载学生数据
 * 打开文件对话框让用户选择数据文件，然后解析并导入学生记录
 */
void MainWindow::onLoadFromFile()
{
    // 打开文件对话框选择要加载的数据文件
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open File", "",
                                                    "Text Files (*.txt *.csv);;All Files (*)");

    // 如果用户取消选择则返回
    if (filePath.isEmpty())
        return;

    // 禁用主窗口
    this->setEnabled(false);
    updateStatus("Importing data in background...");
    displayOutput("Importing data in background... This may take several minutes.");

    // 使用QtConcurrent将文件导入任务放到后台线程执行
    (void)QtConcurrent::run([this, filePath]() {

        // --- 1. 为这个新线程创建独立的数据库连接 ---
        // 我们需要一个唯一的连接名称，例如基于线程ID
        QString connectionName = QString("importer_thread_%1").arg(quintptr(QThread::currentThreadId()));
        int successCount = 0;
        int failCount = 0;

        // 使用花括号确保 QSqlDatabase 对象在 lambda 结束前被销毁
        {
            QSqlDatabase threadDb = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            threadDb.setDatabaseName("students.db");

            if (!threadDb.open()) {
                qWarning() << "Thread DB Error: Failed to open database in thread:" << threadDb.lastError().text();
                // 返回主线程报告错误
                QMetaObject::invokeMethod(this, [this]() {
                    QMessageBox::critical(this, "Error", "Failed to open database in background thread.");
                    this->setEnabled(true);
                    updateStatus("Import failed");
                }, Qt::QueuedConnection);
                return;
            }

            // 尝试打开选中的文件
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qWarning() << "Cannot open file in thread:" << filePath;
                QMetaObject::invokeMethod(this, [this]() {
                    QMessageBox::critical(this, "Error", "Cannot open file in background thread.");
                    this->setEnabled(true);
                    updateStatus("Import failed");
                }, Qt::QueuedConnection);
                threadDb.close(); // 关闭数据库连接
                return;
            }

            QTextStream in(&file);

            // --- 2. 边读边插，分批处理 ---
            const int BATCH_SIZE = 5000; // 每5000条插入一次
            QVariantList studentIDs, names, birthDates, genders, addressNames, coordXs, coordYs;

            // 开始事务
            if (!threadDb.transaction()) {
                 qWarning() << "Failed to start transaction:" << threadDb.lastError().text();
                 QMetaObject::invokeMethod(this, [this]() {
                     QMessageBox::critical(this, "Error", "Failed to start database transaction.");
                     this->setEnabled(true);
                     updateStatus("Import failed");
                 }, Qt::QueuedConnection);
                 file.close();
                 threadDb.close();
                 return;
            }

            QSqlQuery query(threadDb);
            query.prepare("INSERT OR REPLACE INTO students (studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?)");

            while (!in.atEnd())
            {
                QString line = in.readLine().trimmed();
                if (line.isEmpty()) continue;

                QStringList parts = line.split(',');
                if (parts.size() != 7) {
                    failCount++;
                    continue;
                }

                QDate birthDate = QDate::fromString(parts[2].trimmed(), "yyyy-MM-dd");
                if (parts[0].trimmed().isEmpty() || !birthDate.isValid()) {
                    failCount++;
                    continue;
                }

                // 添加到批处理列表
                studentIDs.append(parts[0].trimmed());
                names.append(parts[1].trimmed());
                birthDates.append(birthDate);
                genders.append(parts[3].trimmed());
                addressNames.append(parts[4].trimmed());
                coordXs.append(parts[5].trimmed().toInt());
                coordYs.append(parts[6].trimmed().toInt());

                // --- 3. 达到批处理大小，执行插入 ---
                if (studentIDs.size() >= BATCH_SIZE) {
                    query.addBindValue(studentIDs);
                    query.addBindValue(names);
                    query.addBindValue(birthDates);
                    query.addBindValue(genders);
                    query.addBindValue(addressNames);
                    query.addBindValue(coordXs);
                    query.addBindValue(coordYs);

                    if (!query.execBatch()) {
                        qWarning() << "Batch insert failed:" << query.lastError().text();
                        failCount += studentIDs.size(); // 这批全都算失败
                    } else {
                        successCount += studentIDs.size();
                    }

                    // 清空列表以备下一批
                    studentIDs.clear(); names.clear(); birthDates.clear(); genders.clear();
                    addressNames.clear(); coordXs.clear(); coordYs.clear();
                }
            }

            // --- 4. 插入剩余的记录 ---
            if (!studentIDs.isEmpty()) {
                query.addBindValue(studentIDs);
                query.addBindValue(names);
                query.addBindValue(birthDates);
                query.addBindValue(genders);
                query.addBindValue(addressNames);
                query.addBindValue(coordXs);
                query.addBindValue(coordYs);

                if (!query.execBatch()) {
                    qWarning() << "Final batch insert failed:" << query.lastError().text();
                    failCount += studentIDs.size();
                } else {
                    successCount += studentIDs.size();
                }
            }

            file.close();

            // 提交事务
            if (!threadDb.commit()) {
                qWarning() << "[Debug] 数据库错误: 事务提交失败!" << threadDb.lastError().text();
                qWarning() << "Transaction commit failed:" << threadDb.lastError().text();
                failCount = successCount + failCount; // 提交失败，所有都算失败
                successCount = 0;
                threadDb.rollback();
            }

            threadDb.close();
        }

        // --- 5. 移除线程特定的数据库连接 ---
        QSqlDatabase::removeDatabase(connectionName);

        // --- 6. 导入完成，返回主线程更新UI ---
        QMetaObject::invokeMethod(this, [this, successCount, failCount]() {
            QString message = QString("Successfully imported %1 records\nFailed (parse/insert): %2")
                                  .arg(successCount)
                                  .arg(failCount);
            displayOutput(message);
            updateStatus(message);
            this->setEnabled(true);
        }, Qt::QueuedConnection);
    });
}

void MainWindow::onSaveToFile()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save File", "",
                                                    "Text Files (*.txt *.csv);;All Files (*)");

    if (filePath.isEmpty())
        return;

    // --- UI 准备 ---
    this->setEnabled(false);
    updateStatus("Exporting data in background...");
    displayOutput("Exporting data in background... This may take a few minutes.");

    // --- 使用 QtConcurrent 在后台线程执行导出 ---
    (void)QtConcurrent::run([this, filePath]() {
        QString connectionName = QString("exporter_thread_%1").arg(quintptr(QThread::currentThreadId()));
        int recordCount = 0;
        bool exportError = false;
        QString lastError;

        { // 数据库连接的范围
            QSqlDatabase threadDb = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            threadDb.setDatabaseName("students.db");

            if (!threadDb.open()) {
                qWarning() << "Failed to open database in export thread:" << threadDb.lastError().text();
                exportError = true;
                lastError = threadDb.lastError().text();
            } else {
                QFile file(filePath); // 在这个线程中创建文件对象
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    qWarning() << "Failed to open file in export thread:" << file.errorString();
                    exportError = true;
                    lastError = "Cannot create file in background thread.";
                } else {
                    QTextStream out(&file);
                    QSqlQuery query(threadDb);
                    // (关键) 使用 query.setForwardOnly(true) 优化大查询，减少内存缓冲
                    query.setForwardOnly(true);

                    if (query.exec("SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                                    "FROM students ORDER BY studentID")) {
                        while (query.next()) {
                            out << query.value(0).toString() << ","
                                << query.value(1).toString() << ","
                                << query.value(2).toString() << "," // 日期在数据库中存为TEXT
                                << query.value(3).toString() << ","
                                << query.value(4).toString() << ","
                                << query.value(5).toInt() << ","
                                << query.value(6).toInt() << "\n";
                            recordCount++;
                        }
                    } else {
                        qWarning() << "Failed to query students in export thread:" << query.lastError().text();
                        exportError = true;
                        lastError = query.lastError().text();
                    }
                    file.close();
                }
                threadDb.close();
            }
        } // 数据库连接在此销毁
        QSqlDatabase::removeDatabase(connectionName);

        // --- 返回主线程更新UI ---
        QMetaObject::invokeMethod(this, [this, recordCount, exportError, lastError]() {
            if (exportError) {
                QMessageBox::critical(this, "Error", "Failed to export data: " + lastError);
                updateStatus("Export failed.");
            } else {
                QString message = QString("Saved %1 records to file").arg(recordCount);
                displayOutput(message);
                updateStatus(message);
            }
            this->setEnabled(true);
        }, Qt::QueuedConnection);
    });
}

// ==================== Edit Menu Implementation ====================

void MainWindow::onInsertStudent()
{
    bool ok;

    QString studentID = QInputDialog::getText(this, "Insert Student", "Student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || studentID.isEmpty())
        return;

    // 检查学号是否已经存在
    QSqlQuery query(db);
    query.prepare("SELECT studentID FROM students WHERE studentID = ?");
    query.addBindValue(studentID);
    if (!query.exec() || query.next())
    {
        QMessageBox::warning(this, "Error", "Student ID already exists");
        return;
    }

    QString name = QInputDialog::getText(this, "Insert Student", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    QString dateStr = QInputDialog::getText(this, "Insert Student", "Birth Date (yyyy-MM-dd):", QLineEdit::Normal, "", &ok);
    if (!ok)
        return;
    QDate birthDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!birthDate.isValid())
    {
        QMessageBox::warning(this, "Error", "Invalid date format");
        return;
    }

    QString gender = QInputDialog::getText(this, "Insert Student", "Gender:", QLineEdit::Normal, "", &ok);
    if (!ok)
        return;

    QString addressName = QInputDialog::getText(this, "Insert Student", "Address Name:", QLineEdit::Normal, "", &ok);
    if (!ok)
        return;

    int coordX = QInputDialog::getInt(this, "Insert Student", "Address Coordinate X:", 0, -10000, 10000, 1, &ok);
    if (!ok)
        return;

    int coordY = QInputDialog::getInt(this, "Insert Student", "Address Coordinate Y:", 0, -10000, 10000, 1, &ok);
    if (!ok)
        return;

    // 使用INSERT INTO插入学生数据，不允许覆盖
    query.prepare("INSERT INTO students (studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(studentID);
    query.addBindValue(name);
    query.addBindValue(birthDate); // QDate类型会自动转换为字符串
    query.addBindValue(gender);
    query.addBindValue(addressName);
    query.addBindValue(coordX);
    query.addBindValue(coordY);

    if (query.exec())
    {
        QString message = QString("Student %1 (%2) added successfully").arg(studentID, name);
        displayOutput(message);
        updateStatus(message);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to add student: " + query.lastError().text());
    }
}

void MainWindow::onDeleteStudent()
{
    bool ok;
    QString studentID = QInputDialog::getText(this, "Delete Student", "Enter Student ID to delete:", QLineEdit::Normal, "", &ok);

    if (!ok || studentID.isEmpty())
        return;

    int result = QMessageBox::question(this, "Confirmation", QString("Delete student with ID %1?").arg(studentID), QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes)
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM students WHERE studentID = ?");
        query.addBindValue(studentID);

        if (query.exec() && query.numRowsAffected() > 0)
        {
            QString message = QString("Student %1 deleted").arg(studentID);
            displayOutput(message);
            updateStatus(message);
        }
        else
        {
            QMessageBox::information(this, "Info", "Student not found");
        }
    }
}

// ==================== Query Menu Implementation ====================

void MainWindow::onQueryByID()
{
    bool ok;
    QString studentID = QInputDialog::getText(this, "Query by ID", "Enter Student ID:", QLineEdit::Normal, "", &ok);

    if (!ok || studentID.isEmpty())
        return;

    QSqlQuery query(db);
    query.prepare("SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                 "FROM students WHERE studentID = ?");
    query.addBindValue(studentID);

    if (query.exec() && query.next())
    {
        // 构造Student对象
        Student result;
        result.studentID = query.value(0).toString();
        result.name = query.value(1).toString();
        result.birthDate = query.value(2).toDate();
        result.gender = query.value(3).toString();
        result.addressName = query.value(4).toString();
        result.addressCoordX = query.value(5).toInt();
        result.addressCoordY = query.value(6).toInt();

        QString output = "===== Query Result =====\n";
        output += formatStudentInfo(result);
        displayOutput(output);
        updateStatus("Query successful");
    }
    else
    {
        displayOutput(QString("Student with ID %1 not found").arg(studentID));
        updateStatus("Query failed");
    }
}

void MainWindow::onQueryByName(bool resetPage)
{
    QString name;

    // Handle page reset if requested
    if (resetPage) {
        bool ok;
        name = QInputDialog::getText(this, "Query by Name", "Enter Name:", QLineEdit::Normal, "", &ok);

        if (!ok || name.isEmpty())
            return;

        currentPage = 0;
        lastQueryType = "queryByName";
        lastQueryParam = name;

        // 查询 1: 获取总记录数（精确匹配名称）- 仅在重置页面时执行
        QSqlQuery countQuery(db);
        countQuery.prepare("SELECT COUNT(*) FROM students WHERE name = ?");
        countQuery.addBindValue(name);

        if (!countQuery.exec() || !countQuery.next()) {
            QMessageBox::critical(this, "Error", "Failed to query total count: " + countQuery.lastError().text());
            updatePageControls();
            return;
        }
        totalCount = countQuery.value(0).toInt();
        totalPages = (totalCount + PageSize - 1) / PageSize;
    } else {
        // Use the last query parameter
        name = lastQueryParam.toString();
    }

    if (totalCount == 0) {
        displayOutput(QString("No students found with name %1").arg(name));
        updateStatus("Query complete - no results");
        updatePageControls();
        return;
    }

    // 查询 2: 获取当前页的数据（精确匹配名称，按学号排序）
    QSqlQuery query(db);

    // ==================== 修复开始 ====================
    // 将 LIMIT 和 OFFSET 值直接格式化到字符串中
    int offset = currentPage * PageSize;
    QString sql = QString(
                      "SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                      "FROM students WHERE name = ? ORDER BY studentID LIMIT %1 OFFSET %2"
                      ).arg(PageSize).arg(offset);

    query.prepare(sql);
    query.addBindValue(name); // 仅绑定 'name'
    // ==================== 修复结束 ====================

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to query students: " + query.lastError().text());
        updatePageControls();
        return;
    }

    QVector<Student> results;
    while (query.next()) {
        Student student;
        student.studentID = query.value(0).toString();
        student.name = query.value(1).toString();
        student.birthDate = query.value(2).toDate();
        student.gender = query.value(3).toString();
        student.addressName = query.value(4).toString();
        student.addressCoordX = query.value(5).toInt();
        student.addressCoordY = query.value(6).toInt();
        results.append(student);
    }

    // Simplified output - pagination info now shown in pageLabel
    QString output = QString("===== Query Results for '%1' (Total %2 records) =====\n\n").arg(name).arg(totalCount);
    output += formatMultipleStudents(results);
    displayOutput(output);
    updateStatus(QString("Query complete - found %1 results").arg(totalCount));
    updatePageControls();
}

void MainWindow::onQueryYoungestStudent()
{
    QSqlQuery query(db);
    query.prepare("SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                 "FROM students ORDER BY birthDate DESC LIMIT 1");

    if (!query.exec() || !query.next())
    {
        displayOutput("Contact list is empty");
        updateStatus("Query failed");
        return;
    }

    // 构造Student对象
    Student youngest;
    youngest.studentID = query.value(0).toString();
    youngest.name = query.value(1).toString();
    youngest.birthDate = query.value(2).toDate();
    youngest.gender = query.value(3).toString();
    youngest.addressName = query.value(4).toString();
    youngest.addressCoordX = query.value(5).toInt();
    youngest.addressCoordY = query.value(6).toInt();

    QString output = "===== Youngest Student =====\n";
    output += formatStudentInfo(youngest);
    displayOutput(output);
    updateStatus("Query successful");
}

void MainWindow::onQueryByAddressCoordX(bool resetPage)
{
    int coordX;

    // Handle page reset if requested
    if (resetPage) {
        bool ok;
        coordX = QInputDialog::getInt(this, "Query by Coordinate X", "Enter Coordinate X:", 0, -10000, 10000, 1, &ok);

        if (!ok)
            return;

        currentPage = 0;
        lastQueryType = "queryByAddressCoordX";
        lastQueryParam = coordX;

        // 查询 1: 获取总记录数（精确匹配addressCoordX）- 仅在重置页面时执行
        QSqlQuery countQuery(db);
        countQuery.prepare("SELECT COUNT(*) FROM students WHERE addressCoordX = ?");
        countQuery.addBindValue(coordX);

        if (!countQuery.exec() || !countQuery.next()) {
            QMessageBox::critical(this, "Error", "Failed to query total count: " + countQuery.lastError().text());
            updatePageControls();
            return;
        }
        totalCount = countQuery.value(0).toInt();
        totalPages = (totalCount + PageSize - 1) / PageSize;
    } else {
        // Use the last query parameter
        coordX = lastQueryParam.toInt();
    }

    if (totalCount == 0) {
        displayOutput(QString("No students found at coordinate X = %1").arg(coordX));
        updateStatus("Query complete - no results");
        updatePageControls();
        return;
    }

    // 查询 2: 获取当前页的数据（精确匹配addressCoordX，按学号排序）
    QSqlQuery query(db);

    // ==================== 修复开始 ====================
    // 将 LIMIT 和 OFFSET 值直接格式化到字符串中
    int offset = currentPage * PageSize;
    QString sql = QString(
                      "SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                      "FROM students WHERE addressCoordX = ? ORDER BY studentID LIMIT %1 OFFSET %2"
                      ).arg(PageSize).arg(offset);

    query.prepare(sql);
    query.addBindValue(coordX); // 仅绑定 'coordX'
    // ==================== 修复结束 ====================


    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to query students: " + query.lastError().text());
        updatePageControls();
        return;
    }

    QVector<Student> results;
    while (query.next()) {
        Student student;
        student.studentID = query.value(0).toString();
        student.name = query.value(1).toString();
        student.birthDate = query.value(2).toDate();
        student.gender = query.value(3).toString();
        student.addressName = query.value(4).toString();
        student.addressCoordX = query.value(5).toInt();
        student.addressCoordY = query.value(6).toInt();
        results.append(student);
    }

    // Simplified output - pagination info now shown in pageLabel
    QString output = QString("===== Query Results for X = %1 (Total %2 records) =====\n\n").arg(coordX).arg(totalCount);
    output += formatMultipleStudents(results);
    displayOutput(output);
    updateStatus(QString("Query complete - found %1 results").arg(totalCount));
    updatePageControls();
}
// mainwindow.cpp

/**
 * @brief 按姓名排序显示学生信息
 */
void MainWindow::onDisplaySortByName(bool resetPage)
{
    // Handle page reset if requested
    if (resetPage) {
        currentPage = 0;
        lastQueryType = "displaySortByName";
        lastQueryParam = QVariant();

        // 查询 1: 获取总记录数
        QSqlQuery countQuery(db);
        countQuery.prepare("SELECT COUNT(*) FROM students");

        if (!countQuery.exec() || !countQuery.next()) {
            QMessageBox::critical(this, "Error", "Failed to query total count: " + countQuery.lastError().text());
            return;
        }
        totalCount = countQuery.value(0).toInt();
        totalPages = (totalCount + PageSize - 1) / PageSize;
    }

    if (totalCount == 0) {
        displayOutput("Contact list is empty");
        updatePageControls();
        return;
    }

    QSqlQuery query(db);

    // ==================== 修复开始 ====================
    // 将 LIMIT 和 OFFSET 值直接格式化到字符串中
    int offset = currentPage * PageSize;
    QString sql = QString(
                      "SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                      "FROM students ORDER BY name LIMIT %1 OFFSET %2"
                      ).arg(PageSize).arg(offset);

    // 因为没有 '?' 占位符, 我们可以直接 .exec(sql)
    if (!query.exec(sql)) {
        // ==================== 修复结束 ====================
        QMessageBox::critical(this, "Error", "Failed to query students: " + query.lastError().text());
        updatePageControls();
        return;
    }

    QVector<Student> students;
    while (query.next()) {
        Student student;
        student.studentID = query.value(0).toString();
        student.name = query.value(1).toString();
        student.birthDate = query.value(2).toDate();
        student.gender = query.value(3).toString();
        student.addressName = query.value(4).toString();
        student.addressCoordX = query.value(5).toInt();
        student.addressCoordY = query.value(6).toInt();
        students.append(student);
    }

    QString output = QString("===== Display Sorted by Name (Total %1 records) =====\n\n").arg(totalCount);
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("Display complete (Sorted by Name) - %1 total records").arg(totalCount));
    updatePageControls();
}
/**
 * @brief (已修复) 显示中序遍历（按ID排序）
 *
 * 修复了 QSqlQuery 对象重用 bug。
 * (逻辑正确，保持不变)
 */
void MainWindow::onDisplaySortByID_ASC(bool resetPage)
{
    // Handle page reset if requested
    if (resetPage) {
        currentPage = 0;
        lastQueryType = "displaySortByID_ASC"; // <-- 更新查询类型
        lastQueryParam = QVariant();

        // 查询 1: 获取总记录数
        QSqlQuery countQuery(db);
        countQuery.prepare("SELECT COUNT(*) FROM students");

        if (!countQuery.exec() || !countQuery.next()) {
            QMessageBox::critical(this, "Error", "Failed to query total count: " + countQuery.lastError().text());
            return;
        }
        totalCount = countQuery.value(0).toInt();
        totalPages = (totalCount + PageSize - 1) / PageSize;
    }

    if (totalCount == 0) {
        displayOutput("Contact list is empty");
        updatePageControls();
        return;
    }

    QSqlQuery query(db);

    // ==================== 修复开始 ====================
    // 将 LIMIT 和 OFFSET 值直接格式化到字符串中
    int offset = currentPage * PageSize;
    QString sql = QString(
                      "SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                      "FROM students ORDER BY studentID LIMIT %1 OFFSET %2"
                      ).arg(PageSize).arg(offset);

    // 因为没有 '?' 占位符, 我们可以直接 .exec(sql)
    if (!query.exec(sql)) {
        // ==================== 修复结束 ====================
        QMessageBox::critical(this, "Error", "Failed to query students: " + query.lastError().text());
        updatePageControls();
        return;
    }

    QVector<Student> students;
    while (query.next()) {
        Student student;
        student.studentID = query.value(0).toString();
        student.name = query.value(1).toString();
        student.birthDate = query.value(2).toDate();
        student.gender = query.value(3).toString();
        student.addressName = query.value(4).toString();
        student.addressCoordX = query.value(5).toInt();
        student.addressCoordY = query.value(6).toInt();
        students.append(student);
    }

    // Simplified output - pagination info now shown in pageLabel
    QString output = QString("===== Display Sorted by ID (Ascending) (Total %1 records) =====\n\n")
                         .arg(totalCount);
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("Display complete (Sorted by ID Ascending) - %1 total records").arg(totalCount));
    updatePageControls();
}
/**
 * @brief (已修复) 显示后序遍历（修复为按ID倒序排序）
 *
 * 修复了 QSqlQuery 对象重用 bug。
 * [修复] 逻辑修复：将查询改为 ORDER BY studentID DESC，以区别于中序遍历。
 */
void MainWindow::onDisplaySortByID_DESC(bool resetPage)
{
    // Handle page reset if requested
    if (resetPage) {
        currentPage = 0;
        lastQueryType = "displaySortByID_DESC"; // <-- 确保类型正确
        lastQueryParam = QVariant();

        // 查询 1: 获取总记录数
        QSqlQuery countQuery(db);
        countQuery.prepare("SELECT COUNT(*) FROM students");

        if (!countQuery.exec() || !countQuery.next()) {
            QMessageBox::critical(this, "Error", "Failed to query total count: " + countQuery.lastError().text());
            return;
        }
        totalCount = countQuery.value(0).toInt();
        totalPages = (totalCount + PageSize - 1) / PageSize;
    }

    if (totalCount == 0) {
        displayOutput("Contact list is empty");
        updatePageControls();
        return;
    }

    QSqlQuery query(db);

    // ==================== 修复开始 ====================
    // 将 LIMIT 和 OFFSET 值直接格式化到字符串中
    int offset = currentPage * PageSize;
    QString sql = QString(
                      "SELECT studentID, name, birthDate, gender, addressName, addressCoordX, addressCoordY "
                      "FROM students ORDER BY studentID DESC LIMIT %1 OFFSET %2"
                      ).arg(PageSize).arg(offset);

    // 因为没有 '?' 占位符, 我们可以直接 .exec(sql)
    if (!query.exec(sql)) {
        // ==================== 修复结束 ====================
        QMessageBox::critical(this, "Error", "Failed to query students: " + query.lastError().text());
        updatePageControls();
        return;
    }

    QVector<Student> students;
    while (query.next()) {
        Student student;
        student.studentID = query.value(0).toString();
        student.name = query.value(1).toString();
        student.birthDate = query.value(2).toDate();
        student.gender = query.value(3).toString();
        student.addressName = query.value(4).toString();
        student.addressCoordX = query.value(5).toInt();
        student.addressCoordY = query.value(6).toInt();
        students.append(student);
    }

    // 更新输出文本
    QString output = QString("===== Display Sorted by ID (Descending) (Total %1 records) =====\n\n").arg(totalCount);
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("Display complete (Sorted by ID Descending) - %1 total records").arg(totalCount));
    updatePageControls();
}
// ==================== Help Menu Implementation ====================

void MainWindow::onAbout()
{
    QMessageBox::about(this,
                       "About",
                       "Student Contact Management System\n"
                       "Version 2.0\n\n"
                       "A SQLite database based student information management system optimized for large datasets.\n"
                       "Refactored to use Qt Designer for UI management and support pagination.\n\n"
                       "Developer: Student Management Team\n"
                       "License: MIT");
}

// ==================== Pagination Implementation ====================

void MainWindow::onPrevPage()
{
    if (currentPage > 0) {
        currentPage--;
        reRunLastQuery();
    }
}

void MainWindow::onNextPage()
{
    if (currentPage < (totalPages - 1)) {
        currentPage++;
        reRunLastQuery();
    }
}

void MainWindow::updatePageControls()
{
    if (totalCount == 0) {
        ui->prevPageButton->setEnabled(false);
        ui->nextPageButton->setEnabled(false);
        ui->pageLabel->setText("N/A");
        return;
    }

    // Update page label
    ui->pageLabel->setText(
        QString("Page %1 of %2 (%3 total records)")
            .arg(currentPage + 1)
            .arg(totalPages)
            .arg(totalCount)
    );

    // Update button states
    ui->prevPageButton->setEnabled(currentPage > 0);
    ui->nextPageButton->setEnabled(currentPage < (totalPages - 1));
}

void MainWindow::reRunLastQuery()
{
    if (lastQueryType == "queryByName") {
        onQueryByName(false);
    } else if (lastQueryType == "queryByAddressCoordX") {
        onQueryByAddressCoordX(false);
    } else if (lastQueryType == "displaySortByName") {
        onDisplaySortByName(false);
    } else if (lastQueryType == "displaySortByID_ASC") {
        onDisplaySortByID_ASC(false);
    } else if (lastQueryType == "displaySortByID_DESC") {
        onDisplaySortByID_DESC(false);
    }
}
