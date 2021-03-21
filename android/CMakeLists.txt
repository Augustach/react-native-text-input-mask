cmake_minimum_required(VERSION 3.9.0)

include_directories(
        ../../react-native/React
        ../../react-native/React/Base
        ../../react-native/ReactCommon/jsi
)

add_library(rntextinputmask  # <-- Library name
        SHARED
        # Provides a relative path to your source file(s).
        ../../react-native/ReactCommon/jsi/jsi/jsi.cpp
        cpp-adapter.cpp
)


find_library(
        log-lib # Sets the name of the path variable.
        log
)

target_link_libraries(
        rntextinputmask
        log
        android
)