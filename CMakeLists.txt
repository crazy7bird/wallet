# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (wallet)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES_WALLET
    src/dictionary.c
    src/transactions.c
    src/gecko_api.c
    src/token_search.c
    wallet.c
)

set(SOURCES_ADD
    src/dictionary.c
    src/transactions.c
    src/gecko_api.c
    src/token_search.c
    add.c
)

set(SOURCES_SEARCH
    src/dictionary.c
    src/transactions.c
    src/gecko_api.c
    src/token_search.c
    search.c
)

# Add an executable with the above sources
add_executable(wallet ${SOURCES_WALLET})
add_executable(search ${SOURCES_SEARCH})
add_executable(add ${SOURCES_ADD})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(wallet
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/inc
)
target_include_directories(search
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/inc
)
target_include_directories(add
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/inc
)

# CURL
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(wallet ${CURL_LIBRARIES})
target_link_libraries(search ${CURL_LIBRARIES})
target_link_libraries(add ${CURL_LIBRARIES})

