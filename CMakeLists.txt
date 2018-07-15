CMAKE_MINIMUM_REQUIRED ( VERSION 3.0 )

CMAKE_POLICY ( VERSION 3.0 )

PROJECT ( GPUMonitor )

SET (CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE ( gmonitor
  src/console_writer.cpp
  src/gpu.cpp
  src/main.cpp
  src/manager.cpp
  src/stats_reader.cpp
  )
