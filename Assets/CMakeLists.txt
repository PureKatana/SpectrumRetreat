#Assets
#get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert
)

file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Meshes/**/*.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/Meshes/**/*.tga
    ${CMAKE_CURRENT_SOURCE_DIR}/Meshes/**/*.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/Meshes/**/*.png
	${CMAKE_CURRENT_SOURCE_DIR}/**/*.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.tga
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.png
)

file(GLOB_RECURSE SOUND_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Sounds/**/*.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/Sounds/**/*.mp3
    ${CMAKE_CURRENT_SOURCE_DIR}/Sounds/**/*.ogg
    ${CMAKE_CURRENT_SOURCE_DIR}/Sounds/**/*.mid
)

file(GLOB_RECURSE LEVEL_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Levels/*.txt

)

# Custom targets pour apparaître dans des dossiers séparés dans Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Assets\\"  FILES ${SHADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Assets\\"  FILES ${ASSET_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Assets\\"  FILES ${SOUND_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Assets\\"  FILES ${LEVEL_FILES})

add_custom_target(AssetsFiles SOURCES 
    ${SHADER_FILES}
    ${ASSET_FILES}
    ${SOUND_FILES}
    ${LEVEL_FILES}
)