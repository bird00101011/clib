# clib

采用MSVC构建工具编写Windows下的纯C库。

两种构建工具ninja和CMake，前者用于代码无bug时的构建，后者用于调试bug时使用。

## ninja构建

编译
```
ninja
```

测试
```
test.exe
```

清除
```
ninja -t clean
```

## CMake构建

VSCode编写完CMakeLists.txt后按Ctrl + s就会自动生成build目录，再用VS打开build目录中的.sln文件即可构建，更多VS的使用请参考搜索引擎。