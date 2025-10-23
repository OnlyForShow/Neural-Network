cmake_minimum_required(VERSION 3.10)


project(Neural CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Neural 
		main.cpp
		Visual.cpp
		)

target_link_directories(Neural PRIVATE
	${CMAKE_SOURCE_DIR}/lib
)

target_include_directories(Neural PRIVATE
			${CMAKE_SOURCE_DIR}/include
			)

target_link_libraries(Neural PRIVATE
			raylib
		     )
		
		
		
