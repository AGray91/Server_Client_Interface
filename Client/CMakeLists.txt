cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("AGClient" LANGUAGES CXX)

# Get Source Files...
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
add_executable (AGClient
	${CLIENT_HEADERS}
	${CLIENT_SOURCE}
	${UI_HEADERS}
	${UI_SOURCE}
)

