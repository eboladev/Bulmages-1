# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'modificarcontabilidadbase.ui'
#
# Created: Sat Mar 21 12:46:45 2009
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ModificarContabilidadBase(object):
    def setupUi(self, ModificarContabilidadBase):
        ModificarContabilidadBase.setObjectName("ModificarContabilidadBase")
        ModificarContabilidadBase.resize(QtCore.QSize(QtCore.QRect(0,0,800,608).size()).expandedTo(ModificarContabilidadBase.minimumSizeHint()))

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred,QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(ModificarContabilidadBase.sizePolicy().hasHeightForWidth())
        ModificarContabilidadBase.setSizePolicy(sizePolicy)
        ModificarContabilidadBase.setMaximumSize(QtCore.QSize(2800,2500))
        ModificarContabilidadBase.setWindowIcon(QtGui.QIcon(" "))

        self.gridlayout = QtGui.QGridLayout(ModificarContabilidadBase)
        self.gridlayout.setMargin(0)
        self.gridlayout.setSpacing(0)
        self.gridlayout.setObjectName("gridlayout")

        self.label = QtGui.QLabel(ModificarContabilidadBase)
        self.label.setObjectName("label")
        self.gridlayout.addWidget(self.label,0,0,1,1)

        self.mui_nomempresa = QtGui.QLineEdit(ModificarContabilidadBase)
        self.mui_nomempresa.setObjectName("mui_nomempresa")
        self.gridlayout.addWidget(self.mui_nomempresa,0,1,1,3)

        self.toolButton = QtGui.QToolButton(ModificarContabilidadBase)

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed,QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.toolButton.sizePolicy().hasHeightForWidth())
        self.toolButton.setSizePolicy(sizePolicy)
        self.toolButton.setMaximumSize(QtCore.QSize(15,16777215))
        self.toolButton.setCheckable(True)
        self.toolButton.setChecked(True)
        self.toolButton.setObjectName("toolButton")
        self.gridlayout.addWidget(self.toolButton,0,4,5,1)

        self.mui_descripcion = QtGui.QTextBrowser(ModificarContabilidadBase)
        self.mui_descripcion.setObjectName("mui_descripcion")
        self.gridlayout.addWidget(self.mui_descripcion,0,5,5,1)

        self.label_2 = QtGui.QLabel(ModificarContabilidadBase)
        self.label_2.setObjectName("label_2")
        self.gridlayout.addWidget(self.label_2,1,0,1,1)

        self.mui_nomdb = QtGui.QLineEdit(ModificarContabilidadBase)
        self.mui_nomdb.setReadOnly(True)
        self.mui_nomdb.setObjectName("mui_nomdb")
        self.gridlayout.addWidget(self.mui_nomdb,1,1,1,2)

        self.mui_actualizarDB = QtGui.QCheckBox(ModificarContabilidadBase)
        self.mui_actualizarDB.setChecked(False)
        self.mui_actualizarDB.setObjectName("mui_actualizarDB")
        self.gridlayout.addWidget(self.mui_actualizarDB,1,3,1,1)

        self.label_3 = QtGui.QLabel(ModificarContabilidadBase)
        self.label_3.setObjectName("label_3")
        self.gridlayout.addWidget(self.label_3,2,0,1,1)

        self.mui_databaserevision = QtGui.QLineEdit(ModificarContabilidadBase)
        self.mui_databaserevision.setReadOnly(True)
        self.mui_databaserevision.setObjectName("mui_databaserevision")
        self.gridlayout.addWidget(self.mui_databaserevision,2,1,1,2)

        self.groupBox = QtGui.QGroupBox(ModificarContabilidadBase)
        self.groupBox.setObjectName("groupBox")

        self.hboxlayout = QtGui.QHBoxLayout(self.groupBox)
        self.hboxlayout.setSpacing(0)
        self.hboxlayout.setMargin(0)
        self.hboxlayout.setObjectName("hboxlayout")

        self.mui_vertodos = QtGui.QCheckBox(self.groupBox)
        self.mui_vertodos.setObjectName("mui_vertodos")
        self.hboxlayout.addWidget(self.mui_vertodos)

        spacerItem = QtGui.QSpacerItem(43,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)

        self.hboxlayout1 = QtGui.QHBoxLayout()
        self.hboxlayout1.setSpacing(0)
        self.hboxlayout1.setObjectName("hboxlayout1")

        self.label_5 = QtGui.QLabel(self.groupBox)
        self.label_5.setObjectName("label_5")
        self.hboxlayout1.addWidget(self.label_5)

        self.mui_categoria = QtGui.QComboBox(self.groupBox)
        self.mui_categoria.setObjectName("mui_categoria")
        self.hboxlayout1.addWidget(self.mui_categoria)
        self.hboxlayout.addLayout(self.hboxlayout1)
        self.gridlayout.addWidget(self.groupBox,3,0,1,4)

        self.tabWidget = QtGui.QTabWidget(ModificarContabilidadBase)
        self.tabWidget.setObjectName("tabWidget")

        self.tab = QtGui.QWidget()
        self.tab.setGeometry(QtCore.QRect(0,0,664,405))
        self.tab.setObjectName("tab")

        self.vboxlayout = QtGui.QVBoxLayout(self.tab)
        self.vboxlayout.setMargin(0)
        self.vboxlayout.setObjectName("vboxlayout")

        self.mui_plugins = QtGui.QTableWidget(self.tab)
        self.mui_plugins.setAlternatingRowColors(True)
        self.mui_plugins.setSelectionMode(QtGui.QAbstractItemView.NoSelection)
        self.mui_plugins.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self.mui_plugins.setGridStyle(QtCore.Qt.NoPen)
        self.mui_plugins.setObjectName("mui_plugins")
        self.vboxlayout.addWidget(self.mui_plugins)
        self.tabWidget.addTab(self.tab,"")

        self.tab_3 = QtGui.QWidget()
        self.tab_3.setGeometry(QtCore.QRect(0,0,512,240))
        self.tab_3.setObjectName("tab_3")

        self.vboxlayout1 = QtGui.QVBoxLayout(self.tab_3)
        self.vboxlayout1.setObjectName("vboxlayout1")

        self.mui_textBrowser = QtGui.QTextBrowser(self.tab_3)
        self.mui_textBrowser.setObjectName("mui_textBrowser")
        self.vboxlayout1.addWidget(self.mui_textBrowser)
        self.tabWidget.addTab(self.tab_3,"")
        self.gridlayout.addWidget(self.tabWidget,4,0,1,4)

        self.mui_aceptar = QtGui.QPushButton(ModificarContabilidadBase)
        self.mui_aceptar.setObjectName("mui_aceptar")
        self.gridlayout.addWidget(self.mui_aceptar,5,0,1,2)

        self.mui_hacerbackup = QtGui.QPushButton(ModificarContabilidadBase)
        self.mui_hacerbackup.setObjectName("mui_hacerbackup")
        self.gridlayout.addWidget(self.mui_hacerbackup,5,2,1,2)

        self.mui_cancelar = QtGui.QPushButton(ModificarContabilidadBase)
        self.mui_cancelar.setObjectName("mui_cancelar")
        self.gridlayout.addWidget(self.mui_cancelar,5,4,1,2)
        self.label.setBuddy(self.mui_nomempresa)
        self.label_2.setBuddy(self.mui_nomdb)
        self.label_5.setBuddy(self.mui_categoria)

        self.retranslateUi(ModificarContabilidadBase)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QObject.connect(self.mui_cancelar,QtCore.SIGNAL("released()"),ModificarContabilidadBase.reject)
        QtCore.QObject.connect(self.toolButton,QtCore.SIGNAL("toggled(bool)"),self.mui_descripcion.setHidden)
        QtCore.QMetaObject.connectSlotsByName(ModificarContabilidadBase)

    def retranslateUi(self, ModificarContabilidadBase):
        ModificarContabilidadBase.setWindowTitle(QtGui.QApplication.translate("ModificarContabilidadBase", "Contabilidad", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Nombre Empresa", None, QtGui.QApplication.UnicodeUTF8))
        self.toolButton.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "..", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Nombre Base de Datos", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_actualizarDB.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Actualizar Base de Datos", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Version de la Base de Datos", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("ModificarContabilidadBase", "Opciones de Filtrado de Plugins", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_vertodos.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Mostrar todos los plugins o solo los instalados", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Categoria ", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_plugins.clear()
        self.mui_plugins.setColumnCount(3)
        self.mui_plugins.setRowCount(0)

        headerItem = QtGui.QTableWidgetItem()
        headerItem.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Plugin", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_plugins.setHorizontalHeaderItem(0,headerItem)

        headerItem1 = QtGui.QTableWidgetItem()
        headerItem1.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Descripcion", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_plugins.setHorizontalHeaderItem(1,headerItem1)

        headerItem2 = QtGui.QTableWidgetItem()
        headerItem2.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Version Instalada", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_plugins.setHorizontalHeaderItem(2,headerItem2)
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("ModificarContabilidadBase", "Plugins BulmaCont", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_3), QtGui.QApplication.translate("ModificarContabilidadBase", "Consola", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_aceptar.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Aplicar cambios", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_hacerbackup.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Hacer copia de seguridad", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_cancelar.setText(QtGui.QApplication.translate("ModificarContabilidadBase", "Cerrar", None, QtGui.QApplication.UnicodeUTF8))

