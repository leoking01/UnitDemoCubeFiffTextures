cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project (  UnitDemoCubeFiffTextures  )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/../Libs/lib)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")


if( 0 )
    set(FreeGLUT_DIR  ../Libs/installFreeGlutWin10Vc14/lib/cmake/FreeGLUT )
    find_package(FreeGLUT  REQUIRED  PATHS  ../Libs/installFreeGlutWin10Vc14  )
    include_directories(../Libs/installFreeGlutWin10Vc14/include )
    link_directories(../Libs/installFreeGlutWin10Vc14/lib)
endif ()

set(LIBS glfw3 opengl32 assimp freetype irrKlang
    #    freeglut freeglut_static
    )



configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries requried for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

macro(makeLink src dest target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()


file(GLOB SOURCE
    "src/*.h"
    "src/*.cpp"
    "src/*.vs"
    "src/*.fs"
    "src/*.gs"
    )

set(exeName   ${PROJECT_NAME} )

add_executable( ${exeName} ${SOURCE})
target_link_libraries( ${exeName} ${LIBS})

set_target_properties(${exeName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/${CHAPTER}")

# copy shader files to build directory
file(GLOB SHADERS
    "src/*.vs"
    # "src/*.frag"
    "src/*.fs"
    "src/*.gs"
    )
foreach(SHADER ${SHADERS})
    if(WIN32)
        # configure_file(${SHADER} "test")
        add_custom_command(TARGET ${exeName} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${exeName}>)
    elseif(UNIX AND NOT APPLE)
        file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/../bin/${CHAPTER})
    elseif(APPLE)
        # create symbolic link for *.vs *.fs *.gs
        get_filename_component(SHADERNAME ${SHADER} NAME)
        makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/../bin/${CHAPTER}/${SHADERNAME} ${NAME})
    endif(WIN32)
endforeach(SHADER)
# if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
if(MSVC)
    configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/$exeName.vcxproj.user @ONLY)
endif(MSVC)



include_directories(${CMAKE_SOURCE_DIR}/../Libs/includes)
