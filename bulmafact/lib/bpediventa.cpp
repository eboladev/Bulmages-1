/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "bpediventa.h"

BPediVenta::BPediVenta(QWidget * parent, const char * name) : UIPediVenta(parent, name)
{
//FrameCabeceraLeft = FrameCabecera->pos().x();
//FrameCabeceraTop = FrameCabecera->pos().y();
//FrameCabeceraWidth = FrameCabecera->width();
//FrameCabeceraHeight = FrameCabecera->height();
scrollBar1->setMaxValue (FrameCabecera->height()-contenedorCabecera->height());
scrollBar2->setMaxValue (FrameCabecera->width()-contenedorCabecera->width());
}


BPediVenta::~BPediVenta()
{
}



//Barra Scroll Vertical de la cabezera
void BPediVenta::ScrollCabeceraV(int valor)
{
FrameCabecera->setGeometry(FrameCabecera->pos().x(), - valor , FrameCabecera->width(), FrameCabecera->height());
}

//Barra Scroll Horizontal de la cabezera
void BPediVenta::ScrollCabeceraH(int valor)
{
//FrameCabecera->setGeometry(- valor, baseScrollCabecera->frameRect().left(), baseScrollCabecera->frameRect().width(), FrameCabecera->frameRect().height());
FrameCabecera->setGeometry(- valor, FrameCabecera->pos().y() , FrameCabecera->width(), FrameCabecera->height());
}

/****************************************************************************************/
/* Ajustamos los Frames al tamaño de la ventana                                         */
/****************************************************************************************/
void BPediVenta::resizeEvent(QResizeEvent * evento)
{
//recuadro exterior de la cabezera. (Verticalmente ocupa un 30% del espacio disponible)
contenedorCabecera->setGeometry(70,10,evento->size().width()-75, roundI((evento->size().height()-140)  * 0.4) ); 

//Barras de Scroll Vertical i Horizontal
scrollBar1->setGeometry(contenedorCabecera->width()-20,7,20,contenedorCabecera->height()-25);
scrollBar2->setGeometry(1,contenedorCabecera->height()-20,contenedorCabecera->width()-20,20);
scrollBar1->setMaxValue (FrameCabecera->height()-contenedorCabecera->height());
scrollBar2->setMaxValue (FrameCabecera->width()-contenedorCabecera->width());


//frame oculto sobre el que se desplaza la cabezera
baseScrollCabecera->setGeometry(5,10,contenedorCabecera->width()-30,contenedorCabecera->height()-30); 

//frame que se desplaza. Contiene los elementos de la cabezera
//FrameCabecera->setGeometry(0,0,baseScrollCabecera->width(),FrameCabecera->frameRect().height()); 

//recuadro exterior de las lineas. (Verticalmente ocupa un 50% del espacio disponible)
contenedorLineas->setGeometry(70,10+roundI((evento->size().height()-140) * 0.4),evento->size().width()-75, roundI((evento->size().height()-140)* 0.6) ); 
//Tabla Lineas
tablaLineas->setGeometry(10,20,contenedorLineas->width()-20,contenedorLineas->height()-70); 
//Botones de la zona de lineas
contenedorBotonesLineas->setGeometry(20,contenedorLineas->height()-40,370,32);
//Zona Totales
contenedorTotales->setGeometry(10,10+contenedorLineas->height()+contenedorCabecera->height(),evento->size().width()-15,110);
tablaIVA->setGeometry(250,12,tablaIVA->width()+(evento->size().width()-evento->oldSize().width()),90);
etiquetaTotalAlbaran->setGeometry(contenedorTotales->width()-140,11,120,16);
totalAlbaran->setGeometry(contenedorTotales->width()-140,30,125,25);
}
/******************* Fin Ajustes Tamaño ****************************************************/
