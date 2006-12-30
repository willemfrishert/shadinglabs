#include "CShaderBloom.h"
#include "CMyRenderer.h"

CShaderBloom::CShaderBloom(CMyRenderer* aRenderer)
: CShaderEffect(aRenderer)
, iFirstBlurSampleDistance(0.0078f)
, iSecondBlurSampleDistance( iFirstBlurSampleDistance+0.0078f )
, iThirdBlurSampleDistance( iSecondBlurSampleDistance+0.0078f )
, iFirstBlurMipMapBias(1.0f)
, iSecondBlurMipMapBias(0.0f)
, iThirdBlurMipMapBias(0.0f)
, iThresholdBrightnessValue( 0.9f )
{
	iRenderer = aRenderer;
	for (int i=0; i<KNumberOfPasses; i++)
	{
		iFragmentShader.push_back( new CFragmentShader() );
		iFragmentShader.push_back(new CFragmentShader());
		iShaderProgram.push_back(new CShaderProgram());
	}
	CHECK_GL_ERROR();

	bool result = false;

	iFragmentShader[EBrightPassProgram]->LoadFromFile( "shader/brightpass.frag" );
	iShaderProgram[EBrightPassProgram]->AddShader( iFragmentShader[EBrightPassProgram] );

	iThresholdBrightness = new ShaderUniformValue<float>();
	iThresholdBrightness->setName("thresholdBrightness");
	iThresholdBrightness->setValue( 0.9f );
	iShaderProgram[EBrightPassProgram]->AddUniformObject( iThresholdBrightness );

	result = iShaderProgram[EBrightPassProgram]->Build();
	if (false == result)
	{	
		EXIT(-1);
	}
	CHECK_GL_ERROR();

	iFragmentShader[EBlurProgram] = new CFragmentShader();
	iShaderProgram[EBlurProgram] = new CShaderProgram();

	iFragmentShader[EBlurProgram]->LoadFromFile( "shader/blur.frag" );
	iShaderProgram[EBlurProgram]->AddShader( iFragmentShader[EBlurProgram] );	

	iSampleDistance = new ShaderUniformValue<float>();
	iSampleDistance->setName("sampleDist");
	iSampleDistance->setValue( iFirstBlurSampleDistance );
	iShaderProgram[EBlurProgram]->AddUniformObject( iSampleDistance );

	iMipMapBias = new ShaderUniformValue<float>();
	iMipMapBias->setName("mipmapBias");
	iMipMapBias->setValue( iFirstBlurMipMapBias );
	//	iShaderProgram[ECombineProgram]->AddUniformObject( iMipMapBias );
	iShaderProgram[EBlurProgram]->AddUniformObject( iMipMapBias );


	result &= iShaderProgram[EBlurProgram]->Build();
	if (false == result)
	{	
		EXIT(-1);
	}	
	CHECK_GL_ERROR();

	iFragmentShader[ECombineProgram] = new CFragmentShader();
	iShaderProgram[ECombineProgram] = new CShaderProgram();

	iFragmentShader[ECombineProgram]->LoadFromFile( "shader/combine.frag" );
	iShaderProgram[ECombineProgram]->AddShader( iFragmentShader[ECombineProgram] );	

	iOriginalTexture = new ShaderUniformValue<int>();
	iOriginalTexture->setName("originalTex");
	iOriginalTexture->setValue( 0 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iOriginalTexture );

	iBlurTexture1 = new ShaderUniformValue<int>();
	iBlurTexture1->setName("blurTex1");
	iBlurTexture1->setValue( 1 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture1 );

	iBlurTexture2 = new ShaderUniformValue<int>();
	iBlurTexture2->setName("blurTex2");
	iBlurTexture2->setValue( 2 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture2 );

	iBlurTexture3 = new ShaderUniformValue<int>();
	iBlurTexture3->setName("blurTex3");
	iBlurTexture3->setValue( 3 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture3 );	

	iCombineTextures = new ShaderUniformValue<int>();
	iCombineTextures->setName("combineTextures");
	iCombineTextures->setValue( 1 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iCombineTextures );

	CHECK_GL_ERROR();

	result &= iShaderProgram[ECombineProgram]->Build();
	if (false == result)
	{	
		EXIT(-1);
	}		
	CHECK_GL_ERROR();
	
	InitFramebufferObject();
}

void CShaderBloom::InitFramebufferObject()
{
	iFrameBufferObject = new TFramebufferObject();

	iTextures.push_back( new TTexture(0, false, GL_DEPTH_COMPONENT, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_DEPTH_COMPONENT, GL_FLOAT ) );
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	iTextures.push_back( new TTexture(0, true, GL_RGBA, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );

	iFrameBufferObject->AttachTexture( GL_DEPTH_ATTACHMENT_EXT, iTextures[EDepthTexture]->iTarget, iTextures[EDepthTexture]->iId );
	CHECK_GL_ERROR();
}

CShaderBloom::~CShaderBloom()
{
	delete iThresholdBrightness;
	delete iSampleDistance;
	delete iOriginalTexture;
	delete iBlurTexture1;
	delete iMipMapBias;
	delete iCombineTextures;
}

void CShaderBloom::use(CShaderEffect *aShaderEffect)
{
	iShaderEffect = aShaderEffect;
	use();
}
void CShaderBloom::use()
{
	glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();

	//// render real scene to FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[EPhongTexture]->iTarget, iTextures[EPhongTexture]->iId );//GL_TEXTURE_2D, iColorMapId[EPhongTextureId] );
	CHECK_GL_ERROR();
	bool result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	}  
	CHECK_GL_ERROR();
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[EPhongTexture]->iWidth, iTextures[EPhongTexture]->iHeight );//KTextureWidth, KTextureHeight );
	CHECK_GL_ERROR();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();
	// render objects using Phong shading
	glDisable(GL_TEXTURE_2D);

	iShaderEffect->use();
