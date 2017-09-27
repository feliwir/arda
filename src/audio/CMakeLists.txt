set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/audio.hpp"
"${DIR}/audiobuffer.hpp" 
"${DIR}/audiostream.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/audio.cpp" 
"${DIR}/audiobuffer.cpp" 
"${DIR}/audiostream.cpp" 
PARENT_SCOPE)