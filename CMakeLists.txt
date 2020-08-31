cmake_minimum_required(VERSION 2.8)
project(sample)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


set(CMAKE_CXX_FLAGS "-g -std=c++11")

find_package( OpenCV REQUIRED )
find_package(InferenceEngine 2.1 REQUIRED)
if(InferenceEngine_FOUND)
    message(STATUS "IE is found")
endif()
# spdlog for logging 
if(NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()
# YAML-CPP to read YAML config file
find_package( yaml-cpp REQUIRED )


SET (IE_LIBRARY ${InferenceEngine_LIBRARIES})
# SET (IE_CPU_EXT "IE::ie_cpu_extension")
SET (IE_INCLUDE_DIRS ${InferenceEngine_INCLUDE_DIRS})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Winit-self")
####################################
## to use C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()
####################################

set (GFLAGS_IS_SUBPROJECT TRUE)
set (HAVE_SYS_STAT_H 1)
set (HAVE_INTTYPES_H 1)
set (INTTYPES_FORMAT C99)
set (BUILD_TESTING OFF)

####################### add gflags directory ####################### 
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gflags")
    MESSAGE(STATUS "ADDED GFLAGS")       
    add_subdirectory(thirdparty/gflags)
endif()

####################### add format reader ###########################
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/common/format_reader")
    MESSAGE(STATUS "ADEED FORMAT READER")       
    add_subdirectory(common/format_reader)
endif()



file(GLOB
      SOURCE
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/ConfigReader/*.cpp
)
add_executable(core ${SOURCE})
target_include_directories(core PUBLIC ${IE_INCLUDE_DIRS})
target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")
target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gflags")
target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/format_reader")

target_link_libraries(core  ${OpenCV_LIBS}
                            pthread
                            ${IE_LIBRARY} #openvino
                            ${IE_CPU_EXT} #openvino
                            gflags
                            format_reader
                            spdlog
                            yaml-cpp
)