//	iRenderer->RenderObjects();

	CHECK_GL_ERROR();
	//	glDisable(GL_LIGHTING);
	//	glColor3f(1, 1, 1);
	//	
	//////	glEnable(GL_CULL_FACE);
	//////	glPolygonMode( GL_FRONT, GL_LINE);
	//	
	//	RenderObjects();
	//	
	//////	glPolygonMode( GL_FRONT, GL_FILL);
	//////	glDisable(GL_CULL_FACE);
	//	
	//	glEnable(GL_TEXTURE_2D);	
	//	CHECK_GL_ERROR();

	//	iFrameBufferObject->Enable( false, GL_BACK, iScreenWidth, iScreenHeight );
	//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//	CShaderProgram::DisableAll();
	//	RenderSceneOnQuad( iColorMapId[EPhongTextureId] );	

	// bright pass
	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[EBrightPassTexture]->iTarget, iTextures[EBrightPassTexture]->iId );//GL_TEXTURE_2D, iColorMapId[EBrightPassTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[EBrightPassTexture]->iWidth, iTextures[EBrightPassTexture]->iHeight);//, KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up shading program (bloom effect)
	iThresholdBrightness->setValue( iThresholdBrightnessValue );
	iShaderProgram[EBrightPassProgram]->Enable( true );

	// render screen size quad with Phong texture (real scene -> bloom effect scene)
	//	RenderSceneOnQuad( iColorMapId[EPhongTextureId], false );
	RenderSceneOnQuad( iTextures[EPhongTexture]->iId, false );
	CHECK_GL_ERROR();

	// first blur pass

	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[EFirstBlurTexture]->iTarget, iTextures[EFirstBlurTexture]->iId );// GL_TEXTURE_2D, iColorMapId[EFirstBlurTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[EFirstBlurTexture]->iWidth, iTextures[EFirstBlurTexture]->iHeight);//KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// set up shading program (blur effect)
	iSampleDistance->setValue( iFirstBlurSampleDistance );
	iMipMapBias->setValue( iFirstBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (bloom effect scene -> blur)	
	//	RenderSceneOnQuad( iColorMapId[EBrightPassTextureId], true );
	RenderSceneOnQuad( iTextures[EBrightPassTexture]->iId, true );


	//second blur pass
	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[ESecondBlurTexture]->iTarget, iTextures[ESecondBlurTexture]->iId );//GL_TEXTURE_2D, iColorMapId[ESecondBlurTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[ESecondBlurTexture]->iWidth, iTextures[ESecondBlurTexture]->iHeight);//KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();


	// set up shading program (blur effect)
	iSampleDistance->setValue( iSecondBlurSampleDistance );
	iMipMapBias->setValue( iSecondBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (bloom effect scene -> blur)	
	//RenderSceneOnQuad( iColorMapId[EFirstBlurTextureId], false );
	RenderSceneOnQuad( iTextures[EFirstBlurTexture]->iId, false );
	////disabled the third blur pass

	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[EThirdBlurTexture]->iTarget, iTextures[EThirdBlurTexture]->iId );//GL_TEXTURE_2D, iColorMapId[EThirdBlurTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[EThirdBlurTexture]->iWidth, iTextures[EThirdBlurTexture]->iHeight);//KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// set up shading program (blur effect)
	iSampleDistance->setValue( iThirdBlurSampleDistance );
	iMipMapBias->setValue( iSecondBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (bloom effect scene -> blur)	
	RenderSceneOnQuad( iTextures[ESecondBlurTexture]->iId, false );

	// set up shading program (combiner)
	iShaderProgram[ECombineProgram]->Enable( true );


	CHECK_GL_ERROR();	

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//disable any frame buffer object. We want to render to the screen in this final pass
	iFrameBufferObject->Enable( false, GL_BACK, iRenderer->Width(), iRenderer->Height() );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// blending using multi texture (real scene + blurred bloom scene)
	// render screen size quad with multi texture
	//RenderSceneOnQuad( iTextures[EPhongTexture]->iId, iTextures[EFirstBlurTexture]->iId, iTextures[ESecondBlurTexture]->iId, iTextures[EThirdBlurTexture]->iId );
	//RenderSceneOnQuad( iTextures[EPhongTexture]->iId, iTextures[EFirstBlurTexture]->iId, iTextures[ESecondBlurTexture]->iId );
	//RenderSceneOnQuad( iTextures[EPhongTexture]->iId, iTextures[EFirstBlurTexture]->iId );

	//functions to test separate buffers
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
	//CShaderProgram::DisableAll();
	//RenderSceneOnQuad( iTextures[EPhongTexture]->iId, false );
	//RenderSceneOnQuad( iTextures[EBrightPassTexture]->iId, false );	
	//RenderSceneOnQuad( iTextures[EFirstBlurTexture]->iId, false );
	//RenderSceneOnQuad( iTextures[ESecondBlurTexture]->iId, false );	
	//RenderSceneOnQuad( iTextures[EThirdBlurTexture]->iId, false );
}
