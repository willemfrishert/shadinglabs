//INCLUDES
#include "CMyRenderer.h"
#include "tga.h"
#include "ShaderUniformValue.h"
#include "3ds.h"


const GLdouble KDegreeToRadian= M_PI / 180.0;

//INIT STATIC DATA
CMyRenderer* CMyRenderer::iCurrentRenderer = 0;

//Constructor with the screen size defined
CMyRenderer::CMyRenderer( const int aWidth, const int aHeight )
: iScreenHeight( aHeight )
, iScreenWidth( aWidth )
, iShadingType( EBloom )
, iCurrentTime(0)
, iPreviousTime(0)
, iFpsCountString("")
, iFirstBlurSampleDistance(0.00780f)
, iSecondBlurSampleDistance( iFirstBlurSampleDistance+0.0078f )
, iThirdBlurSampleDistance( 0.0 )//iSecondBlurSampleDistance+0.0078f )
, iFirstBlurMipMapBias(1.0f)
, iSecondBlurMipMapBias(0.0f)
, iThirdBlurMipMapBias(0.0f)
, iThresholdBrightnessValue( 0.9f )
{
	InitMain();
}


//DESTRUCTOR releases the used heap
//
CMyRenderer::~CMyRenderer()
{
	glDeleteTextures( KNumberOfTextures ,iTextureId );
	glDeleteTextures( KNumberOfColorMaps, iColorMapId );

	delete iKa;		
	delete iKd;
	delete iKs;

	delete iLightMap;

	delete iSampleDistance;
	delete iOriginalTexture;
	delete iBlurTexture1;
	delete iMipMapBias;
	delete iCombineTextures;

	delete chevy;

	CMyRenderer::iCurrentRenderer = 0;
}


//
/// Init function for the constructors
void CMyRenderer::InitMain()
{
	//set up the light
	CreateScene();
	InitLights();	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//rotation info changes with the mouse
	iXRotation = 0.0f;
	iYRotation = 0.0f;
	//iZoom = -20.0f;
	iZoom = -5.0f;
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	//	GLfloat materialAmbient[]  = { 0.2f, 0.1f, 0.02f, 1.0f };
	//	GLfloat materialDiffuse[]  = { 0.9f, 0.5f, 0.2f,  1.0f };
	//	GLfloat materialSpecular[] = { 0.7f, 0.6f, 0.5f,  1.0f };
	//	GLfloat materialAmbient[]  = { 0.2f, 0.0f, 0.02f, 1.0f };
	//	GLfloat materialDiffuse[]  = { 0.9f, 0.1f, 0.1f,  1.0f };
	//	GLfloat materialSpecular[] = { 0.7f, 0.6f, 0.5f,  1.0f };
	GLfloat materialAmbient[]  = { 0.2f, 0.0f, 0.02f, 1.0f };
	GLfloat materialDiffuse[]  = { 0.0f, 0.4f, 1.0f,  1.0f };
	GLfloat materialSpecular[] = { 0.5f, 0.6f, 0.7f,  1.0f };	
	GLfloat shininess = 25.0f;

	glMaterialfv( GL_FRONT, GL_AMBIENT,   materialAmbient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE,   materialDiffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR,  materialSpecular );
	glMaterialf(  GL_FRONT, GL_SHININESS, shininess );

	InitPhongShading();

	InitCartoonShadering();

	InitBloomShadingEffect();

	InitFramebufferObject();

	//	CalculateFramesPerSecond();

	CMyRenderer::iCurrentRenderer = this;
}

void CMyRenderer::CreateScene()
{
	Load3ds modelLoader;

	this->chevy = modelLoader.Create("3ds/chevy.3ds");
}

void CMyRenderer::InitPhongShading()
{
	iVertexShader[EPhongProgram] = new CVertexShader();
	iFragmentShader[EPhongProgram] = new CFragmentShader();
	iShaderProgram[EPhongProgram] = new CShaderProgram();

	iVertexShader[EPhongProgram]->LoadFromFile( "shader/phong.vert" );
	iShaderProgram[EPhongProgram]->AddShader( iVertexShader[EPhongProgram] );

	iFragmentShader[EPhongProgram]->LoadFromFile( "shader/phong.frag" );
	iShaderProgram[EPhongProgram]->AddShader( iFragmentShader[EPhongProgram] );

	iKa = new ShaderUniformValue<float>();
	iKa->setName("Ka");
	iKa->setValue( 1.0f );
	iShaderProgram[EPhongProgram]->AddUniformObject( iKa );

	iKd = new ShaderUniformValue<float>();
	iKd->setName("Kd");
	iKd->setValue( 1.0f );
	iShaderProgram[EPhongProgram]->AddUniformObject( iKd );

	iKs = new ShaderUniformValue<float>();
	iKs->setName("Ks");
	iKs->setValue( 1.0f );
	iShaderProgram[EPhongProgram]->AddUniformObject( iKs ); 	

	bool result = iShaderProgram[EPhongProgram]->Build();

	if (false == result)
	{	
		EXIT(-1);
	}	
}

