#include "bulmatpv.h"

#include <q3textedit.h>
#include <QFileDialog>
#include <QPaintDevice>
#include <QMenu>
#include <QImage>
#include <QPixmap>
#include <QToolBar>
#include <QToolButton>
#include <QMenuBar>
#include <QFile>
#include <QStatusBar>
#include <QMessageBox>
#include <QPrinter>
#include <QApplication>
#include <QTextStream>
#include <QPainter>
#include <QCloseEvent>
#include <QWhatsThis>

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"

BulmaTPV::BulmaTPV()
    : QMainWindow(0,Qt::Window)
{
    printer = new QPrinter;
    QPixmap openIcon, saveIcon, printIcon;

    QToolBar *fileTools=new QToolBar("file operations",this);

    openIcon = QPixmap(fileopen);
    QToolButton * fileOpen
	= new QToolButton(openIcon, tr("Open File"), QString::null,
			   this, SLOT(choose()), fileTools, "open file" );

    saveIcon = QPixmap(filesave);
    QToolButton * fileSave
	= new QToolButton( saveIcon, tr("Save File"), QString::null,
			   this, SLOT(save()), fileTools, "save file" );

    printIcon = QPixmap(fileprint);
    QToolButton * filePrint
	= new QToolButton( printIcon, tr("Print File"), QString::null,
			   this, SLOT(print()), fileTools, "print file" );


    (void)QWhatsThis::whatsThisButton(fileTools);

    QString fileOpenText = tr("<p><img source=\"fileopen\"> "
	         "Click this button to open a <em>new file</em>. <br>"
                 "You can also select the <b>Open<Tomeu Borras,,,/b> command "
                 "from the <b>File</b> menu.</p>");

    QWhatsThis::add(fileOpen,fileOpenText);

    Q3MimeSourceFactory::defaultFactory()->setPixmap( "fileopen", openIcon );

    QString fileSaveText = tr("<p>Click this button to save the file you "
                 "are editing. You will be prompted for a file name.\n"
                 "You can also select the <b>Save</b> command "
                 "from the <b>File</b> menu.</p>");

    QWhatsThis::add(fileSave,fileSaveText);

    QString filePrintText = tr("Click this button to print the file you "
                 "are editing.\n You can also select the Print "
                 "command from the File menu.");

    QWhatsThis::add(filePrint,filePrintText);


    QMenu *file=new QMenu(this);
    menuBar()->insertItem(tr("&File"),file);


    file->insertItem(tr("&New"),this,SLOT(newDoc()),Qt::CTRL+Qt::Key_N );

    int id;
    id = file->insertItem( openIcon, tr("&Open..."),
			   this, SLOT(choose()), Qt::CTRL+Qt::Key_O );
    file->setWhatsThis( id, fileOpenText );

    id = file->insertItem( saveIcon, tr("&Save"),
			   this, SLOT(save()), Qt::CTRL+Qt::Key_S );
    file->setWhatsThis( id, fileSaveText );

    id = file->insertItem( tr("Save &As..."), this, SLOT(saveAs()) );
    file->setWhatsThis( id, fileSaveText );

    file->insertSeparator();

    id = file->insertItem( printIcon, tr("&Print..."),
			   this, SLOT(print()), Qt::CTRL+Qt::Key_P );
    file->setWhatsThis( id, filePrintText );

    file->insertSeparator();

    file->insertItem( tr("&Close"), this, SLOT(close()), Qt::CTRL+Qt::Key_W );

    file->insertItem( tr("&Quit"), qApp, SLOT( closeAllWindows() ), Qt::CTRL+Qt::Key_Q );

    menuBar()->insertSeparator();

    QMenu *help=new QMenu(this);
    menuBar()->insertItem( tr("&Help"), help );

    help->insertItem( tr("&About"), this, SLOT(about()), Qt::Key_F1 );
    help->insertItem( tr("About &Qt"), this, SLOT(aboutQt()) );
    help->insertSeparator();
    help->insertItem( tr("What's &This"), this, SLOT(whatsThis()), Qt::SHIFT+Qt::Key_F1 );

    e=new Q3TextEdit(this,"editor");
    e->setFocus();
    setCentralWidget( e );
    statusBar()->message( tr("Ready"), 2000 );

    resize( 450, 600 );
}


