# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'guardarbackupbase.ui'
#
# Created: Tue Jun  2 17:53:05 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_GuardarBackup(object):
    def setupUi(self, GuardarBackup):
        GuardarBackup.setObjectName("GuardarBackup")
        GuardarBackup.resize(412, 393)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(GuardarBackup.sizePolicy().hasHeightForWidth())
        GuardarBackup.setSizePolicy(sizePolicy)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/images/bulmasetup_save.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        GuardarBackup.setWindowIcon(icon)
        self.gridLayout_3 = QtGui.QGridLayout(GuardarBackup)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.guardar_backup = QtGui.QPushButton(GuardarBackup)
        self.guardar_backup.setIcon(icon)
        self.guardar_backup.setObjectName("guardar_backup")
        self.verticalLayout.addWidget(self.guardar_backup)
        self.mui_cancelar = QtGui.QPushButton(GuardarBackup)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/images/bulmasetup_exit.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.mui_cancelar.setIcon(icon1)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.verticalLayout.addWidget(self.mui_cancelar)
        spacerItem1 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)
        self.gridLayout.addLayout(self.verticalLayout, 0, 1, 1, 1)
        self.lista_empresas = QtGui.QTableWidget(GuardarBackup)
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
        self.gridLayout.addWidget(self.lista_empresas, 0, 0, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 0, 0, 1, 1)
        self.gridLayout_3.addLayout(self.gridLayout_2, 0, 0, 1, 1)

        self.retranslateUi(GuardarBackup)
        QtCore.QObject.connect(self.mui_cancelar, QtCore.SIGNAL("released()"), GuardarBackup.close)
        QtCore.QMetaObject.connectSlotsByName(GuardarBackup)

    def retranslateUi(self, GuardarBackup):
        GuardarBackup.setWindowTitle(QtGui.QApplication.translate("GuardarBackup", "Crear Backup de Empresa", None, QtGui.QApplication.UnicodeUTF8))
        self.guardar_backup.setText(QtGui.QApplication.translate("GuardarBackup", "Backup", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("GuardarBackup", "Salir", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("GuardarBackup", "Empresa", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("GuardarBackup", "Base de Datos", None, QtGui.QApplication.UnicodeUTF8))
        self.lista_empresas.horizontalHeaderItem(2).setText(QtGui.QApplication.translate("GuardarBackup", "Tipo", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
