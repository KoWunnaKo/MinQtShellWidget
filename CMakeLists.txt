#***************************************************************************
#*   copyright       : (C) 2020 by Transporter                             *
#*   https://github.com/OgreTransporter/MinQtShellWidget                   *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#***************************************************************************

cmake_minimum_required(VERSION 3.10.0)

project(MinQtShellWidget)
if(NOT APPLE)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(MinQtShellWidget
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp
    main.cpp
    qshell.h
	qshell.cpp
)

target_link_libraries(MinQtShellWidget Qt5::Widgets)

if(WIN32)
	set(deploy_cmd "execute_process(COMMAND \"${WINDEPLOYQT_EXECUTABLE}\" --no-compiler-runtime --pdb")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		string(APPEND deploy_cmd " --debug")
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		string(APPEND deploy_cmd " --release-with-debug-info")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		string(APPEND deploy_cmd " --release")
	else()
	endif()
	string(APPEND deploy_cmd " \"${CMAKE_INSTALL_PREFIX}/MinQtShellWidget.exe\" WORKING_DIRECTORY \"${_qt_bin_dir}\")")
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    install(CODE ${deploy_cmd})
	if(MSVC)
		set(QT_USE_IMPORTED_TARGETS TRUE)
		set(QT_BIN_DIR ${_qt5Core_install_prefix}/bin)
		set(MSVC_QT_USER ${CMAKE_CURRENT_BINARY_DIR}/MinQtShellWidget.vcxproj.user)
		file(WRITE ${MSVC_QT_USER} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
		file(APPEND ${MSVC_QT_USER} "<Project ToolsVersion=\"12.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n")
		file(APPEND ${MSVC_QT_USER} "  <PropertyGroup>\n")
		file(APPEND ${MSVC_QT_USER} "    <LocalDebuggerEnvironment>PATH=${QT_BIN_DIR};%PATH%</LocalDebuggerEnvironment>\n")
		file(APPEND ${MSVC_QT_USER} "  </PropertyGroup>\n")
		file(APPEND ${MSVC_QT_USER} "</Project>\n")
		set_target_properties(MinQtShellWidget PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS_RELEASE "/ENTRY:mainCRTStartup"
		)
	endif() 
endif()
