#pragma once

#include "Basic.h"
#include "CShader.h"
#include "ShaderUniformObject.h"
#include "ShaderAttributeObject.h"

class CShadingControl
	{
	//METHODS
public:
    CShadingControl();            // Standard Constructor
    ~CShadingControl();           // Destructor

    void AddShader(CShader* Shader); // Add a Vertex or Fragment Program
    
    bool Rebuild();		// Clean program and Compile Shaders;
    bool Build();       // Compile Shaders & Link program
    bool LinkerLog();   // get Linker messages

    bool Enable(bool aState);	//!< use Shader. OpenGL calls will go through shader.
	
	static void DisableAll();
	
	void AddUniformObject(ShaderUniformObject* obj);
	
	void AddAttributeObject(ShaderAttributeObject* obj);	
    
	void UpdateProgramUniformObjects();
	//METHODS
private:
    GLint GetUniLoc(const GLcharARB *name);      // get location of a variable
    
	//VARIABLES
private:	
	GLuint iShaderProgramId;            // Shader Object
 
	std::vector<CShader*> iShaders;     // List of all Shader Programs
	
	list< ShaderUniformObject* > uniformObjects;
	list< ShaderAttributeObject* > attributeObjects;
	};

