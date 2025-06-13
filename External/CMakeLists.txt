

add_subdirectory(Dependencies)
add_subdirectory(Resources)
add_subdirectory(LowRenderer)
add_subdirectory(DataStructure)


# Create an interface library to aggregate all sub-libraries
add_library(External INTERFACE)

# Optional: For Visual Studio folders
set_target_properties(External PROPERTIES FOLDER "External")

target_include_directories(External INTERFACE
	${DATA_STRUCT_INCLUDE_DIR}
	${LOW_RENDERER_INCLUDE_DIR}
	${RESOURCES_INCLUDE_DIR}
	${DEPENDENCIES_INCLUDE_DIR}
)

target_link_libraries(External INTERFACE

	${DATA_STRUCT}
	${LOW_RENDERER}
	${RESOURCES_NAME}  
	${DEPENDENCIES_NAME}
)

set(EXTERNAL External PARENT_SCOPE)
