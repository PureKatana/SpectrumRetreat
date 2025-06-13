#LowRenderer

set(LOW_RENDERER LowRenderer) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_library(${LOW_RENDERER})

target_sources(${LOW_RENDERER} PRIVATE ${PROJECT_FILES})

target_include_directories(${LOW_RENDERER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_include_directories(${LOW_RENDERER} PRIVATE ${LIB_INCLUDE_DIR})
target_include_directories(${LOW_RENDERER} PRIVATE ${DEPENDENCIES_INCLUDE_DIR})
target_include_directories(${LOW_RENDERER} PRIVATE ${RESOURCES_INCLUDE_DIR})


target_link_libraries(${LOW_RENDERER} PRIVATE ${LIB_NAME})
target_link_libraries(${LOW_RENDERER} PRIVATE ${DEPENDENCIES_NAME})
target_link_libraries(${LOW_RENDERER} PRIVATE ${RESOURCES_NAME})

set(LOW_RENDERER ${LOW_RENDERER} PARENT_SCOPE) #outer scope variable only

set(LOW_RENDERER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)
