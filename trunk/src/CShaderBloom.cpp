#include "CShaderBloom.h"
#include "CMyRenderer.h"

CShaderBloom::CShaderBloom(CMyRenderer* aRenderer)
: CShaderEffect(aRenderer)
, iFirstBlurSampleDistance(0.015f)
, iSecondBlurSampleDistance( 0.025f )
, iThirdBlurSampleDistance( 0.04f )
, iThresholdBrightnessValue( 0.9f )
{
	iRenderer = aRenderer;
	for (int i=0; i<KNumberOfPasses; i++)
	{
		iFragmentShader.push_back( new CFragmentShader() );
		iFragmentShader.push_back( new CFragmentShader() );
		iShaderProgram.push_back( new CShaderProgram() );
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

	//iFragmentShader[EBlurProgram]->LoadFromFile( "shader/blur.frag" );
	//iShaderProgram[EBlurProgram]->AddShader( iFragmentShader[EBlurProgram] );	

	iFragmentShader[EBlurProgram]->LoadFromFile( "shader/blureffect.frag" );
	iShaderProgram[EBlurProgram]->AddShader( iFragmentShader[EBlurProgram] );

	iSampleDistance = new ShaderUniformValue<float>();
	iSampleDistance->setName("sampleDistance");
	iSampleDistance->setValue( iFirstBlurSampleDistance );
	iShaderProgram[EBlurProgram]->AddUniformObject( iSampleDistance );

	iHorizontalBlur = new ShaderUniformValue<bool>();
	iHorizontalBlur->setName("horizontalBlur");
	iHorizontalBlur->setValue( true );
	iShaderProgram[EBlurProgram]->AddUniformObject( iHorizontalBlur );

	//iMipMapBias = new ShaderUniformValue<float>();
	//iMipMapBias->setName("mipmapBias");
	//iMipMapBias->setValue( iFirstBlurMipMapBias );
	//iShaderProgram[EBlurProgram]->AddUniformObject( iMipMapBias );

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
	iOriginalTexture->setName("originalTexture");
	iOriginalTexture->setValue( 0 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iOriginalTexture );

	iBlurTexture1 = new ShaderUniformValue<int>();
	iBlurTexture1->setName("blurTexture1");
	iBlurTexture1->setValue( 1 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture1 );

	iBlurTexture2 = new ShaderUniformValue<int>();
	iBlurTexture2->setName("blurTexture2");
	iBlurTexture2->setValue( 2 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture2 );

	iBlurTexture3 = new ShaderUniformValue<int>();
	iBlurTexture3->setName("blurTexture3");
	iBlurTexture3->setValue( 3 );
	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture3 );	

	//iCombineTextures = new ShaderUniformValue<int>();
	//iCombineTextures->setName("combineTextures");
	//iCombineTextures->setValue( 1 );
	//iShaderProgram[ECombineProgram]->AddUniformObject( iCombineTextures );

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

	//EDepthTexture
	iTextures.push_back( new TTexture(0, false, GL_DEPTH_COMPONENT, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_DEPTH_COMPONENT, GL_FLOAT ) );
	//EPhongTexture
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	//EBrightPassTexture

	iTextures.push_back( new TTexture(0, true, GL_RGBA, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	//EFirstBlurTexture
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	//ESecondBlurTexture
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	//EThirdBlurTexture
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );
	//ETempBlurTexture
	iTextures.push_back( new TTexture(0, false, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,GL_REPLACE,KTextureHeight, KTextureWidth, GL_RGBA, GL_FLOAT ) );


	/************************************************************************/
	/* FIX?: if depth component does not have enough precision (>=24 bit),  
	/*       artifacts appear. GL_DEPTH_COMPONENT24 generates an error.
	/*       Therefore, use render buffer
	/************************************************************************/
	//iFrameBufferObject->AttachTexture( GL_DEPTH_ATTACHMENT_EXT, iTextures[EDepthTexture]->iTarget, iTextures[EDepthTexture]->iId );

	GLuint depth_rb;
    glGenRenderbuffersEXT(1, &depth_rb);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, 512, 512);
	iFrameBufferObject->AttachRenderBuffer(GL_DEPTH_ATTACHMENT_EXT, depth_rb );
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	CHECK_GL_ERROR();
}

CShaderBloom::~CShaderBloom()
{
	delete iFrameBufferObject;

	delete iThresholdBrightness;
	delete iSampleDistance;
	delete iOriginalTexture;
	delete iBlurTexture1;
	delete iBlurTexture2;
	delete iBlurTexture3;
	//delete iCombineTextures;
	delete iHorizontalBlur;

	iShaderEffect = NULL;
	iRenderer = NULL;
}

void CShaderBloom::use(CShaderEffect *aShaderEffect)
{
	iShaderEffect = aShaderEffect;
	use();
}

