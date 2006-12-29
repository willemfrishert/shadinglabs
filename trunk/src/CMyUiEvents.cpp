
//INCLUDE FILES
#include "CMyUiEvents.h"


//CONSTANTS
#ifdef USE_RADIANS
const float KAngleChangeAmount = 0.001f;
#elif defined USE_DEGREES
const float KAngleChangeAmount = 0.05f;
#endif

const float KScaleChangeAmount = 1.0f;
const float KFocusChangeAmount = 0.0005f;
const float KFocusAreaAmount = 1.0f;

//INIT STATIC DATA
CMyUiEvents* CMyUiEvents::iCurrentUi = 0;

//Constructor, saves a pointer to the renderer
CMyUiEvents::CMyUiEvents( CMyRenderer* aRenderer )
{
	iRenderer = aRenderer;

	iXRotation = 0;
	iYRotation = 0;
	iXRotationOld = 0;
	iYRotationOld = 0;

	iMouseX = 0;
	iMouseY = 0;

	CMyUiEvents::iCurrentUi = this;
}

//Destructor
CMyUiEvents::~CMyUiEvents()
{
	CMyUiEvents::iCurrentUi = 0;
}

//KEYBOARD Functionality for GLUT
//--------------------------------
void CMyUiEvents::ProcessNormalKeys(unsigned char aKey, int aX, int aY)
{
	switch (aKey)
	{
	case 3:             /* Ctrl-C */
	case 27:            /* ESC */
		exit(0);
		break;
	case ']':
		iRenderer->iMipMapBias->setValue(iRenderer->iMipMapBias->getValue()+1.0);
		break;
	case '[':
		iRenderer->iMipMapBias->setValue(iRenderer->iMipMapBias->getValue()-1.0);
		break;
	case 'q':
		iRenderer->iFirstBlurSampleDistance -= 0.0005f;
		break;		
	case 'w':
		iRenderer->iFirstBlurSampleDistance += 0.0005f;
		break;
	case 'a':
		iRenderer->iSecondBlurSampleDistance -= 0.0005f;
		break;		
	case 's':
		iRenderer->iSecondBlurSampleDistance += 0.0005f;
		break;
		//		case 'z':
		//			iRenderer->iThirdBlurSampleDistance -= 0.0005f;
		//			break;		
		//		case 'x':
		//			iRenderer->iThirdBlurSampleDistance += 0.0005f;
		//			break;
	case 'e':
		iRenderer->iThresholdBrightnessValue -= 0.1f;
		break;		
	case 'r':
		iRenderer->iThresholdBrightnessValue += 0.1f;
		break;
	case '0':
		iRenderer->iShadingType = CMyRenderer::ENone;
		break;
	case '1':
		iRenderer->iShadingType = CMyRenderer::EPhong;
		break;
	case '2':
		iRenderer->iShadingType = CMyRenderer::ECartoon;
		break;
	case '3':
		iRenderer->iShadingType = CMyRenderer::EBloom;
		break;
	case 't':
		if ( 1 == iRenderer->iCombineTextures->getValue() )
			iRenderer->iCombineTextures->setValue( 0 );
		else
			iRenderer->iCombineTextures->setValue( 1 );
		break;
	}
}

// Used to handle the keyboard input (not ASCII Characters)
void CMyUiEvents::ProcessCursorKeys(int aKey, int aX, int aY)
{
}

void CMyUiEvents::ProcessMouseEvent( int aButton, int aState, int aX, int aY )
{	
	iMouseY = aY;
	iMouseX = aX;

	switch (aButton)
	{
	case GLUT_LEFT_BUTTON:
		{
			if (GLUT_DOWN == aState)
			{
				iMouseButtonDown = EMouseDownLeft;
				iXRotationOld = iXRotation;
				iYRotationOld = iYRotation;
			}
			else
			{
				iMouseButtonDown = EMouseUp;
			}
			break;
		}
	case GLUT_RIGHT_BUTTON:
		{
			if (GLUT_DOWN == aState)
			{
				iMouseButtonDown = EMouseDownRight;
			}
			else
			{
				iMouseButtonDown = EMouseUp;
			}
			break;
		}
	default:
		{
			break;
		}
	}
}

void CMyUiEvents::ProcessMouseMotionEvent( int aX, int aY )
{
	switch ( iMouseButtonDown )
	{
	case EMouseDownLeft:
		{		
			iXRotation = iXRotationOld + (float)(aY - iMouseY) / 4.0;
			iYRotation = iYRotationOld + (float)(aX - iMouseX) / 4.0;

			iRenderer->iXRotation = iXRotation;
			iRenderer->iYRotation = iYRotation;			
			break;
		}
	case EMouseDownRight:
		{
			iRenderer->iZoom = iRenderer->iZoom + ( ( ( (float)aY - (float)this->iMouseY) / iRenderer->iScreenHeight) * 20);
			this->iMouseY = aY;				
			break;
		}
	default:
		{
			break;
		}
	}
}


//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
void ProcessNormalKeysWithUi( unsigned char aKey, int aX, int aY )
{
	if ( NULL != CMyUiEvents::iCurrentUi )
	{
		CMyUiEvents::iCurrentUi->ProcessNormalKeys( aKey, aX, aY  );
	}
}

void ProcessCursorKeysWithUi( int aKey, int aX, int aY )
{
	if ( NULL != CMyUiEvents::iCurrentUi )
	{
		CMyUiEvents::iCurrentUi->ProcessCursorKeys( aKey, aX, aY );
	}
}

void ProcessMouseEventWithUi( int aButton, int aState, int aX, int aY)
{
	if ( NULL != CMyUiEvents::iCurrentUi )
	{
		CMyUiEvents::iCurrentUi->ProcessMouseEvent( aButton, aState, aX, aY );
	}
}

void ProcessMouseMotionEventWithUi( int aX, int aY )
{
	if ( NULL != CMyUiEvents::iCurrentUi )
	{
		CMyUiEvents::iCurrentUi->ProcessMouseMotionEvent( aX, aY );
	}
}