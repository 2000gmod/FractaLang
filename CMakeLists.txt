cmake_minimum_required(VERSION 3.20)

project(FractaLang)

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)

find_package(LLVM 19.1.1 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281
) # 10.2.1

FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum
    GIT_TAG        v0.9.7
) # 10.2.1

FetchContent_MakeAvailable(magic_enum)

set(UtilsSources
    src/Utils/Error.cpp
)

set(ParsingSources
    src/Parsing/Scanner.cpp
    src/Parsing/Token.cpp
    src/Parsing/Parser.cpp
)


set(SOURCES
    src/Main.cpp
    ${ParsingSources}
    ${UtilsSources}
)


add_executable(fractac ${SOURCES})

llvm_map_components_to_libnames(LLVM_LIBS
    core
    support
)

target_link_libraries(fractac PRIVATE ${LLVM_LIBS})
target_link_libraries(fractac PRIVATE fmt::fmt)
target_link_libraries(fractac PRIVATE magic_enum)