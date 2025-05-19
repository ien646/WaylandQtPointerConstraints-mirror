cmake_minimum_required(VERSION 3.25)
project(WaylandQtPointerConstraints)

option(USE_CUSTOM_NAMESPACE "Put project classes into a user defined namespace" OFF)
option(CUSTOM_NAMESPACE "Custom namespace used to wrap project classes" "")
option(BUILD_EXAMPLES "Build example projects" ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui WaylandClient Widgets)

set(CMAKE_CXX_STANDARD 23)

set(TARGET_NAME "WaylandQtPointerConstraints")

find_program(WAYLAND_SCANNER wayland-scanner)
if (NOT WAYLAND_SCANNER)
    message(FATAL_ERROR "wayland-scanner was not found!\nCheck if the executable is available in PATH")
endif ()

execute_process(COMMAND bash -c ${CMAKE_SOURCE_DIR}/generate-protocols.sh)

add_library(
        ${TARGET_NAME}
        LockPointer.cpp
        ConfinePointer.cpp
        ${CMAKE_BINARY_DIR}/generated-protocols/pointer-constraints.c
)

if (USE_CUSTOM_NAMESPACE)
    target_compile_definitions(WAYLAND_QT_POINTER_CONSTRAINTS_CUSTOM_NAMESPACE=${CUSTOM_NAMESPACE})
endif ()

target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::GuiPrivate
        Qt6::WaylandClient
        Qt6::WaylandClientPrivate
        Qt6::Widgets
        wayland-client)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_BINARY_DIR}/generated-protocols)

if (BUILD_EXAMPLES)
    add_executable(LockingButton examples/LockingButton.cpp)
    add_executable(ConfinedButton examples/ConfinedButton.cpp)
    target_link_libraries(LockingButton ${TARGET_NAME} Qt6::Widgets)
    target_link_libraries(ConfinedButton ${TARGET_NAME} Qt6::Widgets)
endif ()