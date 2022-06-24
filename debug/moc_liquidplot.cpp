/****************************************************************************
** Meta object code from reading C++ file 'liquidplot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LiquidPlot/include/liquidplot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'liquidplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LiquidPlot_t {
    QByteArrayData data[16];
    char stringdata0[164];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LiquidPlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LiquidPlot_t qt_meta_stringdata_LiquidPlot = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LiquidPlot"
QT_MOC_LITERAL(1, 11, 10), // "SetPercent"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "nPercent"
QT_MOC_LITERAL(4, 32, 10), // "GetPercent"
QT_MOC_LITERAL(5, 43, 11), // "SetPlotType"
QT_MOC_LITERAL(6, 55, 9), // "EPlotType"
QT_MOC_LITERAL(7, 65, 9), // "ePlotType"
QT_MOC_LITERAL(8, 75, 8), // "SetSpeed"
QT_MOC_LITERAL(9, 84, 6), // "nSpeed"
QT_MOC_LITERAL(10, 91, 8), // "GetSpeed"
QT_MOC_LITERAL(11, 100, 13), // "SetWaterColor"
QT_MOC_LITERAL(12, 114, 10), // "colorWater"
QT_MOC_LITERAL(13, 125, 13), // "GetWaterColor"
QT_MOC_LITERAL(14, 139, 11), // "EnableSpace"
QT_MOC_LITERAL(15, 151, 12) // "bEnableSpace"

    },
    "LiquidPlot\0SetPercent\0\0nPercent\0"
    "GetPercent\0SetPlotType\0EPlotType\0"
    "ePlotType\0SetSpeed\0nSpeed\0GetSpeed\0"
    "SetWaterColor\0colorWater\0GetWaterColor\0"
    "EnableSpace\0bEnableSpace"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LiquidPlot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    0,   57,    2, 0x0a /* Public */,
       5,    1,   58,    2, 0x0a /* Public */,
       8,    1,   61,    2, 0x0a /* Public */,
      10,    0,   64,    2, 0x0a /* Public */,
      11,    1,   65,    2, 0x0a /* Public */,
      13,    0,   68,    2, 0x0a /* Public */,
      14,    1,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QColor,   12,
    QMetaType::QColor,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void LiquidPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LiquidPlot *_t = static_cast<LiquidPlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetPercent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: { int _r = _t->GetPercent();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->SetPlotType((*reinterpret_cast< EPlotType(*)>(_a[1]))); break;
        case 3: _t->SetSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: { int _r = _t->GetSpeed();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->SetWaterColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 6: { QColor _r = _t->GetWaterColor();
            if (_a[0]) *reinterpret_cast< QColor*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->EnableSpace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject LiquidPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LiquidPlot.data,
      qt_meta_data_LiquidPlot,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LiquidPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LiquidPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LiquidPlot.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LiquidPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
