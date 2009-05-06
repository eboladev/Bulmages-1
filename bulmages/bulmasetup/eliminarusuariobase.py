# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'eliminarusuariobase.ui'
#
# Created: Wed May  6 12:38:34 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EliminarUsuario(object):
    def setupUi(self, EliminarUsuario):
        EliminarUsuario.setObjectName("EliminarUsuario")
        EliminarUsuario.resize(400, 200)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EliminarUsuario.sizePolicy().hasHeightForWidth())
        EliminarUsuario.setSizePolicy(sizePolicy)
        EliminarUsuario.setMinimumSize(QtCore.QSize(400, 200))
        EliminarUsuario.setMaximumSize(QtCore.QSize(400, 200))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_eliminarusuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        EliminarUsuario.setWindowIcon(icon)
        self.mui_eliminarusuario = QtGui.QPushButton(EliminarUsuario)
        self.mui_eliminarusuario.setGeometry(QtCore.QRect(290, 70, 91, 24))
        self.mui_eliminarusuario.setIcon(icon)
        self.mui_eliminarusuario.setObjectName("mui_eliminarusuario")
        self.mui_cancelar = QtGui.QPushButton(EliminarUsuario)
        self.mui_cancelar.setGeometry(QtCore.QRect(290, 110, 91, 24))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon1)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.listWidget = QtGui.QListWidget(EliminarUsuario)
        self.listWidget.setGeometry(QtCore.QRect(10, 10, 261, 181))
        self.listWidget.setObjectName("listWidget")

        self.retranslateUi(EliminarUsuario)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), EliminarUsuario.close)
        QtCore.QMetaObject.connectSlotsByName(EliminarUsuario)

    def retranslateUi(self, EliminarUsuario):
        EliminarUsuario.setWindowTitle(QtGui.QApplication.translate("EliminarUsuario", "Eliminar Usuario", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_eliminarusuario.setText(QtGui.QApplication.translate("EliminarUsuario", "Eliminar", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("EliminarUsuario", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