BulmaTPV::~BulmaTPV()
{
    delete printer;
}



void BulmaTPV::newDoc()
{
    BulmaTPV *ed = new BulmaTPV;
    ed->setCaption(tr("Qt Example - Application"));
    ed->show();
}

void BulmaTPV::choose()
{
    QString fn=QFileDialog::getOpenFileName(QString::null,QString::null,this);
    if(!fn.isEmpty())
	load(fn);
    else
	statusBar()->message( tr("Loading aborted"), 2000 );
}


void BulmaTPV::load( const QString &fileName )
{
    QFile f( fileName );
    if ( !f.open( QIODevice::ReadOnly ) )
	return;

    QTextStream ts( &f );
    e->setText( ts.read() );
    e->setModified( FALSE );
    setCaption( fileName );
    statusBar()->message( tr("Loaded document %1").arg(fileName), 2000 );
}


void BulmaTPV::save()
{
    if ( filename.isEmpty() ) {
	saveAs();
	return;
    }

    QString text = e->text();
    QFile f( filename );
    if ( !f.open( QIODevice::WriteOnly ) ) {
	statusBar()->message( tr("Could not write to %1").arg(filename),
			      2000 );
	return;
    }

    QTextStream t( &f );
    t << text;
    f.close();

    e->setModified( FALSE );

    setCaption( filename );

    statusBar()->message( tr( "File %1 saved" ).arg( filename ), 2000 );
}


void BulmaTPV::saveAs()
{
    QString fn=QFileDialog::getSaveFileName(QString::null,QString::null,this);
    if(!fn.isEmpty()){
	filename=fn;
	save();
    }else
	statusBar()->message(tr("Saving aborted"),2000);
}


void BulmaTPV::print()
{
    // ###### Rewrite to use QSimpleRichText to print here as well
    const int Margin = 10;
    int pageNo = 1;

    if (printer->setup(this)){		// printer dialog
	statusBar()->message( tr("Printing...") );
	QPainter p;
	if(!p.begin(printer))               // paint on printer
	    return;

	p.setFont( e->font() );
	int yPos	= 0;			// y-position for each line
	QFontMetrics fm = p.fontMetrics();
	for(int i=0;i<e->lines();i++){
	    if(Margin+yPos>printer->height()-Margin){
		QString msg("Printing (page ");
		msg+=QString::number(++pageNo);
		msg+=")...";
		statusBar()->message(msg);
		printer->newPage();		// no more room on this page
		yPos=0;			// back to top of page
	    }
	    p.drawText( Margin, Margin + yPos,
			printer->width(), fm.lineSpacing(),
			Qt::ExpandTabs | Qt::DontClip,
			e->text( i ) );
	    yPos = yPos + fm.lineSpacing();
	}
	p.end();				// send job to printer
	statusBar()->message( tr("Printing completed"), 2000 );
    } else {
	statusBar()->message( tr("Printing aborted"), 2000 );
    }
}

void BulmaTPV::closeEvent( QCloseEvent* ce )
{
    if ( !e->isModified() ) {
	ce->accept();
	return;
    }

    switch( QMessageBox::information( this, tr("Qt Application Example"),
				      tr("Do you want to save the changes"
				      " to the document?"),
				      tr("Yes"), tr("No"), tr("Cancel"),
				      0, 1 ) ) {
    case 0:
	save();
	ce->accept();
	break;
    case 1:
	ce->accept();
	break;
    case 2:
    default: // just for sanity
	ce->ignore();
	break;
    }
}


void BulmaTPV::about()
{
    QMessageBox::about( this, tr("Qt Application Example"),
			tr("This example demonstrates simple use of "
			"QMainWindow,\nQMenuBar and QToolBar."));
}


void BulmaTPV::aboutQt()
{
    QMessageBox::aboutQt( this, tr("Qt Application Example") );
}
