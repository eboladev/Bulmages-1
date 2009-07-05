/********************************************************************************
** Form generated from reading ui file 'bulmasetupbase.ui'
**
** Created: Sun Jul 5 22:45:49 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BULMASETUPBASE_H
#define UI_BULMASETUPBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QPushButton *nuevo_usuario;
    QPushButton *editar_usuario;
    QPushButton *permisos_usuario;
    QPushButton *eliminar_usuario;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QPushButton *mui_crearbulmacont;
    QPushButton *mui_crearbulmafact;
    QPushButton *mui_adminempresas;
    QPushButton *eliminar_empresa;
    QPushButton *mui_restbackup;
    QPushButton *mui_creabackup;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *mui_acerca;
    QSpacerItem *horizontalSpacer;
    QPushButton *mui_salir;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(778, 400);
        MainWindow->setMinimumSize(QSize(732, 400));
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/iglues.org.svg")), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(209, 209));
        label->setMaximumSize(QSize(209, 209));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/iglues.org.svg")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        nuevo_usuario = new QPushButton(groupBox_3);
        nuevo_usuario->setObjectName(QString::fromUtf8("nuevo_usuario"));
        nuevo_usuario->setEnabled(false);
        nuevo_usuario->setFocusPolicy(Qt::NoFocus);
        nuevo_usuario->setLayoutDirection(Qt::LeftToRight);
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/images/add_user_postgresql.svg")), QIcon::Normal, QIcon::Off);
        nuevo_usuario->setIcon(icon1);
        nuevo_usuario->setIconSize(QSize(38, 38));

        gridLayout_2->addWidget(nuevo_usuario, 0, 0, 1, 1);

        editar_usuario = new QPushButton(groupBox_3);
        editar_usuario->setObjectName(QString::fromUtf8("editar_usuario"));
        editar_usuario->setEnabled(false);
        editar_usuario->setFocusPolicy(Qt::NoFocus);
        editar_usuario->setLayoutDirection(Qt::LeftToRight);
        QIcon icon2;
        icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/edit_user_postgresql.svg")), QIcon::Normal, QIcon::Off);
        editar_usuario->setIcon(icon2);
        editar_usuario->setIconSize(QSize(38, 38));

        gridLayout_2->addWidget(editar_usuario, 0, 1, 1, 1);

        permisos_usuario = new QPushButton(groupBox_3);
        permisos_usuario->setObjectName(QString::fromUtf8("permisos_usuario"));
        permisos_usuario->setEnabled(false);
        permisos_usuario->setFocusPolicy(Qt::NoFocus);
        permisos_usuario->setLayoutDirection(Qt::LeftToRight);
        permisos_usuario->setIcon(icon2);
        permisos_usuario->setIconSize(QSize(38, 38));

        gridLayout_2->addWidget(permisos_usuario, 1, 0, 1, 1);

        eliminar_usuario = new QPushButton(groupBox_3);
        eliminar_usuario->setObjectName(QString::fromUtf8("eliminar_usuario"));
        eliminar_usuario->setEnabled(false);
        eliminar_usuario->setFocusPolicy(Qt::NoFocus);
        eliminar_usuario->setLayoutDirection(Qt::LeftToRight);
        QIcon icon3;
        icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/trash_user_postgresql.svg")), QIcon::Normal, QIcon::Off);
        eliminar_usuario->setIcon(icon3);
        eliminar_usuario->setIconSize(QSize(38, 38));

        gridLayout_2->addWidget(eliminar_usuario, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mui_crearbulmacont = new QPushButton(groupBox_2);
        mui_crearbulmacont->setObjectName(QString::fromUtf8("mui_crearbulmacont"));
        mui_crearbulmacont->setEnabled(false);
        mui_crearbulmacont->setFocusPolicy(Qt::NoFocus);
        mui_crearbulmacont->setLayoutDirection(Qt::LeftToRight);
        QIcon icon4;
        icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/iconbulmacont.svg")), QIcon::Normal, QIcon::Off);
        mui_crearbulmacont->setIcon(icon4);
        mui_crearbulmacont->setIconSize(QSize(38, 38));

        gridLayout->addWidget(mui_crearbulmacont, 0, 0, 1, 1);

        mui_crearbulmafact = new QPushButton(groupBox_2);
        mui_crearbulmafact->setObjectName(QString::fromUtf8("mui_crearbulmafact"));
        mui_crearbulmafact->setEnabled(false);
        mui_crearbulmafact->setFocusPolicy(Qt::NoFocus);
        mui_crearbulmafact->setLayoutDirection(Qt::LeftToRight);
        QIcon icon5;
        icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/iconbulmafact.svg")), QIcon::Normal, QIcon::Off);
        mui_crearbulmafact->setIcon(icon5);
        mui_crearbulmafact->setIconSize(QSize(38, 38));

        gridLayout->addWidget(mui_crearbulmafact, 0, 1, 1, 1);

        mui_adminempresas = new QPushButton(groupBox_2);
        mui_adminempresas->setObjectName(QString::fromUtf8("mui_adminempresas"));
        mui_adminempresas->setEnabled(false);
        mui_adminempresas->setFocusPolicy(Qt::NoFocus);
        mui_adminempresas->setLayoutDirection(Qt::LeftToRight);
        QIcon icon6;
        icon6.addPixmap(QPixmap(QString::fromUtf8(":/images/manage_company.svg")), QIcon::Normal, QIcon::Off);
        mui_adminempresas->setIcon(icon6);
        mui_adminempresas->setIconSize(QSize(32, 32));

        gridLayout->addWidget(mui_adminempresas, 1, 0, 1, 1);

        eliminar_empresa = new QPushButton(groupBox_2);
        eliminar_empresa->setObjectName(QString::fromUtf8("eliminar_empresa"));
        eliminar_empresa->setEnabled(false);
        eliminar_empresa->setFocusPolicy(Qt::NoFocus);
        eliminar_empresa->setLayoutDirection(Qt::LeftToRight);
        QIcon icon7;
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/trash_company.svg")), QIcon::Normal, QIcon::Off);
        eliminar_empresa->setIcon(icon7);
        eliminar_empresa->setIconSize(QSize(32, 32));

        gridLayout->addWidget(eliminar_empresa, 1, 1, 1, 1);

        mui_restbackup = new QPushButton(groupBox_2);
        mui_restbackup->setObjectName(QString::fromUtf8("mui_restbackup"));
        mui_restbackup->setEnabled(false);
        mui_restbackup->setFocusPolicy(Qt::NoFocus);
        mui_restbackup->setLayoutDirection(Qt::LeftToRight);
        QIcon icon8;
        icon8.addPixmap(QPixmap(QString::fromUtf8(":/images/restore_database.svg")), QIcon::Normal, QIcon::Off);
        mui_restbackup->setIcon(icon8);
        mui_restbackup->setIconSize(QSize(32, 32));

        gridLayout->addWidget(mui_restbackup, 2, 1, 1, 1);

        mui_creabackup = new QPushButton(groupBox_2);
        mui_creabackup->setObjectName(QString::fromUtf8("mui_creabackup"));
        mui_creabackup->setEnabled(false);
        mui_creabackup->setFocusPolicy(Qt::NoFocus);
        mui_creabackup->setLayoutDirection(Qt::LeftToRight);
        QIcon icon9;
        icon9.addPixmap(QPixmap(QString::fromUtf8(":/images/backup_database.svg")), QIcon::Normal, QIcon::Off);
        mui_creabackup->setIcon(icon9);
        mui_creabackup->setIconSize(QSize(32, 32));

        gridLayout->addWidget(mui_creabackup, 2, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mui_acerca = new QPushButton(centralwidget);
        mui_acerca->setObjectName(QString::fromUtf8("mui_acerca"));
        mui_acerca->setIcon(icon);

        horizontalLayout->addWidget(mui_acerca);

        horizontalSpacer = new QSpacerItem(388, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        mui_salir = new QPushButton(centralwidget);
        mui_salir->setObjectName(QString::fromUtf8("mui_salir"));
        QIcon icon10;
        icon10.addPixmap(QPixmap(QString::fromUtf8(":/images/bulmasetup_exit.png")), QIcon::Normal, QIcon::Off);
        mui_salir->setIcon(icon10);

        horizontalLayout->addWidget(mui_salir);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(mui_salir, SIGNAL(released()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BulmaSetup", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">BulmaSetup v 0.12</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Administracion de Bases</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">de Datos para BulmaGes</p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Gesti\303\263n de Usuarios", 0, QApplication::UnicodeUTF8));
        nuevo_usuario->setText(QApplication::translate("MainWindow", "Nuevos Usuario de PostgreSQL", 0, QApplication::UnicodeUTF8));
        editar_usuario->setText(QApplication::translate("MainWindow", "Editar Usuarios de PostgreSQL", 0, QApplication::UnicodeUTF8));
        permisos_usuario->setText(QApplication::translate("MainWindow", "Modificar Permisos de PostgreSQL", 0, QApplication::UnicodeUTF8));
        eliminar_usuario->setText(QApplication::translate("MainWindow", "Eliminar Usuarios de PostgreSQL", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Gesti\303\263n de Empresas", 0, QApplication::UnicodeUTF8));
        mui_crearbulmacont->setText(QApplication::translate("MainWindow", "Nueva Empresa de Contabilidad", 0, QApplication::UnicodeUTF8));
        mui_crearbulmafact->setText(QApplication::translate("MainWindow", "Nueva Empresa de Facturaci\303\263n / TPV", 0, QApplication::UnicodeUTF8));
        mui_adminempresas->setText(QApplication::translate("MainWindow", "Administrar Empresas Existentes", 0, QApplication::UnicodeUTF8));
        eliminar_empresa->setText(QApplication::translate("MainWindow", "Eliminar Empresas Existentes", 0, QApplication::UnicodeUTF8));
        mui_restbackup->setText(QApplication::translate("MainWindow", "Restaurar copia de seguridad", 0, QApplication::UnicodeUTF8));
        mui_creabackup->setText(QApplication::translate("MainWindow", "Crear Copia de Seguridad", 0, QApplication::UnicodeUTF8));
        mui_acerca->setText(QApplication::translate("MainWindow", "Acerca de BulmaSetup", 0, QApplication::UnicodeUTF8));
        mui_salir->setText(QApplication::translate("MainWindow", "Salir", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BULMASETUPBASE_H
