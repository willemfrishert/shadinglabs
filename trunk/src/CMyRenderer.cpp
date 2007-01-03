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
, iShadingType( ENone )
, iCurrentTime( 0 )
, iPreviousTime( 0 )
, iZoom( -18.0f )
, iFpsCountString("")
{
	InitMain();
}


//DESTRUCTOR releases the used heap
//
CMyRenderer::~CMyRenderer()
{
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

	iShaderPhong = new CShaderPhong( this );
	iShaderCartoon = new CShaderCartoon( this );
	iShaderBloom = new CShaderBloom( this );

	CalculateFramesPerSecond();

	CMyRenderer::iCurrentRenderer = this;
}

void CMyRenderer::CreateScene()
{
	Load3ds modelLoader;

	this->chevy = modelLoader.Create("3ds/chevy.3ds");
}

void CMyRenderer::InitLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//CREATE THE LIGHT
	GLfloat light0_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_specular[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_position[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	glLightfv (GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv (GL_LIGHT0, GL_POSITION, light0_position); 
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
		ostr << "FPS:" << setprecision ( 3 ) << setw(3) << iFrame*1000.0/( iCurrentTime - iPreviousTime );
		ostr << " iFSD:" << iShaderBloom->getFirstBlurSampleDistance();
		ostr << " iSSD:" << iShaderBloom->getSecondBlurSampleDistance();
		ostr << " iTSD:" << iShaderBloom->getThirdBlurSampleDistance();
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
	
//	glRotatef(25, 1, 0, 0);
	glRotatef(10, 0, 1, 0);	
	glRotatef(10, 1, 0, 0);

	chevy->draw();

	glPopMatrix();
}

//void CMyRenderer::RenderObjects()
//{
//	glMatrixMode( GL_MODELVIEW );
//	glPushMatrix();
//	glTranslatef(0, 0, iZoom);
//	/* "World" rotation, controlled by mouse */
//	glRotatef(iXRotation, 1, 0, 0);
//	glRotatef(iYRotation, 0, 1, 0);
//
//	//glRotatef(27, 0, 1, 0);	
//	//glRotatef(-100, 1, 0, 0);
//	//	glPushMatrix();
//
//	//chevy->draw();
//	//	glPopMatrix();
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++) 
//		{
//			glPushMatrix();
//
//			glTranslatef(i-1, j-1, 0);
//			//	glRotatef(iWobble, 0.5, 1, 0.3);
//
//			switch (i*3 + j)
//			{
//			case 0:
//				glShadeModel(GL_FLAT);
//				glutSolidCube(0.5);
//				break;
//
//			case 1:
//				glShadeModel(GL_SMOOTH);
//				glutSolidSphere(0.4, 24, 18);
//				break;
//
//			case 2:
//				glShadeModel(GL_SMOOTH);
//				glutSolidCone(0.3, 0.6, 24, 2);
//				break;
//
//			case 3:
//				glShadeModel(GL_SMOOTH);
//				glutSolidTorus(0.1, 0.3, 12, 24);
//				break;
//
//			case 4:
//				glShadeModel(GL_FLAT);
//				glScalef(0.2, 0.2, 0.2);
//				glEnable(GL_NORMALIZE);
//				glutSolidDodecahedron();
//				break;
//
//			case 5:
//				glShadeModel(GL_FLAT);
//				glScalef(0.4, 0.4, 0.4);
//				glutSolidOctahedron();     /* Still normalizing */
//				break;
//
//			case 6:
//				glShadeModel(GL_FLAT);
//				glScalef(0.4, 0.4, 0.4);
//				glutSolidTetrahedron();    /* Still normalizing */
//				break;
//
//			case 7:
//				glShadeModel(GL_FLAT);
//				glScalef(0.35, 0.35, 0.35);
//				glutSolidIcosahedron();    /* Still normalizing */
//				glDisable(GL_NORMALIZE);
//				break;
//			case 8:
//				glLineWidth(4.0);
//				glBegin(GL_LINES);
//				glVertex3f(-0.5, 0.0, 0.0);
//				glVertex3f(0.5, 0.0, 0.0);
//				glEnd();
//				glLineWidth(1.0);
//				break;
//
//				//				case 8:
//				//					glShadeModel(GL_SMOOTH);
//				//					glutSolidTeapot(0.4);
//				//					break;
//
//			default:
//				break;
//			}
//			glPopMatrix();
//		}
//	}
//	glPopMatrix();
//}


void CMyRenderer::RenderScene()
{
	//glClearColor(0.1, 0.1, 0.1, 1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );	
	glLoadIdentity();
	
	CShaderProgram::DisableAll();

	switch ( iShadingType )
	{
	case EPhong:
		{
			glPolygonMode(GL_BACK,GL_FILL);
			iShaderPhong->use();
			break;				
		}
	case ECartoon:
		{
			iShaderCartoon->use();
			break;				
		}
	case EBloom:
		{
			glPolygonMode(GL_BACK,GL_FILL);
			iShaderBloom->use( iShaderPhong );
			break;
		}
	default:
		{
			glPolygonMode(GL_BACK,GL_FILL);
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_LIGHTING );
			RenderObjects();
			break;
		}
	}
	CHECK_GL_ERROR();
#ifdef SHOW_FPS
	CShaderProgram::DisableAll();
	CalculateFramesPerSecond();
	DrawText(); 
	CHECK_GL_ERROR();	
#endif
	glFlush();

	glutSwapBuffers();	
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
	gluPerspective(45, (GLdouble)iScreenWidth/(GLdouble)iScreenHeight, 0.1, 1000);
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


