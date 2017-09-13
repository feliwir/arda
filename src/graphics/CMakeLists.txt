set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(gl)

if(WIN32)
    option(ARDA_BUILD_D3D "Add a Direct3D rendering backend" true)
    
    if(ARDA_BUILD_D3D)
        add_subdirectory(d3d)
    endif(ARDA_BUILD_D3D)
endif(WIN32)

set(HDRS ${HDRS}
"${DIR}/buffer.hpp"
"${DIR}/drawable.hpp" 
"${DIR}/graphics.hpp" 
"${DIR}/image.hpp" 
"${DIR}/renderer.hpp" 
"${DIR}/sprite.hpp" 
"${DIR}/texture.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/buffer.cpp" 
"${DIR}/graphics.cpp" 
"${DIR}/image.cpp" 
"${DIR}/renderer.cpp"
"${DIR}/sprite.cpp" 
"${DIR}/texture.cpp" 
PARENT_SCOPE)

