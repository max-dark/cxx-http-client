cmake_minimum_required(VERSION 3.15)
project(Streamed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# select libs
set(LIBS_LIST)

find_package(Threads REQUIRED)

if (WIN32)
    list(APPEND LIBS_LIST ws2_32)
endif()


add_executable(streamed)

target_sources(streamed
    PRIVATE
        main.cxx

        service/client.cxx service/client.hxx
)

target_include_directories(streamed
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(streamed
    PRIVATE
        ${LIBS_LIST} Threads::Threads
)