void CMyRenderer::InitCartoonShadering()
{
	if ( !(iTextureId[0] = loadTGATexture("textures/lightmap.tga") ) ) 
	{
		EXIT(-1);
	}

	iVertexShader[ECartoonProgram] = new CVertexShader();
	iFragmentShader[ECartoonProgram] = new CFragmentShader();
	iShaderProgram[ECartoonProgram] = new CShaderProgram();

	iVertexShader[ECartoonProgram]->LoadFromFile( "shader/toontexture.vert" );
	iShaderProgram[ECartoonProgram]->AddShader( iVertexShader[ECartoonProgram] );

	iFragmentShader[ECartoonProgram]->LoadFromFile( "shader/toontexture.frag" );
	iShaderProgram[ECartoonProgram]->AddShader( iFragmentShader[ECartoonProgram] );

	iLightMap = new ShaderUniformValue<int>();
	iLightMap->setName("lightMap");
	iLightMap->setValue( 0 );

	bool result = iShaderProgram[ECartoonProgram]->Build();	
	if (false == result)
	{	
		EXIT(-1);
	}

	iShaderProgram[ECartoonProgram]->AddUniformObject( iLightMap );
}


void CMyRenderer::InitBloomShadingEffect()
{
	CHECK_GL_ERROR();

	bool result = false;

	iFragmentShader[EBrightPassProgram] = new CFragmentShader();
	iShaderProgram[EBrightPassProgram] = new CShaderProgram();

	iFragmentShader[EBrightPassProgram]->LoadFromFile( "shader/brightpass.frag" );
	iShaderProgram[EBrightPassProgram]->AddShader( iFragmentShader[EBrightPassProgram] );

	iThresholdBrightness = new ShaderUniformValue<float>();
	iThresholdBrightness->setName("thresholdBrightness");
	iThresholdBrightness->setValue( iThresholdBrightnessValue );
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

	//	iBlurTexture3 = new ShaderUniformValue<int>();
	//	iBlurTexture3->setName("blurTex3");
	//	iBlurTexture3->setValue( 3 );
	//	iShaderProgram[ECombineProgram]->AddUniformObject( iBlurTexture3 );	

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

}

void CMyRenderer::InitFramebufferObject()
{
	iFrameBufferObject = new TFramebufferObject();

	glGenTextures( 1, &iDepthMapId );
	glBindTexture( GL_TEXTURE_2D, iDepthMapId );

	// set texture parameters
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	// set up the texture size, type, format,...
	glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_DEPTH_COMPONENT, KTextureWidth, KTextureHeight, KTextureBorder, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

	// attach the depth to the frame buffer
	iFrameBufferObject->AttachTexture( GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, iDepthMapId );

	glGenTextures( KNumberOfColorMaps, iColorMapId );
	for(int i=0; i< KNumberOfColorMaps; i++)
	{
		glBindTexture( GL_TEXTURE_2D, iColorMapId[i] );
		//		if ( i == EFirstBlurTextureId )
		if (i == EBrightPassTextureId)
		{
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);			
		}
		else
		{
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );			
		}
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_RGBA, KTextureWidth, KTextureHeight, KTextureBorder, GL_RGBA, GL_FLOAT, NULL );

		if (i == EBrightPassTextureId )
		{
			glGenerateMipmapEXT( GL_TEXTURE_2D );
		}
	}
	CHECK_GL_ERROR();
}

void CMyRenderer::InitLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//CREATE THE LIGHT
	GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };

	glLightfv (GL_LIGHT0, GL_POSITION, light_position); 
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
}

void CMyRenderer::UpdateScene( int aValue )
{
	iAngle += 1.0f;
	if (iAngle>180.0) iAngle -= 360.0f;
	iWobble = 150.0f*sin(iAngle*KDegreeToRadian);
	glutPostRedisplay();

	glutTimerFunc( 10,UpdateSceneWithRenderer, 0 );
}

