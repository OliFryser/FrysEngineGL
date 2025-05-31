# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

project ("FrysEngineGLExample")
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../OpenGL")

set(LIBRARIES_SOURCE_PATH ${CMAKE_SOURCE_DIR}/libraries)
include_directories(
    ${EXTERNAL_DIR}/Include
    ${LIBRARIES_SOURCE_PATH}/frysgl/include
    ${LIBRARIES_SOURCE_PATH}/stb/include
    ${LIBRARIES_SOURCE_PATH}/glm/include
)

link_directories(${EXTERNAL_DIR}/Libs)

# Include sub-projects.
add_subdirectory ("FrysEngineGL")
