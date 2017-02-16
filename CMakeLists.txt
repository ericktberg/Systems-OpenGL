cmake_minimum_required(VERSION 2.8)

project(ClothSim)

set(GLFW_SOURCE_DIR "lib/glfw")
set(GLM_SOURCE_DIR "lib/glm")
set(GLEW_SOURCE_DIR "lib/glew")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")

find_package(OpenGL)

set(SOURCES
	src/callbacks.cpp
	src/Camera.cpp
	src/Cloth.cpp
	src/DynamicObject.cpp
	src/glew.c
	src/main.cpp
	src/PendulumSpring.cpp
	src/Plane.cpp
	src/Program.cpp
	src/Projectile.cpp
	src/RenderableObject.cpp
	src/Shader.cpp
	src/Sphere.cpp
	src/Spline.cpp
	src/Subdivider.cpp
	src/UtilGLEW.cpp
	)
set(HEADERS
	include/Camera.h
	include/Cloth.h
	include/DynamicObject.h
	include/Edge.h
	include/Face.h
	include/GL/glew.h
	include/PendulumSpring.h
	include/Plane.h
	include/Program.h
	include/Projectile.h
	include/RenderableObject.h
	include/Shader.h
	include/Sphere.h
	include/Spline.h
	include/Subdivider.h
	include/Vertex.h
	include/util/UtilGLEW.h
	)
	
add_executable(app ${SOURCES} ${HEADERS})


add_subdirectory(${GLFW_SOURCE_DIR})

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${GLEW_SOURCE_DIR}/include)
	

target_link_libraries(app glfw ${OPENGL_gl_LIBRARY})

set(SHADERS 
	shaders/Fragment/simpleFragment.glsl
	shaders/Vertex/simple3D.glsl
	)

add_definitions(
  -DGLEW_STATIC
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


