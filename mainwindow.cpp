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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StudentMessageManagementSystemClass) // 正确初始化ui指针
{
    // 第一行必须调用Qt Designer生成的UI
    ui->setupUi(this);

    // Test Chinese message box display (commented out)
    // QMessageBox::information(this, "编码测试", "你好，世界！");

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
    connect(ui->actionQueryByName, &QAction::triggered, this, &MainWindow::onQueryByName);
    connect(ui->actionQueryYoungest, &QAction::triggered, this, &MainWindow::onQueryYoungestStudent);
    connect(ui->actionQueryByCoordX, &QAction::triggered, this, &MainWindow::onQueryByAddressCoordX);

    // 显示菜单
    connect(ui->actionDisplayPreorder, &QAction::triggered, this, &MainWindow::onDisplayPreorder);
    connect(ui->actionDisplayInorder, &QAction::triggered, this, &MainWindow::onDisplayInorder);
    connect(ui->actionDisplayPostorder, &QAction::triggered, this, &MainWindow::onDisplayPostorder);

    // 帮助菜单
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    // 显示欢迎信息
    displayOutput(
        QString::fromUtf8("欢迎使用学生通讯录管理系统\n\n") +
        QString::fromUtf8("主要功能:\n") +
        QString::fromUtf8("* 文件: 新建, 打开, 保存, 退出\n") +
        QString::fromUtf8("* 编辑: 添加和删除学生记录\n") +
        QString::fromUtf8("* 查询: 按学号、姓名查询，最小年龄、按地址坐标查询\n") +
        QString::fromUtf8("* 显示: 前序遍历、中序遍历、后序遍历\n") +
        QString::fromUtf8("* 帮助: 关于软件\n"));
}

MainWindow::~MainWindow()
{
    // 释放ui指针，避免内存泄漏
    delete ui;
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
        studentTree.clear();
        displayOutput("Contact list cleared");
        updateStatus("Contact list cleared");
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

    // 尝试打开选中的文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Cannot open file");
        return;
    }

    // 清除当前所有学生数据
    studentTree.clear();

    // 创建文本流对象用于读取文件内容
    QTextStream in(&file);

    // 统计成功和失败的记录数
    int successCount = 0;
    int failCount = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(',');
        if (parts.size() != 7)
        {
            failCount++;
            continue;
        }

        Student student;
        student.studentID = parts[0].trimmed();
        student.name = parts[1].trimmed();
        student.birthDate = QDate::fromString(parts[2].trimmed(), "yyyy-MM-dd");
        student.gender = parts[3].trimmed();
        student.addressName = parts[4].trimmed();
        student.addressCoordX = parts[5].trimmed().toInt();
        student.addressCoordY = parts[6].trimmed().toInt();

        if (studentTree.insert(student))
        {
            successCount++;
        }
        else
        {
            failCount++;
        }
    }

    file.close();

    QString message = QString("Successfully imported %1 records\nFailed: %2")
                          .arg(successCount)
                          .arg(failCount);
    displayOutput(message);
    updateStatus(message);
}

void MainWindow::onSaveToFile()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save File", "",
                                                    "Text Files (*.txt *.csv);;All Files (*)");

    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Cannot create file");
        return;
    }

    QVector<Student> allStudents = studentTree.preorderTraversal();

    QTextStream out(&file);
    for (const auto &student : allStudents)
    {
        out << student.studentID << ","
            << student.name << ","
            << student.birthDate.toString("yyyy-MM-dd") << ","
            << student.gender << ","
            << student.addressName << ","
            << student.addressCoordX << ","
            << student.addressCoordY << "\n";
    }

    file.close();

    QString message = QString("Saved %1 records to file").arg(allStudents.size());
    displayOutput(message);
    updateStatus(message);
}

// ==================== Edit Menu Implementation ====================

