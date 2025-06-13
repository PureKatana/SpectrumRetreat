# Dependencies

get_filename_component(DEPENDENCIES_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
#set(DEPENDENCIES_NAME Depecencies) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c
)
list(FILTER PROJECT_FILES EXCLUDE REGEX ${CMAKE_CURRENT_BINARY_DIR})

add_library(${DEPENDENCIES_NAME})

# --- GLFW3 Dependency ---
# GLFW3 is generally a static library, so 'STATIC IMPORTED GLOBAL' is correct.
# It's good practice to specify the full .lib name for clarity and robustness.
add_library(glfw3 STATIC IMPORTED GLOBAL)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib" # Changed from *.lib to glfw3.lib
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Header"
)

# --- irrKlang Dependency ---
# THIS IS THE KEY CHANGE: irrKlang is a DLL, so it must be defined as 'SHARED'
add_library(irrKlang SHARED IMPORTED GLOBAL) # CHANGED from STATIC to SHARED
set_target_properties(irrKlang PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/irrKlang.lib" # This is the .lib file for linking
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/irrKlang.lib" # Explicitly specifying the import library
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Header"
)

target_sources(${DEPENDENCIES_NAME} PRIVATE ${PROJECT_FILES})

target_include_directories(${DEPENDENCIES_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)

# Link your DEPENDENCIES_NAME library with glfw3 and irrKlang
target_link_libraries(${DEPENDENCIES_NAME} PRIVATE glfw3 irrKlang)

set(DEPENDENCIES_NAME ${DEPENDENCIES_NAME} PARENT_SCOPE) #outer scope variable only

set(DEPENDENCIES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)