#pragma once

#include <QWidget>

#include <pointer-constraints.h>

class QPlatformNativeInterface;

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
namespace WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
{
#endif

    class ConfinePointer final
    {
    public:
        explicit ConfinePointer(QWindow* window);
        ~ConfinePointer();

        void confinePointer(QRect region);
        void releasePointer();

        [[nodiscard]] bool isPointerConfined() const;

    private:
        QWindow* _window = nullptr;
        bool _confined = false;
        QPlatformNativeInterface* _nativeInterface = nullptr;
        wl_compositor* _compositor = nullptr;
        wl_display* _display = nullptr;
        zwp_pointer_constraints_v1* _pointerConstraintsV1 = nullptr;
        zwp_confined_pointer_v1* _confinedPointerV1 = nullptr;
        wl_region* _confinedRegion = nullptr;
        wl_registry_listener _registryListener = {};
    };

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
}
#endif
