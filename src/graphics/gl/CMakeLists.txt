set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/flextGL.h"
"${DIR}/gl_buffer.hpp" 
"${DIR}/gl_renderer.hpp" 
"${DIR}/gl_shader.hpp" 
"${DIR}/gl_sprite.hpp" 
"${DIR}/gl_texture.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/flextGL.c"
"${DIR}/gl_buffer.cpp" 
"${DIR}/gl_renderer.cpp" 
"${DIR}/gl_shader.cpp" 
"${DIR}/gl_sprite.cpp" 
"${DIR}/gl_texture.cpp" 
PARENT_SCOPE)