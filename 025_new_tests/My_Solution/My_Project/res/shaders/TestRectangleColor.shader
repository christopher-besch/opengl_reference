#shader vertex
//            <- no deprecated functions allowed
#version 330 core

//                 <- attribute index
//                       <- casting vec 2 to vec4
layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};





#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_tex_coord;

//           <- uniform variable
uniform vec4 u_color;


void main()
{
	color = u_color;
};
