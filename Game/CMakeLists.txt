#Game
# set the target name (the visual studio project)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(EXE_NAME ${CURRENT_FOLDER_NAME})
add_executable(${EXE_NAME})

file(GLOB_RECURSE PROJECT_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl

	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++

	${CMAKE_CURRENT_SOURCE_DIR}/*.txt
	${CMAKE_CURRENT_SOURCE_DIR}/*.md)

target_sources(${EXE_NAME} PRIVATE ${PROJECT_FILES})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header/Physics)

# --- Visual Studio Solution Explorer Organization ---

# First, glob for all header files in the main Header directory (excluding Physics for now)
file(GLOB ROOT_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/*.inl"
)

# Then, glob specifically for headers in the Physics subfolder
file(GLOB PHYSICS_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/Physics/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/Physics/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header/Physics/*.inl"
)

# Create a source group for the main Header files (under "Header Files" in VS)
source_group("Header Files" FILES ${ROOT_HEADER_FILES})

# Create a nested source group for Physics headers
# The backslash "Header Files\\Physics" creates the virtual folder structure in VS
source_group("Header Files\\Physics" FILES ${PHYSICS_HEADER_FILES})

set(DATA_STRUCT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/External/DataStructure/Header)
set(LOW_RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/External/LowRenderer/Header)
set(RESOURCES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/External/Resources/Header)

target_include_directories(${EXE_NAME} PRIVATE ${DATA_STRUCT_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${LOW_RENDERER_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${RESOURCES_INCLUDE_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${DEPENDENCIES_INCLUDE_DIR})

set(DATA_STRUCT DataStructure)
set(LOW_RENDERER LowRenderer)
set(RESOURCES_NAME Resources)


target_link_libraries(${EXE_NAME} PRIVATE ${DATA_STRUCT})
target_link_libraries(${EXE_NAME} PRIVATE ${LOW_RENDERER})
target_link_libraries(${EXE_NAME} PRIVATE ${RESOURCES_NAME})
target_link_libraries(${EXE_NAME} PRIVATE ${DEPENDENCIES_NAME})
target_link_libraries(${EXE_NAME} PRIVATE glfw3)
target_link_libraries(${EXE_NAME} PRIVATE opengl32)
target_link_libraries(${EXE_NAME} PRIVATE user32)
target_link_libraries(${EXE_NAME} PRIVATE gdi32)
target_link_libraries(${EXE_NAME} PRIVATE shell32)

set(BUILD_UNIT_TEST OFF)

target_include_directories(${EXE_NAME} PRIVATE ${LIB_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})


add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Shaders"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Shaders"
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Meshes"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Meshes"
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets/Sounds"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/Sounds"
)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/External/Dependencies/Binaries/irrKlang.dll" # Source path of the DLL
            "$<TARGET_FILE_DIR:${EXE_NAME}>/" # Destination: executable's output directory
)
