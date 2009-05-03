# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'bulmasetupbase.ui'
#
# Created: Sun May  3 18:41:19 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(557, 459)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/iglues.org.svg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtGui.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(20, 20, 209, 200))
        self.label.setMinimumSize(QtCore.QSize(209, 200))
        self.label.setMaximumSize(QtCore.QSize(209, 150))
        self.label.setPixmap(QtGui.QPixmap(":/images/iglues.org.svg"))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.mui_gestionusuarios = QtGui.QPushButton(self.centralwidget)
        self.mui_gestionusuarios.setGeometry(QtCore.QRect(250, 30, 291, 61))
        self.mui_gestionusuarios.setFocusPolicy(QtCore.Qt.NoFocus)
        self.mui_gestionusuarios.setLayoutDirection(QtCore.Qt.LeftToRight)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_usuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_gestionusuarios.setIcon(icon1)
        self.mui_gestionusuarios.setIconSize(QtCore.QSize(48, 48))
        self.mui_gestionusuarios.setObjectName("mui_gestionusuarios")
        self.mui_crearbulmacont = QtGui.QPushButton(self.centralwidget)
        self.mui_crearbulmacont.setGeometry(QtCore.QRect(250, 100, 291, 61))
        self.mui_crearbulmacont.setFocusPolicy(QtCore.Qt.NoFocus)
        self.mui_crearbulmacont.setLayoutDirection(QtCore.Qt.LeftToRight)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/images/bulmasetup_bulmacont.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_crearbulmacont.setIcon(icon2)
        self.mui_crearbulmacont.setIconSize(QtCore.QSize(48, 48))
        self.mui_crearbulmacont.setObjectName("mui_crearbulmacont")
        self.mui_crearbulmafact = QtGui.QPushButton(self.centralwidget)
        self.mui_crearbulmafact.setGeometry(QtCore.QRect(250, 170, 291, 61))
        self.mui_crearbulmafact.setFocusPolicy(QtCore.Qt.NoFocus)
        self.mui_crearbulmafact.setLayoutDirection(QtCore.Qt.LeftToRight)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/images/bulmasetup_bulmafact.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_crearbulmafact.setIcon(icon3)
        self.mui_crearbulmafact.setIconSize(QtCore.QSize(48, 48))
        self.mui_crearbulmafact.setObjectName("mui_crearbulmafact")
        self.label_2 = QtGui.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(30, 250, 171, 81))
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName("label_2")
        self.mui_salir = QtGui.QPushButton(self.centralwidget)
        self.mui_salir.setGeometry(QtCore.QRect(460, 420, 75, 24))
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_salir.setIcon(icon4)
        self.mui_salir.setObjectName("mui_salir")
        self.line = QtGui.QFrame(self.centralwidget)
        self.line.setGeometry(QtCore.QRect(230, 20, 20, 331))
        self.line.setFrameShape(QtGui.QFrame.VLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName("line")
        self.mui_adminempresas = QtGui.QPushButton(self.centralwidget)
        self.mui_adminempresas.setGeometry(QtCore.QRect(250, 240, 291, 61))
        self.mui_adminempresas.setFocusPolicy(QtCore.Qt.NoFocus)
        self.mui_adminempresas.setLayoutDirection(QtCore.Qt.LeftToRight)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(":/images/bulmasetup_empresa.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_adminempresas.setIcon(icon5)
        self.mui_adminempresas.setIconSize(QtCore.QSize(48, 48))
        self.mui_adminempresas.setObjectName("mui_adminempresas")
        self.mui_restbackup = QtGui.QPushButton(self.centralwidget)
        self.mui_restbackup.setGeometry(QtCore.QRect(250, 310, 291, 61))
        self.mui_restbackup.setFocusPolicy(QtCore.Qt.NoFocus)
        self.mui_restbackup.setLayoutDirection(QtCore.Qt.LeftToRight)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(":/images/bulmasetup_cargarempresa.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_restbackup.setIcon(icon6)
        self.mui_restbackup.setIconSize(QtCore.QSize(48, 48))
        self.mui_restbackup.setObjectName("mui_restbackup")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QObject.connect(self.mui_salir, QtCore.SIGNAL("released()"), MainWindow.close)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "BulmaSetup", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_gestionusuarios.setText(QtGui.QApplication.translate("MainWindow", "Gestionar Usuarios de PostgreSQL", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_crearbulmacont.setText(QtGui.QApplication.translate("MainWindow", "Nueva Empresa de Contabilidad", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_crearbulmafact.setText(QtGui.QApplication.translate("MainWindow", "Nueva Empresa de Facturacion / TPV", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans Serif\'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">BulmaSetup v 0.11</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Administracion de Bases</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">de Datos para BulmaGes</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_salir.setText(QtGui.QApplication.translate("MainWindow", "Salir", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_adminempresas.setText(QtGui.QApplication.translate("MainWindow", "Administrar Empresas Existentes", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_restbackup.setText(QtGui.QApplication.translate("MainWindow", "Restaurar una Copia de Seguridad", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
