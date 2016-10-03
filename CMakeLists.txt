cmake_minimum_required(VERSION 3.6.1)

project(esp-ticker-server VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(MySQL)

add_definitions(-fexceptions -frtti)

add_executable(${CMAKE_PROJECT_NAME}
   src/main.cc
   src/database/database.cc
)


set(ETS_DB_NAME "esp_ticker" CACHE STRING "Database name to store ticker data into")
set(ETS_USERNAME "root" CACHE STRING "Database user name")
set(ETS_PASSWORD "password" CACHE STRING "Database password")
set(ETS_SOCI_PATH "" CACHE PATH "Path to the SOCI.cmake file")
configure_file("${CMAKE_CURRENT_LIST_DIR}/templates/db_settings.h.in" "${CMAKE_CURRENT_BINARY_DIR}/db_settings.h")
configure_file("${CMAKE_CURRENT_LIST_DIR}/templates/index.php.in" "${CMAKE_CURRENT_BINARY_DIR}/index.php" @ONLY)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ASIO_STANDALONE)
set_target_properties(
   ${CMAKE_PROJECT_NAME} PROPERTIES
   CXX_STANDARD 14
   CXX_STANDARD_REQUIRED ON
)

find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

target_include_directories(
   ${CMAKE_PROJECT_NAME}
   PRIVATE ${MYSQL_INCLUDE_DIR}
   PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/
   PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
   PUBLIC third_party/soci/include
   PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third_party/asio/asio/include
)

find_file(soci_file SOCI.cmake PATHS "/usr/local/cmake" "${ETS_SOCI_PATH}")
include(${soci_file})
target_link_libraries(${CMAKE_PROJECT_NAME} SOCI::soci_core_static SOCI::soci_mysql_static)
