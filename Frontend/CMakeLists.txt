cmake_minimum_required(VERSION 3.16)
project(ScheduleManagerGUI)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt5库
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network Multimedia)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# 启用Qt5的MOC
set(CMAKE_AUTOMOC ON)

# 设置包含目录
include_directories(include)

# 源文件 (在src目录下)
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

# 头文件 (在include目录下)
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

# 链接Qt5库和OpenSSL库
target_link_libraries(ScheduleManagerGUI
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Multimedia
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 设置可执行文件输出到根目录
set_target_properties(ScheduleManagerGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
