/**
 * @file       student.h
 * @brief      学生信息结构体定义，用于通讯录管理系统
 * @copyright  Copyright (c) 2025
 * @license    MIT
 * @author     lzq
 * @version    1.0
 * @date       2025-11-13
 *
 * @par        版本历史:
 *             V1.0:[lzq] [2025-11-13] [创建文件并定义学生信息结构体]
 */

#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDate>

/**
 * @struct Student
 * @brief  用于二叉搜索树存储的学生信息结构体
 *
 * 该结构体存储完整的学生档案信息，将存储在二叉搜索树中，以学生ID（studentID）作为唯一的
 * 排序和搜索键。
 */
struct Student
{
    // Core student identification
    QString studentID;      ///< Student ID (学号) - Primary key for BST sorting
    QString name;           ///< Student name (姓名)
    
    // Demographic information
    QDate birthDate;        ///< Birth date (出生日期)
    QString gender;         ///< Gender (性别) - typically "男" (Male) or "女" (Female)
    
    // Address information
    QString addressName;    ///< Address location name (地址名)
    int addressCoordX;      ///< Address X coordinate (地址横坐标)
    int addressCoordY;      ///< Address Y coordinate (地址纵坐标)
    
    /**
     * @brief Default constructor initializing all fields to empty/default values
     */
    Student() 
        : studentID(""), name(""), birthDate(QDate()), 
          gender(""), addressName(""), addressCoordX(0), addressCoordY(0)
    {
    }
    
    /**
     * @brief Parameterized constructor for creating a Student with all fields
     * @param id    Student ID
     * @param n     Student name
     * @param bd    Birth date
     * @param g     Gender
     * @param addr  Address name
     * @param x     X coordinate
     * @param y     Y coordinate
     */
    Student(const QString& id, const QString& n, const QDate& bd, 
            const QString& g, const QString& addr, int x, int y)
        : studentID(id), name(n), birthDate(bd), gender(g), 
          addressName(addr), addressCoordX(x), addressCoordY(y)
    {
    }
    
    /**
     * @brief Comparison operator for BST operations (based on studentID)
     * @param other The other Student object to compare with
     * @return true if this student's ID is less than other's ID
     */
    bool operator<(const Student& other) const
    {
        return studentID < other.studentID;
    }
    
    /**
     * @brief Equality operator
     * @param other The other Student object to compare with
     * @return true if student IDs are equal
     */
    bool operator==(const Student& other) const
    {
        return studentID == other.studentID;
    }
};

#endif // STUDENT_H
