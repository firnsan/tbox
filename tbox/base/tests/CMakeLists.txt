add_executable(Logger_test Logger_test.cc)
target_link_libraries(Logger_test tbox)

add_executable(Thread_test Thread_test.cc)
target_link_libraries(Thread_test tbox)

add_executable(BlockingQueue_test BlockingQueue_test.cc)
target_link_libraries(BlockingQueue_test tbox)

add_executable(ThreadPool_test ThreadPool_test.cc)
target_link_libraries(ThreadPool_test tbox)