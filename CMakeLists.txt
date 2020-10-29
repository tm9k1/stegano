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

add_subdirectory(src)

  # DEPLOY

  find_program(QMAKE_EXE qmake)
  find_program(CQT_EXE cqtdeployer)

  if(NOT EXISTS ${QMAKE_EXE})
      message(WARNING "Could not find EXECUTABLE qmake in the system. Target DEPLOY will NOT work.")
  endif()

  if(NOT EXISTS ${CQT_EXE})
      message(WARNING "Could not find EXECUTABLE cqtdeployer in the system. Target DEPLOY will NOT work.\nInstall from here: https://github.com/QuasarApp/CQtDeployer/releases/")
  endif()

  set(OUTPUT_EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}stegano)

  if(WIN32)
    string(APPEND OUTPUT_EXECUTABLE ".exe")
  endif()

  add_custom_target(
      deploy
      COMMAND ${CQT_EXE} clear -bin ${OUTPUT_EXECUTABLE} -qmake ${QMAKE_EXE}
              -targetDir ${PROJECT_SOURCE_DIR}/release -libDir ${PROJECT_SOURCE_DIR}
              -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 3
      COMMENT "${CQT_EXE} -clear -bin ${OUTPUT_EXECUTABLE} -qmake ${QMAKE_EXE}
              -targetDir ${PROJECT_SOURCE_DIR}/release -libDir ${PROJECT_SOURCE_DIR}
              -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 3"
  )
