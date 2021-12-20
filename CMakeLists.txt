cmake_minimum_required(VERSION 3.10)
project(graph_statistics)

add_executable(run graph_statistics.cpp)

# Pick the C++ standard to compile with.
# Abseil currently supports C++11, C++14, and C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ABSL_PROPAGATE_CXX_STD ON)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_error LANGUAGES CXX)
if(lto_supported)
  set_target_properties(run PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION true
  )
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(external/abseil-cpp)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(run absl::flat_hash_map absl::flat_hash_set absl::flags absl::flags_parse OpenMP::OpenMP_CXX)
  else()
  target_link_libraries(run absl::flat_hash_map absl::flat_hash_set absl::flags absl::flags_parse)
endif()

target_compile_features(run PUBLIC cxx_std_17)
if(CMAKE_CXX_STANDARD LESS 17)
  message(FATAL_ERROR
      "requires CMAKE_CXX_STANDARD >= 17 (got: ${CMAKE_CXX_STANDARD})")
endif()