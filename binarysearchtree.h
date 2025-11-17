/**
 * @file       binarysearchtree.h
 * @brief      学生数据存储和管理的二叉搜索树实现
 * @copyright  Copyright (c) 2025
 * @license    MIT
 * @author     lzq
 * @version    1.0
 * @date       2025-11-13
 *
 * @par        版本历史:
 *             V1.0:[lzq] [2025-11-13] [创建文件并实现二叉搜索树核心功能]
 *
 * @par        性能与扩展性分析:
 *             当数据量达到千万级时，当前基于内存的二叉搜索树会面临以下问题:
 *             1. 内存耗尽: 千万级数据需要数GB内存，会导致系统内存溢出
 *             2. 树深度不平衡: 如果插入数据是有序的，可能形成链式结构，查询性能退化为O(n)
 *             3. 缓存效率低: 链式结构的内存不连续，CPU缓存命中率低
 *
 *             解决方案:
 *             1. 使用自平衡二叉树(AVL树或红黑树)保证O(log n)性能
 *             2. 采用B+树或B树结构，提高内存空间利用和磁盘I/O效率
 *             3. 使用SQLite/MySQL等数据库，支持持久化和大规模数据存储
 *             4. 实现分页加载机制，在需要时才从数据库加载数据到内存
 */

#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "student.h"
#include <QVector>
#include <QList>
#include <functional>
#include <memory>

/**
 * @class BinarySearchTree
 * @brief 用于高效学生数据管理的二叉搜索树
 *
 * 该类实现了一个完整的二叉搜索树，用于存储和管理学生信息。它支持标准的BST操作：插入、删除、
 * 查询和各种遍历方法。
 *
 * @tparam T 数据类型（本应用中为Student）
 */
template<typename T>
class BinarySearchTree
{
private:
    /**
     * @struct Node
     * @brief 二叉搜索树的内部节点结构
     */
    struct Node
    {
        T data;                          ///< 存储在该节点中的学生数据
        std::shared_ptr<Node> left;      ///< 左子树
        std::shared_ptr<Node> right;     ///< 右子树
        
        /**
         * @brief 节点构造函数
         * @param value 要存储在节点中的数据
         */
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };
    
    std::shared_ptr<Node> root;  ///< BST的根节点
    
public:
    /**
     * @brief 构造函数 - 初始化一个空的BST
     */
    BinarySearchTree() : root(nullptr) {}

    /**
     * @brief 析构函数 - 通过shared_ptr自动释放所有节点
     */
    ~BinarySearchTree() = default;
    
    /**
     * @brief 向BST中插入一个新的学生记录
     * @param[in] data 要插入的学生对象
     * @return 如果插入成功返回true，如果学生ID已存在返回false
     * @note 时间复杂度: 平均情况O(log n)，最坏情况O(n)（非平衡树）
     */
    bool insert(const T& data)
    {
        return insertRecursive(root, data);
    }
    
    /**
     * @brief 根据学生ID删除学生记录
     * @param[in] studentID 要删除的学生的ID
     * @return 如果删除成功返回true，如果学生未找到返回false
     * @note 处理所有三种情况：叶子节点、有一个孩子的节点、有两个孩子的节点
     * @note 时间复杂度: 平均情况O(log n)，最坏情况O(n)
     */
    bool deleteStudent(const QString& studentID)
    {
        return deleteRecursive(root, studentID);
    }
    
    /**
     * @brief 根据ID查找学生
     * @param[in] studentID 要查找的ID
     * @param[out] result 如果找到，将包含学生数据
     * @return 如果找到学生返回true，否则返回false
     * @note 时间复杂度: 平均情况O(log n)，最坏情况O(n)
     */
    bool search(const QString& studentID, T& result) const
    {
        return searchRecursive(root, studentID, result);
    }
    
    /**
     * @brief Check if a student ID exists in the tree
     * @param[in] studentID The ID to check
     * @return true if exists, false otherwise
     */
    bool contains(const QString& studentID) const
    {
        T dummy;
        return search(studentID, dummy);
    }
    
    /**
     * @brief Get all students with a specific name
     * @param[in] name The name to search for
     * @return QVector containing all students with matching name
     * @note Time Complexity: O(n) - requires full tree traversal
     * @note Performance Note: This operation must scan entire tree. For large datasets,
     *       consider indexing by name or using database backend for faster results
     */
    QVector<T> searchByName(const QString& name) const
    {
        QVector<T> results;
        searchByNameRecursive(root, name, results);
        return results;
    }
    
