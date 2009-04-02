# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gestionusuariosbase.ui'
#
# Created: Thu Apr  2 17:11:53 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_GestionUsuarios(object):
    def setupUi(self, GestionUsuarios):
        GestionUsuarios.setObjectName("GestionUsuarios")
        GestionUsuarios.resize(355, 150)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(GestionUsuarios.sizePolicy().hasHeightForWidth())
        GestionUsuarios.setSizePolicy(sizePolicy)
        GestionUsuarios.setMinimumSize(QtCore.QSize(355, 150))
        GestionUsuarios.setMaximumSize(QtCore.QSize(355, 118))
        GestionUsuarios.setAcceptDrops(False)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_usuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        GestionUsuarios.setWindowIcon(icon)
        self.gridlayout = QtGui.QGridLayout(GestionUsuarios)
        self.gridlayout.setContentsMargins(-1, -1, 9, -1)
        self.gridlayout.setHorizontalSpacing(6)
        self.gridlayout.setObjectName("gridlayout")
        self.crear_usuario_postgres = QtGui.QPushButton(GestionUsuarios)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_nuevousuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.crear_usuario_postgres.setIcon(icon1)
        self.crear_usuario_postgres.setObjectName("crear_usuario_postgres")
        self.gridlayout.addWidget(self.crear_usuario_postgres, 0, 0, 1, 1)
        self.editar_usuario_postgres = QtGui.QPushButton(GestionUsuarios)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/images/bulmasetup_editarusuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.editar_usuario_postgres.setIcon(icon2)
        self.editar_usuario_postgres.setObjectName("editar_usuario_postgres")
        self.gridlayout.addWidget(self.editar_usuario_postgres, 2, 0, 1, 1)
        self.eliminar_usuario_postgres = QtGui.QPushButton(GestionUsuarios)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/images/bulmasetup_eliminarusuario.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.eliminar_usuario_postgres.setIcon(icon3)
        self.eliminar_usuario_postgres.setObjectName("eliminar_usuario_postgres")
        self.gridlayout.addWidget(self.eliminar_usuario_postgres, 3, 0, 1, 1)
        self.mui_cancelar = QtGui.QPushButton(GestionUsuarios)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Ignored, QtGui.QSizePolicy.Ignored)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.mui_cancelar.sizePolicy().hasHeightForWidth())
        self.mui_cancelar.setSizePolicy(sizePolicy)
        self.mui_cancelar.setAutoDefault(True)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.gridlayout.addWidget(self.mui_cancelar, 4, 0, 1, 1)

        self.retranslateUi(GestionUsuarios)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), GestionUsuarios.close)
        QtCore.QMetaObject.connectSlotsByName(GestionUsuarios)

    def retranslateUi(self, GestionUsuarios):
        GestionUsuarios.setWindowTitle(QtGui.QApplication.translate("GestionUsuarios", "Gestión de Usuarios", None, QtGui.QApplication.UnicodeUTF8))
        self.crear_usuario_postgres.setText(QtGui.QApplication.translate("GestionUsuarios", "Crear Usuario de PostgreSQL", None, QtGui.QApplication.UnicodeUTF8))
        self.editar_usuario_postgres.setText(QtGui.QApplication.translate("GestionUsuarios", "Editar Usuario de PostgreSQL", None, QtGui.QApplication.UnicodeUTF8))
        self.eliminar_usuario_postgres.setText(QtGui.QApplication.translate("GestionUsuarios", "Eliminar Usuarios de PostgreSQL", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("GestionUsuarios", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
