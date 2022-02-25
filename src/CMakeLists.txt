cmake_minimum_required(VERSION 3.10)

#project name
project(GoAnalysis VERSION 1.0)

#specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(Config.h.in Config.h)

#option(USE_CRAWL "Use my own crawl library" ON)

#add sub libray
add_subdirectory(crawl)
add_subdirectory(utils)
list(APPEND EXTRA_LIBS crawl)
list(APPEND EXTRA_LIBS utils)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/crawl")
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/utils")

#port json library
include(FetchContent)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

#executable
add_executable(GoAnalysis main.cpp)

target_link_libraries(GoAnalysis PUBLIC ${EXTRA_LIBS} pthread nlohmann_json::nlohmann_json)

#add the library tree to the search path for include files
target_include_directories(GoAnalysis PUBLIC
			"${PROJECT_BINARY_DIR}"
			${EXTRA_INCLUDES}
			) 
