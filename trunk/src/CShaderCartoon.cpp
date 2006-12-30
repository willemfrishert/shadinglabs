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

	TTexture* texture = new TTexture();
	texture->iId = id;

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
	glEnable(GL_TEXTURE_2D);

	// disable Cartoon program
	iShaderProgram[0]->Enable( false );
	CHECK_GL_ERROR();

	//render backside:
	glEnable(GL_CULL_FACE);
	glCullFace( GL_FRONT );
	glPolygonMode( GL_BACK, GL_LINE);
	CHECK_GL_ERROR();

	// change the polygon offset
	glEnable( GL_POLYGON_OFFSET_LINE );
	glPolygonOffset( 0.75, 1.0 );
	glLineWidth( 5.0f );

	glColor3f(0, 0, 0);
	CHECK_GL_ERROR();
	iRenderer->RenderObjects();
	CHECK_GL_ERROR();

	glColor3f(1,1,1);
	CHECK_GL_ERROR();

	glDisable( GL_POLYGON_OFFSET_LINE );

	glPolygonMode( GL_BACK, GL_FILL);

	glDisable(GL_CULL_FACE);
	glCullFace( GL_BACK );
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();
	CHECK_GL_ERROR();

	iShaderProgram[0]->Enable( true );

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, iTextures[0]->iId );

	iRenderer->RenderObjects();
}