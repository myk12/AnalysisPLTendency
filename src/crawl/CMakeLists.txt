add_library(crawl crawl.cpp)

target_link_libraries(crawl PUBLIC pthread nlohmann_json::nlohmann_json)

#add the library tree to the search path for inclue files
target_include_directories(crawl PUBLIC 
			"${PROJECT_SOURCE_DIR}/json"
			)
