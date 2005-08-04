#include <stdio.h>
#include "pluginadocumental.h"
#include "empresa.h"
#include "intapunts3view.h"
#include "adocumental.h"

// Inclusi� de imagenes
#include "napuntedoc.xpm"
#include "ndoc.xpm"

#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <qdockwindow.h>
#include <qmainwindow.h>
#include <qtoolbutton.h>
#include <qimage.h>
#include <qlabel.h>

void entryPoint(Bulmages01 *bges) {
    QPixmap *img = new QPixmap(napuntedoc);
    QPixmap *img1 = new QPixmap(ndoc);

    empresa *emp = &(bges->empresaactual);
    intapunts3view* intapunts = emp->intapuntsempresa();
    myplugin1 *pub = new myplugin1(emp);
    intapunts->hide();
    QHBoxLayout *layoutPlugins = intapunts->layoutPlugins;

    QToolButton *m_adocumental = new QToolButton( intapunts, "m_adocumental" );
    m_adocumental->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, m_adocumental->sizePolicy().hasHeightForWidth() ) );
    m_adocumental->setIconSet( QIconSet( *img ) );
    layoutPlugins->addWidget( m_adocumental );


    QObject::connect(m_adocumental, SIGNAL(clicked()), pub, SLOT(boton_adjuntar()));
    QToolButton *m_adocumental1 = new QToolButton( intapunts, "m_adocumental2" );
    m_adocumental1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, m_adocumental1->sizePolicy().hasHeightForWidth() ) );
    m_adocumental1->setIconSet( QIconSet( *img1 ) );
    layoutPlugins->addWidget( m_adocumental1 );
    QObject::connect(m_adocumental1, SIGNAL(clicked()), pub, SLOT(boton_nuevoasientodocumental()));
    intapunts->showMaximized();

    /// Hacemos la entrada de menu
  QAction *ArchDoc = new QAction("Archivo Documental",*img1, "&Archivo Documental", 0, bges);
  ArchDoc->setStatusTip("Archivo Documental");
  ArchDoc->setWhatsThis("Archivo Documental");
  bges->pHerramientas()->insertSeparator();
  ArchDoc->addTo(bges->pHerramientas());
  QObject::connect(ArchDoc, SIGNAL(activated()), pub, SLOT(archDoc()));    
}


