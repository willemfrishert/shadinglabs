#pragma once

#include "Basic.h"
#include "CMyRenderer.h"


/** \brief User interface class.
*
* This class is for functions of user interaction.
*/
class CMyUiEvents
	{
	//PUBLIC FUNCTIONS
	public:
		//CONSTRUCTORS
		//CMyUiEvents();
		CMyUiEvents(CMyRenderer* aRenderer);

		//DESTRUCTOR
		~CMyUiEvents();

		//MEMBER FUNCTIONS
		void ProcessNormalKeys( unsigned char aKey, int aX, int aY );
		void ProcessCursorKeys( int aKey, int aX, int aY );
		void ProcessMouseEvent( int aButton, int aState, int aX, int aY );
		void ProcessMouseMotionEvent( int aX, int aY );

	//PUBLIC DATA
	public:
		static CMyUiEvents* iCurrentUi;

	//PRIVATE DATA
	private:
		enum TMouseDownStatus
			{
			EMouseUp = 0,
			EMouseDownLeft,
			EMouseDownRight
			};

		CMyRenderer* iRenderer;
		TMouseDownStatus iMouseButtonDown;

		GLdouble iAngle;
		GLdouble KDegreeToRadian;
		GLfloat iXRotation;
		GLfloat iYRotation;
		GLfloat iXRotationOld;
		GLfloat iYRotationOld;
		int mouseState;
		int iMouseY;
		int iMouseX;
		
	};

extern void ProcessNormalKeysWithUi( unsigned char aKey, int aX, int aY );
extern void ProcessCursorKeysWithUi( int aKey, int aX, int aY );
extern void ProcessMouseEventWithUi( int aButton, int aState, int aX, int aY);
extern void ProcessMouseMotionEventWithUi( int aX, int aY );
extern void ProcessWandSelectionWithUi( GLfloat* wandPos, GLfloat* wandFront );
extern void ProcessReadKeyboardWithUi();

