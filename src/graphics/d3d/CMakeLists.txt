set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(DirectX REQUIRED)

target_include_directories(${PROJECT} PRIVATE ${DirectX_INCLUDE_DIR})

target_compile_definitions(${PROJECT} PRIVATE ${PROJECT}_USE_D3D)

set(HDRS ${HDRS}
"${DIR}/d3d_buffer.hpp"
"${DIR}/d3d_layout.hpp"
"${DIR}/d3d_renderer.hpp" 
"${DIR}/d3d_sprite.hpp" 
"${DIR}/d3d_texture.hpp"
"${DIR}/d3d_util.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/d3d_buffer.cpp"
"${DIR}/d3d_layout.cpp"
"${DIR}/d3d_renderer.cpp" 
"${DIR}/d3d_sprite.cpp" 
"${DIR}/d3d_texture.cpp"
PARENT_SCOPE)