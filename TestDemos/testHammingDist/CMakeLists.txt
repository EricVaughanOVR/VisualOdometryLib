include_directories(../../CorrespondenceLib)

add_executable(testHammingDist testHammingDist.cpp)

target_link_libraries(testHammingDist debug CorrespondenceLib)

target_link_libraries(testHammingDist optimized CorrespondenceLib)
