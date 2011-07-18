/****************************************************************************
** Meta object code from reading C++ file 'reportinterface.h'
**
** Created: Fri Jul 1 00:17:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../libs/report/reportinterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reportinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Report__ReportInterface[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
      13,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      38,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   24,   24,   24, 0x0a,
      65,   24,   24,   24, 0x0a,
      88,   78,   24,   24, 0x08,

 // properties: name, type, flags
     126,  118, 0x0a095103,
     131,  118, 0x0a095103,
     138,  118, 0x0a094103,
     157,  145, 0x08094103,
     165,  145, 0x08094103,
     169,  118, 0x0a094103,
     186,  181, 0x01094103,
     202,  181, 0x01094103,
     219,  181, 0x01094103,
     244,  236, 0x41094103,
     263,  181, 0x01094103,
     279,  181, 0x01095103,
     294,  118, 0x0a094103,

       0        // eod
};

static const char qt_meta_stringdata_Report__ReportInterface[] = {
    "Report::ReportInterface\0\0beforeExec()\0"
    "afterExec()\0cancelReport()\0showReport()\0"
    "exception\0scriptException(QScriptValue)\0"
    "QString\0name\0author\0script\0QVariantMap\0"
    "queries\0uis\0printerName\0bool\0"
    "showPrintDialog\0showPrintPreview\0"
    "showSplashScreen\0QPixmap\0splashScreenPixmap\0"
    "showExitConfirm\0showAfterClose\0"
    "customTempPath\0"
};

const QMetaObject Report::ReportInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Report__ReportInterface,
      qt_meta_data_Report__ReportInterface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Report::ReportInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Report::ReportInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Report::ReportInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Report__ReportInterface))
        return static_cast<void*>(const_cast< ReportInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int Report::ReportInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: beforeExec(); break;
        case 1: afterExec(); break;
        case 2: cancelReport(); break;
        case 3: showReport(); break;
        case 4: scriptException((*reinterpret_cast< const QScriptValue(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = name(); break;
        case 1: *reinterpret_cast< QString*>(_v) = author(); break;
        case 2: *reinterpret_cast< QString*>(_v) = script(); break;
        case 3: *reinterpret_cast< QVariantMap*>(_v) = queries(); break;
        case 4: *reinterpret_cast< QVariantMap*>(_v) = uis(); break;
        case 5: *reinterpret_cast< QString*>(_v) = printerName(); break;
        case 6: *reinterpret_cast< bool*>(_v) = showPrintDialog(); break;
        case 7: *reinterpret_cast< bool*>(_v) = showPrintPreview(); break;
        case 8: *reinterpret_cast< bool*>(_v) = showSplashScreen(); break;
        case 9: *reinterpret_cast< QPixmap*>(_v) = splashScreenPixmap(); break;
        case 10: *reinterpret_cast< bool*>(_v) = showExitConfirm(); break;
        case 11: *reinterpret_cast< bool*>(_v) = showAfterClose(); break;
        case 12: *reinterpret_cast< QString*>(_v) = customTempPath(); break;
        }
        _id -= 13;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setName(*reinterpret_cast< QString*>(_v)); break;
        case 1: setAuthor(*reinterpret_cast< QString*>(_v)); break;
        case 2: setScript(*reinterpret_cast< QString*>(_v)); break;
        case 3: setQueries(*reinterpret_cast< QVariantMap*>(_v)); break;
        case 4: setUis(*reinterpret_cast< QVariantMap*>(_v)); break;
        case 5: setPrinterName(*reinterpret_cast< QString*>(_v)); break;
        case 6: setShowPrintDialog(*reinterpret_cast< bool*>(_v)); break;
        case 7: setShowPrintPreview(*reinterpret_cast< bool*>(_v)); break;
        case 8: setShowSplashScreen(*reinterpret_cast< bool*>(_v)); break;
        case 9: setSplashScreenPixmap(*reinterpret_cast< QPixmap*>(_v)); break;
        case 10: setShowExitConfirm(*reinterpret_cast< bool*>(_v)); break;
        case 11: setShowAfterClose(*reinterpret_cast< bool*>(_v)); break;
        case 12: setCustomTempPath(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 13;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 13;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Report::ReportInterface::beforeExec()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Report::ReportInterface::afterExec()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
