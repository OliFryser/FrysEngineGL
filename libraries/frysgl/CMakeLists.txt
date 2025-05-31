
SET(target_inc "")
FILE(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR}/include/frysGL ${CMAKE_CURRENT_LIST_DIR}/include/frysGL/*)
FOREACH(subdir ${subdirectories})
	file(GLOB_RECURSE subdir_inc "${CMAKE_CURRENT_LIST_DIR}/include/frysGL/${subdir}/*.h" )
	source_group(${subdir} FILES ${subdir_inc})
	LIST(APPEND target_inc ${subdir_inc})
ENDFOREACH()

SET(target_src "")
FILE(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR}/src/frysGL ${CMAKE_CURRENT_LIST_DIR}/src/frysGL/*)
FOREACH(subdir ${subdirectories})
	file(GLOB_RECURSE subdir_src "${CMAKE_CURRENT_LIST_DIR}/src/frysGL/${subdir}/*.cpp" )
	source_group(${subdir} FILES ${subdir_src})
	LIST(APPEND target_src ${subdir_src})
ENDFOREACH()

#add_library(frysGL STATIC ${target_inc} ${target_src})
