/****************************************************************************
** Meta object code from reading C++ file 'topindexdialog.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gui/topindex/topindexdialog.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'topindexdialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TopIndexDialog_t {
    QByteArrayData data[17];
    char stringdata0[357];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopIndexDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopIndexDialog_t qt_meta_stringdata_TopIndexDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TopIndexDialog"
QT_MOC_LITERAL(1, 15, 22), // "on_clearButton_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 24), // "on_defaultButton_clicked"
QT_MOC_LITERAL(4, 64, 22), // "on_startButton_clicked"
QT_MOC_LITERAL(5, 87, 21), // "on_exitButton_clicked"
QT_MOC_LITERAL(6, 109, 23), // "on_outputButton_clicked"
QT_MOC_LITERAL(7, 133, 29), // "on_databaseFileButton_clicked"
QT_MOC_LITERAL(8, 163, 29), // "on_fixedModFileButton_clicked"
QT_MOC_LITERAL(9, 193, 39), // "on_fixedModComboBox_currentIn..."
QT_MOC_LITERAL(10, 233, 5), // "index"
QT_MOC_LITERAL(11, 239, 23), // "on_NONECheckBox_clicked"
QT_MOC_LITERAL(12, 263, 7), // "checked"
QT_MOC_LITERAL(13, 271, 22), // "on_NMECheckBox_clicked"
QT_MOC_LITERAL(14, 294, 24), // "on_NMEACCheckBox_clicked"
QT_MOC_LITERAL(15, 319, 22), // "on_MACCheckBox_clicked"
QT_MOC_LITERAL(16, 342, 14) // "nterminalerror"

    },
    "TopIndexDialog\0on_clearButton_clicked\0"
    "\0on_defaultButton_clicked\0"
    "on_startButton_clicked\0on_exitButton_clicked\0"
    "on_outputButton_clicked\0"
    "on_databaseFileButton_clicked\0"
    "on_fixedModFileButton_clicked\0"
    "on_fixedModComboBox_currentIndexChanged\0"
    "index\0on_NONECheckBox_clicked\0checked\0"
    "on_NMECheckBox_clicked\0on_NMEACCheckBox_clicked\0"
    "on_MACCheckBox_clicked\0nterminalerror"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopIndexDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    1,   86,    2, 0x08 /* Private */,
      11,    1,   89,    2, 0x08 /* Private */,
      13,    1,   92,    2, 0x08 /* Private */,
      14,    1,   95,    2, 0x08 /* Private */,
      15,    1,   98,    2, 0x08 /* Private */,
      16,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Bool,

       0        // eod
};

void TopIndexDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TopIndexDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_clearButton_clicked(); break;
        case 1: _t->on_defaultButton_clicked(); break;
        case 2: _t->on_startButton_clicked(); break;
        case 3: _t->on_exitButton_clicked(); break;
        case 4: _t->on_outputButton_clicked(); break;
        case 5: _t->on_databaseFileButton_clicked(); break;
        case 6: _t->on_fixedModFileButton_clicked(); break;
        case 7: _t->on_fixedModComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_NONECheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_NMECheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_NMEACCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_MACCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: { bool _r = _t->nterminalerror();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TopIndexDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_TopIndexDialog.data,
    qt_meta_data_TopIndexDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TopIndexDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopIndexDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TopIndexDialog.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TopIndexDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
