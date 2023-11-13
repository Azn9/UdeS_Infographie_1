cmake_minimum_required(VERSION 3.14)
project(PetitMoteur3D)

# Add library
add_executable(PetitMoteur3D WIN32
    PetitMoteur3D.cpp
    ${CMAKE_SOURCE_DIR}/PetitMoteur3D/PetitMoteur3D.rc
)

# Include directories
target_include_directories(PetitMoteur3D PRIVATE
    libs/physx_include
    PetitMoteur3DCore
)

# Include directories
target_include_directories(PetitMoteur3D PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/physx_include
    ${CMAKE_SOURCE_DIR}/PetitMoteur3DCore
    ${CMAKE_SOURCE_DIR}/PM3DApi
)

link_directories(${CMAKE_SOURCE_DIR}/libs)

# Link libraries
target_link_libraries(PetitMoteur3D PRIVATE
    debug ${CMAKE_SOURCE_DIR}/libs/debug/effects11d.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/effects11.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysX_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysX_64.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysXCommon_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysXCommon_64.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysXCooking_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysXCooking_64.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysXExtensions_static_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysXExtensions_static_64.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysXFoundation_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysXFoundation_64.lib
    debug ${CMAKE_SOURCE_DIR}/libs/debug/PhysXPvdSDK_static_64.lib optimized ${CMAKE_SOURCE_DIR}/libs/release/PhysXPvdSDK_static_64.lib
    dinput8.lib
    dxguid.lib
    d3d11.lib
    d3dcompiler.lib
    dxgi.lib
    GameTest
    PetitMoteur3DCore
    PM3DApi
)

# Post-build command to copy DLLs
add_custom_command(TARGET PetitMoteur3D POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/dlls" "$<TARGET_FILE_DIR:PetitMoteur3D>"
)

set_target_properties(PetitMoteur3D PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/PetitMoteur3DCore/data"
)

set_property(TARGET PetitMoteur3D PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

add_compile_definitions(
    $<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>
    $<$<NOT:$<CONFIG:Debug>>:_ITERATOR_DEBUG_LEVEL=0>
)
