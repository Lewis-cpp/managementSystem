/**
 * @file       mainwindow.h
 * @brief      学生通讯录管理系统的主应用窗口
 * @copyright  Copyright (c) 2025
 * @license    MIT
 * @author     lzq
 * @version    1.1 (使用Qt Designer简化)
 * @date       2025-11-13
 *
 * @par        版本历史:
 *             V1.0: [lzq] [2025-11-13] [创建文件并实现基本功能]
 *             V1.1: [lzq] [2025-11-13] [重构以完全使用Qt Designer，简化代码]
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "student.h" // 确保包含了 student.h

 // 向前声明 Qt Designer 生成的 UI 类
QT_BEGIN_NAMESPACE
namespace Ui { class StudentMessageManagementSystemClass; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief 学生通讯录管理系统的主应用窗口，提供UI界面
 *        该版本已重构，完全利用Qt Designer的UI文件，将UI布局与业务逻辑分离。
 */
    class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow
     * @param[in] parent Parent widget (usually nullptr for top-level window)
     */
    MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~MainWindow();

    // 槽函数保持不变，因为它们是业务逻辑的核心
private slots:
    // File Menu Slots
    void onNewContactList();
    void onLoadFromFile();
    void onSaveToFile();

    // Edit Menu Slots
    void onInsertStudent();
    void onDeleteStudent();

    // Query Menu Slots
    void onQueryByID();
    void onQueryByName();
    void onQueryYoungestStudent();
    void onQueryByAddressCoordX();

    // Display Menu Slots
    void onDisplayPreorder();
    void onDisplayInorder();
    void onDisplayPostorder();

    // Help Menu Slots
    void onAbout();

private:
    // 辅助函数保持不变
    QString formatStudentInfo(const Student& student) const;
    QString formatMultipleStudents(const QVector<Student>& students) const;
    void updateStatus(const QString& message);
    void displayOutput(const QString& text);
    void appendOutput(const QString& text);

    // SQLite 数据库相关辅助函数
    void initDatabase();
    void createTable();

    // ==================== 成员变量 ====================

    // 指向由 Qt Designer 生成的 UI 类的指针
    Ui::StudentMessageManagementSystemClass* ui;

    // 核心数据结构 - SQLite 数据库连接
    QSqlDatabase db;

    // 分页相关
    int currentPage;
    const int PageSize = 100;
};

#endif // MAINWINDOW_H
