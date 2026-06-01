cmake_minimum_required(VERSION 3.16)
project(Denia_Emits_Nonstop_Intrumentals_AnyServers  VERSION 1.0 LANGUAGES CXX)

# these set up like backbone of project. i dont think needed change

# Chnage to the version u want
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# If include something then un comment these

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(FETCHCONTENT_QUIET OFF)
# include(FetchContent)

# example fetching external library
# FetchContent_Declare(
#    nlohmann_json
#    GIT_REPOSITORY https://github.com/nlohmann/json.git
#    GIT_TAG v3.11.3
#)
# FetchContent_MakeAvailable(nlohmann_json)

# example using installed library
find_package(dpp REQUIRED) # this tella cmake to must find it or else crash the program

# Set source file to that destination. souldn't be fix idk
set(SOURCE_FILES src/main.cpp)
add_executable(Denia ${SOURCE_FILES})
set_target_properties(Denia PROPERTIES OUTPUT_NAME "Denia")


target_link_libraries(Denia PRIVATE
    dpp::dpp
)
