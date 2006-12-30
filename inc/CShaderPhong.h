#pragma once

#include "CShaderEffect.h"
#include "ShaderUniformValue.h"

class CShaderPhong: public CShaderEffect
{
	//METHOD
public:
	CShaderPhong(CMyRenderer* aRenderer);
	virtual ~CShaderPhong();
	virtual void use();
protected:
private:

	//VARIABLES
public:
protected:
private:
	ShaderUniformValue<float>* iKa;		
	ShaderUniformValue<float>* iKd;
	ShaderUniformValue<float>* iKs;
};