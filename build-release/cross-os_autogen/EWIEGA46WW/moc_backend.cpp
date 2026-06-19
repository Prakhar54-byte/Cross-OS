/****************************************************************************
** Meta object code from reading C++ file 'backend.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../backend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
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
struct qt_meta_tag_ZN7BackendE_t {};
} // unnamed namespace

template <> constexpr inline auto Backend::qt_create_metaobjectdata<qt_meta_tag_ZN7BackendE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Backend",
        "sharedPathChanged",
        "",
        "historyTextChanged",
        "clipboardSyncChanged",
        "lanSyncChanged",
        "peerListChanged",
        "noteReceived",
        "sender",
        "text",
        "mountedDrivesChanged",
        "droppedFilesChanged",
        "saveNote",
        "type",
        "saveFileDrop",
        "QUrl",
        "url",
        "loadHistory",
        "clearHistory",
        "copyToClipboard",
        "openSharedFolder",
        "deleteNote",
        "index",
        "togglePin",
        "restoreNote",
        "deleteNoteForever",
        "refreshMountedDrives",
        "refreshDroppedFiles",
        "openFileExternal",
        "filePath",
        "deleteDroppedFile",
        "fileName",
        "dropsDirectory",
        "onClipboardChanged",
        "sendDiscoveryBroadcast",
        "processPendingDatagrams",
        "handleNewConnection",
        "readClientData",
        "checkInactivePeers",
        "sharedPath",
        "historyText",
        "clipboardSync",
        "lanSync",
        "peerList",
        "localIP",
        "currentOS",
        "deviceName",
        "notes",
        "QVariantList",
        "trashNotes",
        "mountedDrives",
        "droppedFiles"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sharedPathChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'historyTextChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'clipboardSyncChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'lanSyncChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'peerListChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'noteReceived'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
        // Signal 'mountedDrivesChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'droppedFilesChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'saveNote'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 13 },
        }}),
        // Slot 'saveNote'
        QtMocHelpers::SlotData<void(const QString &)>(12, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'saveFileDrop'
        QtMocHelpers::SlotData<void(const QUrl &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
        // Slot 'loadHistory'
        QtMocHelpers::SlotData<QString()>(17, 2, QMC::AccessPublic, QMetaType::QString),
        // Slot 'clearHistory'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'copyToClipboard'
        QtMocHelpers::SlotData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'openSharedFolder'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'deleteNote'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'togglePin'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'restoreNote'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'deleteNoteForever'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'refreshMountedDrives'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshDroppedFiles'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'openFileExternal'
        QtMocHelpers::SlotData<void(const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 29 },
        }}),
        // Slot 'deleteDroppedFile'
        QtMocHelpers::SlotData<void(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 31 },
        }}),
        // Slot 'dropsDirectory'
        QtMocHelpers::SlotData<QString()>(32, 2, QMC::AccessPublic, QMetaType::QString),
        // Slot 'onClipboardChanged'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'sendDiscoveryBroadcast'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processPendingDatagrams'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleNewConnection'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'readClientData'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'checkInactivePeers'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'sharedPath'
        QtMocHelpers::PropertyData<QString>(39, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'historyText'
        QtMocHelpers::PropertyData<QString>(40, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'clipboardSync'
        QtMocHelpers::PropertyData<bool>(41, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'lanSync'
        QtMocHelpers::PropertyData<bool>(42, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'peerList'
        QtMocHelpers::PropertyData<QStringList>(43, QMetaType::QStringList, QMC::DefaultPropertyFlags, 4),
        // property 'localIP'
        QtMocHelpers::PropertyData<QString>(44, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'currentOS'
        QtMocHelpers::PropertyData<QString>(45, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'deviceName'
        QtMocHelpers::PropertyData<QString>(46, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'notes'
        QtMocHelpers::PropertyData<QVariantList>(47, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'trashNotes'
        QtMocHelpers::PropertyData<QVariantList>(49, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'mountedDrives'
        QtMocHelpers::PropertyData<QVariantList>(50, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'droppedFiles'
        QtMocHelpers::PropertyData<QVariantList>(51, 0x80000000 | 48, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 7),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Backend, qt_meta_tag_ZN7BackendE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Backend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7BackendE_t>.metaTypes,
    nullptr
} };

void Backend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Backend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sharedPathChanged(); break;
        case 1: _t->historyTextChanged(); break;
        case 2: _t->clipboardSyncChanged(); break;
        case 3: _t->lanSyncChanged(); break;
        case 4: _t->peerListChanged(); break;
        case 5: _t->noteReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->mountedDrivesChanged(); break;
        case 7: _t->droppedFilesChanged(); break;
        case 8: _t->saveNote((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->saveNote((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->saveFileDrop((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 11: { QString _r = _t->loadHistory();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->clearHistory(); break;
        case 13: _t->copyToClipboard((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->openSharedFolder(); break;
        case 15: _t->deleteNote((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->togglePin((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->restoreNote((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->deleteNoteForever((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->refreshMountedDrives(); break;
        case 20: _t->refreshDroppedFiles(); break;
        case 21: _t->openFileExternal((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->deleteDroppedFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: { QString _r = _t->dropsDirectory();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->onClipboardChanged(); break;
        case 25: _t->sendDiscoveryBroadcast(); break;
        case 26: _t->processPendingDatagrams(); break;
        case 27: _t->handleNewConnection(); break;
        case 28: _t->readClientData(); break;
        case 29: _t->checkInactivePeers(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::sharedPathChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::historyTextChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::clipboardSyncChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::lanSyncChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::peerListChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)(const QString & , const QString & )>(_a, &Backend::noteReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::mountedDrivesChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Backend::*)()>(_a, &Backend::droppedFilesChanged, 7))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->sharedPath(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->historyText(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->clipboardSync(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->lanSync(); break;
        case 4: *reinterpret_cast<QStringList*>(_v) = _t->peerList(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->localIP(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->currentOS(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->deviceName(); break;
        case 8: *reinterpret_cast<QVariantList*>(_v) = _t->notes(); break;
        case 9: *reinterpret_cast<QVariantList*>(_v) = _t->trashNotes(); break;
        case 10: *reinterpret_cast<QVariantList*>(_v) = _t->mountedDrives(); break;
        case 11: *reinterpret_cast<QVariantList*>(_v) = _t->droppedFiles(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSharedPath(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setClipboardSync(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setLanSync(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Backend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Backend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7BackendE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Backend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 30;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Backend::sharedPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Backend::historyTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Backend::clipboardSyncChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Backend::lanSyncChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Backend::peerListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Backend::noteReceived(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void Backend::mountedDrivesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Backend::droppedFilesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
