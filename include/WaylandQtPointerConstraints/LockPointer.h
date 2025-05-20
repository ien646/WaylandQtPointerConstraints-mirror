#pragma once

#include <QWidget>

#include <pointer-constraints.h>
#include <relative-pointer.h>

class QPlatformNativeInterface;

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
namespace WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
{
#endif

    class LockPointer : public QObject
    {
        Q_OBJECT
    public:
        explicit LockPointer(QWindow* window);
        ~LockPointer();

        void lockPointer(QRect region);
        void unlockPointer();

        [[nodiscard]] bool isPointerLocked() const;

    signals:
        void mouseMovementDelta(QPoint delta);

    private:
        QWindow* _window = nullptr;
        bool _locked = false;
        QPlatformNativeInterface* _nativeInterface = nullptr;
        wl_compositor* _compositor = nullptr;
        wl_display* _display = nullptr;
        zwp_pointer_constraints_v1* _pointerConstraintsV1 = nullptr;
        zwp_locked_pointer_v1* _lockedPointerV1 = nullptr;
        wl_region* _lockedRegion = nullptr;
        wl_registry_listener _registryListener = {};
        zwp_relative_pointer_manager_v1* _relativePointerManagerV1 = nullptr;
        zwp_relative_pointer_v1* _relativePointerV1 = nullptr;
        zwp_relative_pointer_v1_listener _relativePointerListenerV1 = {};

        void emitMouseMovementDelta(QPoint delta);
    };

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
}
#endif
