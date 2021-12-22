cmake_minimum_required(VERSION 3.0)
project(kvell)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")


include_directories(.)
include_directories($ENV{BOOST_ROOT})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/thirdpart/lib)

file(GLOB LIB_SRC
    "src/*"
)

# set(LIB_SRC
#     src/log.cpp
#     src/util.cpp
#     src/env.cpp
#     src/config.cpp
# )

set(LIBS
    kvell
    yaml-cpp
    pthread
    dl
)

add_library(kvell SHARED ${LIB_SRC})
kvell_add_executable(test_log "test/test_log.cpp" kvell "${LIBS}")
kvell_add_executable(test_config "test/test_config.cpp" kvell "${LIBS}")
kvell_add_executable(test_hook "test/test_hook.cpp" kvell "${LIBS}")
kvell_add_executable(test_scheduler "test/test_scheduler.cpp" kvell "${LIBS}")
kvell_add_executable(test_thread "test/test_thread.cpp" kvell "${LIBS}")
kvell_add_executable(test_fiber "test/test_fiber.cpp" kvell "${LIBS}")
kvell_add_executable(test_iomanager "test/test_iomanager.cpp" kvell "${LIBS}")
#add_executable(test_log "test/test_log.cpp")
#add_dependencies(test_log kvell)
#target_link_libraries(test_log kvell yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)