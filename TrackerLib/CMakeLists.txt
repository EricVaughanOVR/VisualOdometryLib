include_directories(${OpenCV_INCLUDE} ../CorrespondenceLib ../OdometryLib)
link_directories(${OpenCV_LIB})

add_library(TrackerLib TrackerIfc.h Tracker.h Tracker.cpp Camera.hpp Sensor.hpp)

target_link_libraries(TrackerLib optimized CorrespondenceLib optimized OdometryLib optimized opencv_core246 optimized opencv_highgui246)

target_link_libraries(TrackerLib debug CorrespondenceLib debug OdometryLib debug opencv_core246 debug opencv_highgui246)
