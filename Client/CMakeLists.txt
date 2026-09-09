cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("AGClient" LANGUAGES CXX)

# Get Source Files...
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src")
set(CLIENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src/Client")
set(UI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src/UI")

# Seperate Source and Headers...
file(GLOB_RECURSE CLIENT_HEADERS "${CLIENT_SRC}/*.h")
file(GLOB_RECURSE CLIENT_SOURCE "${CLIENT_SRC}/*.cpp")
file(GLOB_RECURSE UI_HEADERS "${UI_SRC}/*.h")
file(GLOB_RECURSE UI_SOURCE "${UI_SRC}/*.cpp")

# Define Filters
source_group("Header Files\\Client" FILES ${CLIENT_HEADERS})
source_group("Header Files\\UI" FILES ${UI_HEADERS})
source_group("Source Files\\Client" FILES ${CLIENT_SOURCE})
source_group("Source Files\\UI" FILES ${UI_SOURCE})

# Define the Executable...
add_executable(AGClient
	${MAIN_SRC}/Main.cpp
	${CLIENT_HEADERS}
	${CLIENT_SOURCE}
	${UI_HEADERS}
	${UI_SOURCE}
)

# Set Executable type to Win32
set_target_properties(AGClient PROPERTIES WIN32_EXECUTABLE TRUE)

# Link Libraries
target_link_libraries(AGClient PRIVATE AGCommon)
