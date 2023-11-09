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
    ${CMAKE_SOURCE_DIR}/libs/effects11d.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysX_64.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysXCommon_64.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysXCooking_64.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysXExtensions_static_64.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysXFoundation_64.lib
    ${CMAKE_SOURCE_DIR}/libs/PhysXPvdSDK_static_64.lib
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
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/PetitMoteur3DCore/data" "$<TARGET_FILE_DIR:PetitMoteur3D>"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/PetitMoteur3DCore/shader" "$<TARGET_FILE_DIR:PetitMoteur3D>"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/PetitMoteur3DCore/DefaultObj" "$<TARGET_FILE_DIR:PetitMoteur3D>"
)

set_target_properties(PetitMoteur3D PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:PetitMoteur3D>"
)
