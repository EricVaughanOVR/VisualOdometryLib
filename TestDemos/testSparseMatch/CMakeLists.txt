include_directories(../../CorrespondenceLib ../../FastLib ${OpenCV_INCLUDE})
link_directories(${OpenCV_LIB})

add_executable(testSparseMatch testSparseMatch.cpp)

target_link_libraries(testSparseMatch debug CorrespondenceLib debug FastLib debug opencv_core246d debug opencv_highgui246d debug opencv_features2d246d)

target_link_libraries(testSparseMatch optimized CorrespondenceLib optimized FastLib optimized opencv_core246 optimized opencv_highgui246 optimized opencv_features2d246)
