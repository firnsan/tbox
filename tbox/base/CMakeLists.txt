set (tbox_base_SRCS
  Logger.cc
  LogStream.cc
  Thread.cc
  TimeStamp.cc
  ThreadPool.cc
  )

add_library(tbox_base ${tbox_base_SRCS})
target_link_libraries(tbox_base pthread)

add_subdirectory(tests)