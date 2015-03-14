/****************************************************************************
** Meta object code from reading C++ file 'RobotMovementWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RobotMovementWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RobotMovementWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RobotMovementWidget_t {
    QByteArrayData data[11];
    char stringdata[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RobotMovementWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RobotMovementWidget_t qt_meta_stringdata_RobotMovementWidget = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 14),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 26),
QT_MOC_LITERAL(4, 63, 18),
QT_MOC_LITERAL(5, 82, 7),
QT_MOC_LITERAL(6, 90, 20),
QT_MOC_LITERAL(7, 111, 10),
QT_MOC_LITERAL(8, 122, 12),
QT_MOC_LITERAL(9, 135, 12),
QT_MOC_LITERAL(10, 148, 13)
    },
    "RobotMovementWidget\0hasNewPosition\0\0"
    "updateSimulateRadioButtons\0"
    "updateMovementType\0i_isSet\0"
    "updatePositionWidget\0movementUp\0"
    "movementDown\0movementLeft\0movementRight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RobotMovementWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x0a /* Public */,
       4,    1,   56,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    0,   61,    2, 0x0a /* Public */,
       9,    0,   62,    2, 0x0a /* Public */,
      10,    0,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RobotMovementWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RobotMovementWidget *_t = static_cast<RobotMovementWidget *>(_o);
        switch (_id) {
        case 0: _t->hasNewPosition(); break;
        case 1: _t->updateSimulateRadioButtons(); break;
        case 2: _t->updateMovementType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->updatePositionWidget(); break;
        case 4: _t->movementUp(); break;
        case 5: _t->movementDown(); break;
        case 6: _t->movementLeft(); break;
        case 7: _t->movementRight(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RobotMovementWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RobotMovementWidget::hasNewPosition)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject RobotMovementWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RobotMovementWidget.data,
      qt_meta_data_RobotMovementWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *RobotMovementWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RobotMovementWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RobotMovementWidget.stringdata))
        return static_cast<void*>(const_cast< RobotMovementWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int RobotMovementWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void RobotMovementWidget::hasNewPosition()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
