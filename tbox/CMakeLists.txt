set (tbox_SRCS
  Logger.cc
  LogStream.cc
  )

add_library(tbox ${tbox_SRCS})

add_subdirectory(tests)