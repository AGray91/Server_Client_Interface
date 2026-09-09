cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("AGServer" LANGUAGES CXX)

# Get Source Files...
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src")
set(SERVER_SRC 	"${CMAKE_CURRENT_SOURCE_DIR}/Src/Server")
set(UI_SRC 	"${CMAKE_CURRENT_SOURCE_DIR}/Src/UI")

# Seperate Source and Headers...
file(GLOB_RECURSE SERVER_HEADERS 	"${SERVER_SRC}/*.h")
file(GLOB_RECURSE SERVER_SOURCE 	"${SERVER_SRC}/*.cpp")
file(GLOB_RECURSE UI_HEADERS 		"${UI_SRC}/*.h")
file(GLOB_RECURSE UI_SOURCE 		"${UI_SRC}/*.cpp")

# Define Filters
source_group("Header Files\\Server" 	FILES ${SERVER_HEADERS})
source_group("Header Files\\UI" 	FILES ${UI_HEADERS})
source_group("Source Files\\Server" 	FILES ${SERVER_SOURCE})
source_group("Source Files\\UI" 	FILES ${UI_SOURCE})

# Define the Executable...
add_executable(AGServer
	${MAIN_SRC}/Main.cpp
	${SERVER_HEADERS}
	${SERVER_SOURCE}
	${UI_HEADERS}
	${UI_SOURCE}
)

# Set Executable type to Win32
set_target_properties(AGServer PROPERTIES WIN32_EXECUTABLE TRUE)

# Link Libraries
target_link_libraries(AGServer PRIVATE AGCommon)
