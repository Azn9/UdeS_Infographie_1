cmake_minimum_required(VERSION 3.14)
project(PetitMoteur3DCore)

set(CMAKE_EXECUTABLE_ENABLE_EXPORTS ON)

message(STATUS "")
message(STATUS "========== PetitMoteur3DCore ==========")
message(STATUS "Loading from ${CMAKE_CURRENT_SOURCE_DIR}...") 

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

FILE(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	 "${CMAKE_CURRENT_SOURCE_DIR}/**/*.hpp"
	 "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h"
)

FILE(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	 "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

FILE(GLOB LIBS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	 "${CMAKE_CURRENT_SOURCE_DIR}/../libs/*.lib"
)

message(STATUS "Headers: ${HEADERS}")
message(STATUS "Sources: ${SOURCES}")
message(STATUS "Libs: ${LIBS}")
message(STATUS "=============================")
message(STATUS "")

# Add executable
add_library(PetitMoteur3DCore STATIC
    ${SOURCES}
    ${HEADERS}
    ${CMAKE_SOURCE_DIR}/PetitMoteur3D/PetitMoteur3D.rc
)

# Include directories
target_include_directories(PetitMoteur3DCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/physx_include
    ${CMAKE_SOURCE_DIR}/PM3DApi
)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# Link libraries
target_link_libraries(PetitMoteur3DCore PRIVATE
    dinput8.lib
    dxguid.lib
    d3d11.lib
    d3dcompiler.lib
    dxgi.lib
    PM3DApi
)