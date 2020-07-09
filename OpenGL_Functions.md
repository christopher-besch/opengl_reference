# OpenGl Functions

## glGetString
return a string describing the current GL connection

## glGenBuffers
generate buffer object names

## glBindBuffer
bind (select -> for use) a named buffer object to a specified target

buffer == 0 -> any previously bound buffer object gets unbound -> frees memory

buffer are local to the shared object space of the current GL rendering context

## glBufferData
creates and initializes / overwrites a buffer object's data store

data == NULL -> no data gets copied

buffer object currently bound to target is used

## glEnableVertexAttribArray
enable a generic vertex attribute array

## glVertexAttribPointer
define an array of generic vertex attribute data

## glCreateShader
creates a shader object

## glShaderSource
replaces the source code in a shader object

## glCompileShader
compiles a shader object

## glGetShaderiv
returns a parameter from a shader object

## glGetShaderInfoLog
returns the information log for a shader object

in length the length of the output string (excluding NULL termination character) gets returned, unless length is set to NULL

## glDeleteShader
deletes a shader object

undoes glCreateShader

shader objects that are attached to a program doesn't get immediately deleted -> gets flagged for deletion when detached

## glCreateProgram
creates a program object

program objects are used to link (multiple) shader together

## glAttachShader
attaches a shader object to a program object -> this shader will be included in the link operations that will be performed on the specified program


## glLinkProgram
links a program object

when the program is currently in use:
- linking successful -> new executables will get installed
- linking not successful -> old executables will stay and the programs link status will be set to GL_FALSE

##glValidateProgram
validates a program object

typically useful only during development

## glUseProgram
installs a program object as part of current rendering state

## glClear
clear buffers to preset values

## glDrawArrays
render primitives from array data

## glDeleteProgram
deletes a program object

undoes glCreateProgram

when the program is currently in use -> flagged for deletion -> gets deleted when not in use any more

## glDrawElements
render primitives from array data

## glDebugMessageCallback
specify a callback to receive debugging messages from the GL

## glEnable
enable server-side GL capabilities

## glGetError
return error information

should be called repeatedly -> until returns GL_NO_ERROR

## glGetUniformLocation
returns the location of a uniform variable

querying a specified uniform variable in a specified program

has to be called for each element <- when querying an array

has to be used after linking

## glUniform4f
specify the value of a uniform variable for the current program object

operates on the currently used program

## glGenVertexArrays
generate vertex array object names

## glBindVertexArray
bind a vertex array object
