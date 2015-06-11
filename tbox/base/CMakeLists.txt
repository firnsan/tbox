set (tbox_SRCS
  Logger.cc
  LogStream.cc
  Thread.cc
  TimeStamp.cc
  ThreadPool.cc
  )

add_library(tbox ${tbox_SRCS})
target_link_libraries(tbox pthread)

add_subdirectory(tests)