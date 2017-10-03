set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED filesystem regex date_time)

find_package(FFMPEG REQUIRED)

find_package(OpenAL REQUIRED)

find_package(OpenGL REQUIRED)

find_package(JPEG REQUIRED)

add_executable(${PROJECT} "core/main.cpp")

string(TOLOWER ${PROJECT} PROJECT_DEST)

install(TARGETS ${PROJECT}
        RUNTIME DESTINATION bin COMPONENT binaries
        LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT binaries
        ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT libraries)

set(SRCS)
set(HDRS)

#Add all components
add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(filesystem)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(gui)
add_subdirectory(ini)
add_subdirectory(map)
add_subdirectory(media)
add_subdirectory(threading)
add_subdirectory(video)

target_sources(${PROJECT} PRIVATE ${SRCS} ${HDRS})

target_compile_definitions(${PROJECT} PRIVATE ENGINE_NAME="${PROJECT}")

target_link_libraries(${PROJECT} 
                        glfw 
                        ${OPENGL_gl_LIBRARY}
                        ${OPENAL_LIBRARY}
                        ${JPEG_LIBRARIES}
                        ${Boost_LIBRARIES} 
                        ${FFMPEG_LIBRARIES})

if(ARDA_BUILD_D3D)
    target_link_libraries(${PROJECT} d3d11)
endif(ARDA_BUILD_D3D)

target_include_directories(${PROJECT} PRIVATE 
                            glfw 
                            ${OPENGL_INCLUDE_DIR}
                            ${OPENAL_INCLUDE_DIR}
                            ${JPEG_INCLUDE_DIR}
                            ${Boost_INCLUDE_DIRS} 
                            ${FFMPEG_INCLUDE_DIRS}
                            ${PROJECT_ROOT}/deps/glm
                            ${PROJECT_ROOT}/deps/gli
                            ${PROJECT_ROOT}/deps/cxxopts/include 
                            ${PROJECT_ROOT}/deps/rapidjson/include)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        target_compile_options(${PROJECT} PRIVATE "/std:c++latest")
    endif()
endif()