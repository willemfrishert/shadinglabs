#pragma once

#include "Basic.h"
#include "TTexture.h"
#include "CShaderProgram.h"
#include "CShader.h"
#include "CFragmentShader.h"
#include "CVertexShader.h"

class CMyRenderer;

class CShaderEffect
{
	//METHODS
public:
	CShaderEffect(CMyRenderer* aRenderer);
	~CShaderEffect();
	virtual void use(){};
	void RenderSceneOnQuad( TTexture& aTexture );
	void RenderSceneOnQuad( TTexture& aTexture0, TTexture &aTexture1 );		
	void RenderSceneOnQuad( TTexture& aTexture0, TTexture& aTexture1, TTexture& aTexture2 );
	void RenderSceneOnQuad( TTexture& aTexture0, TTexture& aTexture1, TTexture& aTexture2, TTexture& aTexture3 );
protected:


private:

	//VARIABLES
public:
protected:
	CMyRenderer* iRenderer;
	vector<TTexture*> iTextures;
	vector<CShaderProgram*> iShaderProgram;
	vector<CShader*> iVertexShader;
	vector<CShader*> iFragmentShader;

private:
};

