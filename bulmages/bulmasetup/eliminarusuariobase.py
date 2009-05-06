# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'eliminarusuariobase.ui'
#
# Created: Wed May  6 17:00:49 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EliminarUsuario(object):
    def setupUi(self, EliminarUsuario):
        EliminarUsuario.setObjectName("EliminarUsuario")
        EliminarUsuario.resize(260, 220)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EliminarUsuario.sizePolicy().hasHeightForWidth())
        EliminarUsuario.setSizePolicy(sizePolicy)
        EliminarUsuario.setMinimumSize(QtCore.QSize(200, 120))
        EliminarUsuario.setMaximumSize(QtCore.QSize(300, 300))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_eliminarusuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        EliminarUsuario.setWindowIcon(icon)
        self.gridLayout_3 = QtGui.QGridLayout(EliminarUsuario)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.listWidget = QtGui.QListWidget(EliminarUsuario)
        self.listWidget.setObjectName("listWidget")
        self.gridLayout_2.addWidget(self.listWidget, 0, 0, 1, 1)
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 0, 0, 1, 1)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)
        self.mui_eliminarusuario = QtGui.QPushButton(EliminarUsuario)
        self.mui_eliminarusuario.setIcon(icon)
        self.mui_eliminarusuario.setObjectName("mui_eliminarusuario")
        self.verticalLayout.addWidget(self.mui_eliminarusuario)
        self.mui_cancelar = QtGui.QPushButton(EliminarUsuario)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon1)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.verticalLayout.addWidget(self.mui_cancelar)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)
        self.gridLayout.addLayout(self.verticalLayout, 0, 1, 1, 1)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem3, 0, 2, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 0, 1, 1, 1)
        self.gridLayout_3.addLayout(self.gridLayout_2, 0, 0, 1, 1)

        self.retranslateUi(EliminarUsuario)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), EliminarUsuario.close)
        QtCore.QMetaObject.connectSlotsByName(EliminarUsuario)

    def retranslateUi(self, EliminarUsuario):
        EliminarUsuario.setWindowTitle(QtGui.QApplication.translate("EliminarUsuario", "Eliminar Usuario", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_eliminarusuario.setText(QtGui.QApplication.translate("EliminarUsuario", "Eliminar", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("EliminarUsuario", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
