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
	void RenderSceneOnQuad( GLuint aColorMapId, bool aGenerateMipMap );
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1 );		
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2 );
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2, GLuint aColorMapId3 );
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

