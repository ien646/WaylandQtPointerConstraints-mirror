rm wayland-protocols -rf
git clone https://gitlab.freedesktop.org/wayland/wayland-protocols

mkdir generated-protocols

wayland-scanner client-header wayland-protocols/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml generated-protocols/pointer-constraints.h
wayland-scanner private-code wayland-protocols/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml generated-protocols/pointer-constraints.c

wayland-scanner client-header wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml generated-protocols/relative-pointer.h
wayland-scanner private-code wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml generated-protocols/relative-pointer.c
