file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.qrc *.ui)

find_package(glm CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(stb REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Core Gui)

# 文件系统
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/src/Assets")

configure_file(
	${PROJECT_SOURCE_DIR}/config.h.in
	${PROJECT_BINARY_DIR}/config.h
)

set(PROJECT_SOURCES ${source})

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

if(MSVC)
    add_compile_options(/EHsc)
endif()

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        .
		./Application
        ./Component
		./Graphic
        ./Input
        ./Gui
        ./IO
	    ./Render
        ./Scene
	    ./Util

        ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Qt::Core
        Qt::Gui
        Qt::Widgets 
        Qt::OpenGLWidgets

        glm::glm
        glog::glog
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${ASSETS_DIR}
	${CMAKE_BINARY_DIR}/Assets
)