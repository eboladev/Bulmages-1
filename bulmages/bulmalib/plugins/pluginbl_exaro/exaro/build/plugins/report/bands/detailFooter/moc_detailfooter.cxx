/****************************************************************************
** Meta object code from reading C++ file 'detailfooter.h'
**
** Created: Fri Jul 1 00:21:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../plugins/report/bands/detailFooter/detailfooter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'detailfooter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DetailFooter[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      21,   13, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DetailFooter[] = {
    "DetailFooter\0QString\0groupField\0"
};

const QMetaObject DetailFooter::staticMetaObject = {
    { &Report::BandInterface::staticMetaObject, qt_meta_stringdata_DetailFooter,
      qt_meta_data_DetailFooter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DetailFooter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DetailFooter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DetailFooter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DetailFooter))
        return static_cast<void*>(const_cast< DetailFooter*>(this));
    if (!strcmp(_clname, "ro.bigendian.Report.ItemInterface/1.0"))
        return static_cast< Report::ItemInterface*>(const_cast< DetailFooter*>(this));
    typedef Report::BandInterface QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int DetailFooter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Report::BandInterface QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = groupField(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setGroupField(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE