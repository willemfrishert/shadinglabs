#include "ShaderUniformObject.h"

ShaderUniformObject::ShaderUniformObject(void)
: iHasChanged( true )
{
	this->iName = NULL;
	this->iLocation = -1;
}

ShaderUniformObject::~ShaderUniformObject(void)
{
	if ( iName != NULL)
	{
		free( iName );
	}
}

/**
 * @param name
 * @param shaderProgram
 */
int ShaderUniformObject::assignLocation(GLuint shaderProgram)
{
	if ( iName != NULL)
	{
		this->iLocation = glGetUniformLocation(shaderProgram, iName);
	}
	
	if(this->iLocation == -1)
	{
		size_t size = sizeof("Failed to locate uniform variable ");
		size_t total = sizeof(char) * size + strlen(iName) + 1;
		char* str = (char*)malloc( total );
#ifdef WIN32
		strcpy_s(str, size, "Failed to locate uniform variable ");
		strcat_s(str, total, iName);
#else
		strcpy(str, "Failed to locate uniform variable ");
		strcat(str, iName);
#endif
		printf("Binding warning:\n %s\n", str);

		return -1;
	}

	return 0;
}

/**
* @param name the uniform attribute name
*/
void ShaderUniformObject::setName(char* aName)
{
	size_t len = strlen(aName) + 1;
	this->iName = (char*) malloc(sizeof(char) * len);
#ifdef WIN32
	strcpy_s(this->iName, len, aName);
#else
	strncpy(this->iName, aName, len);
#endif
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint aLocation, float aValue)
{
	glUniform1f(aLocation, aValue);

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint location, int value)
{
	glUniform1i(location, value);

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint aLocation, float* aValue, int aCount)
{
	if ( aCount == 3 )
	{
		glUniform3f(aLocation, aValue[0], aValue[1], aValue[2]);
	} else if (aCount == 4)
	{
		glUniform4f(aLocation, aValue[0], aValue[1], aValue[2], aValue[3]);
	} else
	{
		glUniform1fv(aLocation, aCount, aValue);
	}

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}

/**
 * @param location openGL variable location
 * @param value the value
 * @param count the number of elements
 */
void ShaderUniformObject::setUniform(GLint aLocation, GLint* aValue, GLsizei aCount )
{
	if ( aCount == 3 )
	{
		glUniform3i(aLocation, aValue[0], aValue[1], aValue[2]);
	} else if (aCount == 4)
	{
		glUniform4i(aLocation, aValue[0], aValue[1], aValue[2], aValue[3]);
	} else
	{
		glUniform1iv(aLocation, aCount, aValue);
	}

	// mark it as not changed because it has been registered now
	this->setHasChanged( false );
}