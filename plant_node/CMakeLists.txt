cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "plant_node")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

enable_language( C CXX ASM )

pico_sdk_init()

add_executable(${projname}
        src/plant_node.cpp
        src/access_point.cpp
        src/tcp_client.cpp
        lib/dhcpserver/dhcpserver.c
        lib/dnsserver/dnsserver.c
        )

target_include_directories(${projname} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/lib/dhcpserver
        ${CMAKE_CURRENT_LIST_DIR}/lib/dnsserver
        ${CMAKE_CURRENT_LIST_DIR}/inc
        )

target_link_libraries(${projname}
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        )

pico_add_extra_outputs(${projname})
