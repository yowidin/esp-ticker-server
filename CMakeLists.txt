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

include(ExternalProject)
ExternalProject_Add(
   SOCI
   GIT_REPOSITORY https://github.com/SOCI/soci
   GIT_TAG c5e37d2731b8af732e6efb22b10e9bfc035ded49
   INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/soci_install"
   CMAKE_ARGS 
      -DWITH_SQLITE3=ON
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/soci_install
      -DSOCI_TESTS=OFF
)
add_dependencies(${CMAKE_PROJECT_NAME} SOCI)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/soci_install/cmake)
include(SOCI)

target_link_libraries(${CMAKE_PROJECT_NAME} SOCI::soci_core_static SOCI::soci_mysql_static)
