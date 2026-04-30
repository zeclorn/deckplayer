/****************************************************************************
** Meta object code from reading C++ file 'AppState.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/AppState.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppState.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8AppStateE_t {};
} // unnamed namespace

template <> constexpr inline auto AppState::qt_create_metaobjectdata<qt_meta_tag_ZN8AppStateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppState",
        "playerVisibleChanged",
        "",
        "resumePromptVisibleChanged",
        "currentMediaChanged",
        "showHiddenChanged",
        "setShowHidden",
        "showHidden",
        "openEntry",
        "path",
        "isDirectory",
        "isPlayable",
        "goBack",
        "navigateUp",
        "closePlayer",
        "chooseResume",
        "chooseStartOver",
        "savePlaybackPosition",
        "positionMs",
        "clearResume",
        "formatDuration",
        "durationMs",
        "browserModel",
        "FileBrowserModel*",
        "playerVisible",
        "resumePromptVisible",
        "currentMediaPath",
        "currentMediaTitle",
        "playbackStartPositionMs",
        "playbackRequestId",
        "pendingMediaTitle",
        "pendingResumePositionMs"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'playerVisibleChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'resumePromptVisibleChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentMediaChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'showHiddenChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setShowHidden'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 },
        }}),
        // Method 'openEntry'
        QtMocHelpers::MethodData<void(const QString &, bool, bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::Bool, 10 }, { QMetaType::Bool, 11 },
        }}),
        // Method 'goBack'
        QtMocHelpers::MethodData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'navigateUp'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'closePlayer'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'chooseResume'
        QtMocHelpers::MethodData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'chooseStartOver'
        QtMocHelpers::MethodData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'savePlaybackPosition'
        QtMocHelpers::MethodData<void(qint64)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 18 },
        }}),
        // Method 'clearResume'
        QtMocHelpers::MethodData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'formatDuration'
        QtMocHelpers::MethodData<QString(qint64) const>(20, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::LongLong, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'browserModel'
        QtMocHelpers::PropertyData<FileBrowserModel*>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'playerVisible'
        QtMocHelpers::PropertyData<bool>(24, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'resumePromptVisible'
        QtMocHelpers::PropertyData<bool>(25, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'currentMediaPath'
        QtMocHelpers::PropertyData<QString>(26, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'currentMediaTitle'
        QtMocHelpers::PropertyData<QString>(27, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'playbackStartPositionMs'
        QtMocHelpers::PropertyData<qint64>(28, QMetaType::LongLong, QMC::DefaultPropertyFlags, 2),
        // property 'playbackRequestId'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 2),
        // property 'pendingMediaTitle'
        QtMocHelpers::PropertyData<QString>(30, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'pendingResumePositionMs'
        QtMocHelpers::PropertyData<qint64>(31, QMetaType::LongLong, QMC::DefaultPropertyFlags, 1),
        // property 'showHidden'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AppState, qt_meta_tag_ZN8AppStateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppState::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8AppStateE_t>.metaTypes,
    nullptr
} };

void AppState::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppState *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playerVisibleChanged(); break;
        case 1: _t->resumePromptVisibleChanged(); break;
        case 2: _t->currentMediaChanged(); break;
        case 3: _t->showHiddenChanged(); break;
        case 4: _t->setShowHidden((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->openEntry((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 6: _t->goBack(); break;
        case 7: _t->navigateUp(); break;
        case 8: _t->closePlayer(); break;
        case 9: _t->chooseResume(); break;
        case 10: _t->chooseStartOver(); break;
        case 11: _t->savePlaybackPosition((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 12: _t->clearResume((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: { QString _r = _t->formatDuration((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::playerVisibleChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::resumePromptVisibleChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::currentMediaChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)()>(_a, &AppState::showHiddenChanged, 3))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< FileBrowserModel* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<FileBrowserModel**>(_v) = _t->browserModel(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->playerVisible(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->resumePromptVisible(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->currentMediaPath(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->currentMediaTitle(); break;
        case 5: *reinterpret_cast<qint64*>(_v) = _t->playbackStartPositionMs(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->playbackRequestId(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->pendingMediaTitle(); break;
        case 8: *reinterpret_cast<qint64*>(_v) = _t->pendingResumePositionMs(); break;
        case 9: *reinterpret_cast<bool*>(_v) = _t->showHidden(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 9: _t->setShowHidden(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AppState::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppState::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AppState::playerVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AppState::resumePromptVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AppState::currentMediaChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AppState::showHiddenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
