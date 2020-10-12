cmake_minimum_required(VERSION 3.14)

project(STEGANO LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Debug)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets REQUIRED)

if(ANDROID)
    add_library(stegano SHARED
      main.cpp
      imageproc.cpp
      steganographylogic.cpp
      qml.qrc
    )
else()
    add_executable(stegano
      main.cpp
      imageproc.cpp
      steganographylogic.cpp
      qml.qrc
    )
endif()

target_compile_definitions(stegano
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(stegano
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets)


# DEPLOY TARGET

find_program(Q_MAKE_EXE qmake)
find_program(CQT_EXE cqtdeployer)

IF(NOT EXISTS ${CQT_EXE})
    message( FATAL_ERROR "WARNING: the cqtdeployer not exits please install the cqtdeployer and run cmake again!")
endif(NOT EXISTS ${CQT_EXE})

set(OUTPUT_EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}stegano)
if (WIN32)
    set(OUTPUT_EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}stegano.exe)
endif (WIN32)

ADD_CUSTOM_TARGET(
    deploy
    COMMAND cqtdeployer clear -bin ${OUTPUT_EXECUTABLE} -qmake ${Q_MAKE_EXE} -targetDir ${PROJECT_SOURCE_DIR}/release -libDir ${PROJECT_SOURCE_DIR} -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 3
    COMMENT "cqtdeployer clear -bin ${OUTPUT_EXECUTABLE} -qmake ${Q_MAKE_EXE} -targetDir ${PROJECT_SOURCE_DIR}/release -libDir ${PROJECT_SOURCE_DIR} -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 3"
)
