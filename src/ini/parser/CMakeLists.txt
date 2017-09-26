set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/block.hpp"
"${DIR}/context.hpp"
"${DIR}/lexer.hpp"
"${DIR}/nugget.hpp"
"${DIR}/parser.hpp" 
"${DIR}/property.hpp" 
"${DIR}/string_util.hpp"
"${DIR}/token.hpp"
"${DIR}/tokenstream.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/lexer.cpp"
"${DIR}/parser.cpp"
"${DIR}/property.cpp"
"${DIR}/token.cpp" 
PARENT_SCOPE)