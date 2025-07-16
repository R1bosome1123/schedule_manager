cmake_minimum_required(VERSION 3.16)
project(ScheduleManagerBackend)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找所需的包
find_package(Boost REQUIRED COMPONENTS system)

# 添加 OpenSSL
find_package(OpenSSL REQUIRED)

# 查找libsodium
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SODIUM QUIET libsodium)
endif()

if(NOT SODIUM_FOUND)
    # 尝试手动查找libsodium
    find_library(SODIUM_LIBRARY NAMES sodium libsodium)
    find_path(SODIUM_INCLUDE_DIR sodium.h)
    if(SODIUM_LIBRARY AND SODIUM_INCLUDE_DIR)
        set(SODIUM_FOUND TRUE)
        set(SODIUM_LIBRARIES ${SODIUM_LIBRARY})
        set(SODIUM_INCLUDE_DIRS ${SODIUM_INCLUDE_DIR})
    endif()
endif()

# 查找nlohmann_json，提供多种查找方式
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(nlohmann_json QUIET nlohmann_json)
    endif()
    if(NOT nlohmann_json_FOUND)
        # 如果仍然找不到，尝试在系统路径中查找头文件
        find_path(nlohmann_json_INCLUDE_DIR nlohmann/json.hpp
            PATHS /usr/include /usr/local/include
        )
        if(nlohmann_json_INCLUDE_DIR)
            set(nlohmann_json_FOUND TRUE)
            set(nlohmann_json_INCLUDE_DIRS ${nlohmann_json_INCLUDE_DIR})
        endif()
    endif()
endif()

# 尝试查找Qt6，如果找不到则使用Qt5
find_package(Qt6 QUIET COMPONENTS Core)
if(NOT Qt6_FOUND)
    find_package(Qt5 QUIET COMPONENTS Core)
    if(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
    else()
        message(WARNING "Neither Qt6 nor Qt5 found. Some features may not work.")
        set(QT_VERSION_MAJOR 0)
    endif()
else()
    set(QT_VERSION_MAJOR 6)
endif()

# 设置Qt相关配置
if(QT_VERSION_MAJOR GREATER 0)
    set(CMAKE_AUTOMOC ON)
endif()

# 打印依赖库查找状态
message(STATUS "Boost found: ${Boost_FOUND}")
message(STATUS "nlohmann_json found: ${nlohmann_json_FOUND}")
message(STATUS "libsodium found: ${SODIUM_FOUND}")
message(STATUS "Qt version: ${QT_VERSION_MAJOR}")

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加源文件
set(SOURCES
    main.cpp
    server.cpp
    task.cc
    useraccount.cc
    info_prompt.cpp
)

# 添加头文件
set(HEADERS
    server.hpp
    task.h
    useraccount.h
    info_prompt.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(${PROJECT_NAME} 
    Boost::system
    pthread
)

# 链接libsodium
if(SODIUM_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SODIUM_LIBRARIES})
else()
    message(WARNING "libsodium not found. Install it with: sudo apt-get install libsodium-dev")
endif()

# 链接Qt库
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} Qt6::Core)
elseif(QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${PROJECT_NAME} Qt5::Core)
endif()

# 链接nlohmann_json
if(nlohmann_json_FOUND)
    if(TARGET nlohmann_json::nlohmann_json)
        target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
    else()
        target_include_directories(${PROJECT_NAME} PRIVATE ${nlohmann_json_INCLUDE_DIRS})
        if(nlohmann_json_LIBRARIES)
            target_link_libraries(${PROJECT_NAME} ${nlohmann_json_LIBRARIES})
        endif()
    endif()
endif()


target_link_libraries(ScheduleManagerBackend
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 设置编译选项
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 添加编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BOOST_ASIO_HAS_STD_CHRONO
)
