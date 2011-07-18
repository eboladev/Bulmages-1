/****************************************************************************
** Meta object code from reading C++ file 'designeruiwidget.h'
**
** Created: Fri Jul 1 00:22:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../apps/designer/designeruiwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'designeruiwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Report__DesignerUiWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x0a,
      39,   25,   25,   25, 0x0a,
      52,   25,   25,   25, 0x0a,
      63,   25,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Report__DesignerUiWidget[] = {
    "Report::DesignerUiWidget\0\0createItem()\0"
    "deleteItem()\0editItem()\0editName()\0"
};

const QMetaObject Report::DesignerUiWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Report__DesignerUiWidget,
      qt_meta_data_Report__DesignerUiWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Report::DesignerUiWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Report::DesignerUiWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Report::DesignerUiWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Report__DesignerUiWidget))
        return static_cast<void*>(const_cast< DesignerUiWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Report::DesignerUiWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createItem(); break;
        case 1: deleteItem(); break;
        case 2: editItem(); break;
        case 3: editName(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
