file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.hpp)

add_executable(${PROJECT} ${SRCS} ${HDRS})

target_link_libraries(${PROJECT} glfw)
target_include_directories(${PROJECT} PRIVATE glfw)