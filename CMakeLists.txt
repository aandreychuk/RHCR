project(lifelong)

cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Wextra" )
set( CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++" )

IF(NOT CMAKE_BUILD_TYPE)
    #SET(CMAKE_BUILD_TYPE "DEBUG")
    SET(CMAKE_BUILD_TYPE "RELEASE")
    #SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
    #SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

# Find Boost
set(Boost_USE_STATIC_LIBS   ON)
find_package( Boost 1.49.0 REQUIRED COMPONENTS program_options system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )


add_executable(lifelong ${SOURCES} )
target_link_libraries(lifelong PRIVATE ${Boost_LIBRARIES})


