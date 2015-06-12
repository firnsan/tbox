set (tbox_net_SRCS
  EventLoop.cc
  Poller.cc
  Channel.cc
  poller/PollPoller.cc
  )

add_library(tbox_net ${tbox_net_SRCS})
target_link_libraries(tbox_net pthread)

add_subdirectory(tests)