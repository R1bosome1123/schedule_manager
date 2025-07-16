cmake_minimum_required(VERSION 3.16)
project(Backend)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 查找依赖包
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SODIUM_INCLUDE_DIRS})

# 收集源文件
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接库
target_link_libraries(${PROJECT_NAME}
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    Qt5::Core
    ${SODIUM_LIBRARIES}
    pthread
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin
)

# 创建必要的目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dataset)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/log)

# 安装规则（可选）
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 复制资源文件（如果需要）
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/key/
    DESTINATION bin/key
    FILES_MATCHING PATTERN "*.pem"
)

# 打印配置信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Sources found: ${SOURCES}")
