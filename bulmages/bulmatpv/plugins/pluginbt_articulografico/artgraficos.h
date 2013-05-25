#ifndef ARTGRAFICOS_H
#define ARTGRAFICOS_H

#include <QtXml/QDomDocument>
#include <QtWidgets/QTableWidget>

#include "blfunctions.h"
#include "blwidget.h"
#include "ui_artgraficosbase.h"
#include "btcompany.h"


class ArtGraficos : public BlWidget, public Ui_ArtGraficosBase
{
    Q_OBJECT

public:
    QString m_articulos[100][100];
    QDomNode m_nodarticulos[100][100];
    QDomDocument m_doc;
    int m_numPantallas;
    int m_pantallaActual;

public:
    ArtGraficos ( BlMainCompany *emp, QWidget *parent );
    virtual ~ArtGraficos();
    void cargaXML ( QString );
    void muestraPantalla ( int );
    void ponPantallas();

public slots:
    virtual void on_mui_list_cellClicked ( int, int );
    virtual void on_mui_botonSiguiente_clicked();
    virtual void on_mui_botonAnterior_clicked();
    virtual void pulsadoBoton();
};

#endif

