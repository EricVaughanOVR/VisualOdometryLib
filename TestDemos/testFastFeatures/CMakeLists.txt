include_directories(${OpenCV_INCLUDE} ../../FastLib ../../CorrespondenceLib)
link_directories(${OpenCV_LIB})

add_executable(testFastFeatures testFastFeatures.cpp)

target_link_libraries(testFastFeatures optimized FastLib optimized opencv_core246 optimized opencv_highgui246 optimized opencv_imgproc246)

target_link_libraries(testFastFeatures debug FastLib debug opencv_core246d debug opencv_highgui246d debug opencv_imgproc246d)
