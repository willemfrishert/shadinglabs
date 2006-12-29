#pragma once

class CShaderEffect
{
	//METHODS
public:
	CShaderEffect(CMyRenderer* aRenderer);
	virtual CShaderEffect();
	virtual void use() = 0;

protected:
	void RenderSceneOnQuad( GLuint aColorMapId, bool aGenerateMipMap );
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1 );		
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2 );
	void RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2, GLuint aColorMapId3 );

private:

	//VARIABLES
public:
protected:
	CMyRenderer* iRenderer;
	TTexture* iTextures;
private:
};

