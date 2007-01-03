#include "Basic.h"
#include "CMyRenderer.h"
#include "CMyUiEvents.h"

//CONSTANTS
const int KWindowPositionX = 250;
const int KWindowPositionY = 150;
//const int KWindowWidth = 800;
//const int KWindowHeight = 600;
const int KWindowWidth = 512;
const int KWindowHeight = 512;

// GL ERROR CHECK
int CheckGLError(char *aFile, int aLine)
{
	int result = 0;
	GLenum glError = glGetError();

	while (glError != GL_NO_ERROR)
	{
		cout << "Error in file " << aFile << " at line: " << aLine << endl;
		cout << "GL Error #" << glError << ": " << gluErrorString(glError) << endl << endl;
		result = 1;
		glError = glGetError();
	}

	return result;
}

void exit(char *aFile, int aLine, int aExitCode)
{
	string dummy;
	CheckGLError(aFile, aLine);

	cout << "Press any key to continue: ";
	getchar();

	exit(aExitCode);
}


void initContext(int argc, char **argv)
{
	glutInit( &argc, argv );

	//Init Window
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition( KWindowPositionX, KWindowPositionY );
	glutInitWindowSize( KWindowWidth, KWindowHeight );
	glutCreateWindow("Lets do some shading");

#ifdef _WIN32
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif	

	//Set some Glut functions
	glutKeyboardFunc( ProcessNormalKeysWithUi );
	glutSpecialFunc(  ProcessCursorKeysWithUi );
	glutMouseFunc(    ProcessMouseEventWithUi );
	glutMotionFunc(   ProcessMouseMotionEventWithUi );

	glutDisplayFunc(   RenderSceneWithRenderer );
	glutTimerFunc( 10,UpdateSceneWithRenderer, 10 );
	//	glutIdleFunc( UpdateSceneWithRenderer );
	glutReshapeFunc(   ResizeSceneWithRenderer );
}


/**
* Main method.
* This method is called first an it will do the following:
*  -# Creates the renderer and ui objects
*  -# Initializes Glut
*  -# Calls renderer->CreateMesh to create a CMesh
*  -# Starts the glutMainLoop()
* @param argc amount of arguments for glutInit().
* @param argv arguments for glutInit().
* @return 0
*/
int main(int argc, char **argv)
{
	//Initialize GL using GLUT functions
	initContext( argc, argv );

	//Init renderer
	CMyRenderer* renderer = new CMyRenderer( KWindowWidth, KWindowHeight );
	CMyUiEvents* ui = new CMyUiEvents( renderer );

	//glutFullScreen();

	//START GL loop
	glutMainLoop();

	//  GLint i;
	//  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &i);
	//  cout << "i = " << i << endl;

	delete ui;
	delete renderer;
	return 0;
}
