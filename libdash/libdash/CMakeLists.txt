
cmake_minimum_required(VERSION 3.3)

set(MODULE_NAME dash)
set (DASH_SOURCE_PATH .)

project(${MODULE_NAME} LANGUAGES CXX)

add_definitions(-DNO_CURL)

file(GLOB_RECURSE libdash_source ${DASH_SOURCE_PATH}/source/*.cpp)
file(GLOB_RECURSE libdash_source_headers ${DASH_SOURCE_PATH}/source/*.h)
file(GLOB_RECURSE libdash_headers ${DASH_SOURCE_PATH}/include/*.h)

source_group("API_HEADERS" FILES ${libdash_headers})

foreach(each ${libdash_source})
    get_filename_component(file_path ${each} DIRECTORY)
    get_filename_component(file_name ${each} NAME)
    string(REPLACE "${DASH_SOURCE_PATH}/source/" "" file_path ${file_path})
    string(REPLACE "${DASH_SOURCE_PATH}/source" "" file_path ${file_path})
    string(REPLACE "/" "\\\\" file_path "${file_path}")
    if("${file_path}" STREQUAL "")
        source_group("" FILES ${each})
    else()
        source_group("${file_path}" FILES ${each})
    endif()
endforeach()

foreach(each ${libdash_source_headers})
    get_filename_component(file_path ${each} DIRECTORY)
    get_filename_component(file_name ${each} NAME)
    string(REPLACE "${DASH_SOURCE_PATH}/source/" "" file_path ${file_path})
    string(REPLACE "${DASH_SOURCE_PATH}/source" "" file_path ${file_path})
    string(REPLACE "/" "\\\\" file_path "${file_path}")
    if("${file_path}" STREQUAL "")
        source_group("" FILES ${each})
    else()
        source_group("${file_path}" FILES ${each})
    endif()
endforeach()


file(GLOB_RECURSE xml_source ${DASH_SOURCE_PATH}/xml/*.cpp)
file(GLOB_RECURSE xml_source_headers ${DASH_SOURCE_PATH}/xml/*.h)
source_group("XmlParser" FILES ${xml_source} ${xml_source_headers})


add_library(${MODULE_NAME} SHARED ${libdash_source} ${libdash_source_headers} ${libdash_headers} ${xml_source} ${xml_source_headers})

target_link_libraries(${MODULE_NAME} ${PLATDEPS})

target_include_directories(${MODULE_NAME} PRIVATE ${DASH_SOURCE_PATH}/include)
target_include_directories(${MODULE_NAME} PRIVATE ${DASH_SOURCE_PATH}/xml)

