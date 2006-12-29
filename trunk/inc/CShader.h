#pragma once

#include "Basic.h"

class CShader
{
	friend class CShadingControl;

	public:
    CShader();
    ~CShader();
    
    int LoadFromFile(char* filename);   //!< read file, if result is 0 everything is ok. -1: File not found, -2: Empty File, -3: no memory
    void LoadFromMemory(const char* program); //!< load program from char array, make sure program is 0 terminated! 
    
    bool Compile(void);         //!< compile program

    bool CompilerLog(void);  //!< get compiler messages
 
	protected:

    GLint iShaderId;         //!< Program Object
    GLubyte* iShaderSource;          //!< ASCII Source-Code
    
    bool iCompiled;            //!< true if compiled
    bool iMemoryAllocated;               //!< true if shader allocated memory
	};

