cmake_minimum_required(VERSION 3.10)
project(ScheduleManager VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

# 默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 打印构建信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

#####

# 查找libsodium库
find_path(SODIUM_INCLUDE_DIR sodium.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/homebrew/include
        /usr/include/sodium
    PATH_SUFFIXES
        sodium
)

find_library(SODIUM_LIBRARY
    NAMES sodium libsodium
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/homebrew/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/aarch64-linux-gnu
)

if(NOT SODIUM_INCLUDE_DIR OR NOT SODIUM_LIBRARY)
    message(FATAL_ERROR "无法找到libsodium库。请确保已安装：
    Ubuntu/Debian: sudo apt-get install libsodium-dev
    CentOS/RHEL: sudo yum install libsodium-devel
    macOS: brew install libsodium")
endif()

message(STATUS "Found libsodium include dir: ${SODIUM_INCLUDE_DIR}")
message(STATUS "Found libsodium library: ${SODIUM_LIBRARY}")

# 查找线程库
find_package(Threads REQUIRED)

#####

# 收集源文件
set(MAIN_SOURCES
    main.cpp
    useraccount.cc
    task.cc
    task_manager.cpp
    info_prompt.cpp
    scan.cpp
    thread_lock.cpp
)


# 检查可选源文件是否存在并添加到构建中
foreach(src ${OPTIONAL_SOURCES})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
        list(APPEND MAIN_SOURCES ${src})
        message(STATUS "Adding optional source: ${src}")
    else()
        message(STATUS "Optional source not found: ${src}")
    endif()
endforeach()

set(HEADERS
    useraccount.h
    task.h
    task_manager.h
    info_prompt.h
    scan.h
    thread_lock.h
    library.h
    FLAG.h
    schedule.h
)

# 检查主要源文件是否存在
foreach(src ${MAIN_SOURCES})
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
        message(WARNING "Required source file not found: ${src}")
    endif()
endforeach()

# 添加主可执行文件
add_executable(schedule_manager ${MAIN_SOURCES})

# 添加头文件搜索路径
target_include_directories(schedule_manager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SODIUM_INCLUDE_DIR}
)

# 链接库
target_link_libraries(schedule_manager PRIVATE
    ${SODIUM_LIBRARY}
    Threads::Threads
)

# 设置可执行文件属性
set_target_properties(schedule_manager PROPERTIES
    OUTPUT_NAME "schedule_manager"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#####

# 可选：构建用户管理示例程序
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/user_manager_example.cc")
    add_executable(user_manager_example
        user_manager_example.cc
        useraccount.cc
    )
    
    target_include_directories(user_manager_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SODIUM_INCLUDE_DIR}
    )
    
    target_link_libraries(user_manager_example PRIVATE
        ${SODIUM_LIBRARY}
    )
    
    set_target_properties(user_manager_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    message(STATUS "Building user manager example")
endif()

#####

# 复制数据文件到构建目录
file(GLOB DATA_FILES "*.dat" "*.txt" "*.json")
foreach(data_file ${DATA_FILES})
    get_filename_component(file_name ${data_file} NAME)
    configure_file(${data_file} ${CMAKE_BINARY_DIR}/bin/${file_name} COPYONLY)
    message(STATUS "Copying data file: ${file_name}")
endforeach()

# 创建必要的目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 安装配置
install(TARGETS schedule_manager
    RUNTIME DESTINATION bin
)

if(TARGET user_manager_example)
    install(TARGETS user_manager_example
        RUNTIME DESTINATION bin
    )
endif()

# 打印最终信息
message(STATUS "")
message(STATUS "=== Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Executable: ${CMAKE_BINARY_DIR}/bin/schedule_manager")
message(STATUS "")
message(STATUS "Build commands:")
message(STATUS "  make                    # Build the project")
message(STATUS "  make install           # Install binaries")
message(STATUS "")
message(STATUS "Run commands:")
message(STATUS "  ./bin/schedule_manager  # Run the main program")
if(TARGET user_manager_example)
    message(STATUS "  ./bin/user_manager_example  # Run user manager example")
endif()
message(STATUS "================================")
