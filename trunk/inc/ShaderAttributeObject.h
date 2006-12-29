#pragma once
#include "Basic.h"

/**
 * @description Represents a vertex attribute object.
 * 
 * @file ShaderProgram.h
 * @author Joao Pedro Jorge
 *  modified Willem Frishert
 */
class ShaderAttributeObject
{
public:
	ShaderAttributeObject(char* name);
	virtual ~ShaderAttributeObject(void);

	int assignLocation(GLuint shaderProgram);

	GLuint getIndex() const;

protected:
	GLuint iIndex;
	char* iName;
};

inline
GLuint ShaderAttributeObject::getIndex() const
{
	return this->iIndex;
}