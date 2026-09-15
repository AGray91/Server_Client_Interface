cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("AGCommon" LANGUAGES CXX)

# Get Source Files...
set(WIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/AGWin32")
set(SERVER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/AGS_Server")

# Seperate Source and Headers...
file(GLOB_RECURSE WIN_HEADERS 	"${WIN_SRC}/*.h")
file(GLOB_RECURSE WIN_SOURCE 	"${WIN_SRC}/*.cpp")
file(GLOB_RECURSE SERV_HEADERS	"${SERVER_SRC}/*.h")
file(GLOB_RECURSE SERV_SOURCE	"${SERVER_SRC}/*.cpp")

# Define Filters
source_group("Header Files\\AGWin32"	FILES ${WIN_HEADERS})
source_group("Source Files\\AGWin32" 	FILES ${WIN_SOURCE})
source_group("Header Files\\AGS_Server"	FILES ${SERV_HEADERS})
source_group("Source Files\\AGS_Server"	FILES ${SERV_SOURCE})

# Define the Executable...
add_library(AGCommon STATIC
	${WIN_HEADERS}
	${WIN_SOURCE}
	${SERV_HEADERS}
	${SERV_SOURCE}
)

target_include_directories(AGCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AGCommon PRIVATE comctl32)
