# 学生通讯录管理系统 (Student Contact Management System)

## 项目概述

这是一个基于 **二叉搜索树 (Binary Search Tree, BST)** 的学生信息管理应用程序，使用 Qt 框架实现用户界面。系统支持学生信息的添加、删除、修改、查询、文件导入/导出以及多种遍历显示功能。

## 核心功能

- ✅ **二叉搜索树实现**: 高效的学生数据存储与检索
- ✅ **完整的 CRUD 操作**: 添加、删除、修改、查询学生信息
- ✅ **多维度查询**: 按学号、姓名、年龄范围、地址等多种方式查询
- ✅ **多种遍历显示**: 前序遍历、中序遍历、后序遍历
- ✅ **文件 I/O**: 支持从文件读取和写入学生数据
- ✅ **友好的 GUI**: 基于 Qt 的菜单式用户界面

## 项目结构

```
StudentMessageManagementSystem/
├── binarysearchtree.h          # 二叉搜索树模板实现
├── main.cpp                   # 程序入口
├── mainwindow.cpp             # 主窗口实现
├── mainwindow.h               # 主窗口头文件
├── output.txt                 # 输出文件
├── sample_data.txt            # 示例数据文件
├── student.h                  # 学生信息结构体定义
├── StudentMessageManagemantSystem.pro  # Qt 项目配置文件
├── StudentMessageManagementSystem.cpp    # 应用程序实现
├── StudentMessageManagementSystem.h      # 应用程序头文件
├── StudentMessageManagementSystem.ui     # UI 设计文件
└── README.md                 # 项目说明文档
```

## 核心数据结构

### 1. Student (学生信息结构体)
```cpp
struct Student {
    QString studentID;      // 学号 (主键) - BST 排序关键字
    QString name;           // 姓名
    QDate birthDate;        // 出生日期
    QString gender;         // 性别
    QString addressName;    // 地址名称
    int addressCoordX;      // 地址坐标 X
    int addressCoordY;      // 地址坐标 Y
};
```

### 2. BinarySearchTree<T> (二叉搜索树)

**模板类**提供以下核心方法：

| 操作 | 函数签名 | 时间复杂度 | 说明 |
|------|----------|------------|------|
| 插入 | `bool insert(const T& data)` | O(log n) 平均 | 插入学生记录到 BST |
| 删除 | `bool deleteStudent(const QString& id)` | O(log n) 平均 | 根据学号删除学生 |
| 查询 | `bool search(const QString& id, T& result)` | O(log n) 平均 | 根据学号快速查询 |
| 遍历 | `QVector<T> preorderTraversal()` | O(n) | 前序遍历 |
|      | `QVector<T> inorderTraversal()` | O(n) | 中序遍历（按学号排序） |
|      | `QVector<T> postorderTraversal()` | O(n) | 后序遍历 |

**高级查询功能**:

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| 按姓名查询 | O(n) | 检索所有同名学生 |
| 按地址坐标查询 | O(n) | 检索特定坐标的学生 |
| 查询年龄最小学生 | O(n) | 检索出生日期最新的学生 |

## 用户界面

### 菜单结构

#### 1. 文件菜单 (File Menu)
- **新建通讯录**: 清空当前学生数据
- **从文件读取**: 打开文件对话框，从文件加载学生数据
- **写入文件**: 将当前学生数据保存到文件
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
- **前序遍历**: 按 根-左-右 顺序显示所有学生
- **中序遍历**: 按 左-根-右 顺序显示（按学号排序）
- **后序遍历**: 按 左-右-根 顺序显示所有学生

#### 5. 帮助菜单 (Help Menu)
- **关于**: 显示软件版本和开发者信息

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
2021003,王五,1998-03-10,男,上海市黄浦区,121,31
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

- **操作系统**: Windows 10/11, macOS, Linux
- **IDE**: Qt Creator
- **编译器**: MinGW 或 MSVC
- **Qt 版本**: Qt 5.15+ 或 Qt 6.x
- **C++ 标准**: C++14 以上

### 构建方式 (Qt Creator)

1. **打开项目**
   ```
   在 Qt Creator 中打开 StudentMessageManagemantSystem.pro
   ```

