/********************************************************************************
** Form generated from reading UI file 'StudentMessageManagementSystem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STUDENTMESSAGEMANAGEMENTSYSTEM_H
#define UI_STUDENTMESSAGEMANAGEMENTSYSTEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StudentMessageManagementSystemClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionInsert;
    QAction *actionDelete;
    QAction *actionQueryByID;
    QAction *actionQueryByName;
    QAction *actionQueryYoungest;
    QAction *actionQueryByCoordX;
    QAction *actionDisplayPreorder;
    QAction *actionDisplayInorder;
    QAction *actionDisplayPostorder;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *displayArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuQuery;
    QMenu *menuDisplay;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *StudentMessageManagementSystemClass)
    {
        if (StudentMessageManagementSystemClass->objectName().isEmpty())
            StudentMessageManagementSystemClass->setObjectName("StudentMessageManagementSystemClass");
        StudentMessageManagementSystemClass->resize(800, 600);
        actionNew = new QAction(StudentMessageManagementSystemClass);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(StudentMessageManagementSystemClass);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(StudentMessageManagementSystemClass);
        actionSave->setObjectName("actionSave");
        actionExit = new QAction(StudentMessageManagementSystemClass);
        actionExit->setObjectName("actionExit");
        actionInsert = new QAction(StudentMessageManagementSystemClass);
        actionInsert->setObjectName("actionInsert");
        actionDelete = new QAction(StudentMessageManagementSystemClass);
        actionDelete->setObjectName("actionDelete");
        actionQueryByID = new QAction(StudentMessageManagementSystemClass);
        actionQueryByID->setObjectName("actionQueryByID");
        actionQueryByName = new QAction(StudentMessageManagementSystemClass);
        actionQueryByName->setObjectName("actionQueryByName");
        actionQueryYoungest = new QAction(StudentMessageManagementSystemClass);
        actionQueryYoungest->setObjectName("actionQueryYoungest");
        actionQueryByCoordX = new QAction(StudentMessageManagementSystemClass);
        actionQueryByCoordX->setObjectName("actionQueryByCoordX");
        actionDisplayPreorder = new QAction(StudentMessageManagementSystemClass);
        actionDisplayPreorder->setObjectName("actionDisplayPreorder");
        actionDisplayInorder = new QAction(StudentMessageManagementSystemClass);
        actionDisplayInorder->setObjectName("actionDisplayInorder");
        actionDisplayPostorder = new QAction(StudentMessageManagementSystemClass);
        actionDisplayPostorder->setObjectName("actionDisplayPostorder");
        actionAbout = new QAction(StudentMessageManagementSystemClass);
        actionAbout->setObjectName("actionAbout");
        centralWidget = new QWidget(StudentMessageManagementSystemClass);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName("verticalLayout");
        displayArea = new QTextEdit(centralWidget);
        displayArea->setObjectName("displayArea");
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        font.setPointSize(11);
        displayArea->setFont(font);
        displayArea->setReadOnly(true);

        verticalLayout->addWidget(displayArea);

        StudentMessageManagementSystemClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StudentMessageManagementSystemClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName("menuEdit");
        menuQuery = new QMenu(menuBar);
        menuQuery->setObjectName("menuQuery");
        menuDisplay = new QMenu(menuBar);
        menuDisplay->setObjectName("menuDisplay");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        StudentMessageManagementSystemClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(StudentMessageManagementSystemClass);
        statusBar->setObjectName("statusBar");
        StudentMessageManagementSystemClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuQuery->menuAction());
        menuBar->addAction(menuDisplay->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionInsert);
        menuEdit->addAction(actionDelete);
        menuQuery->addAction(actionQueryByID);
        menuQuery->addAction(actionQueryByName);
        menuQuery->addAction(actionQueryYoungest);
        menuQuery->addAction(actionQueryByCoordX);
        menuDisplay->addAction(actionDisplayPreorder);
        menuDisplay->addAction(actionDisplayInorder);
        menuDisplay->addAction(actionDisplayPostorder);
        menuHelp->addAction(actionAbout);

        retranslateUi(StudentMessageManagementSystemClass);

        QMetaObject::connectSlotsByName(StudentMessageManagementSystemClass);
    } // setupUi

    void retranslateUi(QMainWindow *StudentMessageManagementSystemClass)
    {
        StudentMessageManagementSystemClass->setWindowTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\255\246\347\224\237\351\200\232\350\256\257\345\275\225\347\256\241\347\220\206\347\263\273\347\273\237 (Student Contact Management System)", nullptr));
        actionNew->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\226\260\345\273\272(&N)", nullptr));
        actionOpen->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\344\273\216\346\226\207\344\273\266\350\257\273\345\217\226(&O)", nullptr));
        actionSave->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\206\231\345\205\245\346\226\207\344\273\266(&S)", nullptr));
        actionExit->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\351\200\200\345\207\272(&X)", nullptr));
        actionInsert->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\217\222\345\205\245\345\255\246\347\224\237\344\277\241\346\201\257(&I)", nullptr));
        actionDelete->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\210\240\351\231\244\345\255\246\347\224\237\344\277\241\346\201\257(&D)", nullptr));
        actionQueryByID->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\214\211\345\255\246\345\217\267\346\237\245\350\257\242...", nullptr));
        actionQueryByName->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\214\211\345\247\223\345\220\215\346\237\245\350\257\242...", nullptr));
        actionQueryYoungest->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\237\245\350\257\242\345\271\264\351\276\204\346\234\200\345\260\217\347\232\204\345\255\246\347\224\237", nullptr));
        actionQueryByCoordX->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\214\211\345\234\260\345\235\200\346\250\252\345\235\220\346\240\207\346\237\245\350\257\242...", nullptr));
        actionDisplayPreorder->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\211\215\345\272\217\351\201\215\345\216\206\346\230\276\347\244\272", nullptr));
        actionDisplayInorder->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\344\270\255\345\272\217\351\201\215\345\216\206\346\230\276\347\244\272", nullptr));
        actionDisplayPostorder->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\220\216\345\272\217\351\201\215\345\216\206\346\230\276\347\244\272", nullptr));
        actionAbout->setText(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\205\263\344\272\216(&A)", nullptr));
        menuFile->setTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\226\207\344\273\266(&F)", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\347\274\226\350\276\221(&E)", nullptr));
        menuQuery->setTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\237\245\350\257\242(&Q)", nullptr));
        menuDisplay->setTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\346\230\276\347\244\272(&D)", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("StudentMessageManagementSystemClass", "\345\270\256\345\212\251(&H)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StudentMessageManagementSystemClass: public Ui_StudentMessageManagementSystemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENTMESSAGEMANAGEMENTSYSTEM_H
