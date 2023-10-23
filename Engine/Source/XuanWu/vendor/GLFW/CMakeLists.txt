cmake_minimum_required(VERSION 3.10)
project(GLFW)

#启用VS项目文件夹分类
option(USE_SOLUTION_FOLDERS "启用VS项目文件夹分类" ON)

set(
    SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW/glfw3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW/glfw3native.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glfw_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/context.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window.c
)

if(WIN32)
    set(SRC ${SRC} 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_init.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_joystick.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_monitor.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_time.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_thread.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/win32_window.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wgl_context.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/egl_context.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osmesa_context.c
    )
endif()

add_library(glfw3 STATIC ${SRC})

if(WIN32)
    target_compile_definitions(glfw3 PRIVATE _GLFW_WIN32 _CRT_SECURE_NO_WARNINGS)
endif()

if(WIN32)
    if(MSVC)
        # 对 AllFile 变量里面的所有文件分类(保留资源管理器的目录结构)
        foreach(fileItem ${SRC})       
            # Get the directory of the source file
            get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
            # message(${PARENT_DIR})
            # Remove common directory prefix to make the group
            string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
            # message(${GROUP})
            # Make sure we are using windows slashes
            string(REPLACE "/" "\\" GROUP "${GROUP}")
            # Group into "Source Files" and "Header Files"
            set(GROUP "${GROUP}")

            source_group("${GROUP}" FILES "${fileItem}")
        endforeach()

        #设置项目位置
        set_target_properties(glfw3 PROPERTIES FOLDER "Dependencies")
    endif()
endif()

# 配合使用
# if(USE_SOLUTION_FOLDERS)
#     set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# else()
#     set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
# endif()

# 设置库生成的路径
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

# 根据构建类型设置编译选项
target_compile_options(glfw3 PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O2>
)