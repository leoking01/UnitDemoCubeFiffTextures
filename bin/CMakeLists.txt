cmake_minimum_required(VERSION 3.10.0)

project(07_Texture CXX)

include(CMakePrintHelpers)

include(../cmake/Find3rdLibs.cmake)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}  GL::GLLIBS)
execute_process(COMMAND sh -c "cp ${PROJECT_SOURCE_DIR}/../prebuilts/dll/*.dll  ${PROJECT_SOURCE_DIR}/")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
