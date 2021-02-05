# Cmake cookbook 阅读笔记

原文链接：https://www.bookstack.cn/read/CMake-Cookbook/

## 第1章 从可执行文件到库

### 1.1 单个源文件编译为可执行文件

最简单的`CMakeLists.txt`：

```cmake
# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language, 建议设定LANGUAGES
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world hello-world.cpp)
```

构建命令可以通过`cmake --build .`实现，windows下不需要打开visual studio，再进行build。

其他构建命令可以通过`cmake --build . --target help`查看：

```
$ cmake --build . --target help
The following are some of the valid targets for this Makefile:
... all (the default if no target is provided)
... clean
... depend
... rebuild_cache
... hello-world
... edit_cache
... hello-world.o
... hello-world.i
... hello-world.s
```

通过`cmake --build . --target clean`执行。

### 1.3 构建和链接静态库和动态库

示例中有三个文件：

- `hello_world.cpp`，main函数实现的地方，用来调用库中具体实现；
- `Message.hpp`,`Message.cpp`，接口实现的地方，示例用来生成库；

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(recipe-03 LANGUAGES CXX)

# 创建目标静态库
add_library(message-objs
  OBJECT          # STATIC: 用来创建静态库
                  # SHARED： 用来创建动态库
                  # OBJECT：用来将列表中的源码编译到目标文件中
                  # MODULE：不链接到项目中的任何目标，可以进行动态加载，可以用于构建运行时插件
  Message.hpp
  Message.cpp)
  
add_library(message-shared 
  SHARED 
  $<TARGET_OBJECTS:message-objs>) # 生成器表达式是CMake在生成时(即配置之后)构造，用于生成特定于配置的构建输出。

# 如果需要生成同名的静态库和动态库，需要放开如下代码
# set_target_properties(message-shared
#  PROPERTIES
#    OUTPUT_NAME "message")

add_library(message-static
  STATIC
  $<TARGET_OBJECT:message-objs>)
  
# 如果需要生成同名的静态库和动态库，需要放开如下代码
# set_target_properties(message-static
#  PROPERTIES
#    OUTPUT_NAME "message")

add_executable(hello_world hello_world.cpp)
target_link_library(hello_world message-static)
```

### 1.4 用条件句控制编译

```CMake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-04 LANGUAGES CXX)

set(USE_LIBRARY OFF) # 如果将逻辑变量设置为以下任意一种：1、ON、YES、true、Y或非零数，则逻辑变量为true
                     # 如果将逻辑变量设置为以下任意一种：0、OFF、NO、false、N、IGNORE、NOTFOUND、空字符串，或者以-NOTFOUND为后缀，则逻辑变量为false
                     
set(BUILD_SHARED_LIBS OFF) # CMake的一个全局标志

list(APPEND _sources Message.hpp Message.cpp) # 创建一个数组，用下划线标识是个局部变量
if(USE_LIBRARY)
  add_library(message ${_sources}) # 根据BUILD_SHARED_LIBS是不是OFF，确定构建的是动态库还是静态库
  add_executable(hello-world hello-world.cpp)
  target_link_libraries(hello-world message)
else()
  add_executable(hello-world hello-world.cpp ${_sources})
