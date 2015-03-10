 
# Project name
project( Project_OpenGL )

# CMake Configuration.
###################################################################################################
# Find includes in corresponding build directories
set( CMAKE_INCLUDE_CURRENT_DIR ON )
# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC TRUE )
set( CMAKE_AUTOMOC_RELAXED_MODE TRUE )

# Parse files in project.
###################################################################################################
set( SOURCE_FILES )
set( LIBS )

# Core.
#################################################
set( CORE_SRCS
	 ${CMAKE_CURRENT_SOURCE_DIR}/Core/App.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Core/App.h 
	 ${CMAKE_CURRENT_SOURCE_DIR}/Core/GUIState.cpp 
	 ${CMAKE_CURRENT_SOURCE_DIR}/Core/GUIState.h 
	 ${CMAKE_CURRENT_SOURCE_DIR}/Core/Config.h  )

set( SOURCE_FILES ${SOURCE_FILES} ${CORE_SRCS} )
source_group( "Core" FILES ${CORE_SRCS} )

# Utils.
#################################################
set( UTILS_SRCS
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Camera.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Camera.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Shader.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/Shader.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/ShaderProgram.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/ShaderProgram.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/QuadBlit.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/QuadBlit.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/imgui/imgui.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/imgui/imgui.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/imgui/imguiRenderGL3.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/imgui/imguiRenderGL3.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Utils/imgui/stb_truetype.h
 )

set( SOURCE_FILES ${SOURCE_FILES} ${UTILS_SRCS} )

source_group( "Utils" FILES ${UTILS_SRCS} )

# WorldObject
#################################################
set( WO_SRCS
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Cube.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Cube.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Plane.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Plane.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Scene.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Scene.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Object3D.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Object3D.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Light.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/Light.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/AreaLight.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/WorldObject/AreaLight.cpp
 )

set( SOURCE_FILES ${SOURCE_FILES} ${WO_SRCS} )

source_group( "WorldObject" FILES ${WO_SRCS} )

# Renderer
#################################################
set( RENDERER_SRCS
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/GBufferRenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/GBufferRenderer.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/BlitRenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/BlitRenderer.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/IlluminationRenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/IlluminationRenderer.h
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/ShadowRenderer.cpp
	 ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/ShadowRenderer.h )

set( SOURCE_FILES ${SOURCE_FILES} ${RENDERER_SRCS} )

source_group( "Renderer" FILES ${RENDERER_SRCS} )

# Visual Leak Detector.
#################################################
if( MSVC AND OPTION_BUILD_USE_VLD )
	include("${BASE_SOURCE_PATH}/CMake/FindVLD.cmake")
	if( VLD_FOUND )
		message( "VLD FOUND" )
		include_directories( ${VLD_INCLUDE_DIRS} )
		set(LIBS ${LIBS} ${VLD_LIBRARIES})
	endif()
endif()


# Libraries.
###################################################################################################	 
link_directories( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )

# Include directories.
###################################################################################################	

# OPENGL
find_package(OpenGL REQUIRED)

# GLEW
find_package(GLEW REQUIRED)
ADD_DEFINITIONS( -DGLEW_STATIC=1 )

# GLFW

include_directories( ${CMAKE_SOURCE_DIR}/external/glm ) #GLM
include_directories( ${CMAKE_SOURCE_DIR}/external/stb ) #STB
include_directories( ${CMAKE_SOURCE_DIR}/external/glfw/include ) #GLFW
include_directories( ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} )

set( LIBS ${LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} )

# Adding files to exec
set( SOURCE_FILES
	${SOURCE_FILES} 
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )
	
set_source_files_properties( ${SOURCE_FILES} PROPERTIES GENERATED true )

# Create an exec with the sources files
add_executable( ${EXE_NAME} ${SOURCE_FILES} )

target_link_libraries(${EXE_NAME} glfw ${LIBS})
