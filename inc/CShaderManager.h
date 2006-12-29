#pragma once

#include "Basic.h"
#include "CShaderProgram.h"
//#include "temp.h"

//-----------------------------------------------------------------------------
// To simplify the process loading/compiling/linking shaders I created this
// high level interface to setup a vertex/fragment shader.

class CShaderManager
{
public:
    CShaderManager();
    ~CShaderManager();

    CShaderProgram* LoadFromFile(char* vertexFile, char* fragmentFile);    // load vertex/fragment shader from file
    CShaderProgram* LoadFromMemory(const char* vertexMem, const char* fragmentMem);

    bool Free(CShaderProgram* o);
		
private:
    std::vector<CShaderProgram*>  _shaderProgramList;
};