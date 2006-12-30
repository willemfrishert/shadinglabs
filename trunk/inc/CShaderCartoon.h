#pragma once

#include "CShaderEffect.h"
#include "ShaderUniformValue.h"

class CShaderCartoon: public CShaderEffect
{
	//METHOD
public:
	CShaderCartoon(CMyRenderer* aRenderer);
	virtual ~CShaderCartoon();
	virtual void use();
protected:
private:

	//VARIABLES
public:
protected:
private:
	ShaderUniformValue<int>* iLightMap;

	ShaderUniformValue<float>* iThresholdBrightness;
	float iThresholdBrightnessValue;
};