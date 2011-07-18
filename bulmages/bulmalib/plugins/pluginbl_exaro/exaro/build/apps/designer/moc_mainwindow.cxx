/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Jul 1 00:22:33 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../apps/designer/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   11,   11,   11, 0x0a,
      98,   11,   11,   11, 0x0a,
     114,   11,   11,   11, 0x09,
     124,   11,   11,   11, 0x09,
     137,   11,   11,   11, 0x09,
     157,   12,   11,   11, 0x09,
     188,   11,   11,   11, 0x09,
     197,   11,   11,   11, 0x09,
     207,   11,   11,   11, 0x09,
     222,   11,   11,   11, 0x09,
     236,   11,   11,   11, 0x09,
     249,   11,   11,   11, 0x09,
     262,   11,   11,   11, 0x09,
     277,   11,   11,   11, 0x09,
     297,  290,   11,   11, 0x09,
     317,   11,   11,   11, 0x09,
     329,   11,   11,   11, 0x09,
     344,   11,   11,   11, 0x09,
     366,   11,   11,   11, 0x09,
     392,   11,   11,   11, 0x09,
     416,   11,   11,   11, 0x09,
     432,   11,   11,   11, 0x09,
     448,   11,   11,   11, 0x09,
     455,   11,   11,   11, 0x09,
     463,   11,   11,   11, 0x09,
     469,   11,   11,   11, 0x09,
     492,  475,   11,   11, 0x09,
     531,   11,   11,   11, 0x09,
     539,   11,   11,   11, 0x09,
     549,   11,   11,   11, 0x09,
     560,   11,   11,   11, 0x09,
     599,  571,   11,   11, 0x09,
     664,  627,   11,   11, 0x09,
     747,  716,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_mainWindow[] = {
    "mainWindow\0\0,\0"
    "setCurrentIndex(QModelIndex,QItemSelectionModel::SelectionFlags)\0"
    "selectLastObject()\0useOpenGL(bool)\0"
    "newPage()\0removePage()\0currentChanged(int)\0"
    "itemSelected(QObject*,QPointF)\0zoomIn()\0"
    "zoomOut()\0zoomOriginal()\0zoomWYSIWYG()\0"
    "editScript()\0saveReport()\0saveReportAs()\0"
    "openReport()\0report\0openReport(QString)\0"
    "newReport()\0openTemplate()\0"
    "openTemplate(QString)\0prepareLastTemplateMenu()\0"
    "prepareLastReportMenu()\0executeReport()\0"
    "setupDatabase()\0copy()\0paste()\0del()\0"
    "cut()\0current,previous\0"
    "objectChanged(QModelIndex,QModelIndex)\0"
    "about()\0options()\0saveItem()\0openItem()\0"
    "movedItem,movedFromPosition\0"
    "itemMoved(QObject*,QPointF)\0"
    "obj,propertyName,old_value,new_value\0"
    "propertyChanged(QObject*,QString,QVariant,QVariant)\0"
    "object,newGeometry,oldGeometry\0"
    "itemGeometryChanged(QObject*,QRectF,QRectF)\0"
};

const QMetaObject mainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_mainWindow,
      qt_meta_data_mainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainWindow))
        return static_cast<void*>(const_cast< mainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int mainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setCurrentIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< QItemSelectionModel::SelectionFlags(*)>(_a[2]))); break;
        case 1: selectLastObject(); break;
        case 2: useOpenGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: newPage(); break;
        case 4: removePage(); break;
        case 5: currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: itemSelected((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 7: zoomIn(); break;
        case 8: zoomOut(); break;
        case 9: zoomOriginal(); break;
        case 10: zoomWYSIWYG(); break;
        case 11: editScript(); break;
        case 12: saveReport(); break;
        case 13: saveReportAs(); break;
        case 14: openReport(); break;
        case 15: openReport((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: newReport(); break;
        case 17: openTemplate(); break;
        case 18: openTemplate((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: prepareLastTemplateMenu(); break;
        case 20: prepareLastReportMenu(); break;
        case 21: executeReport(); break;
        case 22: setupDatabase(); break;
        case 23: copy(); break;
        case 24: paste(); break;
        case 25: del(); break;
        case 26: cut(); break;
        case 27: objectChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 28: about(); break;
        case 29: options(); break;
        case 30: saveItem(); break;
        case 31: openItem(); break;
        case 32: itemMoved((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 33: propertyChanged((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 34: itemGeometryChanged((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2])),(*reinterpret_cast< QRectF(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void mainWindow::setCurrentIndex(const QModelIndex & _t1, QItemSelectionModel::SelectionFlags _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
