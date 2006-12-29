#include "ShaderAttributeObject.h"


ShaderAttributeObject::ShaderAttributeObject(char* aName)
{
	assert( aName != NULL );
	size_t len = strlen(aName) + 1;
	this->iName = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->iName, len, aName);
#else
	strncpy(this->iName, aName, len);
#endif
	this->iIndex = 0;
}

ShaderAttributeObject::~ShaderAttributeObject(void)
{
	free( iName );
}

/**
* @param name
* @param shaderProgram
*/
int ShaderAttributeObject::assignLocation(GLuint aShaderProgramId)
{
	// this function returns a GLint because it can be a -1
	// but the index that is passed to the other functions
	// is a GLuint
	GLint loc = glGetAttribLocation(aShaderProgramId, iName);

	if(loc == -1)
	{
		size_t size = sizeof("Failed to locate attribute variable ");
		size_t total = sizeof(char) * size + strlen(iName) + 1;
		char* str = (char*)malloc( total );
#ifdef WIN32
		strcpy_s(str, size, "Failed to locate attribute variable ");
		strcat_s(str, total, iName);
#else
		strcpy(str, "Failed to locate uniform variable ");
		strcat(str, iName);
#endif
		printf("Binding warning:\n %s\n", str);

		return -1;
	}

	this->iIndex = loc;

	return 0;
}