void MainWindow::onInsertStudent()
{
    bool ok;

    QString studentID = QInputDialog::getText(this, "Insert Student", "Student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || studentID.isEmpty())
        return;

    Student dummy;
    if (studentTree.search(studentID, dummy))
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

    Student newStudent(studentID, name, birthDate, gender, addressName, coordX, coordY);
    if (studentTree.insert(newStudent))
    {
        QString message = QString("Student %1 (%2) added successfully").arg(studentID, name);
        displayOutput(message);
        updateStatus(message);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to add student");
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
        if (studentTree.deleteStudent(studentID))
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

    Student result;
    if (studentTree.search(studentID, result))
    {
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

void MainWindow::onQueryByName()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Query by Name", "Enter Name:", QLineEdit::Normal, "", &ok);

    if (!ok || name.isEmpty())
        return;

    QVector<Student> results = studentTree.searchByName(name);

    if (results.isEmpty())
    {
        displayOutput(QString("No students found with name %1").arg(name));
        updateStatus("Query complete - no results");
    }
    else
    {
        QString output = QString("===== Query Results (Found %1) =====\n\n").arg(results.size());
        output += formatMultipleStudents(results);
        displayOutput(output);
        updateStatus(QString("Query complete - found %1 results").arg(results.size()));
    }
}

void MainWindow::onQueryYoungestStudent()
{
    Student youngest;
    if (studentTree.findYoungestStudent(youngest))
    {
        QString output = "===== Youngest Student =====\n";
        output += formatStudentInfo(youngest);
        displayOutput(output);
        updateStatus("Query successful");
    }
    else
    {
        displayOutput("Contact list is empty");
        updateStatus("Query failed");
    }
}

void MainWindow::onQueryByAddressCoordX()
{
    bool ok;
    int coordX = QInputDialog::getInt(this, "Query by Coordinate X", "Enter Coordinate X:", 0, -10000, 10000, 1, &ok);

    if (!ok)
        return;

    QVector<Student> results = studentTree.searchByAddressCoordX(coordX);

    if (results.isEmpty())
    {
        displayOutput(QString("No students found at coordinate X = %1").arg(coordX));
        updateStatus("Query complete - no results");
    }
    else
    {
        QString output = QString("===== Query Results (Found %1) =====\n\n").arg(results.size());
        output += formatMultipleStudents(results);
        displayOutput(output);
        updateStatus(QString("Query complete - found %1 results").arg(results.size()));
    }
}

// ==================== Display Menu Implementation ====================

void MainWindow::onDisplayPreorder()
{
    if (studentTree.isEmpty())
    {
        displayOutput("Contact list is empty");
        return;
    }

    QVector<Student> students = studentTree.preorderTraversal();
    QString output = QString("===== Pre-order Traversal (Total %1) =====\n\n").arg(students.size());
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("Pre-order traversal complete - %1 records").arg(students.size()));
}

void MainWindow::onDisplayInorder()
{
    if (studentTree.isEmpty())
    {
        displayOutput("Contact list is empty");
        return;
    }

    QVector<Student> students = studentTree.inorderTraversal();
    QString output = QString("===== In-order Traversal (Total %1) =====\n"
                             "Sorted by Student ID\n\n")
                         .arg(students.size());
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("In-order traversal complete - %1 records").arg(students.size()));
}

void MainWindow::onDisplayPostorder()
{
    if (studentTree.isEmpty())
    {
        displayOutput("Contact list is empty");
        return;
    }

    QVector<Student> students = studentTree.postorderTraversal();
    QString output = QString("===== Post-order Traversal (Total %1) =====\n\n").arg(students.size());
    output += formatMultipleStudents(students);
    displayOutput(output);
    updateStatus(QString("Post-order traversal complete - %1 records").arg(students.size()));
}

// ==================== Help Menu Implementation ====================

void MainWindow::onAbout()
{
    QMessageBox::about(this,
                       "About",
                       "Student Contact Management System\n"
                       "Version 1.1\n\n"
                       "A binary search tree based student information management system.\n"
                       "Refactored to use Qt Designer for UI management.\n\n"
                       "Developer: Student Management Team\n"
                       "License: MIT");
}
