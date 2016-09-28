cmake_minimum_required(VERSION 3.6.1)

project(esp-ticker-server VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${CMAKE_PROJECT_NAME}
   src/main.cc
   src/database/database.cc
)

target_include_directories(
   ${CMAKE_PROJECT_NAME} 
   PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
   PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third_party/asio/asio/include
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ASIO_STANDALONE)
set_target_properties(
   ${CMAKE_PROJECT_NAME} PROPERTIES 
   CXX_STANDARD 14 
   CXX_STANDARD_REQUIRED ON
)

find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

# TODO Add database https://github.com/SOCI/soci
