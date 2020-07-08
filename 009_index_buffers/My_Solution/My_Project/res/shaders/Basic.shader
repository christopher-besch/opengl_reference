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


void main()
{
   color = vec4(0.35, 0.27, 0.9, 1.0);
};
