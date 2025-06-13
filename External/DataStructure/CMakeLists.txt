# Datastructure

set(DATA_STRUCT DataStructure) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_library(${DATA_STRUCT})

target_sources(${DATA_STRUCT} PRIVATE ${PROJECT_FILES})

target_include_directories(${DATA_STRUCT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_include_directories(${DATA_STRUCT} PRIVATE ${LIB_INCLUDE_DIR})
target_include_directories(${DATA_STRUCT} PRIVATE ${DEPENDENCIES_INCLUDE_DIR})
target_include_directories(${DATA_STRUCT} PRIVATE ${RESOURCES_INCLUDE_DIR})
target_include_directories(${DATA_STRUCT} PRIVATE ${LOW_RENDERER_INCLUDE_DIR})

target_link_libraries(${DATA_STRUCT} PRIVATE ${LIB_NAME})
target_link_libraries(${DATA_STRUCT} PRIVATE ${RESOURCES_NAME})
target_link_libraries(${DATA_STRUCT} PRIVATE ${LOW_RENDERER})
target_link_libraries(${DATA_STRUCT} PRIVATE ${DEPENDENCIES_NAME})

set(DATA_STRUCT ${DATA_STRUCT} PARENT_SCOPE) #outer scope variable only

set(DATA_STRUCT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)