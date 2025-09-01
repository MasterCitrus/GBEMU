cmake_minimum_required(VERSION 3.10)
project(GBEMU)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE EXTERNAL_SOURCES CONFIGURE_DEPENDS
	dep/imgui/imgui.cpp
	dep/imgui/imgui_demo.cpp
	dep/imgui_draw.cpp
	dep/imgui_tables.cpp
	dep/imgui_widgets.cpp
	dep/imgui/backends/imgui_impl_glfw.cpp
	dep/imgui/backends/imgui_impl_opengl3.cpp
	dep/glad/src/glad.c
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

source_group(TREE ${CMAKE_SOURCE_DIR}/src/ PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/ PREFIX "Header Files" FILES ${HEADERS})

add_executable(${PROJECT_NAME} ${EXTERNAL_SOURCES} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
	src
	glfw/include
	glad/include
	imgui
	imgui/backends
)

add_subdirectory(dep/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)