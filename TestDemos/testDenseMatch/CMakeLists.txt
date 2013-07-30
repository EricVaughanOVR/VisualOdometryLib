include_directories(../../CorrespondenceLib ../../FastLib ${OpenCV_INCLUDE})
link_directories(${OpenCV_LIB})

add_executable(testDenseMatch testDenseMatch.cpp)


target_link_libraries(testDenseMatch debug CorrespondenceLib debug FastLib debug opencv_core246d debug opencv_highgui246d)

target_link_libraries(testDenseMatch optimized CorrespondenceLib optimized FastLib optimized opencv_core246 optimized opencv_highgui246)