endif()
```

### 1.7 切换构建类型

`CMAKE_BUILD_TYPE`可以用来控制构建类型。该变量对应的值有：

1. **Debug**：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件。
2. **Release**：用于构建的优化的库或可执行文件，不包含调试符号。
3. **RelWithDebInfo**：用于构建较少的优化库或可执行文件，包含调试符号。
4. **MinSizeRel**：用于不增加目标代码大小的优化方式，来构建库或可执行文件。

```cmake
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
```

通过`cmake -D CMAKE_BUILD_TYPE=Debug ..`进行切换。

对于Visual Studio可以支持多个Build Type设置的情况，可以使用`CMAKE_CONFIGURATION_TYPES`这个变量，如：

```
$ cmake .. -G"Visual Studio 12 2017 Win64" -D CMAKE_CONFIGURATION_TYPES="Release;Debug"
```

在构建的时候进行区分：

```
$ cmake --build . --config Release
```

### 1.8 设置编译器选项

- 直接修改全局的环境变量：`CMAKE_CXX_FLAGS `,`CMAKE_CXX_FLAGS_DEBUG`,`CMAKE_CXX_FLAGS_RELEASE `
- 利用target_compile_option进行为目标设定编译选项，主要用该方案更为合适；

```cmake
set(COMPILER_FLAGS)
set(COMPILER_FLAGS_DEBUG)
set(COMPILER_FLAGS_RELEASE)
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions")
  list(APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions" "-Qunused-arguments" "-fcolor-diagnostics")
  list(APPEND CXX_FLAGS_DEBUG "-Wdocumentation")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()

target_compile_option(compute-areas
  PRIVATE
  ${CXX_FLAGS}
  "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"      # 根据配置使用DEBUG or RELEASE
  "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
)
```

可见性配置如下：

- **PRIVATE**，编译选项会应用于给定的目标，不会传递给与目标相关的目标。我们的示例中， 即使`compute-areas`将链接到`geometry`库，`compute-areas`也不会继承`geometry`目标上设置的编译器选项。
- **INTERFACE**，给定的编译选项将只应用于指定目标，并传递给与目标相关的目标。
- **PUBLIC**，编译选项将应用于指定目标和使用它的目标。

### 1.9 为语言设定标准

用来确定使用的c++哪个标准。

```cmake
    set_target_properties(animals
      PROPERTIES
        CXX_STANDARD 14               # 设置想要的c++标准
        CXX_EXTENSIONS OFF            # 是不是使用ISO C++之外，编译器的扩展
        CXX_STANDARD_REQUIRED ON      # 找不到对应的标准，则报错
        POSITION_INDEPENDENT_CODE 1   # 避免在使用一些编译器构建DSO时出现问题
    )
```

要求windows上导出所有库符号，可以借用cmake：

```
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
```

## 第2章 检测环境

### 2.1 检测操作系统

```cmake
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message(STATUS "Configuring on/for Linux")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        message(STATUS "Configuring on/for macOS")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message(STATUS "Configuring on/for Windows")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
        message(STATUS "Configuring on/for IBM AIX")
    else()
        message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
    endif()
```

### 2.2 处理和平台相关的宏定义

```c++
#ifdef IS_WINDOWS
  return std::string("Hello from Windows!");
#elif IS_LINUX
  return std::string("Hello from Linux!");
```

cmake中可以使用`target_compile_definitions`。

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(hello-world PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(hello-world PUBLIC "IS_WINDOWS")
endif()
```

### 2.5 检测处理器指令集

定义`config.h.in`，如下：

```c++
#pragma once
#define NUMBER_OF_LOGICAL_CORES @_NUMBER_OF_LOGICAL_CORES@
#define NUMBER_OF_PHYSICAL_CORES @_NUMBER_OF_PHYSICAL_CORES@
#define TOTAL_VIRTUAL_MEMORY @_TOTAL_VIRTUAL_MEMORY@
#define AVAILABLE_VIRTUAL_MEMORY @_AVAILABLE_VIRTUAL_MEMORY@
#define TOTAL_PHYSICAL_MEMORY @_TOTAL_PHYSICAL_MEMORY@
#define AVAILABLE_PHYSICAL_MEMORY @_AVAILABLE_PHYSICAL_MEMORY@
....
```

cmake如下：

```cmake
foreach(key
  IN ITEMS
    NUMBER_OF_LOGICAL_CORES
    NUMBER_OF_PHYSICAL_CORES
    TOTAL_VIRTUAL_MEMORY
    AVAILABLE_VIRTUAL_MEMORY
    TOTAL_PHYSICAL_MEMORY
    AVAILABLE_PHYSICAL_MEMORY
  )
  cmake_host_system_information(RESULT _${key} QUERY ${key})
  # ： cmake_host_system_information(RESULT <variable> QUERY <key> ...)，将key查询结果存入到variable中
  # https://cmake.org/cmake/help/latest/command/cmake_host_system_information.html
endforeach()

configure_file(config.h.in config.h @ONLY)
```

configure_file使用参见：https://www.cnblogs.com/gaox97329498/p/10952732.html

### 2.6 为Eigen库使能向量化

https://www.bookstack.cn/read/CMake-Cookbook/content-chapter2-2.6-chinese.md

## 第3章 检测外部库和程序

### 3.1 检测python解释器

```cmake
# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(recipe-01 LANGUAGES NONE)

# detect python
find_package(PythonInterp REQUIRED)

# Execute a tiny Python script
execute_process(
  COMMAND
    ${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')"
  RESULT_VARIABLE _status
  OUTPUT_VARIABLE _hello_world
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

message(STATUS "RESULT_VARIABLE is: ${_status}")
message(STATUS "OUTPUT_VARIABLE is: ${_hello_world}")

# compare the "manual" messages with the following handy helper
include(CMakePrintHelpers)
cmake_print_variables(_status _hello_world)
```

当调用`find_package(<name>)`的时候，会去找对应的cmake模块文件`Find<name>.cmake`。可以使用`cmake --help-module FindPythonInterp`寻求帮助。

### 3.3 检测python模块和包

例子中有三个新的CMake命令，需要`include(FindPackageHandleStandardArgs)`：

- `execute_process`: 这个在上一节中介绍过了。
- `add_custom_command`
- `find_package_handle_standard_args`

```cmake
# add custom command
add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
                                          ${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
  )
  
# make sure building pure-embedding triggers the above
# custom command
target_sources(pure-embedding
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  )
  
#-----------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy
  FOUND_VAR NumPy_FOUND
  REQUIRED_VARS NumPy
  VERSION_VAR _numpy_version
  )
```

`find_package_handle_standard_args`进一步了解，参见：https://cmake.org/cmake/help/v3.0/module/FindPackageHandleStandardArgs.html

### 3.9 检测外部库： I. 使用pkg-config

```cmake
find_package(PkgConfig REQUIRED QUIET)

pkg_search_module(  
  ZeroMQ                       # 命名搜索ZeroMQ库结果目标：PkgConfig::ZeroMQ
  REQUIRED
    libzeromq libzmq lib0mq    # 不同操作系统和包，可能对应的名称不一样
  IMPORTED_TARGET              # 导入目标，可以通过PkgConfig::ZeroMQ的方式使用
  )

if(TARGET PkgConfig::ZeroMQ)
  message(STATUS "Found ZeroMQ")
endif()

add_executable(hwserver hwserver.c)

target_link_libraries(hwserver PkgConfig::ZeroMQ)

add_executable(hwclient hwclient.c)

target_link_libraries(hwclient PkgConfig::ZeroMQ)
```

### 3.10 检测外部库：II. 自定义find模块

需要告诉cmakelists文件自定义的find模块的位置。

```cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(ZeroMQ REQUIRED)
add_executable(hwserver hwserver.c)
target_include_directories(hwserver PRIVATE ${ZeroMQ_INCLUDE_DIRS})
target_link_directories(hwserver PRIVATE ${ZeroMQ_LIBRARIES})
```

具体实现见：https://www.bookstack.cn/read/CMake-Cookbook/content-chapter3-3.10-chinese.md

## 第4章 创建和运行测试

### 4.1 创建一个简单的单元测试

