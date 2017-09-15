/****************************************************************************
** Meta object code from reading C++ file 'ANTChannel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ANT/ANTChannel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ANTChannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ANTChannel_t {
    QByteArrayData data[23];
    char stringdata[266];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ANTChannel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ANTChannel_t qt_meta_stringdata_ANTChannel = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 11),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 6),
QT_MOC_LITERAL(4, 31, 13),
QT_MOC_LITERAL(5, 45, 9),
QT_MOC_LITERAL(6, 55, 8),
QT_MOC_LITERAL(7, 64, 7),
QT_MOC_LITERAL(8, 72, 8),
QT_MOC_LITERAL(9, 81, 8),
QT_MOC_LITERAL(10, 90, 9),
QT_MOC_LITERAL(11, 100, 13),
QT_MOC_LITERAL(12, 114, 14),
QT_MOC_LITERAL(13, 129, 19),
QT_MOC_LITERAL(14, 149, 18),
QT_MOC_LITERAL(15, 168, 6),
QT_MOC_LITERAL(16, 175, 8),
QT_MOC_LITERAL(17, 184, 15),
QT_MOC_LITERAL(18, 200, 7),
QT_MOC_LITERAL(19, 208, 14),
QT_MOC_LITERAL(20, 223, 16),
QT_MOC_LITERAL(21, 240, 16),
QT_MOC_LITERAL(22, 257, 7)
    },
    "ANTChannel\0channelInfo\0\0number\0"
    "device_number\0device_id\0dropInfo\0"
    "dropped\0received\0lostInfo\0staleInfo\0"
    "searchTimeout\0searchComplete\0"
    "broadcastTimerStart\0broadcastTimerStop\0"
    "rrData\0uint16_t\0measurementTime\0uint8_t\0"
    "heartrateBeats\0instantHeartrate\0"
    "antRemoteControl\0command\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ANTChannel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06,
       6,    3,   71,    2, 0x06,
       9,    1,   78,    2, 0x06,
      10,    1,   81,    2, 0x06,
      11,    1,   84,    2, 0x06,
      12,    1,   87,    2, 0x06,
      13,    1,   90,    2, 0x06,
      14,    1,   93,    2, 0x06,
      15,    3,   96,    2, 0x06,
      21,    1,  103,    2, 0x06,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    7,    8,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 18, 0x80000000 | 18,   17,   19,   20,
    QMetaType::Void, 0x80000000 | 16,   22,

       0        // eod
};

void ANTChannel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ANTChannel *_t = static_cast<ANTChannel *>(_o);
        switch (_id) {
        case 0: _t->channelInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->dropInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->lostInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->staleInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->searchTimeout((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->searchComplete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->broadcastTimerStart((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->broadcastTimerStop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->rrData((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< uint8_t(*)>(_a[3]))); break;
        case 9: _t->antRemoteControl((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ANTChannel::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::channelInfo)) {
                *result = 0;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::dropInfo)) {
                *result = 1;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::lostInfo)) {
                *result = 2;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::staleInfo)) {
                *result = 3;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::searchTimeout)) {
                *result = 4;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::searchComplete)) {
                *result = 5;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::broadcastTimerStart)) {
                *result = 6;
            }
        }
        {
            typedef void (ANTChannel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::broadcastTimerStop)) {
                *result = 7;
            }
        }
        {
            typedef void (ANTChannel::*_t)(uint16_t , uint8_t , uint8_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::rrData)) {
                *result = 8;
            }
        }
        {
            typedef void (ANTChannel::*_t)(uint16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ANTChannel::antRemoteControl)) {
                *result = 9;
            }
        }
    }
}

const QMetaObject ANTChannel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ANTChannel.data,
      qt_meta_data_ANTChannel,  qt_static_metacall, 0, 0}
};


const QMetaObject *ANTChannel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ANTChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ANTChannel.stringdata))
        return static_cast<void*>(const_cast< ANTChannel*>(this));
    return QObject::qt_metacast(_clname);
}

int ANTChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ANTChannel::channelInfo(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ANTChannel::dropInfo(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ANTChannel::lostInfo(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ANTChannel::staleInfo(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ANTChannel::searchTimeout(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ANTChannel::searchComplete(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ANTChannel::broadcastTimerStart(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ANTChannel::broadcastTimerStop(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ANTChannel::rrData(uint16_t _t1, uint8_t _t2, uint8_t _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ANTChannel::antRemoteControl(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
