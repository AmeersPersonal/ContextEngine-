#TODO: refactor this to the client side and ,ake a seprate one for main and server
cmake_minimum_required(VERSION 3.29)
project(ContextEngine_)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")
set(JPEG_ROOT_DIR "C:/libjpeg-turbo-gcc64")

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
)
FetchContent_MakeAvailable(SFML)

add_executable(ContextEngine_
        src/client/client.cpp
        src/client/client.hpp
        src/main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)

target_link_libraries(ContextEngine_ PRIVATE SFML::Graphics SFML::Audio SFML::Network)


include_directories(${JPEG_ROOT_DIR}/include)
link_directories(${JPEG_ROOT_DIR}/lib)
include_directories(src)
include_directories(client)