cmake_minimum_required(VERSION 3.10)   # 指定最低 CMake 版本

project(clib VERSION 1.0)          # 定义项目名称和版本

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
add_library(clib src/arraylist.c src/types.c)        # 创建一个库目标 clib
add_executable(test src/test.c)  # 创建一个可执行文件目标 MyExecutable

# 链接库到可执行文件
target_link_libraries(test clib)