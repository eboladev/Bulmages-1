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

#ifndef QWEBCAM_H
#define QWEBCAM_H

#include <QImage>
#include <QPixmap>
#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QDebug>

#include <cv.h>
#include <highgui.h>


class QWebCam : public QLabel
{
    Q_OBJECT

protected:
    void mouseDoubleClickEvent ( QMouseEvent * event ) ;

public:
    QWebCam ( QWidget * parent = 0 );
    ~QWebCam();
    static QImage Ipl2QImage ( const IplImage *newImage );
//  void resizeEvent ( QResizeEvent * event ) ;

public slots:
    void captureLoop();
    QImage takeShot();

private:
    QPixmap *pixa;
    QImage  qImage;
    QTimer  *timer;
    IplImage* image;
    CvCapture* capture;
};

#endif