void CMyRenderer::UpdateScene()
{
	iAngle += 1.0f;
	if (iAngle>180.0) iAngle -= 360.0f;
	iWobble = 150.0f*sin(iAngle*KDegreeToRadian);
	glutPostRedisplay();
}

/** \brief Method that calculates iFrames per second
*
*	Method that uses glut to calculate iFrames per second. It update the iFpsCountString every  1000 calculation.
*	This method is to uncertain to do any scientific conclusions but give you a chance to evaluate the
*	speed of you code.<br>
*	The more math you implement the slower the iFrame rate should be, but not to slowe.
*
*/
void CMyRenderer::CalculateFramesPerSecond()
{
	ostringstream ostr;

	iFrame++;
	iCurrentTime = glutGet(GLUT_ELAPSED_TIME);

	if ( iCurrentTime - iPreviousTime > 1000 )
	{
		ostr << "FPS:" << setprecision ( 3 ) << setw(3) << iFrame*1000.0/( iCurrentTime - iPreviousTime ) << " iFSD:" << iFirstBlurSampleDistance << " iSSD:" << iSecondBlurSampleDistance;// << " iTSD:" << iThirdBlurSampleDistance;
		iFpsCountString = ostr.str();
		iPreviousTime = iCurrentTime;
		iFrame = 0;
	}

}

/** \brief Method that draws text to the glut window
*
*	This method draws text to the glut window at the position aX = -0.85 and aY = -0.95
*	If you want to print you own text, you might want to change the method to something like
*	DrawText(int aX, int aY, cons char *string)
*
*/
void CMyRenderer::DrawText() const
{
	float aX( -0.95f ), aY( -0.95f );

	CHECK_GL_ERROR();	
	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	CHECK_GL_ERROR();	
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );
	glMatrixMode( GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	CHECK_GL_ERROR();	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR();	
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(aX, aY);
	for (int i=0, len=static_cast<int>(iFpsCountString.length()); i < len; i++)
	{
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, iFpsCountString[i] );
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, iFpsCountString[i] );
	}
	//glFlush();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();

	CHECK_GL_ERROR(); 
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	CHECK_GL_ERROR();	
}

// *************	RENDERING METHODS *********** /

/** \brief Method that specifies how the screen is rendered
*/


void CMyRenderer::RenderObjects()
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef(0, 0, iZoom);
	/* "World" rotation, controlled by mouse */
	glRotatef(iXRotation, 1, 0, 0);
	glRotatef(iYRotation, 0, 1, 0);

	//glRotatef(27, 0, 1, 0);	
	//glRotatef(-100, 1, 0, 0);
	//	glPushMatrix();

	//chevy->draw();
	//	glPopMatrix();
		for (int i = 0; i < 3; i++)
			{
			for (int j = 0; j < 3; j++) 
				{
				glPushMatrix();
				
				glTranslatef(i-1, j-1, 0);
			//	glRotatef(iWobble, 0.5, 1, 0.3);
	      
				switch (i*3 + j)
					{
					case 0:
						glShadeModel(GL_FLAT);
						glutSolidCube(0.5);
						break;
	          
					case 1:
						glShadeModel(GL_SMOOTH);
						glutSolidSphere(0.4, 24, 18);
						break;
	          
					case 2:
						glShadeModel(GL_SMOOTH);
						glutSolidCone(0.3, 0.6, 24, 2);
						break;
	          
					case 3:
						glShadeModel(GL_SMOOTH);
						glutSolidTorus(0.1, 0.3, 12, 24);
						break;
	          
					case 4:
						glShadeModel(GL_FLAT);
						glScalef(0.2, 0.2, 0.2);
						glEnable(GL_NORMALIZE);
						glutSolidDodecahedron();
						break;
	          
					case 5:
						glShadeModel(GL_FLAT);
						glScalef(0.4, 0.4, 0.4);
						glutSolidOctahedron();     /* Still normalizing */
						break;
	          
					case 6:
						glShadeModel(GL_FLAT);
						glScalef(0.4, 0.4, 0.4);
						glutSolidTetrahedron();    /* Still normalizing */
						break;
	          
					case 7:
						glShadeModel(GL_FLAT);
						glScalef(0.35, 0.35, 0.35);
						glutSolidIcosahedron();    /* Still normalizing */
						glDisable(GL_NORMALIZE);
						break;
					case 8:
						glLineWidth(4.0);
						glBegin(GL_LINES);
						glVertex3f(-0.5, 0.0, 0.0);
						glVertex3f(0.5, 0.0, 0.0);
						glEnd();
						glLineWidth(1.0);
						break;
						
	//				case 8:
	//					glShadeModel(GL_SMOOTH);
	//					glutSolidTeapot(0.4);
	//					break;
	          
					default:
						break;
					}
				glPopMatrix();
				}
			}
	glPopMatrix();
}

