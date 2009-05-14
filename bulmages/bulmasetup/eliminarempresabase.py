# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'eliminarempresabase.ui'
#
# Created: Thu May 14 18:34:10 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EliminarEmpresa(object):
    def setupUi(self, EliminarEmpresa):
        EliminarEmpresa.setObjectName("EliminarEmpresa")
        EliminarEmpresa.resize(480, 300)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EliminarEmpresa.sizePolicy().hasHeightForWidth())
        EliminarEmpresa.setSizePolicy(sizePolicy)
        EliminarEmpresa.setMinimumSize(QtCore.QSize(480, 300))
        EliminarEmpresa.setMaximumSize(QtCore.QSize(480, 300))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_empresabase.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        EliminarEmpresa.setWindowIcon(icon)
        self.gridLayout_2 = QtGui.QGridLayout(EliminarEmpresa)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.label = QtGui.QLabel(EliminarEmpresa)
        self.label.setObjectName("label")
        self.gridLayout_2.addWidget(self.label, 0, 0, 1, 1)
        self.label_2 = QtGui.QLabel(EliminarEmpresa)
        self.label_2.setObjectName("label_2")
        self.gridLayout_2.addWidget(self.label_2, 0, 1, 1, 1)
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        spacerItem = QtGui.QSpacerItem(13, 275, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 0, 0, 1, 1)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)
        self.eliminar_empresa = QtGui.QPushButton(EliminarEmpresa)
        self.eliminar_empresa.setIcon(icon)
        self.eliminar_empresa.setObjectName("eliminar_empresa")
        self.verticalLayout.addWidget(self.eliminar_empresa)
        self.mui_cancelar = QtGui.QPushButton(EliminarEmpresa)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon1)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.verticalLayout.addWidget(self.mui_cancelar)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)
        self.gridLayout.addLayout(self.verticalLayout, 0, 1, 1, 1)
        spacerItem3 = QtGui.QSpacerItem(13, 275, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem3, 0, 2, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 0, 2, 2, 1)
        self.listWidgetFact = QtGui.QListWidget(EliminarEmpresa)
        self.listWidgetFact.setObjectName("listWidgetFact")
        self.gridLayout_2.addWidget(self.listWidgetFact, 1, 0, 1, 1)
        self.listWidgetCont = QtGui.QListWidget(EliminarEmpresa)
        self.listWidgetCont.setObjectName("listWidgetCont")
        self.gridLayout_2.addWidget(self.listWidgetCont, 1, 1, 1, 1)

        self.retranslateUi(EliminarEmpresa)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), EliminarEmpresa.close)
        QtCore.QMetaObject.connectSlotsByName(EliminarEmpresa)

    def retranslateUi(self, EliminarEmpresa):
        EliminarEmpresa.setWindowTitle(QtGui.QApplication.translate("EliminarEmpresa", "Eliminar Empresa", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("EliminarEmpresa", "Bases de Datos de BulmaFact", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("EliminarEmpresa", "Bases de Datos de BulmaCont", None, QtGui.QApplication.UnicodeUTF8))
        self.eliminar_empresa.setText(QtGui.QApplication.translate("EliminarEmpresa", "Eliminar", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("EliminarEmpresa", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
