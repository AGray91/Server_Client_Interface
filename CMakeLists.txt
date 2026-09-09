cmake_minimum_required(VERSION 3.10)
project(AGNetworkTool)

add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(Common)

# Set Server as startup project...
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AGServer)