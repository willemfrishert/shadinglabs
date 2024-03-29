#pragma once

#include "Basic.h"
#include "CShaderProgram.h"
#include "CVertexShader.h"
#include "CFragmentShader.h"
#include "TFramebufferObject.h"
#include "ShaderUniformValue.h"
#include "CMesh.h"
#include "TTexture.h"
#include "CShaderPhong.h"
#include "CShaderCartoon.h"
#include "CShaderBloom.h"

/** \brief Rendering class.
*
* This class contains all the functions necessary for rendering.
*/
class CMyRenderer
	{
		
	friend class CMyUiEvents;	
	//PUBLIC FUNCTIONS
	//------------------
	public:

		//CONSTRUCTORS
		/**
		* Default constructor.
		* Initializes private data, and sets the window size to ( aWidth, aHeight ).
		* @param aWidth width of the screen
		* @param aHeight height of the screen
		*/
		CMyRenderer( const int aWidth, const int aHeight );
		
		//DESTRUCTOR
		//------------------
		/**
		* Default destructor.
		*/
		~CMyRenderer();

		void UpdateScene( int aValue );
		void UpdateScene();
		
		//MEMBER FUNCTIONS
		///Renders a scene. This function is called from OpenGL, and renders a scene.
		void RenderScene();
		
		///Resize the rendering window
		void ResizeScene(const int aWidth, const int aHeight);
		
		//GETTERS
		//---------------------------------------
		/**
		* Get a width of the screen.
		* @return iScreenWidth
		*/
		int Width() const
			{ return iScreenWidth; };

		/**
		* Get a height of the screen.
		* @return iScreenHeight
		*/
		int Height() const
			{ return iScreenHeight; };

		//Render current object
		void RenderObjects();

		CShaderBloom* getShaderBloom()
		{
			return iShaderBloom;
		}

	//PRIVATE FUNCTIONS
	//------------------
	private:
		//Constructors will call this one
		void InitMain();
		
		//Create scene
		void CreateScene();

		//Constructors will call this one
		void InitLights();
		
		//Render objects with Phong shading program
		void RenderPhongObjects();
		
		//Render objects with Cartoon shading program
		void RenderCartoonObjects();
		
		//Render objects with Bloom shading program
		void RenderBloomObjects();

		// FPS calculation. Frames drawn in a second.
		void CalculateFramesPerSecond();

		/// Text output. Prints text on the screen.
		void DrawText() const;
    
	//PUBLIC STATIC DATA
	//------------------
	public:
		/// A static pointer to the current renderer object.
		/// This is used to be able to pass rendering method to OpenGL.
		static CMyRenderer* iCurrentRenderer;

	//PRIVATE DATA
	//------------------
	private:
		string  iFpsCountString;	//< String for FPS
		
		int iFrame;
		float iCurrentTime;		//< Current time
		float iPreviousTime;

		int iScreenHeight;	//< The height of the screen
		int iScreenWidth;	//< The width of the screen
		
		//rotation info changes with the mouse
		GLfloat iXRotation;
		GLfloat iYRotation;
		
		//zoom changes with the mouse
		GLfloat iZoom;
		
		GLfloat iAngle;
		GLfloat iWobble;
		
		//GLuint iTextureId[KNumberOfTextures];
  //  
		//CShaderProgram* iShaderProgram[KNumberOfShadingPrograms];
		//CShader* iVertexShader[KNumberOfShadingPrograms];
		//CShader* iFragmentShader[KNumberOfShadingPrograms];
		//
		//TFramebufferObject* iFrameBufferObject;

		CShaderPhong* iShaderPhong;
		CShaderCartoon* iShaderCartoon;
		CShaderBloom* iShaderBloom;

		//enum TTextureId
		//{
		//	EDepthTexture = 0,
		//	EPhongTexture,
		//	EBrightPassTexture,
		//	EFirstBlurTexture,
		//	ESecondBlurTexture,
		//	EThirdBlurTexture
		//};		

		//TTexture* iTextures[KNumberOfTextureMaps+KNumberOfDepthMaps];
		
		enum TShading
		{
			EPhong = 0,
			ECartoon,
			EBloom,
			ENone,
		} iShadingType;
		
		//ShaderUniformValue<int>* iLightMap;
		//
		//ShaderUniformValue<float>* iThresholdBrightness;
		//float iThresholdBrightnessValue;
		//
		//ShaderUniformValue<float>* iSampleDistance;
		//float iFirstBlurSampleDistance;
		//float iSecondBlurSampleDistance;
		//float iThirdBlurSampleDistance;
		//
		//ShaderUniformValue<int>* iOriginalTexture;
		//ShaderUniformValue<int>* iBlurTexture1;
		//ShaderUniformValue<int>* iBlurTexture2;
		//ShaderUniformValue<int>* iBlurTexture3;
		//ShaderUniformValue<float>* iMipMapBias;
		//ShaderUniformValue<int>* iCombineTextures;
		//float iFirstBlurMipMapBias;
		//float iSecondBlurMipMapBias;
		//float iThirdBlurMipMapBias;
		
		CMesh* chevy;
	};


//EXTERNAL FUNCTIONS
//Reference to rendering function. Used to pass the render object's rendering function to OpenGL.
extern void RenderSceneWithRenderer();

//Reference to rendering function. Used to update variables at a certain time interval.
extern void UpdateSceneWithRenderer( int value );

//Reference to rendering function. Used to update variables while idle.
extern void UpdateSceneWithRenderer();

//Reference to resize function. Used to pass the render object's resize function to OpenGL.
extern void ResizeSceneWithRenderer( int aWidth, int aHeight );