    /**
     * @brief Get all students with a specific address X coordinate
     * @param[in] coordX The X coordinate to search for
     * @return QVector containing all students at that X coordinate
     * @note Time Complexity: O(n) - requires full tree traversal
     */
    QVector<T> searchByAddressCoordX(int coordX) const
    {
        QVector<T> results;
        searchByAddressCoordXRecursive(root, coordX, results);
        return results;
    }
    
    /**
     * @brief Find the youngest student (latest birth date)
     * @param[out] student Will contain the youngest student if found
     * @return true if tree is not empty, false otherwise
     * @note Time Complexity: O(n) - requires full tree traversal
     */
    bool findYoungestStudent(T& student) const
    {
        if (!root) return false;
        
        QVector<T> allStudents;
        inorderTraversalRecursive(root, allStudents);
        
        if (allStudents.isEmpty()) return false;
        
        T youngest = allStudents[0];
        for (const auto& s : allStudents)
        {
            if (s.birthDate > youngest.birthDate)
            {
                youngest = s;
            }
        }
        student = youngest;
        return true;
    }
    
    /**
     * @brief Get all students in pre-order traversal
     * @return QVector of students in pre-order sequence (root, left, right)
     * @note Time Complexity: O(n)
     */
    QVector<T> preorderTraversal() const
    {
        QVector<T> result;
        preorderTraversalRecursive(root, result);
        return result;
    }
    
    /**
     * @brief Get all students in in-order traversal (sorted by student ID)
     * @return QVector of students in in-order sequence (left, root, right)
     *         Results are sorted by student ID
     * @note Time Complexity: O(n)
     */
    QVector<T> inorderTraversal() const
    {
        QVector<T> result;
        inorderTraversalRecursive(root, result);
        return result;
    }
    
    /**
     * @brief Get all students in post-order traversal
     * @return QVector of students in post-order sequence (left, right, root)
     * @note Time Complexity: O(n)
     */
    QVector<T> postorderTraversal() const
    {
        QVector<T> result;
        postorderTraversalRecursive(root, result);
        return result;
    }
    
    /**
     * @brief Clear all students from the tree
     * @note All nodes are automatically deallocated via shared_ptr
     */
    void clear()
    {
        root = nullptr;
    }
    
    /**
     * @brief Check if the tree is empty
     * @return true if tree contains no students
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }
    
    /**
     * @brief Get the total number of students in the tree
     * @return Number of nodes in the tree
     * @note Time Complexity: O(n)
     */
    int size() const
    {
        return sizeRecursive(root);
    }
    
private:
    // ==================== 递归辅助方法 ====================
    
    /**
     * @brief Recursive helper for insertion
     * @param[in,out] node Current node in traversal
     * @param[in] data The student data to insert
     * @return true if inserted, false if ID already exists
     */
    bool insertRecursive(std::shared_ptr<Node>& node, const T& data)
    {
        if (!node)
        {
            node = std::make_shared<Node>(data);
            return true;
        }
        
        // 如果数据小于当前节点，向左子树插入
        if (data < node->data)
        {
            return insertRecursive(node->left, data);
        }
        // 如果数据大于当前节点，向右子树插入
        else if (node->data < data)
        {
            return insertRecursive(node->right, data);
        }
        // 重复的ID - 插入失败
        else
        {
            return false;
        }
    }
    
    /**
     * @brief Recursive helper for deletion
     * @param[in,out] node Current node in traversal
     * @param[in] studentID The ID of student to delete
     * @return true if deleted, false if not found
     */
    bool deleteRecursive(std::shared_ptr<Node>& node, const QString& studentID)
    {
        if (!node) return false;
        
        // 定位要删除的节点
        if (studentID < node->data.studentID)
        {
            return deleteRecursive(node->left, studentID);
        }
        else if (studentID > node->data.studentID)
        {
            return deleteRecursive(node->right, studentID);
        }
        else
        {
            // 找到要删除的节点
            // 情况 1: 节点是叶子节点（没有子节点）
            if (!node->left && !node->right)
            {
                node = nullptr;
                return true;
            }
            // 情况 2: 节点只有右子节点
            else if (!node->left)
            {
                node = node->right;
                return true;
            }
            // 情况 3: 节点只有左子节点
            else if (!node->right)
            {
                node = node->left;
                return true;
            }
            // 情况 4: 节点有两个子节点
            else
            {
                // 找到中序后继节点（右子树中最小的节点）
                std::shared_ptr<Node> successorParent = node;
                std::shared_ptr<Node> successor = node->right;
                
                while (successor->left)
                {
                    successorParent = successor;
                    successor = successor->left;
                }
                
                // 用后继节点的数据替换当前节点的数据
                node->data = successor->data;
                
                // 删除后继节点
                if (successorParent == node)
                {
                    // 后继节点是当前节点的右子节点
                    node->right = successor->right;
                }
                else
                {
                    // 后继节点在当前节点右子节点的左子树中
                    successorParent->left = successor->right;
                }
                
                return true;
            }
        }
    }
    
