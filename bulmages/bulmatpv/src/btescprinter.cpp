/***************************************************************************
 *   Copyright (C) 2008 by Oriol Farras Sanz   *
 *   wereoffs@gmail.com   *
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

#include "btescprinter.h"
#include <QImage>
#include <QFile>
#include <QString>
#include "btcp850eurotextcodec.h"
#include <QChar>
#include <math.h>

#define TWOPI 6.283185307179586476925287

BtEscPrinter::BtEscPrinter()
{
    device = QString ( "/dev/lp0" );
}

BtEscPrinter::BtEscPrinter ( QString device )
{
    this->device = device;
}

void BtEscPrinter::print()
{
    QFile file ( device );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        //ERROR
    } // end if

    file.write ( buffer );
    file.close();
}

void BtEscPrinter::printAndClearBuffer()
{
    print();
    clearBuffer();
}

void BtEscPrinter::clearBuffer()
{
    buffer.clear();
}

void BtEscPrinter::initializePrinter()
{
    const char header[2] = {0x1B, 0x40};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::printText ( QString text )
{
    QTextCodec *codec = new BtCP850EuroTextCodec ( 0 );
    if ( codec == NULL ) {
        //ERROR
        return;
    }
    buffer.append ( codec->fromUnicode ( text ) );
}

void BtEscPrinter::printImage ( QString path )
{
    QImage img = QImage ( path );

    if ( !img.isNull() ) {
        if ( img.depth() != 1 ) {
            img = img.convertToFormat ( QImage::Format_Mono, Qt::MonoOnly );
        }
        if ( !img.isNull() ) {
            unsigned  short bytesX = img.bytesPerLine();
            unsigned  short dotsY = img.height();
            if ( img.width() / 8 > 65535 || img.height() > 2303 ) {
                //ERROR IMAGE too large
            } else {
                int numBytes = img.numBytes();
                char* data = ( char* ) img.bits();

                char XL = ( char ) ( bytesX );
                char XH = ( char ) ( bytesX >> 8 );
                char YL = ( char ) ( dotsY );
                char YH = ( char ) ( dotsY >> 8 );

                const char header[8] = {0x1D, 0x76, 0x30, 0x00, XL, XH, YL, YH};

                buffer.append ( QByteArray::fromRawData ( header, 8 ) );
                buffer.append ( QByteArray::fromRawData ( data, numBytes ) );
            }
        } else {
            //ERROR CONVERT IMAGE
        }
    } else {
        //ERROR LOADING LOGO
    }
}


//CHARACTER ESC CODES
void BtEscPrinter::setCharacterSpacing ( unsigned char n )
{
    const char header[3] = {0x1b, 0x20, ( char ) n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setCharacterCodeTable ( characterCodeTable n )
{
    const char header[3] = {0x1B, 0x74, ( char ) n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setCharacterPrintMode ( char mask )
{
    const char header[3] = {0x1B, 0x21, mask};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setUnderlineMode ( bool mode )
{
    char n = mode ? 1 : 0;
    const char header[3] = {0x1B, 0x2D, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setCharacterSize ( char mask )
{
    const char header[3] = {0x1D, 0x21, mask};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setSmoothing ( bool mode )
{
    char n = mode ? 1 : 0;
    const char header[3] = {0x1D, 0x62, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setDoubleStrike ( bool mode )
{
    char n = mode ? 1 : 0;
    const char header[3] = {0x1B, 0x47, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::turnUpsideDown ( bool mode )
{
    char n = mode ? 1 : 0;
    const char header[3] = {0x1B, 0x7B, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::turn90CWRotation ( bool onoff, bool extraSpacing )
{
    char n;
    onoff == 1 ? n = 1 + ( char ) extraSpacing : n = 0;
    const char header[3] = {0x1B, 0x56, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::turnWhiteBlack ( bool mode )
{
    char n = mode ? 1 : 0;
    const char header[3] = {0x1D, 0x42, n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setColor ( printColor color )
{
    const char header[7] = {0x1D, 0x28, 0x4E, 0x02, 0x00, 48, ( char ) color};
    buffer.append ( QByteArray::fromRawData ( header, 7 ) );
}

//PRINT POSITION COMMANDS
void BtEscPrinter::horizontalTab()
{
    buffer.append ( 0x09 );
}

void BtEscPrinter::setHorizontalTabPos ( int numtabs, char* tabspos )
{
    if ( 0 >= numtabs > 32 )
        return;
    const char header[2] = {0x1B, 0x44};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
    buffer.append ( QByteArray::fromRawData ( tabspos, numtabs ) );
    buffer.append ( ( char ) 0x00 );
}

void BtEscPrinter::setLeftMargin ( unsigned short margin )
{
    char nL = ( char ) ( margin );
    char nH = ( char ) ( margin >> 8 );
    const char header[4] = { 0x1D, 0x4C, nL, nH };
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

void BtEscPrinter::setPrintingAreaWidth ( unsigned short width ) //width default = 512
{
    char nL = ( char ) ( width );
    char nH = ( char ) ( width >> 8 );
    const char header[4] = { 0x1D, 0x57, nL, nH };
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

void BtEscPrinter::setJustification ( justification j )
{
    const char header[3] = {0x1B, 0x61, ( char ) j};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setHAbsolutePos ( unsigned short pos )
{
    char nL = ( char ) pos;
    char nH = ( char ) ( pos >> 8 );

    const char header[4] = {0x1B, 0x24, nL, nH};
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

void BtEscPrinter::setHRelativePos ( unsigned short pos )
{
    char nL = ( char ) pos;
    char nH = ( char ) ( pos >> 8 );

    const char header[4] = {0x1B, 0x5C, nL, nH};
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

//Barcode commands
void BtEscPrinter::printBarCode ( barcodeSystem m, int n, char* codeData )
{
    int error = 0;
    switch ( m ) {
    case upca:
        if ( ( 11 <= n <= 12 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 48 <= codeData[i] <= 57 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case upce:
        if ( ( 11 <= n <= 12 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 48 <= codeData[i] <= 57 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case jan13:
        if ( ( 12 <= n <= 13 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 48 <= codeData[i] <= 57 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case jan8:
        if ( ( 7 <= n <= 8 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 48 <= codeData[i] <= 57 ) ) {
                    error = 2; //BADBARCODE
                }
            }
        } else {
            error = 1;//BADBARCODE
        }
        break;

    case code39:
        if ( ( 1 <= n <= 255 ) ) {
            for ( int i = 0; i < n; i++ ) {
                int d = codeData[i];
                if ( ! ( 48 <= d <= 57 ) && ! ( 65 <= d <= 90 ) && d != 32 && d != 36 && d != 37 && d != 43 && d != 45 && d != 46 && d != 47 && d != 42 )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case itf:
        if ( ( 1 <= n <= 255 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 48 <= codeData[i] <= 57 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case codabar:
        if ( ( 1 <= n <= 255 ) ) {
            for ( int i = 0; i < n; i++ ) {
                int d = codeData[i];
                if ( ! ( 48 <= d <= 57 ) && ! ( 65 <= d <= 68 ) && d != 36 && d != 43 && d != 45 && d != 46 && d != 47 && d != 58 )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case code93:
        if ( ( 1 <= n <= 255 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 0 <= codeData[i] <= 127 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;

    case code128:
        if ( ( 2 <= n <= 255 ) ) {
            for ( int i = 0; i < n; i++ ) {
                if ( ! ( 0 <= codeData[i] <= 127 ) )
                    error = 2; //BADBARCODE
            }
        } else
            error = 1;//BADBARCODE
        break;
    default:
        error = 3;
        break;//return;
    }
    if ( error ) {
        switch ( error ) {
        case 1:
            //NUMERO DE CODIGOS INCORRECTO
            break;
        case 2:
            //CARACTER INCORRECTO
            break;
        case 3:
            //TIPO DE CODIGO DE BARRAS NO SOPORTADO
            break;
        }

    } else {

        const char header[3] = {0x1D, 0x6B, ( char ) m};
        buffer.append ( QByteArray::fromRawData ( header, 3 ) );
        buffer.append ( QByteArray::fromRawData ( codeData, n ) );
        buffer.append ( ( char ) 0x00 );
    }
}

void BtEscPrinter::setBarCodeHeight ( int n )
{
    const char header[3] = {0x1D, 0x68, ( char ) n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setBarCodeWidth ( int n ) // 1<=n<=6 on tm-t88IV
{
    if ( ! ( 2 <= n <= 6 ) )
        return; //ERROR

    const char header[3] = {0x1D, 0x77, ( char ) n};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setBarcodeFormat ( int width, int height, barCodeTextPos pos, printerFont font ) // 1<=width<=6 on tm-t88IV
{
    setBarCodeHeight ( height );
    setBarCodeWidth ( width );

    const char header[3] = {0x1D, 0x48, ( char ) pos};
    const char header2[3] = {0x1D, 0x66, ( char ) font};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
    buffer.append ( QByteArray::fromRawData ( header2, 3 ) );
}


//Page mode
void BtEscPrinter::selectPageMode()
{
    const char header[2] = {0x1B, 0x4C};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::setPrintArea ( unsigned short x, unsigned short y, unsigned short dx, unsigned short dy )
{
    char xL = ( char ) x;
    char xH = ( char ) ( x >> 8 );
    char yL = ( char ) y;
    char yH = ( char ) ( y >> 8 );
    char dxL = ( char ) dx;
    char dxH = ( char ) ( dx >> 8 );
    char dyL = ( char ) dy;
    char dyH = ( char ) ( dy >> 8 );

    const char header[10] = {0x1B, 0x57, xL, xH, yL, yH, dxL, dxH, dyL, dyH};
    buffer.append ( QByteArray::fromRawData ( header, 10 ) );
}

void BtEscPrinter::setPrintDirection ( printDirection dir )
{
    const char header[3] = {0x1B, 0x54, ( char ) dir};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::setVAbsolutePos ( unsigned short pos )
{
    char nL = ( char ) pos;
    char nH = ( char ) ( pos >> 8 );

    const char header[4] = {0x1D, 0x24, nL, nH};
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

void BtEscPrinter::setVRelativePos ( unsigned short pos )
{
    char nL = ( char ) pos;
    char nH = ( char ) ( pos >> 8 );

    const char header[4] = {0x1D, 0x5C, nL, nH};
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

void BtEscPrinter::cancelPrintData()
{
    buffer.append ( 0x18 );
}

//MACRO FUNCTION COMMANDS
void BtEscPrinter::startEndMacro()
{
    const char header[2] = {0x1D, 0x3A};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::executeMacro ( int rep, int timePeriod, bool mode )
{
    const char header[5] = {0x1D, 0x5E, ( char ) rep, ( char ) timePeriod, ( char ) mode };
    buffer.append ( QByteArray::fromRawData ( header, 5 ) );
}

//MECHANISM CONTROL COMMANDS
void BtEscPrinter::turnUnidirPrintMode ( bool onoff ) //No on TM-t88II TM-L60II
{
    const char header[3] = {0x1B, 0x55, ( char ) onoff};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::returnHome() //No on TM-t88II TM-L60II
{
    const char header[2] = {0x1B, 0x3C};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::cutPaper ( bool partialcut ) //ONLY partial cut on tm-t88II
{
    const char header[3] = {0x1D, 0x56, ( char ) partialcut};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::cutPaperAndFeed ( bool partialcut, int feed ) //ONLY partial cut on tm-t88II
{
    char m = partialcut ? 66 : 65;
    const char header[4] = {0x1D, 0x56, m, ( char ) feed};
    buffer.append ( QByteArray::fromRawData ( header, 4 ) );
}

//Print commands
void BtEscPrinter::printAndLineFeed()
{
    buffer.append ( 0x0A );
}

void BtEscPrinter::printAndReturnStandardMode()
{
    buffer.append ( 0x0C );
}

void BtEscPrinter::printAndCarriageReturn()
{
    buffer.append ( 0x0D );
}

void BtEscPrinter::printDataPageMode()
{
    const char header[2] = {0x1B, 0x0C};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::printAndFeedPaper ( int feed )
{
    const char header[3] = {0x1B, 0x4A, ( char ) feed};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::printAndFeedLines ( int lines )
{
    const char header[3] = {0x1B, 0x64, ( char ) lines};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

void BtEscPrinter::printAndReverseFeed ( int feed ) //Not compatible TM-t88II III
{
    const char header[3] = {0x1B, 0x4B, ( char ) feed};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

//Line spacing commands
void BtEscPrinter::selectDefaultLineSpacing()
{
    const char header[2] = {0x1B, 0x32};
    buffer.append ( QByteArray::fromRawData ( header, 2 ) );
}

void BtEscPrinter::setLineSpacing ( int lspacing )
{
    const char header[3] = {0x1B, 0x33, ( char ) lspacing};
    buffer.append ( QByteArray::fromRawData ( header, 3 ) );
}

//2D CODES
void BtEscPrinter::printQRCode ( qrModel model, int dotSize, qrErrorCorrection error, unsigned short numData, char* data ) //1<=dotSize<=16
{
    numData += 3;
    char pl = ( char ) numData;
    char ph = ( char ) ( numData >> 8 );

    const char selectModel[9] = {0x1D, 0x28, 0x6B, 0x04, 0x00, 0x31, 0x41, ( char ) model, 0x00};
    const char setSize[8] = {0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x43, ( char ) dotSize};
    const char errorCorrection[8] = {0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x45, ( char ) error};


    const char storeData[8] = {0x1D, 0x28, 0x6B, pl, ph, 0x31, 0x50, 0x30};
    const char print[8] = {0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x51, 0x30};

    buffer.append ( QByteArray::fromRawData ( storeData, 8 ) );
    buffer.append ( QByteArray::fromRawData ( data, numData - 3 ) );
    buffer.append ( QByteArray::fromRawData ( selectModel, 9 ) );
    buffer.append ( QByteArray::fromRawData ( setSize, 8 ) );
    buffer.append ( QByteArray::fromRawData ( errorCorrection, 8 ) );

    buffer.append ( QByteArray::fromRawData ( print, 8 ) );
}

void BtEscPrinter::test()
{
    initializePrinter();
    setCharacterCodeTable ( page19 );
    setJustification ( center );

    printImage ( "/usr/local/share/bulmages/images/splashbulmafact.jpg" );
    printText ( "\n" );
    setJustification ( left );

    setCharacterPrintMode ( CHARACTER_FONTB_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH | UNDERLINE_MODE );
    printText ( "CHARACTER_FONTB_SELECTED|EMPHASIZED_MODE|DOUBLE_HEIGHT|DOUBLE_WIDTH|UNDERLINE_MODE\n" );
    setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    printText ( "CHARACTER_FONTB_SELECTED\n" );

    setUnderlineMode ( 1 );
    printText ( "setUnderlineMode (1)\n" );
    setUnderlineMode ( 0 );
    printText ( "setUnderlineMode (0)\n" );

    setCharacterSize ( CHAR_WIDTH_3 | CHAR_HEIGHT_3 );
    printText ( "setCharacterSize ( CHAR_WIDTH_3|CHAR_HEIGHT_3 );\n" );
    setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );
    printText ( "setCharacterSize ( CHAR_WIDTH_1|CHAR_HEIGHT_1 );\n" );

    setSmoothing ( 0 );
    printText ( "setSmoothing ( 0 );\n" );

    setSmoothing ( 1 );
    printText ( "setSmoothing ( 1 );\n" );

    setDoubleStrike ( 1 );
    printText ( "setDoubleStrike ( 1 )\n" );
    setDoubleStrike ( 0 );
    printText ( "setDoubleStrike ( 0 )\n" );

    turnUpsideDown ( 1 );
    printText ( "turnUpsideDown ( 1 )\n" );
    turnUpsideDown ( 0 );
    printText ( "turnUpsideDown ( 0 )\n" );

    turn90CWRotation ( TRUE, TRUE );
    printText ( "turn90CWRotation ( 1 )\n" );
    turn90CWRotation ( FALSE, FALSE );
    printText ( "turn90CWRotation ( 0 )\n" );

    turnWhiteBlack ( 1 );
    printText ( "turnWhiteBlack ( 1 )\n" );
    turnWhiteBlack ( 0 );
    printText ( "turnWhiteBlack ( 0 )\n" );

    setColor ( red );
    printText ( "setColor ( red )\n" );
    setColor ( black );
    printText ( "setColor ( black )\n" );

    //MECHANISM CONTROL COMMANDS
    turnUnidirPrintMode (  TRUE ); //No on TM-t88II TM-L60II
    printText ( "turnUnidirPrintMode ( TRUE )\n" );
    turnUnidirPrintMode (  FALSE ); //No on TM-t88II TM-L60II
    printText ( "turnUnidirPrintMode ( FALSE )\n" );

    //PRINT POSITION COMMANDS
    char tabspos[2] = {8, 30};
    setHorizontalTabPos ( 2, tabspos );
    horizontalTab();
    printText ( "Horizpos=8" );
    horizontalTab();
    printText ( "Horizpos = 30\n" );

    setLeftMargin ( 50 );
    printText ( "setLeftMargin ( 50 )\n" );
    setLeftMargin ( 0 );
    printText ( "setLeftMargin ( 0 )\n" );

    setPrintingAreaWidth ( 100 ); //width default = 512
    printText ( "setPrintingAreaWidth ( 10 )\n" );
    setPrintingAreaWidth ( 512 ); //width default = 512
    printText ( "setPrintingAreaWidth ( 512 )\n" );

    setJustification ( right );
    printText ( "setJustification ( right )\n" );
    setJustification ( center );
    printText ( "setJustification ( center )\n" );
    setJustification ( left );
    printText ( "setJustification ( left )\n" );

    //BARCODE
    setJustification ( center );
    setBarcodeFormat ( 2, 50, both, fontA );
    char barcode[11] = {'A', 'Z', 'I', 'P', 'P', 'L', 'M', 'Z', 'T', 'W', '1'};
    printBarCode ( code39, 11, barcode );

    //QRCODE
    QByteArray tmp;
    tmp = QString ( "Esto es un QRCODE. Un es pregunta quantes dades es poden arribar a posar en un qrcode" ).toAscii();
    char* qrdata = tmp.data();
    unsigned short numData = tmp.size();
    printQRCode ( qrModel2, 8, qrlevelL, numData, qrdata );

    //pagemode

    selectPageMode();
    setPrintArea ( 100, 100, 350, 700 );

    float x, y;
    int reps = 100;
    int radius = 150;
    int dx = 150;
    int dy = 200;
    for ( float i = 0; i < reps; i++ ) {
        float angle = TWOPI * ( ( float ) ( i / reps ) );
        unsigned short x = ( unsigned short ) ( radius * cosf ( angle ) + dx );
        unsigned short y = 2 * ( unsigned short ) ( radius * sinf ( angle ) + dy );
        setVAbsolutePos ( y );
        setHAbsolutePos ( x );
        printText ( "€" );

    }

    printAndReturnStandardMode();

    selectPageMode();
    setPrintArea ( 0, 0, 500, 600 );
    reps = 200;
    int p = 400;
    int r = 100;
    for ( float i = 0; i < reps; i++ ) {
        float angle = TWOPI * ( ( float ) ( i / reps ) );
        unsigned short x = p * ( i / ( float ) ( reps - 1 ) );
        unsigned short y = 2 * ( unsigned short ) ( r * sinf ( angle ) + dy );
        setVAbsolutePos ( y );
        setHAbsolutePos ( x );
        printText ( "." );
    }
    printDataPageMode();

    for ( float i = 0; i < reps; i++ ) {
        float angle = TWOPI * ( ( float ) ( i / reps ) );
        unsigned short x = p * ( i / ( float ) ( reps - 1 ) );
        unsigned short y = 2 * ( unsigned short ) ( r * cosf ( angle ) + dy );
        setVAbsolutePos ( y );
        setHAbsolutePos ( x );
        printText ( "." );
    }
    printAndReturnStandardMode();

    cutPaperAndFeed ( TRUE, 10 );
    print();
}

