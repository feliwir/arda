file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.hpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED filesystem iostreams regex)

add_executable(${PROJECT} ${SRCS} ${HDRS})

target_compile_definitions(${PROJECT} PRIVATE ENGINE_NAME="${PROJECT}")
target_link_libraries(${PROJECT} glfw ${Boost_LIBRARIES})
target_include_directories(${PROJECT} PRIVATE 
                            glfw ${Boost_INCLUDE_DIRS}
                            ${PROJECT_ROOT}/deps/cxxopts/include 
                            ${PROJECT_ROOT}/deps/rapidjson/include)