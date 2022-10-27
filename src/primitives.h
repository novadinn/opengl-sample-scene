#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>

#include "platform.h"

namespace primitives {
    global_variable std::vector<float> cube_positions = {
	-1.0f, -1.0f, -1.0f,  
	1.0f, -1.0f, -1.0f,  
	1.0f,  1.0f, -1.0f,  
	1.0f,  1.0f, -1.0f,  
        -1.0f,  1.0f, -1.0f,  
        -1.0f, -1.0f, -1.0f,  

        -1.0f, -1.0f,  1.0f,  
	1.0f, -1.0f,  1.0f,  
	1.0f,  1.0f,  1.0f,  
	1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f,  1.0f,  
        -1.0f, -1.0f,  1.0f,  

        -1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 

	1.0f,  1.0f,  1.0f,  
	1.0f,  1.0f, -1.0f,  
	1.0f, -1.0f, -1.0f,  
	1.0f, -1.0f, -1.0f,  
	1.0f, -1.0f,  1.0f,  
	1.0f,  1.0f,  1.0f,  

        -1.0f, -1.0f, -1.0f,  
	1.0f, -1.0f, -1.0f,  
	1.0f, -1.0f,  1.0f,  
	1.0f, -1.0f,  1.0f,  
        -1.0f, -1.0f,  1.0f,  
        -1.0f, -1.0f, -1.0f,  

        -1.0f,  1.0f, -1.0f,  
	1.0f,  1.0f, -1.0f,  
	1.0f,  1.0f,  1.0f,  
	1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f, -1.0f 
    };
    global_variable std::vector<float> cube_normals = {
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  

	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  

	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f
    };
    global_variable std::vector<float> cube_tex_coords = {
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	
	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
	
	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
	
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	1.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  1.0f,
	
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	1.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  1.0f
    };

    global_variable std::vector<float> plane_positions = {
	-1.0f,  0.0f, -1.0f,  
	1.0f,  0.0f, -1.0f,  
	1.0f,  0.0f,  1.0f,  
	1.0f,  0.0f,  1.0f,  
        -1.0f,  0.0f,  1.0f,  
        -1.0f,  0.0f, -1.0f 
    };
    global_variable std::vector<float> plane_normals = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
    };
    global_variable std::vector<float> plane_tex_coords = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	1.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  1.0f
    };

    global_variable std::vector<float> square_positions = {
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f
    };
}

#endif
