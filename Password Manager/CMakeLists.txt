cmake_minimum_required(VERSION 3.27)
project(Password_Manager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion")

find_package(OpenMP REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(Curses REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)
link_directories(/usr/local/lib)
include_directories(/usr/local/include/sqlcipher)

add_executable(Password_Manager main.cpp cryptographyFunctions.cpp miscellaneousFunctions.cpp
		loginScreen.cpp)

target_link_libraries(Password_Manager OpenMP::OpenMP_CXX)
target_link_libraries(Password_Manager cryptopp::cryptopp)
target_link_libraries(Password_Manager ${CURSES_LIBRARIES})
target_link_libraries(Password_Manager sqlite_orm::sqlite_orm)
target_link_libraries(Password_Manager sqlcipher)
