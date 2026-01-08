# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.30)

project (FrysEngineGL DESCRIPTION "An simple OpenGL engine"
                      LANGUAGES CXX C)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        # Update submodules as needed
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        endif()
    endif()

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/CMakeLists.txt")
        message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
    endif()

    set(CMAKE_CXX_STANDARD 23)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()


# ----- DEPENDENCIES  ----
add_subdirectory(extern)


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

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Include sub-projects.
add_subdirectory("frysgl")
add_subdirectory("FrysEngineGL")
