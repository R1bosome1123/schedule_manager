cmake_minimum_required(VERSION 3.16)
project(ScheduleManagerGUI)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 所需模块
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Multimedia)

# 查找 OpenSSL 库
find_package(OpenSSL REQUIRED)

# 启用 Qt 的自动 MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含头文件目录
include_directories(include)

# 源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cc
    src/login_window.cpp
    src/register_window.cpp
    src/client.cpp
    src/task.cc
    src/task_manager.cc
    src/scan.cc
)

# 头文件
set(HEADERS
    include/mainwindow.h
    include/login_window.h
    include/register_window.h
    include/client.hpp
    include/task.h
    include/task_manager.h
    include/scan.h
    include/thread_lock.h
    include/FLAG.h
    include/uniqueid.h
)

# 创建可执行文件
add_executable(ScheduleManagerGUI ${SOURCES} ${HEADERS})

# 链接 Qt6 和 OpenSSL 库
target_link_libraries(ScheduleManagerGUI
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 设置程序输出目录
set_target_properties(ScheduleManagerGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

