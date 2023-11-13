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

set_property(TARGET PM3DApiDefaultImpl PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

add_compile_definitions(
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>
    $<$<NOT:$<CONFIG:Debug>>:_ITERATOR_DEBUG_LEVEL=0>
)
