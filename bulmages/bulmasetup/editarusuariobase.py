# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'editarusuariobase.ui'
#
# Created: Tue May 12 16:28:38 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EditarUsuario(object):
    def setupUi(self, EditarUsuario):
        EditarUsuario.setObjectName("EditarUsuario")
        EditarUsuario.resize(293, 218)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EditarUsuario.sizePolicy().hasHeightForWidth())
        EditarUsuario.setSizePolicy(sizePolicy)
        EditarUsuario.setMinimumSize(QtCore.QSize(293, 218))
        EditarUsuario.setMaximumSize(QtCore.QSize(293, 218))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_editarusuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        EditarUsuario.setWindowIcon(icon)
        self.gridLayout_3 = QtGui.QGridLayout(EditarUsuario)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.listWidget = QtGui.QListWidget(EditarUsuario)
        self.listWidget.setMinimumSize(QtCore.QSize(150, 200))
        self.listWidget.setObjectName("listWidget")
        self.gridLayout_3.addWidget(self.listWidget, 0, 0, 1, 1)
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.Radial_su = QtGui.QRadioButton(EditarUsuario)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.Radial_su.setFont(font)
        self.Radial_su.setObjectName("Radial_su")
        self.gridLayout_2.addWidget(self.Radial_su, 0, 0, 1, 1)
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.CheckBox_password = QtGui.QCheckBox(EditarUsuario)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.CheckBox_password.setFont(font)
        self.CheckBox_password.setObjectName("CheckBox_password")
        self.gridLayout.addWidget(self.CheckBox_password, 0, 0, 1, 1)
        self.lineEdit = QtGui.QLineEdit(EditarUsuario)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 1, 0, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 1, 0, 1, 1)
        spacerItem = QtGui.QSpacerItem(17, 13, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.gridLayout_2.addItem(spacerItem, 2, 0, 1, 1)
        self.aplicar_cambios = QtGui.QPushButton(EditarUsuario)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_save.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.aplicar_cambios.setIcon(icon1)
        self.aplicar_cambios.setObjectName("aplicar_cambios")
        self.gridLayout_2.addWidget(self.aplicar_cambios, 3, 0, 1, 1)
        self.mui_cancelar = QtGui.QPushButton(EditarUsuario)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon2)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.gridLayout_2.addWidget(self.mui_cancelar, 4, 0, 1, 1)
        self.gridLayout_3.addLayout(self.gridLayout_2, 0, 1, 1, 1)

        self.retranslateUi(EditarUsuario)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), EditarUsuario.close)
        QtCore.QMetaObject.connectSlotsByName(EditarUsuario)

    def retranslateUi(self, EditarUsuario):
        EditarUsuario.setWindowTitle(QtGui.QApplication.translate("EditarUsuario", "Editar Usuario", None, QtGui.QApplication.UnicodeUTF8))
        self.Radial_su.setText(QtGui.QApplication.translate("EditarUsuario", "Super Usuario", None, QtGui.QApplication.UnicodeUTF8))
        self.CheckBox_password.setText(QtGui.QApplication.translate("EditarUsuario", "Cambiar Password", None, QtGui.QApplication.UnicodeUTF8))
        self.aplicar_cambios.setText(QtGui.QApplication.translate("EditarUsuario", "Aplicar", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("EditarUsuario", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
