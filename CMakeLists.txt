cmake_minimum_required (VERSION 2.6)
project (Main)

set (SFML_DIR "c:/Coding_Packages/SFML/SFML-2.2" CACHE STRING "SFML Directory")

include_directories("${SFML_DIR}/include")
link_directories("${SFML_DIR}/lib")
include_directories("${CMAKE_BINARY_DIR}/headers")

if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/win")
endif(WIN32)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/linux")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  
file(GLOB ALL_SOURCES "sources/*.cpp")
file(GLOB ALL_HEADERS "headers/*.h")


foreach (single_source ${ALL_SOURCES})
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -pedantic")
endforeach()

# add the executable
add_executable(Main ${ALL_SOURCES} ${ALL_HEADERS})


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries (Main lsfml-system lsfml-window lsfml-graphics lsfml-audio)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(WIN32)
  target_link_libraries (Main sfml-system-2 sfml-window-2 sfml-graphics-2 sfml-audio-2)
endif(WIN32)