void CShaderBloom::use()
{
	// create initial texture
	GenerateInitialTexture( *iTextures[EShaderEffectTexture], iShaderEffect );

	CHECK_GL_ERROR();

	// brightness pass
	BrightnessPass( *iTextures[EShaderEffectTexture], *iTextures[EBrightPassTexture] );

	CHECK_GL_ERROR();

	// first blur pass
	BlurTexture( *iTextures[EBrightPassTexture], *iTextures[EFirstBlurTexture], iFirstBlurSampleDistance );

	//second blur pass
	BlurTexture( *iTextures[EFirstBlurTexture], *iTextures[ESecondBlurTexture], iSecondBlurSampleDistance );

	//second blur pass
	BlurTexture( *iTextures[ESecondBlurTexture], *iTextures[EThirdBlurTexture], iThirdBlurSampleDistance );

	//combine textures
	CombineTextures( *iTextures[EShaderEffectTexture],
					 *iTextures[EBrightPassTexture],
					 *iTextures[ESecondBlurTexture], 
					 *iTextures[EThirdBlurTexture] );

	//functions to test separate buffers
	//disable any frame buffer object. We want to render to the screen in this final pass
	//iFrameBufferObject->Enable( false, GL_BACK, iRenderer->Width(), iRenderer->Height() );
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
	//CShaderProgram::DisableAll();
	//RenderSceneOnQuad( *iTextures[EShaderEffectTexture] );
	//RenderSceneOnQuad( *iTextures[EBrightPassTexture] );	
	//RenderSceneOnQuad( *iTextures[EFirstBlurTexture] );
	//RenderSceneOnQuad( *iTextures[ESecondBlurTexture] );	
	//RenderSceneOnQuad( *iTextures[EThirdBlurTexture] );
}


void CShaderBloom::GenerateInitialTexture( TTexture& aDestination, CShaderEffect* aShaderEffect )
{
	glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();

	//// render real scene to FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, aDestination.iTarget, aDestination.iId );
	CHECK_GL_ERROR();
	bool result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	}  
	CHECK_GL_ERROR();
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, aDestination.iWidth, aDestination.iHeight );
	CHECK_GL_ERROR();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// render objects using the specified shader effect
	glDisable(GL_TEXTURE_2D);
//	aShaderEffect->use();
	iRenderer->RenderObjects();

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
}

void CShaderBloom::BlurTexture( TTexture& aSourceTexture, TTexture& aDestinationTexture, float aSampleDistance )
{
	bool result;

	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, iTextures[ETempBlurTexture]->iTarget, iTextures[ETempBlurTexture]->iId );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, iTextures[ETempBlurTexture]->iWidth, iTextures[ETempBlurTexture]->iHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// set up shading program (blur effect)
	iSampleDistance->setValue( aSampleDistance );
	iHorizontalBlur->setValue( true );
	//iMipMapBias->setValue( iFirstBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (source -> horizontal blur)
	RenderSceneOnQuad( aSourceTexture );

	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, aDestinationTexture.iTarget, aDestinationTexture.iId );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, aDestinationTexture.iWidth, aDestinationTexture.iHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// set up shading program (blur effect)
	iSampleDistance->setValue( aSampleDistance );
	iHorizontalBlur->setValue( false );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (vertical blur -> destination)
	RenderSceneOnQuad( *iTextures[ETempBlurTexture] );
}

void CShaderBloom::BrightnessPass( TTexture& aSource, TTexture& aDestination )
{
	bool result;
	// bright pass
	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, aDestination.iTarget, aDestination.iId );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, aDestination.iWidth, aDestination.iHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up shading program (bloom effect)
	iThresholdBrightness->setValue( iThresholdBrightnessValue );
	iShaderProgram[EBrightPassProgram]->Enable( true );

	// render screen size quad with Phong texture (real scene -> bloom effect scene)
	RenderSceneOnQuad( aSource );
}

void CShaderBloom::CombineTextures( TTexture& aOriginalTexture, TTexture& aBlur1, TTexture& aBlur2, TTexture& aBlur3 )
{
	//// set up shading program (combiner)
	iShaderProgram[ECombineProgram]->Enable( true );

	CHECK_GL_ERROR();	

	//disable any frame buffer object. We want to render to the screen in this final pass
	iFrameBufferObject->Enable( false, GL_BACK, iRenderer->Width(), iRenderer->Height() );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// blending using multi texture (real scene + blurred bloom scene)
	// render screen size quad with multi texture
	RenderSceneOnQuad( aOriginalTexture, aBlur1, aBlur2, aBlur2 );
	//RenderSceneOnQuad( aOriginalTexture, aBlur1, aBlur2 );
	//RenderSceneOnQuad( aOriginalTexture, aBlur1 );
}