//Render objects with Phong shading program
void CMyRenderer::RenderPhongObjects()
{
	CHECK_GL_ERROR();
	// enable Phong shading
	iShaderProgram[EPhongProgram]->Enable( true );

	// render screen size quad with texture (empty -> real scene)
	RenderObjects();
}

//Render objects with Cartoon shading program
void CMyRenderer::RenderCartoonObjects()
{
	// disable Cartoon program
	iShaderProgram[ECartoonProgram]->Enable( false );
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
	RenderObjects();
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

	iShaderProgram[ECartoonProgram]->Enable( true );

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, iTextureId[0] );

	RenderObjects();
}

//Render objects with Bloom shading program
void CMyRenderer::RenderBloomObjects()
{	
	CHECK_GL_ERROR();
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();

	//// render real scene to FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, iColorMapId[EPhongTextureId] );
	CHECK_GL_ERROR();
	bool result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	}  
	CHECK_GL_ERROR();
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, KTextureWidth, KTextureHeight );
	CHECK_GL_ERROR();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();
	// render objects using Phong shading
	glDisable(GL_TEXTURE_2D);

	RenderPhongObjects();

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
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, iColorMapId[EBrightPassTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up shading program (bloom effect)
	iThresholdBrightness->setValue( iThresholdBrightnessValue );
	iShaderProgram[EBrightPassProgram]->Enable( true );

	// render screen size quad with Phong texture (real scene -> bloom effect scene)
	RenderSceneOnQuad( iColorMapId[EPhongTextureId], false );

	CHECK_GL_ERROR();

	// first blur pass

	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, iColorMapId[EFirstBlurTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();

	// set up shading program (blur effect)
	iSampleDistance->setValue( iFirstBlurSampleDistance );
	iMipMapBias->setValue( iFirstBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (bloom effect scene -> blur)	
	RenderSceneOnQuad( iColorMapId[EBrightPassTextureId], true );



	//second blur pass
	// change FBO texture (color attachment)
	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, iColorMapId[ESecondBlurTextureId] );
	result = iFrameBufferObject->IsValid();
	if (false == result)
	{	
		EXIT(-1);
	} 
	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, KTextureWidth, KTextureHeight );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CHECK_GL_ERROR();


	// set up shading program (blur effect)
	iSampleDistance->setValue( iSecondBlurSampleDistance );
	iMipMapBias->setValue( iSecondBlurMipMapBias );
	iShaderProgram[EBlurProgram]->Enable( true );

	// render screen size quad with texture (bloom effect scene -> blur)	
	RenderSceneOnQuad( iColorMapId[EFirstBlurTextureId], false );

	////disabled the third blur pass

	////	// change FBO texture (color attachment)
	////	iFrameBufferObject->AttachTexture( GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, iColorMapId[EThirdBlurTextureId] );
	////	result = iFrameBufferObject->IsValid();
	////	if (false == result)
	////		{	
	////		EXIT(-1);
	////		} 
	////	iFrameBufferObject->Enable( true, GL_COLOR_ATTACHMENT0_EXT, KTextureWidth, KTextureHeight );
	////	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	////	CHECK_GL_ERROR();
	////	
	////	
	////	// set up shading program (blur effect)
	////	iSampleDistance->setValue( iThirdBlurSampleDistance );
	////	iMipMapBias->setValue( iSecondBlurMipMapBias );
	////	iShaderProgram[EBlurProgram]->Enable( true );
	////	
	////	// render screen size quad with texture (bloom effect scene -> blur)	
	////	RenderSceneOnQuad( iColorMapId[ESecondBlurTextureId], false );


	// set up shading program (combiner)
	iShaderProgram[ECombineProgram]->Enable( true );


	CHECK_GL_ERROR();	

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//disable any frame buffer object. We want to render to the screen in this final pass
	iFrameBufferObject->Enable( false, GL_BACK, iScreenWidth, iScreenHeight );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// blending using multi texture (real scene + blurred bloom scene)
	// render screen size quad with multi texture
	//	RenderSceneOnQuad( iColorMapId[EPhongTextureId], iColorMapId[EFirstBlurTextureId], iColorMapId[ESecondBlurTextureId], iColorMapId[EThirdBlurTextureId] );
	RenderSceneOnQuad( iColorMapId[EPhongTextureId], iColorMapId[EFirstBlurTextureId], iColorMapId[ESecondBlurTextureId] );	
	//	RenderSceneOnQuad( iColorMapId[EPhongTextureId], iColorMapId[EFirstBlurTextureId] );	

	//functions to test separate buffers
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
	//CShaderProgram::DisableAll();
	//RenderSceneOnQuad( iColorMapId[EPhongTextureId], false );
	//RenderSceneOnQuad( iColorMapId[EBrightPassTextureId], false );	
	//RenderSceneOnQuad( iColorMapId[EFirstBlurTextureId], false );
	//RenderSceneOnQuad( iColorMapId[ESecondBlurTextureId], false );	
	//RenderSceneOnQuad( iColorMapId[EThirdBlurTextureId], false );

}


