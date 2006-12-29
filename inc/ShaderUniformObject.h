#pragma once
#include "Basic.h"

class ShaderUniformObject
{
public:
	ShaderUniformObject(void);
	virtual ~ShaderUniformObject(void);

	int assignLocation(GLuint aShaderProgramId);
	
	/**
	 * binds the value to the location in memory.
	 * Equivalent to a glUniform* call
	 */
	virtual void use() = 0;

	void setName(char* aName);

	void setHasChanged(bool aChanged);
	bool getHasChanged() const;

	/*
	 *	Wrapping functions
	 */
protected:
	void setUniform(GLint aLocation, float aValue);
	void setUniform(GLint aLocation, int aValue);
	void setUniform(GLint aLocation, float* aValue, int aCount);
	void setUniform(GLint aLocation, GLint* aValue, GLsizei aCount );

protected:
	GLint iLocation;
	char* iName;
	bool iHasChanged;
};

inline
void ShaderUniformObject::setHasChanged(bool aChanged)
{
	this->iHasChanged = aChanged;
}

inline
bool ShaderUniformObject::getHasChanged() const
{
	return this->iHasChanged;
}