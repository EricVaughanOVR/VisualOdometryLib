cmake_minimum_required(VERSION 2.8)

project(StereoTracker)

set(OpenCV_INCLUDE c:/opencv/build/include)
set(OpenCV_LIB c:/opencv/build/x86/vc10/lib)
set(Boost_INCLUDE c:/dev/boost_1_54_0/stage/include)
set(Boost_LIB c:/dev/boost_1_54_0/stage/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_COMPILER_IS_GNUCC)
  set(OpenCV_INCLUDE /cygdrive/c/opencv/build/include)
  set(OpenCV_LIB /cygdrive/c/opencv/build/x64/mingw/lib)
  set(Boost_INCLUDE /cygdrive/c/dev/boost_1_54_0/stage/include)
  set(Boost_LIB /cygdrive/c/dev/boost_1_54_0/stage/lib)

  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-march=native")
endif()

add_subdirectory(CorrespondenceLib)
add_subdirectory(FastLib)
add_subdirectory(OdometryLib)
add_subdirectory(TestDemos)
add_subdirectory(TrackerLib)
