set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/all.hpp"
"${DIR}/ini_gamedata.hpp"
"${DIR}/ini_mappedImage.hpp" 
"${DIR}/ini_music.hpp"
"${DIR}/ini_speech.hpp" 
"${DIR}/ini_video.hpp" 
"${DIR}/ini_weapon.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/ini_gamedata.cpp"
"${DIR}/ini_mappedImage.cpp" 
"${DIR}/ini_music.cpp"
"${DIR}/ini_speech.cpp" 
"${DIR}/ini_video.cpp" 
"${DIR}/ini_weapon.cpp" 
PARENT_SCOPE)