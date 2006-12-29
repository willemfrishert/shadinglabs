/**
 * @description Represents a uniform shader attribute
 * TODO: support for matrices
 * 
 * @file ShaderUniformValue.h
 * @author Joao Pedro Jorge
 *  modified Willem Frishert
 */
#pragma once

#include "ShaderUniformObject.h"

template <class T>
class ShaderUniformValue: public ShaderUniformObject
{
public:
	ShaderUniformValue(void);
	virtual ~ShaderUniformValue(void);

	void setValue(T aValue);
	
	T getValue();
	
	virtual void use();

	// attributes
private:
	T iValue;
};

template <class T>
ShaderUniformValue<T>::ShaderUniformValue(void)
{
}

template <class T>
ShaderUniformValue<T>::~ShaderUniformValue(void)
{
}

/**
 * @param value the attribute value
 */
template <class T>
void ShaderUniformValue<T>::setValue(T aValue)
{
	this->iValue = aValue;

	// NOTE: we must call the upper class and MARK the
	// value as CHANGED
	this->setHasChanged( true );
}

template <class T>
T ShaderUniformValue<T>::getValue()
{
	return this->iValue;
}

template <class T>
void ShaderUniformValue<T>::use()
{
	if ( this->getHasChanged() )
	{
		setUniform(this->iLocation, this->iValue);
	}
}

