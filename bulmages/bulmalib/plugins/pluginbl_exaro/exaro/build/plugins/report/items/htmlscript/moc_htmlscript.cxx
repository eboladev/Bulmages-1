/****************************************************************************
** Meta object code from reading C++ file 'htmlscript.h'
**
** Created: Fri Jul 1 00:22:26 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../plugins/report/items/htmlscript/htmlscript.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'htmlscript.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HtmlScript[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      21,   11, 0x0009510b,
      39,   31, 0x0a095003,

 // enums: name, flags, count, data
      11, 0x1,    2,   24,

 // enum data: key, value
      46, uint(HtmlScript::AutoSizeHorizontally),
      67, uint(HtmlScript::AutoSizeVertically),

       0        // eod
};

static const char qt_meta_stringdata_HtmlScript[] = {
    "HtmlScript\0SizeFlags\0sizeFlags\0QString\0"
    "script\0AutoSizeHorizontally\0"
    "AutoSizeVertically\0"
};

const QMetaObject HtmlScript::staticMetaObject = {
    { &Report::ItemInterface::staticMetaObject, qt_meta_stringdata_HtmlScript,
      qt_meta_data_HtmlScript, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HtmlScript::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HtmlScript::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HtmlScript::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HtmlScript))
        return static_cast<void*>(const_cast< HtmlScript*>(this));
    if (!strcmp(_clname, "ro.bigendian.Report.ItemInterface/1.0"))
        return static_cast< Report::ItemInterface*>(const_cast< HtmlScript*>(this));
    typedef Report::ItemInterface QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int HtmlScript::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Report::ItemInterface QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = QFlag(sizeFlags()); break;
        case 1: *reinterpret_cast< QString*>(_v) = script(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSizeFlags(QFlag(*reinterpret_cast<int*>(_v))); break;
        case 1: setHtmlScript(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE