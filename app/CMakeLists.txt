# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# set binary output folder to libs folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY      "${CMAKE_CURRENT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}")

add_library( dirtyCow
             SHARED
             src/main/jni/dirtyCow.c )
target_link_libraries(  dirtyCow
                        log)
