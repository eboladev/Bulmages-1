# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'eliminarempresabase.ui'
#
# Created: Mon May 18 16:59:47 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_EliminarEmpresa(object):
    def setupUi(self, EliminarEmpresa):
        EliminarEmpresa.setObjectName("EliminarEmpresa")
        EliminarEmpresa.resize(400, 404)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(EliminarEmpresa.sizePolicy().hasHeightForWidth())
        EliminarEmpresa.setSizePolicy(sizePolicy)
        EliminarEmpresa.setMinimumSize(QtCore.QSize(400, 300))
        font = QtGui.QFont()
        EliminarEmpresa.setFont(font)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_eliminarempresa.xpm"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        EliminarEmpresa.setWindowIcon(icon)
        self.gridLayout = QtGui.QGridLayout(EliminarEmpresa)
        self.gridLayout.setObjectName("gridLayout")
        self.label = QtGui.QLabel(EliminarEmpresa)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setStrikeOut(False)
        font.setBold(True)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.lista_empresas = QtGui.QTableWidget(EliminarEmpresa)
        self.lista_empresas.setFocusPolicy(QtCore.Qt.ClickFocus)
        self.lista_empresas.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.lista_empresas.setAutoScroll(True)
        self.lista_empresas.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
        self.lista_empresas.setTabKeyNavigation(False)
        self.lista_empresas.setProperty("showDropIndicator", QtCore.QVariant(False))
        self.lista_empresas.setDragDropOverwriteMode(False)
        self.lista_empresas.setAlternatingRowColors(True)
        self.lista_empresas.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
        self.lista_empresas.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self.lista_empresas.setTextElideMode(QtCore.Qt.ElideLeft)
        self.lista_empresas.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerItem)
        self.lista_empresas.setShowGrid(True)
        self.lista_empresas.setGridStyle(QtCore.Qt.DotLine)
        self.lista_empresas.setWordWrap(True)
        self.lista_empresas.setCornerButtonEnabled(True)
        self.lista_empresas.setObjectName("lista_empresas")
        self.lista_empresas.setColumnCount(3)
        self.lista_empresas.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.lista_empresas.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.lista_empresas.setHorizontalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.lista_empresas.setHorizontalHeaderItem(2, item)
        self.gridLayout.addWidget(self.lista_empresas, 1, 0, 1, 1)
        self.mui_cancelar = QtGui.QPushButton(EliminarEmpresa)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon1)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.gridLayout.addWidget(self.mui_cancelar, 2, 0, 1, 1)

        self.retranslateUi(EliminarEmpresa)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), EliminarEmpresa.close)
        QtCore.QMetaObject.connectSlotsByName(EliminarEmpresa)

    def retranslateUi(self, EliminarEmpresa):
        EliminarEmpresa.setWindowTitle(QtGui.QApplication.translate("EliminarEmpresa", "Eliminar Empresa", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("EliminarEmpresa", "Doble clic en la Empresa que deseas eliminar", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("EliminarEmpresa", "Nombre", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("EliminarEmpresa", "Base de Datos", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(2).setText(QtGui.QApplication.translate("EliminarEmpresa", "Tipo", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("EliminarEmpresa", "Salir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
