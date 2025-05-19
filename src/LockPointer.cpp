#include <WaylandQtPointerConstraints/LockPointer.h>

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include <QWindow>

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
namespace WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
{
#endif

LockPointer::LockPointer(QWindow* window)
    : _window(window)
{
    _registryListener.global = [](void* data, wl_registry* registry, const uint32_t name, const char* interface,
                                  const uint32_t version)
    {
        if (std::string_view(interface) == std::string_view(zwp_pointer_constraints_v1_interface.name))
        {
            *static_cast<zwp_pointer_constraints_v1**>(data) = static_cast<zwp_pointer_constraints_v1*>(
                wl_registry_bind(
                    registry, name, &zwp_pointer_constraints_v1_interface, version));
        }
    };
    _registryListener.global_remove = nullptr;

    _nativeInterface = QGuiApplication::platformNativeInterface();
    _compositor = static_cast<wl_compositor*>(_nativeInterface->nativeResourceForIntegration("compositor"));
    _display = static_cast<wl_display*>(_nativeInterface->nativeResourceForIntegration("wl_display"));
}

LockPointer::~LockPointer()
{
    if (_locked)
    {
        unlockPointer();
    }
    if (_lockedRegion)
    {
        wl_region_destroy(_lockedRegion);
    }
    if (_pointerConstraintsV1)
    {
        zwp_pointer_constraints_v1_destroy(_pointerConstraintsV1);
    }
}

void LockPointer::lockPointer(const QRect region)
{
    if (_locked)
    {
        return;
    }

    auto* waylandSurface = static_cast<wl_surface*>(_nativeInterface->nativeResourceForWindow(
        "surface", _window));

    auto* pointer = static_cast<wl_pointer*>(_nativeInterface->nativeResourceForIntegration("wl_pointer"));

    auto* registry = wl_display_get_registry(_display);
    wl_registry_add_listener(registry, &_registryListener, &_pointerConstraintsV1);

    wl_display_roundtrip(_display);

    if (_lockedRegion)
    {
        wl_region_destroy(_lockedRegion);
        _lockedRegion = nullptr;
    }
    _lockedRegion = wl_compositor_create_region(_compositor);
    wl_region_add(_lockedRegion, region.x(), region.y(), region.width(), region.height());

    _lockedPointerV1 = zwp_pointer_constraints_v1_lock_pointer(_pointerConstraintsV1,
                                                               waylandSurface,
                                                               pointer, _lockedRegion,
                                                               ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT);

    wl_surface_commit(waylandSurface);
    wl_display_roundtrip(_display);

    _locked = true;
}

void LockPointer::unlockPointer()
{
    if (!_locked)
    {
        return;
    }

    zwp_locked_pointer_v1_destroy(_lockedPointerV1);
    wl_display_roundtrip(_display);

    _locked = false;
}

bool LockPointer::isPointerLocked() const
{
    return _locked;
}

#ifdef WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE
}
#endif
