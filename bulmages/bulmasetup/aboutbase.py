/********************************************************************************
** Form generated from reading ui file 'aboutbase.ui'
**
** Created: Sun Jul 5 22:44:54 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUTBASE_H
#define UI_ABOUTBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLabel *mui_lbversion;
    QSpacerItem *spacerItem;
    QTabWidget *TabWidget;
    QWidget *Acercade_2;
    QHBoxLayout *_6;
    QTextEdit *textEdit_sobre;
    QWidget *autores_2;
    QHBoxLayout *_7;
    QTextEdit *textEdit_autores;
    QWidget *soporte_2;
    QHBoxLayout *_8;
    QTextEdit *textEdit_soporte;
    QWidget *licencia_2;
    QHBoxLayout *_9;
    QTextEdit *textEdit_licencia;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem1;
    QPushButton *pushButton_cerrar;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(600, 350);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(About->sizePolicy().hasHeightForWidth());
        About->setSizePolicy(sizePolicy);
        About->setMinimumSize(QSize(600, 350));
        About->setMaximumSize(QSize(600, 350));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(221, 223, 228, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        QBrush brush3(QColor(85, 85, 85, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush3);
        QBrush brush4(QColor(199, 199, 199, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush4);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush5(QColor(239, 239, 239, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush6(QColor(103, 141, 178, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush6);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        QBrush brush7(QColor(0, 0, 238, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Link, brush7);
        QBrush brush8(QColor(82, 24, 139, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush8);
        QBrush brush9(QColor(232, 232, 232, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Link, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        QBrush brush10(QColor(128, 128, 128, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        QBrush brush11(QColor(86, 117, 148, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush11);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Link, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush9);
        About->setPalette(palette);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/iglues.org.svg")), QIcon::Normal, QIcon::Off);
        About->setWindowIcon(icon);
        hboxLayout = new QHBoxLayout(About);
        hboxLayout->setSpacing(5);
        hboxLayout->setMargin(5);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(5);
        vboxLayout->setMargin(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(5);
        hboxLayout1->setMargin(0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label = new QLabel(About);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(48, 48));
        label->setMaximumSize(QSize(48, 48));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/iglues.org.svg")));
        label->setScaledContents(true);

        hboxLayout1->addWidget(label);

        mui_lbversion = new QLabel(About);
        mui_lbversion->setObjectName(QString::fromUtf8("mui_lbversion"));
        QFont font;
        font.setFamily(QString::fromUtf8("Sans Serif"));
        font.setPointSize(9);
        font.setBold(true);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(75);
        font.setStrikeOut(false);
        mui_lbversion->setFont(font);

        hboxLayout1->addWidget(mui_lbversion);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout1);

        TabWidget = new QTabWidget(About);
        TabWidget->setObjectName(QString::fromUtf8("TabWidget"));
        TabWidget->setFocusPolicy(Qt::StrongFocus);
        TabWidget->setLayoutDirection(Qt::LeftToRight);
        Acercade_2 = new QWidget();
        Acercade_2->setObjectName(QString::fromUtf8("Acercade_2"));
        _6 = new QHBoxLayout(Acercade_2);
        _6->setSpacing(5);
        _6->setMargin(5);
        _6->setObjectName(QString::fromUtf8("_6"));
        textEdit_sobre = new QTextEdit(Acercade_2);
        textEdit_sobre->setObjectName(QString::fromUtf8("textEdit_sobre"));
        textEdit_sobre->setFocusPolicy(Qt::StrongFocus);
        textEdit_sobre->setReadOnly(true);

        _6->addWidget(textEdit_sobre);

        TabWidget->addTab(Acercade_2, QString());
        autores_2 = new QWidget();
        autores_2->setObjectName(QString::fromUtf8("autores_2"));
        _7 = new QHBoxLayout(autores_2);
        _7->setSpacing(5);
        _7->setMargin(5);
        _7->setObjectName(QString::fromUtf8("_7"));
        textEdit_autores = new QTextEdit(autores_2);
        textEdit_autores->setObjectName(QString::fromUtf8("textEdit_autores"));
        textEdit_autores->setFocusPolicy(Qt::StrongFocus);
        textEdit_autores->setAutoFormatting(QTextEdit::AutoAll);
        textEdit_autores->setReadOnly(true);

        _7->addWidget(textEdit_autores);

        TabWidget->addTab(autores_2, QString());
        soporte_2 = new QWidget();
        soporte_2->setObjectName(QString::fromUtf8("soporte_2"));
        _8 = new QHBoxLayout(soporte_2);
        _8->setSpacing(5);
        _8->setMargin(5);
        _8->setObjectName(QString::fromUtf8("_8"));
        textEdit_soporte = new QTextEdit(soporte_2);
        textEdit_soporte->setObjectName(QString::fromUtf8("textEdit_soporte"));
        textEdit_soporte->setFocusPolicy(Qt::TabFocus);
        textEdit_soporte->setReadOnly(true);

        _8->addWidget(textEdit_soporte);

        TabWidget->addTab(soporte_2, QString());
        licencia_2 = new QWidget();
        licencia_2->setObjectName(QString::fromUtf8("licencia_2"));
        _9 = new QHBoxLayout(licencia_2);
        _9->setSpacing(5);
        _9->setMargin(5);
        _9->setObjectName(QString::fromUtf8("_9"));
        textEdit_licencia = new QTextEdit(licencia_2);
        textEdit_licencia->setObjectName(QString::fromUtf8("textEdit_licencia"));
        textEdit_licencia->setFocusPolicy(Qt::StrongFocus);
        textEdit_licencia->setReadOnly(true);

        _9->addWidget(textEdit_licencia);

        TabWidget->addTab(licencia_2, QString());

        vboxLayout->addWidget(TabWidget);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(5);
        hboxLayout2->setMargin(0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem1);

        pushButton_cerrar = new QPushButton(About);
        pushButton_cerrar->setObjectName(QString::fromUtf8("pushButton_cerrar"));
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/images/bulmasetup_exit.png")), QIcon::Normal, QIcon::Off);
        pushButton_cerrar->setIcon(icon1);

        hboxLayout2->addWidget(pushButton_cerrar);


        vboxLayout->addLayout(hboxLayout2);


        hboxLayout->addLayout(vboxLayout);


        retranslateUi(About);
        QObject::connect(pushButton_cerrar, SIGNAL(clicked()), About, SLOT(close()));

        TabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "Acerca de Iglues/BulmaSetup", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        mui_lbversion->setText(QApplication::translate("About", "BulmaSetup GPL - Version 0.12.1", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(Acercade_2), QApplication::translate("About", "&Acerca de", 0, QApplication::UnicodeUTF8));
        textEdit_autores->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(autores_2), QApplication::translate("About", "A&utores", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(soporte_2), QApplication::translate("About", "Sopor&te", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(licencia_2), QApplication::translate("About", "Acuerdo de &licencia", 0, QApplication::UnicodeUTF8));
        pushButton_cerrar->setText(QApplication::translate("About", "&Cerrar", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(About);
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBASE_H
