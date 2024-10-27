cmake_minimum_required(VERSION 3.2)

PROJECT(clib)

#head file path，头文件目录
INCLUDE_DIRECTORIES(
include
)

#源文件目录
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)

#添加要编译的可执行文件
ADD_EXECUTABLE(arraylist_test src/arraylist_test.c src/arraylist.c)