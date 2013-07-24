include_directories(../../CorrespondenceLib ${OpenCV_INCLUDE} ${Boost_INCLUDE})
link_directories(${OpenCV_LIB} ${Boost_LIB})

add_executable(testCensusTransformSSE testCensusTransformSSE.cpp)

target_link_libraries(testCensusTransformSSE debug CorrespondenceLib debug opencv_core246d debug opencv_highgui246d)
target_link_libraries(testCensusTransformSSE optimized CorrespondenceLib optimized opencv_core246 optimized opencv_highgui246)
