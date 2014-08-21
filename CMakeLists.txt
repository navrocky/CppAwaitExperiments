project(await_test)
cmake_minimum_required(VERSION 2.8)

find_package (Boost REQUIRED COMPONENTS)
include_directories (${Boost_INCLUDE_DIRS})

find_package (Qt4 REQUIRED QtCore)
include(${QT_USE_FILE})

set(CPPAWAIT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../CppAwait)

add_subdirectory(${CPPAWAIT_DIR} ${CMAKE_CURRENT_BINARY_DIR}/CppAwait)

include_directories(${CPPAWAIT_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (--std=gnu++0x -Wno-unused-parameter -Wno-unused-local-typedefs)
endif()

set(SRC_LIST
    main.cpp
)

qt4_automoc(${SRC_LIST})

add_executable(${PROJECT_NAME}
    ${SRC_LIST}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    cpp_await
)
