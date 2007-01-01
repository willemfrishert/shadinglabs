#pragma once

#include "CShaderEffect.h"
#include "ShaderUniformValue.h"
#include "TFramebufferObject.h"

const int KNumberOfTextures=2;
const int KNumberOfShadingPrograms=5;

const int KNumberOfTextureMaps=5;
const int KNumberOfDepthMaps=1;
const int KTextureWidth=512;
const int KTextureHeight = 512;
const int KNumberOfPasses=5;

const int KTextureBorder=0;
const int KTextureLevel=0;

class CShaderBloom: public CShaderEffect
{
	//METHOD
public:
	CShaderBloom(CMyRenderer* aRenderer);
	virtual ~CShaderBloom();
	virtual void use(CShaderEffect *aShaderEffect);

	void setThresHoldBrightness(float aThresholdBrightnessValue);
	float getThresHoldBrightness();
	void setFirstBlurSampleDistance(float aFirstBlurSampleDistance);
	float getFirstBlurSampleDistance();
	void setSecondBlurSampleDistance(float aSecondBlurSampleDistance);
	float getSecondBlurSampleDistance();
	void setThirdBlurSampleDistance(float aThirdBlurSampleDistance);
	float getThirdBlurSampleDistance();

protected:
private:
	virtual void use();
	void InitFramebufferObject();

	void GenerateInitialTexture( TTexture& aDestination, CShaderEffect* aShaderEffect );
	void BlurTexture( TTexture& aSourceTexture, TTexture& aDestinationTexture, float aSampleDistance );
	void BrightnessPass( TTexture& aSource, TTexture& aDestination );
	void CombineTextures( TTexture& aOriginalTexture, TTexture& aBlur1, TTexture& aBlur2, TTexture& aBlur3 );
	enum TTextureId
	{
		EDepthTexture = 0,
		EShaderEffectTexture,
		EBrightPassTexture,
		EFirstBlurTexture,
		ESecondBlurTexture,
		EThirdBlurTexture,
		ETempBlurTexture
	};

	enum TShadingProgram
	{
		EShaderEffectProgram = 0,
		ECartoonProgram,
		EBrightPassProgram,
		EBlurProgram,
		ECombineProgram
	};

	//VARIABLES
public:
protected:
private:
	TFramebufferObject* iFrameBufferObject;

	ShaderUniformValue<float>* iThresholdBrightness;
	float iThresholdBrightnessValue;

	ShaderUniformValue<float>* iSampleDistance;
	float iFirstBlurSampleDistance;
	float iSecondBlurSampleDistance;
	float iThirdBlurSampleDistance;

	ShaderUniformValue<bool>* iHorizontalBlur;

	ShaderUniformValue<float>* iMipMapBias;
	float iFirstBlurMipMapBias;
	float iSecondBlurMipMapBias;
	float iThirdBlurMipMapBias;

	ShaderUniformValue<int>* iOriginalTexture;
	ShaderUniformValue<int>* iBlurTexture1;
	ShaderUniformValue<int>* iBlurTexture2;
	ShaderUniformValue<int>* iBlurTexture3;

	ShaderUniformValue<int>* iCombineTextures;

	CShaderEffect* iShaderEffect;
};


inline
void CShaderBloom::setThresHoldBrightness(float aThresholdBrightnessValue)
{
	iThresholdBrightness->setValue(aThresholdBrightnessValue);
}

inline
float CShaderBloom::getThresHoldBrightness()
{
	return iThresholdBrightness->getValue();
}

inline
void CShaderBloom::setFirstBlurSampleDistance(float aFirstBlurSampleDistance)
{
	iFirstBlurSampleDistance = aFirstBlurSampleDistance;
}

inline
float CShaderBloom::getFirstBlurSampleDistance()
{
	return iFirstBlurSampleDistance;
};

inline
void CShaderBloom::setSecondBlurSampleDistance(float aSecondBlurSampleDistance)
{
	iSecondBlurSampleDistance = aSecondBlurSampleDistance;
};

inline
float CShaderBloom::getSecondBlurSampleDistance()
{
	return iSecondBlurSampleDistance;
};

inline
void CShaderBloom::setThirdBlurSampleDistance(float aThirdBlurSampleDistance)
{
	iThirdBlurSampleDistance = aThirdBlurSampleDistance;
};

inline
float CShaderBloom::getThirdBlurSampleDistance()
{
	return iThirdBlurSampleDistance;
};

