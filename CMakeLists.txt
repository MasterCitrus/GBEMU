cmake_minimum_required(VERSION 3.10)
project(GBEMU)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

source_group(TREE ${CMAKE_SOURCE_DIR}/src/ PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/ PREFIX "Header Files" FILES ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
	src
	dep/glfw/include
	dep/glad/include
	dep/imgui
	dep/imgui/backends
)

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/dep/glad/src/glad.c)
target_include_directories(glad PUBLIC
	dep/glad/include
)

add_library(nfde STATIC dep/nfde/src/nfd_win.cpp)
target_include_directories(nfde PUBLIC
	dep/nfde/src/include
)

add_library(imgui STATIC 
	dep/imgui/imgui.cpp
	dep/imgui/imgui_demo.cpp
	dep/imgui/imgui_draw.cpp
	dep/imgui/imgui_tables.cpp
	dep/imgui/imgui_widgets.cpp
	dep/imgui/backends/imgui_impl_glfw.cpp
	dep/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
	dep/imgui
	dep/imgui/backends
)

find_package(OpenGL REQUIRED)
add_subdirectory(dep/glfw)
add_subdirectory(dep/sdl)

target_link_libraries(imgui PUBLIC
	glfw
	OpenGL::GL
	SDL3::SDL3
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glad
	imgui
	nfde
)
