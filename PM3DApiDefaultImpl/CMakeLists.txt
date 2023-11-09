cmake_minimum_required(VERSION 3.14)
project(PM3DApiDefaultImpl)

# Add library
add_library(PM3DApiDefaultImpl STATIC
    PM3DApiDefaultImpl.cpp
)

# Include directories
target_include_directories(PM3DApiDefaultImpl PRIVATE
    ${CMAKE_SOURCE_DIR}/PM3DApi
    ${CMAKE_SOURCE_DIR}/PetitMoteur3DCore
)

# Link with PM3DApi
target_link_libraries(PM3DApiDefaultImpl PRIVATE PM3DApi)
