###################################################################################################
# Require CMake version 
cmake_minimum_required( VERSION 2.8.11 )

###################################################################################################
# CMake GUI call
if( NOT CMAKE_BUILD_TYPE ) 
	option( OPTION_BUILD_TYPE_DEBUG "Set build type to Debug." ON )
	option( OPTION_BUILD_TYPE_RELEASE "Set build type to Release." OFF )

	# Debug
	if( OPTION_BUILD_TYPE_DEBUG )
		set( OPTION_BUILD_TYPE_RELEASE OFF CACHE BOOL "turn Release off" FORCE )
		set( CMAKE_BUILD_TYPE "Debug;" )
		set( CMAKE_CONFIGURATION_TYPES "Debug;" CACHE STRING "Reset the configurations to what we need" FORCE ) 
	endif( OPTION_BUILD_TYPE_DEBUG )
	# Release
	if( OPTION_BUILD_TYPE_RELEASE )
		set( OPTION_BUILD_TYPE_DEBUG OFF CACHE BOOL "turn Debug off" FORCE )
		set( CMAKE_BUILD_TYPE "Release;" )	
		set( CMAKE_CONFIGURATION_TYPES "Release;" CACHE STRING "Reset the configurations to what we need" FORCE )
	endif( OPTION_BUILD_TYPE_RELEASE )	
	
	
# Command line call
else() 
	SET( OPTION_BUILD_TYPE_DEBUG OFF )
	SET( OPTION_BUILD_TYPE_RELEASE OFF )

	# Debug
	IF( CMAKE_BUILD_TYPE MATCHES Debug )
		SET( OPTION_BUILD_TYPE_DEBUG ON )	
		set( CMAKE_CONFIGURATION_TYPES "Debug;" CACHE STRING "Reset the configurations to what we need" FORCE )
	ENDIF( CMAKE_BUILD_TYPE MATCHES Debug )
	# Release
	IF( CMAKE_BUILD_TYPE MATCHES Release )
		SET( OPTION_BUILD_TYPE_RELEASE ON )
		set( CMAKE_CONFIGURATION_TYPES "Release;" CACHE STRING "Reset the configurations to what we need" FORCE )
	ENDIF( CMAKE_BUILD_TYPE MATCHES Release )
	
endif()

# Prevent the user from selecting both debug and release
if( OPTION_BUILD_TYPE_DEBUG AND OPTION_BUILD_TYPE_RELEASE )
	message( FATAL_ERROR "Debug and Release are both selected -> make up your mind." )
endif()
# Prevent the user from selecting neither debug or release
if( NOT OPTION_BUILD_TYPE_DEBUG AND NOT OPTION_BUILD_TYPE_RELEASE )
	message( FATAL_ERROR "Neither Debug or Release are selected -> please select desired build type." )
endif()

if( MSVC AND OPTION_BUILD_TYPE_DEBUG )
	option( OPTION_USE_VLD "Use Visual Leak Dectector in Debug mode" ON )
endif()

###################################################################################################
# Project name
project( Project_OpenGL )

# Flag / Variables
set( EXE_NAME "Projet_OpenGL" )
set( BASE_SOURCE_PATH "${PROJECT_SOURCE_DIR}" )

# CMake Configuration.
###################################################################################################
# Find includes in corresponding build directories
set( CMAKE_INCLUDE_CURRENT_DIR ON )
# Instruct CMake to run moc automatically when needed.
set( CMAKE_AUTOMOC TRUE )
set( CMAKE_AUTOMOC_RELAXED_MODE TRUE )


###################################################################################################
# FLAGS
# Visual Studio Specific
if( MSVC )
	
	# Warning lvl 3 for Debug builds
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3" )  #Starting with Warning Lvl 3 the compilation is considered as professional compilation. Lvl 4 is warning as error.
	
	# Microsoft is shit
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE )
	
	# No safe environment for iterator testing
	add_definitions( -D_SECURE_SCL=0 -D_HAS_ITERATOR_DEBUGGING=0 )
	
	# Activate macro
	add_definitions( -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS )
	
	add_definitions( -D_UNICODE -DUNICODE ) # _T("string") or L"string" for system call (low lvl)
	add_definitions( -DNOMINMAX )
	
endif( MSVC )

###################################################################################################

# Set default directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_INCLUDE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/include )

# GCC specifics.
#################################################
if( USING_GCC )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -std=c++0x" )
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb"  )
endif( USING_GCC )

# Doxygen documentation.
###################################################################################################
option( OPTION_BUILD_DOCUMENTATION "Generating the doxygen documentation." OFF )
if( OPTION_BUILD_DOCUMENTATION )
	find_package( Doxygen )
	
	if( DOXYGEN_FOUND )
		if( NOT DOXY_IN_FILE_UPDATED )
			if( DOXYGEN_DOT_PATH )
				CONFIGURE_FILE(${BASE_SOURCE_PATH}/doc/doxygen_configSTDWITHDOT.in ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen_config.in @ONLY)    
			else()
				CONFIGURE_FILE(${BASE_SOURCE_PATH}/doc/doxygen_configSTD.in ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen_config.in @ONLY) 
			endif()
			
			set( DOXY_IN_FILE_UPDATED 1 INTERNAL ) 
		endif()
		
		set( DOXYGEN_PROJECT_NAME "Doxygen_Documentation" )
		add_custom_target(${DOXYGEN_PROJECT_NAME} ALL echo DEPENDS EVE_PROJECT)
		add_custom_command(
			TARGET ${DOXYGEN_PROJECT_NAME}
			COMMAND ${DOXYGEN}
			ARGS "${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen_config.in"
			COMMENT   "Doxygen generate the documentation")
	else()
		message( STATUS "Doxygen not found, cannot generate the documentation." )
	endif()
endif()

# Include folders to build process
###################################################################################################
add_subdirectory( external/glfw )
add_subdirectory( src )
