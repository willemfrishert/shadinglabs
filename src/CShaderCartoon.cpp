#include "CShaderCartoon.h"
#include "Basic.h"
#include "CMyRenderer.h"
#include "tga.h"

CShaderCartoon::CShaderCartoon(CMyRenderer* aRenderer)
: CShaderEffect(aRenderer)
{
	GLuint id = 0;
	
	if ( !(id = loadTGATexture("textures/lightmap.tga") ) ) 
	{
		EXIT(-1);
	}

	TTexture* texture = new TTexture( id );

	//float textureData[16][3] = {{0.2f, 0.2f, 0.2f},
	//							{0.2f, 0.2f, 0.2f},
	//							{0.5f, 0.5f, 0.5f},
	//							{0.5f, 0.5f, 0.5f},
	//							{0.5f, 0.5f, 0.5f},
	//							{0.7f, 0.7f, 0.7f},
	//							{0.7f, 0.7f, 0.7f},
	//							{0.7f, 0.7f, 0.7f},
	//							{0.7f, 0.7f, 0.7f},
	//							{0.7f, 0.7f, 0.7f},
	//							{0.7f, 0.7f, 0.7f},
	//							{1.0f, 1.0f, 1.0f},
	//							{1.0f, 1.0f, 1.0f},
	//							{1.0f, 1.0f, 1.0f},
	//							{1.0f, 1.0f, 1.0f},
	//							{1.0f, 1.0f, 1.0f} };
	//TTexture* texture = new TTexture(0, false, GL_RGB, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_REPLACE, 0, 16, GL_RGB, GL_FLOAT, GL_TEXTURE_1D, 0, 0, textureData );

	iTextures.push_back( texture );

	iVertexShader.push_back(new CVertexShader());
	iFragmentShader.push_back(new CFragmentShader());
	iShaderProgram.push_back(new CShaderProgram());

	iVertexShader[0]->LoadFromFile( "shader/toontexture.vert" );
	iShaderProgram[0]->AddShader( iVertexShader[0] );

	iFragmentShader[0]->LoadFromFile( "shader/toontexture.frag" );
	iShaderProgram[0]->AddShader( iFragmentShader[0] );

	iLightMap = new ShaderUniformValue<int>();
	iLightMap->setName("lightMap");
	iLightMap->setValue( 0 );
	iShaderProgram[0]->AddUniformObject( iLightMap );

	iColorRed = new ShaderUniformValue<float>();
	iColorRed->setName("red");
	iColorRed->setValue( 0.3 );
	iShaderProgram[0]->AddUniformObject( iColorRed );

	iColorGreen = new ShaderUniformValue<float>();
	iColorGreen->setName("green");
	iColorGreen->setValue( 0.6 );
	iShaderProgram[0]->AddUniformObject( iColorGreen );

	iColorBlue = new ShaderUniformValue<float>();
	iColorBlue->setName("blue");
	iColorBlue->setValue( 0.8 );
	iShaderProgram[0]->AddUniformObject( iColorBlue );

	bool result = iShaderProgram[0]->Build();	
	if (false == result)
	{	
		EXIT(-1);
	}
}
CShaderCartoon::~CShaderCartoon()
{
	delete iLightMap;
}
void CShaderCartoon::use()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();
	glActiveTexture(GL_TEXTURE0);
	glEnable( iTextures[0]->iTarget );
	glBindTexture( iTextures[0]->iTarget , iTextures[0]->iId );
CHECK_GL_ERROR();
	iShaderProgram[0]->Enable( true );
	CHECK_GL_ERROR();
	iRenderer->RenderObjects();
	iShaderProgram[0]->Enable( false );
CHECK_GL_ERROR();

	// anti aliasing
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Enable front face culling and draw thick black lines
	glLineWidth( 2.0f );
	glCullFace(GL_FRONT);			
	glPolygonMode(GL_BACK,GL_LINE);	

	iRenderer->RenderObjects();

	//undo everything which was enabled
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glCullFace(GL_BACK);
	glDisable( iTextures[0]->iTarget );

}