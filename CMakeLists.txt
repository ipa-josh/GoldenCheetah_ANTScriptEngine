cmake_minimum_required(VERSION 2.8.11)

project(fec)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets COMPONENTS QtCore QtScript REQUIRED)

# Tell CMake to create the helloworld executable
add_executable(fec WIN32 main.cpp
	ANT/ANT.cpp
	ANT/ANTMessage.cpp
	ANT/ANTChannel.cpp
	ANT/ANTLogger.cpp
	DeviceTypes.cpp
	DeviceConfiguration.cpp
	RealtimeData.cpp
	CalibrationData.cpp
	Settings.cpp
	RegularEvent.cpp
)

# Use the Widgets module from Qt 5.
target_link_libraries(fec Qt5::Widgets Qt5Script)
