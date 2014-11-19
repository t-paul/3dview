cmake_minimum_required(VERSION 3.0.0)

project(3dview)

include(CheckIncludeFileCXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

find_package(PkgConfig)
pkg_check_modules(PC_ASSIMP REQUIRED assimp)
include_directories(${PC_ASSIMP_INCLUDES})

check_include_file_cxx("glm/glm.hpp" HAVE_GLM_H)
if(NOT HAVE_GLM_H)
  message(FATAL_ERROR "Can't find required glm library (see http://glm.g-truc.net/)")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(src)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5OpenGL_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5OpenGL_EXECUTABLE_COMPILE_FLAGS}")

file (GLOB Headers src/*.h ui/*.h)
file (GLOB Sources src/*.cpp ui/*.cpp)
file (GLOB UIs ui/*.ui)
file (GLOB Resources resources/*.qrc)

qt5_add_resources(ResourceSources ${Resources})

add_executable(3dview ${Sources} ${Headers} ${ResourceSources})

qt5_use_modules(3dview Widgets OpenGL)
target_link_libraries(3dview ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${PC_ASSIMP_LIBRARIES})
