# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (wallet)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/dictionary.c
    src/transactions.c
    src/gecko_api.c
    main.c
)

# Add an executable with the above sources
add_executable(wallet ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(wallet
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/inc
)

# CURL
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(wallet ${CURL_LIBRARIES})