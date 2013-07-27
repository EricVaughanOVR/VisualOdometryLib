include_directories(../../CorrespondenceLib) 

add_executable(testStoreSSE16 testStoreSSE16.cpp)

target_link_libraries(testStoreSSE16 debug CorrespondenceLib)
target_link_libraries(testStoreSSE16 optimized CorrespondenceLib)
