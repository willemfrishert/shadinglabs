#include "CShaderPhong.h"
#include "Basic.h"
#include "CMyRenderer.h"

CShaderPhong::CShaderPhong(CMyRenderer* aRenderer)
: CShaderEffect(aRenderer)
{
	iVertexShader.push_back(new CVertexShader());
	iFragmentShader.push_back(new CFragmentShader());
	iShaderProgram.push_back(new CShaderProgram());

	iVertexShader[0]->LoadFromFile( "shader/phong.vert" );
	iShaderProgram[0]->AddShader( iVertexShader[0] );

	iFragmentShader[0]->LoadFromFile( "shader/phong.frag" );
	iShaderProgram[0]->AddShader( iFragmentShader[0] );

	iKa = new ShaderUniformValue<float>();
	iKa->setName("Ka");
	iKa->setValue( 1.0f );
	iShaderProgram[0]->AddUniformObject( iKa );

	iKd = new ShaderUniformValue<float>();
	iKd->setName("Kd");
	iKd->setValue( 1.0f );
	iShaderProgram[0]->AddUniformObject( iKd );

	iKs = new ShaderUniformValue<float>();
	iKs->setName("Ks");
	iKs->setValue( 1.0f );
	iShaderProgram[0]->AddUniformObject( iKs ); 	

	bool result = iShaderProgram[0]->Build();

	if (false == result)
	{	
		EXIT(-1);
	}	
}

CShaderPhong::~CShaderPhong()
{
	delete iKa;
	delete iKd;
	delete iKs;
}

void CShaderPhong::use()
{
	glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	// enable Phong shading
	iShaderProgram[0]->Enable( true );

	// render screen size quad with texture (empty -> real scene)
	iRenderer->RenderObjects();
}