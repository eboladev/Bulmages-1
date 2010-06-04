# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'iniciobase.ui'
#
# Created: Fri Jun  4 18:42:35 2010
#      by: PyQt4 UI code generator 4.7
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(403, 173)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.mui_iniciar = QtGui.QPushButton(self.centralwidget)
        self.mui_iniciar.setGeometry(QtCore.QRect(60, 141, 71, 22))
        self.mui_iniciar.setObjectName("mui_iniciar")
        self.mui_pushButton_salir = QtGui.QPushButton(self.centralwidget)
        self.mui_pushButton_salir.setGeometry(QtCore.QRect(280, 141, 71, 22))
        self.mui_pushButton_salir.setObjectName("mui_pushButton_salir")
        self.mui_progressBar = QtGui.QProgressBar(self.centralwidget)
        self.mui_progressBar.setGeometry(QtCore.QRect(30, 53, 341, 23))
        self.mui_progressBar.setProperty("value", 0)
        self.mui_progressBar.setObjectName("mui_progressBar")
        self.mui_progressBar_2 = QtGui.QProgressBar(self.centralwidget)
        self.mui_progressBar_2.setGeometry(QtCore.QRect(30, 107, 341, 23))
        self.mui_progressBar_2.setProperty("value", 0)
        self.mui_progressBar_2.setObjectName("mui_progressBar_2")
        self.label_tarea = QtGui.QLabel(self.centralwidget)
        self.label_tarea.setGeometry(QtCore.QRect(30, 12, 331, 16))
        self.label_tarea.setObjectName("label_tarea")
        self.label_completado = QtGui.QLabel(self.centralwidget)
        self.label_completado.setGeometry(QtCore.QRect(30, 87, 331, 16))
        self.label_completado.setObjectName("label_completado")
        self.label_imagen = QtGui.QLabel(self.centralwidget)
        self.label_imagen.setGeometry(QtCore.QRect(31, 32, 331, 16))
        self.label_imagen.setObjectName("label_imagen")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QObject.connect(self.mui_pushButton_salir, QtCore.SIGNAL("clicked()"), MainWindow.close)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "btvgencatalog", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_iniciar.setText(QtGui.QApplication.translate("MainWindow", "Iniciar", None, QtGui.QApplication.UnicodeUTF8))
        self.mui_pushButton_salir.setText(QtGui.QApplication.translate("MainWindow", "Salir", None, QtGui.QApplication.UnicodeUTF8))
        self.label_tarea.setText(QtGui.QApplication.translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans Serif\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Tarea:</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_completado.setText(QtGui.QApplication.translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans Serif\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Completado:</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_imagen.setText(QtGui.QApplication.translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans Serif\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Imagen:</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))

