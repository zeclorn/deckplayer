/****************************************************************************
** Meta object code from reading C++ file 'MpvVideoItem.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/MpvVideoItem.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MpvVideoItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12MpvVideoItemE_t {};
} // unnamed namespace

template <> constexpr inline auto MpvVideoItem::qt_create_metaobjectdata<qt_meta_tag_ZN12MpvVideoItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MpvVideoItem",
        "mediaPathChanged",
        "",
        "startPositionMsChanged",
        "requestIdChanged",
        "pausedChanged",
        "loadedChanged",
        "positionChanged",
        "durationChanged",
        "playbackFinished",
        "playbackStopped",
        "processMpvEvents",
        "togglePause",
        "seekForward",
        "seekBackward",
        "cycleSubtitles",
        "cycleAudioTracks",
        "stop",
        "mediaPath",
        "startPositionMs",
        "requestId",
        "paused",
        "loaded",
        "positionMs",
        "durationMs"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'mediaPathChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'startPositionMsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestIdChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausedChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loadedChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'durationChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playbackFinished'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playbackStopped'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'processMpvEvents'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'togglePause'
        QtMocHelpers::MethodData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'seekForward'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'seekBackward'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cycleSubtitles'
        QtMocHelpers::MethodData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cycleAudioTracks'
        QtMocHelpers::MethodData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stop'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'mediaPath'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'startPositionMs'
        QtMocHelpers::PropertyData<qint64>(19, QMetaType::LongLong, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'requestId'
        QtMocHelpers::PropertyData<int>(20, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'paused'
        QtMocHelpers::PropertyData<bool>(21, QMetaType::Bool, QMC::DefaultPropertyFlags, 3),
        // property 'loaded'
        QtMocHelpers::PropertyData<bool>(22, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'positionMs'
        QtMocHelpers::PropertyData<qint64>(23, QMetaType::LongLong, QMC::DefaultPropertyFlags, 5),
        // property 'durationMs'
        QtMocHelpers::PropertyData<qint64>(24, QMetaType::LongLong, QMC::DefaultPropertyFlags, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MpvVideoItem, qt_meta_tag_ZN12MpvVideoItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MpvVideoItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickFramebufferObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MpvVideoItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MpvVideoItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12MpvVideoItemE_t>.metaTypes,
    nullptr
} };

void MpvVideoItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MpvVideoItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mediaPathChanged(); break;
        case 1: _t->startPositionMsChanged(); break;
        case 2: _t->requestIdChanged(); break;
        case 3: _t->pausedChanged(); break;
        case 4: _t->loadedChanged(); break;
        case 5: _t->positionChanged(); break;
        case 6: _t->durationChanged(); break;
        case 7: _t->playbackFinished(); break;
        case 8: _t->playbackStopped(); break;
        case 9: _t->processMpvEvents(); break;
        case 10: _t->togglePause(); break;
        case 11: _t->seekForward(); break;
        case 12: _t->seekBackward(); break;
        case 13: _t->cycleSubtitles(); break;
        case 14: _t->cycleAudioTracks(); break;
        case 15: _t->stop(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::mediaPathChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::startPositionMsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::requestIdChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::pausedChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::loadedChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::positionChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::durationChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::playbackFinished, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (MpvVideoItem::*)()>(_a, &MpvVideoItem::playbackStopped, 8))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->mediaPath(); break;
        case 1: *reinterpret_cast<qint64*>(_v) = _t->startPositionMs(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->requestId(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->paused(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->loaded(); break;
        case 5: *reinterpret_cast<qint64*>(_v) = _t->positionMs(); break;
        case 6: *reinterpret_cast<qint64*>(_v) = _t->durationMs(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMediaPath(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setStartPositionMs(*reinterpret_cast<qint64*>(_v)); break;
        case 2: _t->setRequestId(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *MpvVideoItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MpvVideoItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MpvVideoItemE_t>.strings))
        return static_cast<void*>(this);
    return QQuickFramebufferObject::qt_metacast(_clname);
}

int MpvVideoItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickFramebufferObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MpvVideoItem::mediaPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MpvVideoItem::startPositionMsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MpvVideoItem::requestIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MpvVideoItem::pausedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MpvVideoItem::loadedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MpvVideoItem::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MpvVideoItem::durationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MpvVideoItem::playbackFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void MpvVideoItem::playbackStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
