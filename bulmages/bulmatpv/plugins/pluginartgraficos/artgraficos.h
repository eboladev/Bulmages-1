#ifndef ARTGRAFICOS_H
#define ARTGRAFICOS_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_artgraficosbase.h"
#include "empresatpv.h"
#include <QDomDocument>

class ArtGraficos : public BLWidget, public Ui_ArtGraficosBase {
    Q_OBJECT
public:
	QString m_articulos[100][100];
	QDomDocument m_doc;
	int m_numPantallas;
	int m_pantallaActual;
public:
    ArtGraficos(EmpresaBase *emp, QWidget *parent);
    virtual ~ArtGraficos();
    void cargaXML(QString );
    void muestraPantalla(int);
public slots:
    virtual void on_mui_list_cellClicked(int, int);
    virtual void on_mui_botonSiguiente_clicked();
    virtual void on_mui_botonAnterior_clicked();
};

#endif

