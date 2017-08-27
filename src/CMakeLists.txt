file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE HDRS *.hpp *.h)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED filesystem iostreams regex)

find_package(FFMPEG REQUIRED)

find_package(OpenAL REQUIRED)

find_package(OpenGL REQUIRED)

find_package(JPEG REQUIRED)

add_executable(${PROJECT} ${SRCS} ${HDRS})

target_compile_definitions(${PROJECT} PRIVATE ENGINE_NAME="${PROJECT}")
target_link_libraries(${PROJECT} 
                        glfw 
                        ${OPENGL_gl_LIBRARY}
                        ${OPENAL_LIBRARY}
                        ${JPEG_LIBRARIES}
                        ${Boost_LIBRARIES} 
                        ${FFMPEG_LIBRARIES})

target_include_directories(${PROJECT} PRIVATE 
                            glfw 
                            ${OPENGL_INCLUDE_DIR}
                            ${OPENAL_INCLUDE_DIR}
                            ${JPEG_INCLUDE_DIR}
                            ${Boost_INCLUDE_DIRS} 
                            ${FFMPEG_INCLUDE_DIRS}
                            ${PROJECT_ROOT}/deps/cxxopts/include 
                            ${PROJECT_ROOT}/deps/rapidjson/include)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(${PROJECT} PRIVATE "/std:c++latest")
    endif()
endif()