void CMyRenderer::RenderScene()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();

	switch ( iShadingType )
	{
	case EPhong:
		{
			glDisable(GL_TEXTURE_2D);
			RenderPhongObjects();
			break;				
		}
	case ECartoon:
		{
			glEnable(GL_TEXTURE_2D);
			RenderCartoonObjects();
			break;				
		}
	case EBloom:
		{
			glEnable(GL_TEXTURE_2D);
			RenderBloomObjects();
			break;
		}
	default:
		{
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_LIGHTING );
			CShaderProgram::DisableAll();
			RenderObjects();
			break;
		}
	}
	CHECK_GL_ERROR();
	CShaderProgram::DisableAll();	
	CalculateFramesPerSecond();
	DrawText(); 
	CHECK_GL_ERROR();	

	glFlush();

	glutSwapBuffers();	
}

void CMyRenderer::RenderSceneOnQuad( GLuint aColorMapId, bool aGenerateMipMap )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, aColorMapId );
	if ( aGenerateMipMap )
	{
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	}

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2d( GL_TEXTURE0, 0, 0 );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 0 );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 1 );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 0, 1 );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}


void CMyRenderer::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, aColorMapId0 );

	glActiveTexture( GL_TEXTURE1 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId1 );

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2d( GL_TEXTURE0, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 0 );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 0 );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 1 );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 1 );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_2D );

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}


void CMyRenderer::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, aColorMapId0 );

	glActiveTexture( GL_TEXTURE1 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId1 );

	glActiveTexture( GL_TEXTURE2 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId2 );

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2d( GL_TEXTURE0, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE2, 0, 0 );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE2, 1, 0 );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE2, 1, 1 );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE2, 0, 1 );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE2 );
	glDisable( GL_TEXTURE_2D );	

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}


void CMyRenderer::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2, GLuint aColorMapId3 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, aColorMapId0 );

	glActiveTexture( GL_TEXTURE1 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId1 );

	glActiveTexture( GL_TEXTURE2 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId2 );

	glActiveTexture( GL_TEXTURE3 );
	glEnable( GL_TEXTURE_2D );	
	glBindTexture( GL_TEXTURE_2D, aColorMapId3 );

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2d( GL_TEXTURE0, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE2, 0, 0 );
	glMultiTexCoord2d( GL_TEXTURE3, 0, 0 );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE2, 1, 0 );
	glMultiTexCoord2d( GL_TEXTURE3, 1, 0 );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE2, 1, 1 );
	glMultiTexCoord2d( GL_TEXTURE3, 1, 1 );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2d( GL_TEXTURE0, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE1, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE2, 0, 1 );
	glMultiTexCoord2d( GL_TEXTURE3, 0, 1 );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glActiveTexture( GL_TEXTURE0 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE2 );
	glDisable( GL_TEXTURE_2D );	

	glActiveTexture( GL_TEXTURE3 );
	glDisable( GL_TEXTURE_2D );		

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}



//Resize the window for GLUT
//--------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
{
	iScreenWidth=aWidth;
	iScreenHeight=aHeight;

	glViewport(0, 0, (GLsizei)iScreenWidth, (GLsizei)iScreenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)iScreenWidth/(GLdouble)iScreenHeight, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
//-----------------------------------------------------
void RenderSceneWithRenderer()
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->RenderScene();
}

void UpdateSceneWithRenderer( int aValue )
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->UpdateScene( aValue );
}

void UpdateSceneWithRenderer()
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->UpdateScene();
}

void ResizeSceneWithRenderer( int aWidth, int aHeight )
{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->ResizeScene(aWidth, aHeight);
}