    /**
     * @brief Recursive helper for searching by ID
     * @param[in] node Current node in traversal
     * @param[in] studentID The ID to search for
     * @param[out] result Will contain the student data if found
     * @return true if found, false otherwise
     */
    bool searchRecursive(const std::shared_ptr<Node>& node, 
                       const QString& studentID, T& result) const
    {
        if (!node) return false;
        
        if (studentID < node->data.studentID)
        {
            return searchRecursive(node->left, studentID, result);
        }
        else if (studentID > node->data.studentID)
        {
            return searchRecursive(node->right, studentID, result);
        }
        else
        {
            result = node->data;
            return true;
        }
    }
    
    /**
     * @brief Recursive helper for searching by name
     * @param[in] node Current node in traversal
     * @param[in] name The name to search for
     * @param[out] results Vector to accumulate results
     */
    void searchByNameRecursive(const std::shared_ptr<Node>& node, 
                              const QString& name, QVector<T>& results) const
    {
        if (!node) return;
        
        // 检查当前节点
        if (node->data.name == name)
        {
            results.push_back(node->data);
        }

        // 搜索左右两个子树（必须检查两个子树，因为树是按ID排序的，不是按姓名）
        searchByNameRecursive(node->left, name, results);
        searchByNameRecursive(node->right, name, results);
    }
    
    /**
     * @brief Recursive helper for searching by address X coordinate
     * @param[in] node Current node in traversal
     * @param[in] coordX The X coordinate to search for
     * @param[out] results Vector to accumulate results
     */
    void searchByAddressCoordXRecursive(const std::shared_ptr<Node>& node, 
                                      int coordX, QVector<T>& results) const
    {
        if (!node) return;
        
        // 检查当前节点
        if (node->data.addressCoordX == coordX)
        {
            results.push_back(node->data);
        }
        
        // 搜索左右两个子树（必须检查两个子树，因为树是按ID排序的，不是按坐标）
        searchByAddressCoordXRecursive(node->left, coordX, results);
        searchByAddressCoordXRecursive(node->right, coordX, results);
    }
    
    /**
     * @brief Recursive helper for pre-order traversal (root, left, right)
     */
    void preorderTraversalRecursive(const std::shared_ptr<Node>& node, 
                                   QVector<T>& result) const
    {
        if (!node) return;
        
        result.push_back(node->data);
        preorderTraversalRecursive(node->left, result);
        preorderTraversalRecursive(node->right, result);
    }
    
    /**
     * @brief Recursive helper for in-order traversal (left, root, right)
     *        Results will be sorted by student ID
     */
    void inorderTraversalRecursive(const std::shared_ptr<Node>& node, 
                                  QVector<T>& result) const
    {
        if (!node) return;
        
        inorderTraversalRecursive(node->left, result);
        result.push_back(node->data);
        inorderTraversalRecursive(node->right, result);
    }
    
    /**
     * @brief Recursive helper for post-order traversal (left, right, root)
     */
    void postorderTraversalRecursive(const std::shared_ptr<Node>& node, 
                                    QVector<T>& result) const
    {
        if (!node) return;
        
        postorderTraversalRecursive(node->left, result);
        postorderTraversalRecursive(node->right, result);
        result.push_back(node->data);
    }
    
    /**
     * @brief Recursive helper to count nodes
     */
    int sizeRecursive(const std::shared_ptr<Node>& node) const
    {
        if (!node) return 0;
        return 1 + sizeRecursive(node->left) + sizeRecursive(node->right);
    }
};

#endif // BINARYSEARCHTREE_H
