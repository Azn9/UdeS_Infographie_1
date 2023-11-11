cmake_minimum_required(VERSION 3.14)
project(PetitMoteur3D)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
add_definitions(-DUNICODE -D_UNICODE)

# Include subprojects
add_subdirectory(GameTest)
add_subdirectory(PetitMoteur3D)
add_subdirectory(PM3DApi)
add_subdirectory(PM3DApiDefaultImpl)
add_subdirectory(PetitMoteur3DCore)

# Set the startup project
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "PetitMoteur3D")
