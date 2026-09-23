/****************************************************************************
** Meta object code from reading C++ file 'svgeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "svgeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'svgeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SVGEditor_t {
    QByteArrayData data[9];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SVGEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SVGEditor_t qt_meta_stringdata_SVGEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SVGEditor"
QT_MOC_LITERAL(1, 10, 13), // "windowResized"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10), // "onGenerate"
QT_MOC_LITERAL(4, 36, 14), // "onFileSelected"
QT_MOC_LITERAL(5, 51, 3), // "row"
QT_MOC_LITERAL(6, 55, 14), // "onChooseFolder"
QT_MOC_LITERAL(7, 70, 6), // "onSave"
QT_MOC_LITERAL(8, 77, 6) // "onDraw"

    },
    "SVGEditor\0windowResized\0\0onGenerate\0"
    "onFileSelected\0row\0onChooseFolder\0"
    "onSave\0onDraw"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SVGEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SVGEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SVGEditor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->windowResized(); break;
        case 1: _t->onGenerate(); break;
        case 2: _t->onFileSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onChooseFolder(); break;
        case 4: _t->onSave(); break;
        case 5: _t->onDraw(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SVGEditor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SVGEditor::windowResized)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SVGEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_SVGEditor.data,
    qt_meta_data_SVGEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SVGEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SVGEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SVGEditor.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int SVGEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SVGEditor::windowResized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
