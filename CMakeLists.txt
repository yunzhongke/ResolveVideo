cmake_minimum_required(VERSION 3.5)
project(ResolveVideo)

set(opencv_DIR "/home/yunzhongke/work/ws_opencv/share/OpenCV")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#find_package(OpenCV REQUIRED)
include(${opencv_DIR}/OpenCVConfig.cmake)
message( STATUS "OpenCV library status:" )              # 输出一下得到的变量
message( STATUS "    version: ${OpenCV_VERSION}" )
message( STATUS "    libraries: ${OpenCV_LIBS}" )
message( STATUS "    include path: ${OpenCV_INCLUDE_DIRS}" )
include_directories(${OpenCV_INCLUDE_DIR})
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} pthread)
