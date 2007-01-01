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

	//float textureData[32][3];

	//shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = ;
	//TTexture* texture = new TTexture(0,false,GL_RGB, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_REPLACE, 1, 32, GL_RGB, GL_FLOAT, GL_TEXTURE_1D, 0, 0, textureData);

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
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, iTextures[0]->iId );


	//render backside:
	glEnable(GL_CULL_FACE);
	glCullFace( GL_FRONT );
	glPolygonMode( GL_BACK, GL_LINE);
	CHECK_GL_ERROR();

	// change the polygon offset
	glEnable( GL_POLYGON_OFFSET_LINE );
	glPolygonOffset( 0.75, 1.0 );
	glLineWidth( 7.0f );

	CHECK_GL_ERROR();
	iRenderer->RenderObjects();
	CHECK_GL_ERROR();

	glDisable(GL_CULL_FACE);
	glCullFace( GL_BACK );
	glPolygonMode( GL_BACK, GL_FILL);
	glDisable( GL_POLYGON_OFFSET_LINE );


	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();
	CHECK_GL_ERROR();

	iShaderProgram[0]->Enable( true );
	iRenderer->RenderObjects();
}