2. **配置项目**
   - 确保 Qt 路径已正确配置
   - Tools → Options → Qt Versions

3. **构建**
   ```
   Build → Build Solution (Ctrl+Shift+B)
   ```

4. **运行**
   ```
   Debug → Start Debugging (F5)
   ```

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
3. 系统将加载 8 条学生记录

### 示例 2: 查询学生

**按学号查询**:
1. 菜单: **查询 → 按学号查询**
2. 输入学号: `2021001`
3. 显示该学生的详细信息

**按姓名查询**:
1. 菜单: **查询 → 按姓名查询**
2. 输入姓名: `张三`
3. 显示所有同名学生

### 示例 3: 显示遍历结果

1. 菜单: **显示 → 中序遍历**
2. 系统将按学号排序显示所有学生

## 性能分析

### 时间复杂度分析

| 操作 | 平均复杂度 | 最坏复杂度 | 说明 |
|------|------------|------------|------|
| 插入 | O(log n) | O(n) | 最坏为链式结构 |
| 删除 | O(log n) | O(n) | 标准 BST 未实现自平衡 |
| 查询(ID) | O(log n) | O(n) | 基于学号的快速查询 |
| 查询(姓名) | O(n) | O(n) | 需全树扫描 |
| 遍历 | O(n) | O(n) | 需访问所有节点 |
| 文件 I/O | O(n) | O(n) | n 为记录数 |

### 大数据集处理建议

#### 百万级数据 (1M+ records)

**当前局限**:
- UI 线程阻塞: 加载大量数据会导致界面卡顿
- 内存开销: 频繁的 new/delete 操作
- 查询效率: 非索引字段的查询效率低下

**优化方案**:
1. **后台线程**: 使用 QThread 在后台线程处理文件读写
2. **分页加载**: 分页读取数据，降低内存占用
3. **进度指示**: 显示 QProgressDialog 提示加载进度
4. **内存映射**: 使用 QFile 内存映射提高 I/O 效率
5. **异步查询**: 实现异步查询，避免界面阻塞

#### 千万级数据 (10M+ records)

**解决方案**:
1. **自平衡树**: 使用 AVL 树或红黑树确保 O(log n) 复杂度
2. **B+ 树**: 更适合磁盘存储和范围查询
3. **数据库**: 引入 SQLite/MySQL 等关系型数据库
   - 支持多字段索引查询
   - 支持事务处理
   - 支持 SQL 复杂查询
4. **分布式存储**: 使用 NoSQL 数据库或文件系统分布式存储

## 文档规范

### Doxygen 文档注释

#### 文件头注释
```cpp
/**
 * @file       filename.cpp
 * @brief      文件功能描述
 * @copyright  Copyright (c) 2025 开发团队
 * @license    MIT
 * @author     开发者姓名
 * @version    1.0
 * @date       2025-01-20
 *
 * @par        版本历史:
 *             V1.0: [开发者] [日期] [创建文件]
 */
```

#### 函数注释
```cpp
/**
 * @brief      函数功能描述
 * @param[in]  param1 : 参数1说明
 * @param[out] param2 : 参数2说明（输出参数）
 * @return     返回值说明
 * @note       额外说明信息
 */
```

#### 成员变量注释
```cpp
int counter;  ///< 计数器变量说明
```

## 未来增强

- [ ] 自平衡树 (AVL 树) 实现
- [ ] SQLite 数据库集成
- [ ] 用户权限管理
- [ ] 后台线程优化
- [ ] 高级查询功能
- [ ] 数据统计与报表
- [ ] 多种导出格式 (Excel, PDF)
- [ ] 学生信息编辑功能

## 技术栈

| 技术 | 版本 | 用途 |
|------|------|------|
| C++ | C++14+ | 核心开发语言 |
| Qt | 5.15+ | GUI 框架 |
| STL | C++17 | 标准模板库 |

## 许可证

MIT License - 允许自由使用和修改

## 开发信息

**学生通讯录管理系统开发团队**
日期: 2025-01-20
版本: 1.0

## 联系方式

欢迎提交 Issue 或 Pull Request 来改进系统。

**注**: 本项目为学习演示项目，请勿用于生产环境。
