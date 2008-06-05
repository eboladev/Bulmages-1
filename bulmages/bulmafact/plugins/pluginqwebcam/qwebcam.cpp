/***************************************************************************
 *   Copyright (C) 2008 by Stephane List                                   *
 *   slist@lilotux.net                                                     *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qwebcam.h"

QWebCam::QWebCam ( QWidget * parent ) : QLabel ( parent )
{
    pixa   = NULL;
    image  = 0;
    capture = 0;
    timer  = new QTimer ( this );

    capture = cvCaptureFromCAM ( 0 );
    if ( !capture ) {
        qDebug() << "cannot get webcam...";
        return;
    }
    cvGrabFrame ( capture );                 //recuperer une image, pour voir les dimension et resizer
    image = cvRetrieveFrame ( capture );
    resize ( image->width, image->height );

    timer->start ( 10 );

    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( captureLoop() ) );
}

QWebCam::~QWebCam()
{
    cvReleaseCapture ( &capture );
}

QImage QWebCam::Ipl2QImage ( const IplImage *newImage ) //fct recuperer sur le net, converti un ldImage en QImage
{
    QImage qtemp;
    if ( newImage && cvGetSize ( newImage ).width > 0 ) {
        int x;
        int y;
        char* data = newImage->imageData;

        qtemp = QImage ( newImage->width, newImage->height, QImage::Format_RGB32 );
        for ( y = 0; y < newImage->height; y++, data += newImage->widthStep )
            for ( x = 0; x < newImage->width; x++ ) {
                uint *p = ( uint* ) qtemp.scanLine ( y ) + x;
                *p = qRgb ( data[x * newImage->nChannels+2],
                            data[x * newImage->nChannels+1], data[x * newImage->nChannels] );
            }
    }
    return qtemp;
}

/*
void QWebCam::resizeEvent ( QResizeEvent * event )
{
}
*/

void QWebCam::captureLoop() // appeler par le timer, recupere l'image courante de la webcam et convertit
{
    if ( !capture ) return;
    cvGrabFrame ( capture );
    image = cvRetrieveFrame ( capture );
    if ( image ) {
        cvWaitKey ( 10 );
        if ( pixa == NULL )
            pixa = new QPixmap();
        qImage = Ipl2QImage ( image );
        *pixa = QPixmap::fromImage ( qImage.scaled ( width(), height() ), Qt::AutoColor );
        setPixmap ( *pixa );
    }
}

QImage QWebCam::takeShot()
{
    return qImage;
}
void QWebCam::mouseDoubleClickEvent ( QMouseEvent * event )
{
    if ( isFullScreen() )
        showNormal();
    else
        showFullScreen();
}
