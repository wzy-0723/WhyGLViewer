file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.qrc *.ui)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Core Gui)

set(PROJECT_SOURCES ${source})

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    .
    ./OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE 
        Qt::Core
        Qt::Gui
        Qt::Widgets 
        Qt::OpenGLWidgets
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
