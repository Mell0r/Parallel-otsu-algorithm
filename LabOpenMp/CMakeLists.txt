cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

set(Name "LabOpenMP")
set(Main "src/hard.cpp")
set(Sources "src/hard.cpp")
set(IncludeDirectory "include")

project(${Name})

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
   FIND_PACKAGE(OpenMP)
   IF(OPENMP_FOUND)
       SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   ENDIF()
ENDIF()

add_executable(${Name} ${Main} ${Sources} "src/bit_io.cpp" "src/max_levels_calculators.cpp")

target_include_directories(${Name} PUBLIC ${IncludeDirectory})