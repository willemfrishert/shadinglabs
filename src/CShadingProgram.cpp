#include "CShaderProgram.h"

const int KLogLengthMax=500;

CShadingProgram::CShadingProgram()
{
	iShaderProgramId = glCreateProgram();
}

CShadingProgram::~CShadingProgram()
{
	this->Enable( false );
	for (unsigned int i=0;i<iShaders.size();i++)
	{
		glDetachShader(iShaderProgramId, iShaders[i]->iShaderId);
		CHECK_GL_ERROR(); // if you get an error here, you deleted the Program object first and then
		// the iShaderProgramId! Always delete iShaderProgramIds last!
		delete iShaders[i]; 
	}                      

	glDeleteProgram( iShaderProgramId );
	CHECK_GL_ERROR();
}

void CShadingProgram::AddShader(CShader* Shader)
{
	iShaders.push_back(Shader); 
}

bool CShadingProgram::Rebuild()
{
	// Clean
	GLint linked;
	glGetProgramiv(iShaderProgramId, GL_LINK_STATUS, &linked);
	if (GL_TRUE == linked)  // already Linked, detach everything first
	{
		glDeleteProgram( iShaderProgramId );
		iShaderProgramId = glCreateProgram();
	}

	// and Build again
	return Build();
}


bool CShadingProgram::Build()
{
	bool status = true;

	for (unsigned int i=0;i<iShaders.size();i++)
	{
		status = status && iShaders[i]->Compile();

		if (status)
		{
			GLint shaderType = 0;
			glGetShaderiv( iShaders[i]->iShaderId, GL_SHADER_TYPE, &shaderType );

			cout << " - Attaching ";
			if ( GL_VERTEX_SHADER == shaderType )
			{
				cout << "Vertex Shader: ";
			}
			else
			{
				cout << "Fragment Shader: ";
			}
			cout << iShaders[i]->iShaderId << " to Shader program: " << iShaderProgramId << endl;

			glAttachShader( iShaderProgramId, iShaders[i]->iShaderId );
			CHECK_GL_ERROR();
		}
	}

	if (status)
	{
		cout << " - Linking Shader program: " << iShaderProgramId << endl;		
		glLinkProgram( iShaderProgramId );
		CHECK_GL_ERROR();
		status = LinkerLog();
	}

	if (status)
	{
		// get the locations for the uniform objects
		list< ShaderUniformObject* >::iterator it = this->uniformObjects.begin();
		for (; it != this->uniformObjects.end(); it++)
		{
			(*it)->assignLocation( iShaderProgramId );
		}

		// get the locations for the attribute objects
		list< ShaderAttributeObject* >::iterator attribtIter = this->attributeObjects.begin();
		for (; attribtIter != this->attributeObjects.end(); attribtIter++)
		{
			(*attribtIter)->assignLocation( iShaderProgramId );
		}		
	}

	return status;
}

bool CShadingProgram::LinkerLog()
{    
	GLint status = 0;
	glGetProgramiv( iShaderProgramId, GL_LINK_STATUS, &status );

	if( GL_FALSE == status )
	{
		char infoLog[KLogLengthMax];
		glGetProgramInfoLog( iShaderProgramId, KLogLengthMax, NULL, infoLog);
		cout << " - Program Linking FAILED:" << endl << infoLog << endl  << endl;
	}
	else
	{
		cout << " - Program Linking OK." << endl << endl;
	}
	return ( 0 == status ) ? false : true;
}

bool CShadingProgram::Enable(bool aState)
{
	GLint status=0;

	//disable program
	if ( false == aState )
	{
		glUseProgram(0);
	}
	//enable program
	else
	{
		if ( GL_FALSE == glIsProgram(iShaderProgramId) )
		{
			cout << "Error: Shader program is not a valid shader program\n";
			status = GL_FALSE;
		}

		if (GL_FALSE == status)
		{
			// build the shader program in case it has not been linked yet
			glGetProgramiv( iShaderProgramId, GL_LINK_STATUS, &status);
			if (GL_FALSE == status)
			{
				status = this->Rebuild();
			}			
		}

		if (status)
		{	
			glUseProgram(iShaderProgramId);
			UpdateProgramUniformObjects();
			CHECK_GL_ERROR();
		}
	}
	return (status = GL_FALSE ? false : true);
}

void CShadingProgram::DisableAll()
{
	glUseProgram(0);
}

void CShadingProgram::AddUniformObject(ShaderUniformObject* obj)
{
	uniformObjects.push_back( obj );
}

void CShadingProgram::AddAttributeObject(ShaderAttributeObject* obj)
{
	attributeObjects.push_back( obj );
}

void CShadingProgram::UpdateProgramUniformObjects()
{
	list< ShaderUniformObject* >::const_iterator uniformIter = this->uniformObjects.begin();

	for (; uniformIter != this->uniformObjects.end() ; uniformIter++)
	{
		(*uniformIter)->use();
	}
}