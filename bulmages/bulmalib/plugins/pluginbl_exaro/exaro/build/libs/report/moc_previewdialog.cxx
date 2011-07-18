/****************************************************************************
** Meta object code from reading C++ file 'previewdialog.h'
**
** Created: Fri Jul 1 00:17:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../libs/report/previewdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'previewdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Report__PreviewDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   22,   22,   22, 0x0a,
      55,   22,   22,   22, 0x0a,
      63,   22,   22,   22, 0x0a,
      75,   22,   22,   22, 0x0a,
      86,   22,   22,   22, 0x0a,
     101,   22,   22,   22, 0x0a,
     112,   22,   22,   22, 0x0a,
     132,   22,   22,   22, 0x0a,
     156,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Report__PreviewDialog[] = {
    "Report::PreviewDialog\0\0textNotFound()\0"
    "clearSelection()\0print()\0firstPage()\0"
    "lastPage()\0readPrevious()\0readNext()\0"
    "searchNext(QString)\0searchPrevious(QString)\0"
    "exportDocument()\0"
};

const QMetaObject Report::PreviewDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Report__PreviewDialog,
      qt_meta_data_Report__PreviewDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Report::PreviewDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Report::PreviewDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Report::PreviewDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Report__PreviewDialog))
        return static_cast<void*>(const_cast< PreviewDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Report::PreviewDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: textNotFound(); break;
        case 1: clearSelection(); break;
        case 2: print(); break;
        case 3: firstPage(); break;
        case 4: lastPage(); break;
        case 5: readPrevious(); break;
        case 6: readNext(); break;
        case 7: searchNext((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: searchPrevious((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: exportDocument(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Report::PreviewDialog::textNotFound()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
