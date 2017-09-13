set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(blocks)
add_subdirectory(parser)

set(HDRS ${HDRS}
"${DIR}/ini.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/ini.cpp" 
PARENT_SCOPE)