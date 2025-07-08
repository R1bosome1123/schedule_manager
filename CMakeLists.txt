cmake_minimum_required(VERSION 3.10)
project(AccountManager)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(account_manager
    usermanagerexample.cc
    useraccount.cc
    # 添加其他源文件（如果有）
)

# 添加头文件搜索路径
target_include_directories(account_manager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)