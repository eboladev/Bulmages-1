/****************************************************************************
** Image collection for project 'estadisticas.lib'.
**
** Generated from reading image files: 
**      print.xpm
**
** Created: dom nov 16 21:08:36 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include <qimage.h>
#include <qdict.h>
#include <qmime.h>
#include <qdragobject.h>

// print.xpm
static const unsigned char image_0_data[] = {
    0x00,0x00,0x01,0x00,0x78,0x9c,0x85,0x8e,0x5b,0x0a,0x00,0x20,0x08,0x04,
    0x7d,0xdf,0xff,0xc8,0x85,0x69,0x0a,0x45,0xcd,0x7e,0xad,0x83,0x22,0xc0,
    0x05,0x4c,0xa2,0x52,0xb0,0xbb,0x4b,0xfa,0xe9,0x75,0xa0,0x34,0xcf,0xd4,
    0x6d,0xaf,0x88,0xa2,0x99,0x18,0x34,0x98,0xd3,0xb3,0x99,0x8a,0x6b,0x6d,
    0x0b,0x27,0xed,0xf7,0xfa,0xff,0xc5,0x00,0x9c,0x00,0x01,0x4c
};

static const QRgb image_0_ctable[] = {
    0xc6c6c6,0xff000000,0xffffffff,0xff808080,0xff808000,0xffc0c0c0,0xff800000
};

static struct EmbedImage {
    int width, height, depth;
    const unsigned char *data;
    ulong compressed;
    int numColors;
    const QRgb *colorTable;
    bool alpha;
    const char *name;
} embed_image_vec[] = {
    { 16, 16, 8, (const unsigned char*)image_0_data, 68, 7, image_0_ctable, TRUE, "print.xpm" },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static QImage uic_findImage( const QString& name )
{
    for ( int i=0; embed_image_vec[i].data; i++ ) {
	if ( QString::fromUtf8(embed_image_vec[i].name) == name ) {
	    QByteArray baunzip;
	    baunzip = qUncompress( embed_image_vec[i].data, 
		embed_image_vec[i].compressed );
	    QImage img((uchar*)baunzip.data(),
			embed_image_vec[i].width,
			embed_image_vec[i].height,
			embed_image_vec[i].depth,
			(QRgb*)embed_image_vec[i].colorTable,
			embed_image_vec[i].numColors,
			QImage::BigEndian
		);
	    img = img.copy();
	    if ( embed_image_vec[i].alpha )
		img.setAlphaBuffer(TRUE);
	    return img;
        }
    }
    return QImage();
}

class MimeSourceFactory_estadisticas_lib : public QMimeSourceFactory
{
public:
    MimeSourceFactory_estadisticas_lib() {}
    ~MimeSourceFactory_estadisticas_lib() {}
    const QMimeSource* data( const QString& abs_name ) const {
	const QMimeSource* d = QMimeSourceFactory::data( abs_name );
	if ( d || abs_name.isNull() ) return d;
	QImage img = uic_findImage( abs_name );
	if ( !img.isNull() )
	    ((QMimeSourceFactory*)this)->setImage( abs_name, img );
	return QMimeSourceFactory::data( abs_name );
    };
};

static QMimeSourceFactory* factory = 0;

void qInitImages_estadisticas_lib()
{
    if ( !factory ) {
	factory = new MimeSourceFactory_estadisticas_lib;
	QMimeSourceFactory::defaultFactory()->addFactory( factory );
    }
}

void qCleanupImages_estadisticas_lib()
{
    if ( factory ) {
	QMimeSourceFactory::defaultFactory()->removeFactory( factory );
	delete factory;
	factory = 0;
    }
}

class StaticInitImages_estadisticas_lib
{
public:
    StaticInitImages_estadisticas_lib() { qInitImages_estadisticas_lib(); }
#if defined(Q_OS_SCO) || defined(Q_OS_UNIXWARE)
    ~StaticInitImages_estadisticas_lib() { }
#else
    ~StaticInitImages_estadisticas_lib() { qCleanupImages_estadisticas_lib(); }
#endif
};

static StaticInitImages_estadisticas_lib staticImages;
