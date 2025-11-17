/**
 * @file       main.cpp
 * @brief      学生通讯录管理系统的主入口文件
 * @copyright  Copyright (c) 2025
 * @license    MIT
 * @author     lzq
 * @version    1.0
 * @date       2025-11-13
 *
 * @par        版本历史:
 *             V1.0: [lzq] [2025-11-13] [创建文件并实现主入口功能]
 */

#include "mainwindow.h"
#include <QtWidgets/QApplication>

/**
 * @brief 应用程序的主入口点
 * @param argc 命令行参数计数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char *argv[])
{
    // 创建QApplication实例，这是所有Qt应用程序的核心
    QApplication app(argc, argv);

    // 创建主窗口实例
    MainWindow window;

    // 显示主窗口
    window.show();

    // 进入应用程序事件循环，等待用户交互
    return app.exec();
}
