/********************************************************************************
** Form generated from reading UI file 'die.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIE_H
#define UI_DIE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_die
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *last_score;
    QPushButton *restartButton;
    QPushButton *exitButton;

    void setupUi(QWidget *die)
    {
        if (die->objectName().isEmpty())
            die->setObjectName("die");
        die->resize(400, 300);
        label = new QLabel(die);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 10, 191, 71));
        label_2 = new QLabel(die);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(80, 80, 91, 31));
        last_score = new QLabel(die);
        last_score->setObjectName("last_score");
        last_score->setGeometry(QRect(80, 100, 221, 81));
        last_score->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
"font:50pt"));
        last_score->setAlignment(Qt::AlignmentFlag::AlignCenter);
        restartButton = new QPushButton(die);
        restartButton->setObjectName("restartButton");
        restartButton->setGeometry(QRect(160, 190, 71, 31));
        exitButton = new QPushButton(die);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(160, 230, 71, 31));

        retranslateUi(die);

        QMetaObject::connectSlotsByName(die);
    } // setupUi

    void retranslateUi(QWidget *die)
    {
        die->setWindowTitle(QCoreApplication::translate("die", "Form", nullptr));
        label->setText(QCoreApplication::translate("die", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt; font-weight:700; color:#ffaa00;\">\346\270\270 \346\210\217 \347\273\223 \346\235\237 \357\274\201</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("die", "<html><head/><body><p><span style=\" font-size:12pt;\">\344\275\240\347\232\204\345\276\227\345\210\206\357\274\232</span></p></body></html>", nullptr));
        last_score->setText(QCoreApplication::translate("die", "0", nullptr));
        restartButton->setText(QCoreApplication::translate("die", "\345\206\215\346\235\245\344\270\200\345\261\200", nullptr));
        exitButton->setText(QCoreApplication::translate("die", "\344\270\215\347\216\251\344\272\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class die: public Ui_die {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIE_H
