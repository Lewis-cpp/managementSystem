# 学生通讯录管理系统 (Student Contact Management System)

## 项目概述

这是一个基于 **SQLite 数据库** 的学生信息管理应用程序，使用 Qt 框架实现用户界面。系统支持学生信息的添加、删除、修改、查询、文件导入/导出等功能，提供了高效的数据存储与检索能力。

## 核心功能

- ✅ **SQLite 数据库实现**: 可靠的学生数据持久化存储
- ✅ **完整的 CRUD 操作**: 添加、删除、修改、查询学生信息
- ✅ **多维度查询**: 按学号、姓名、年龄范围、地址等多种方式查询
- ✅ **文件 I/O**: 支持从文件读取和写入学生数据
- ✅ **友好的 GUI**: 基于 Qt 的菜单式用户界面
- ✅ **分页显示**: 支持大量数据的分页查看
- ✅ **实时状态提示**: 操作状态实时反馈

## 技术栈

| 技术 | 版本 | 用途 |
|------|------|------|
| C++ | C++14+ | 核心开发语言 |
| Qt | 5.15+/6.x | GUI 框架 |
| SQLite | 3.x | 数据库引擎 |

## 项目结构

```
StudentMessageManagementSystem/
├── binarysearchtree.h                      # 二叉搜索树模板（旧版本实现）
├── build/                                 # 构建目录
├── generate_data.py                       # 测试数据生成脚本
├── large_data.txt                         # 大数据集示例
├── main.cpp                               # 程序入口
├── mainwindow.cpp                         # 主窗口实现
├── mainwindow.h                           # 主窗口头文件
├── README.md                              # 项目说明文档
├── sample_data.txt                        # 示例数据文件
├── student.h                              # 学生信息结构体定义
├── StudentMessageManagemantSystem.pro     # Qt 项目配置文件
├── StudentMessageManagementSystem.cpp     # 应用程序实现
├── StudentMessageManagementSystem.h       # 应用程序头文件
├── StudentMessageManagementSystem.ui      # UI 设计文件
└── .qtcreator/                           # Qt Creator 配置目录
```

## 核心数据结构

### Student (学生信息结构体)

```cpp
struct Student {
    QString studentID;      // 学号 (数据库主键)
    QString name;           // 姓名
    QDate birthDate;        // 出生日期
    QString gender;         // 性别
    QString addressName;    // 地址名称
    int addressCoordX;      // 地址坐标 X
    int addressCoordY;      // 地址坐标 Y
};
```

## 数据库设计

### 表结构 (students)

| 字段名称 | 数据类型 | 说明 |
|----------|----------|------|
| studentID | TEXT | 学号 (主键) |
| name | TEXT | 姓名 |
| birthDate | TEXT | 出生日期 (格式: yyyy-MM-dd) |
| gender | TEXT | 性别 |
| addressName | TEXT | 地址名称 |
| addressCoordX | INTEGER | 地址坐标 X |
| addressCoordY | INTEGER | 地址坐标 Y |

### 数据库特性

- 自动创建表结构和索引
- 事务支持确保数据完整性
- 高效的 SQL 查询优化

## 用户界面

### 菜单结构

#### 1. 文件菜单 (File Menu)
- **新建通讯录**: 清空当前数据库中的学生数据
- **从文件读取**: 打开文件对话框，从 CSV 文件加载学生数据
- **写入文件**: 将当前数据库数据保存到 CSV 文件
- **退出**: 关闭应用程序

#### 2. 编辑菜单 (Edit Menu)
- **插入学生信息**: 通过一系列对话框输入学生信息
- **删除学生信息**: 根据学号删除学生

#### 3. 查询菜单 (Query Menu)
- **按学号查询**: 根据学号快速查询学生
- **按姓名查询**: 查询所有同名学生
- **查询年龄最小的学生**: 检索年龄最小的学生
- **按地址坐标查询**: 查询特定坐标的学生

#### 4. 显示菜单 (Display Menu)
- **按姓名排序**: 按姓名顺序显示学生
- **按学号排序**: 按学号顺序显示学生

## 文件格式

### 数据导入/导出格式

使用 CSV 格式，每行一条记录，字段间使用逗号分隔：

```
studentID,name,birthDate,gender,addressName,addressCoordX,addressCoordY
```

#### 格式示例:

```
2021001,张三,1999-05-15,男,北京市海淀区,120,39
2021002,李四,1999-08-22,女,北京市朝阳区,116,39
```

#### 字段说明:
- **学号**: 字符串格式，通常为 YYYYXXX (年份+编号)
- **姓名**: 字符串类型
- **出生日期**: 格式 yyyy-MM-dd
- **性别**: "男" 或 "女"
- **地址名称**: 字符串类型
- **地址坐标 X**: 整数范围 -10000 ~ 10000
- **地址坐标 Y**: 整数范围 -10000 ~ 10000

## 构建与运行

### 环境要求

- **操作系统**: Windows, macOS, Linux
- **IDE**: Qt Creator (推荐)
- **编译器**: MinGW 或 MSVC
- **Qt 版本**: Qt 5.15+ 或 Qt 6.x
- **C++ 标准**: C++14 以上

### 构建方式 (Qt Creator)

1. **打开项目**
   - 在 Qt Creator 中打开 `StudentMessageManagemantSystem.pro`

2. **配置项目**
   - 确保 Qt 路径已正确配置
   - 选择合适的 Qt kit

3. **构建**
   - Build → Build All (Ctrl+B)

4. **运行**
   - Run → Start Debugging (F5)

### 构建方式 (命令行)

```bash
# 构建项目
qmake StudentMessageManagemantSystem.pro
make

# 运行程序
./StudentMessageManagementSystem
```

## 使用示例

### 示例 1: 加载示例数据

1. 菜单: **文件 → 从文件读取**
2. 选择 `sample_data.txt` 文件
3. 系统将加载示例学生记录

### 示例 2: 查询学生

**按学号查询**:
1. 菜单: **查询 → 按学号查询**
2. 输入学号: `2021001`
3. 显示该学生的详细信息

**按姓名查询**:
1. 菜单: **查询 → 按姓名查询**
2. 输入姓名: `张三`
3. 显示所有同名学生

### 示例 3: 显示学生列表

1. 菜单: **显示 → 按学号排序**
2. 系统将按学号排序显示所有学生

## 构建配置

### 项目配置文件 (StudentMessageManagemantSystem.pro)

```qmake
QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudentMessageManagementSystem
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp\
        StudentMessageManagementSystem.cpp

HEADERS  += mainwindow.h\
        StudentMessageManagementSystem.h\
        student.h

FORMS    += StudentMessageManagementSystem.ui

CONFIG += c++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
```

## 性能特点

- **高效的数据库查询**: 利用 SQLite 数据库索引提供快速查询
- **分页显示支持**: 支持大量数据的分页查看，提高界面响应速度
- **后台数据处理**: 文件读写等操作在后台线程处理，避免界面阻塞

## 许可证

MIT License - 允许自由使用和修改

## 开发信息

**lzq**
日期: 2025-11-16
版本: 2.0
