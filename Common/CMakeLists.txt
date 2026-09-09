cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("AGCommon" LANGUAGES CXX)

# Get Source Files...
set(WIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Src/Win")

# Seperate Source and Headers...
file(GLOB_RECURSE WIN_HEADERS 	"${WIN_SRC}/*.h")
file(GLOB_RECURSE WIN_SOURCE 	"${WIN_SRC}/*.cpp")

# Define Filters
source_group("Header Files\\Win"	FILES ${WIN_HEADERS})
source_group("Source Files\\Win" 	FILES ${WIN_SOURCE})

# Define the Executable...
add_library(AGCommon STATIC
	${WIN_HEADERS}
	${WIN_SOURCE}
)

target_include_directories(AGCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
