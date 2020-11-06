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

add_library(util util.h)
target_link_libraries(util Qt${QT_VERSION_MAJOR}::Core)
generate_export_header(util)

# Library for utility stuff from Stegano
add_library(Stegano::Util ALIAS util)

target_compile_definitions(stegano
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(stegano
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
  Stegano::Util
  )
