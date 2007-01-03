#pragma once

#include "Basic.h"

class TTexture
{
//METHODS
public:
	TTexture( GLuint aId, GLenum aTarget = GL_TEXTURE_2D);
	TTexture( GLint aMipMapLevel,bool aGenerateMipMaps,GLint aComponents, GLfloat aMaginificationFilter, 
		GLfloat aMinificatationFilter, GLfloat aTextureWrapS, GLfloat aTextureWrapR, GLfloat aEnvMode, 
		GLsizei aHeight,GLsizei aWidth, GLenum aFormat,GLenum aType, GLenum aTarget = GL_TEXTURE_2D, 
		GLint aZSlice = 0, GLint aBorder = 0, GLvoid *aPixels = NULL);
	~TTexture();
protected:
private:

//VARIABLES
public:
	GLuint iId;				// TextureID
	GLenum iTarget;			// GL_TEXTURE_1D, 2D, 3D, CUBEMAP
	GLint iMipMapLevel;		// mipmap level of texture
	bool iGenerateMipMaps;	// wish to generate textures or not?
	GLint iComponents;		// Must be 1, 2, 3, or 4, or one of the following symbolic constants:
							//  GL_DEPTH_COMPONENT, GL_ALPHA, GL_ALPHA4, GL_ALPHA8, GL_ALPHA12,
							//	GL_ALPHA16, GL_LUMINANCE, GL_LUMINANCE4,
							//	GL_LUMINANCE8, GL_LUMINANCE12,
							//	GL_LUMINANCE16, GL_LUMINANCE_ALPHA,
							//	GL_LUMINANCE4_ALPHA4,	GL_LUMINANCE6_ALPHA2,
							//	GL_LUMINANCE8_ALPHA8,	GL_LUMINANCE12_ALPHA4,
							//	GL_LUMINANCE12_ALPHA12,
							//	GL_LUMINANCE16_ALPHA16, GL_INTENSITY,
							//	GL_INTENSITY4, GL_INTENSITY8,
							//	GL_INTENSITY12, GL_INTENSITY16, GL_R3_G3_B2,
							//	GL_RGB, GL_RGB4, GL_RGB5, GL_RGB8, GL_RGB10,
							//	GL_RGB12, GL_RGB16, GL_RGBA, GL_RGBA2,
							//	GL_RGBA4, GL_RGB5_A1,	GL_RGBA8, GL_RGB10_A2,
							//	GL_RGBA12, or	GL_RGBA16.
	GLfloat iMaginificationFilter; // GL_LINEAR, GL_NEAREST
	GLfloat iMinificationFilter; // GL_LINEAR, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR
	GLfloat iTextureWrapS;	// GL_CLAMP_TO_EDGE, GL_REPEAT
	GLfloat iTextureWrapR;	// GL_CLAMP_TO_EDGE, GL_REPEAT
	GLfloat iEnvMode;		// GL_MODULATE, GL_DECAL, GL_REPLACE and GL_BLEND.
	GLsizei iHeight;		// texture height (power of 2)
	GLsizei iWidth;			// texture width (power of 2)
	GLint iBorder;			// texture border (0 or 1)
	GLenum iFormat;			// GL_DEPTH_COMPONENT, GL_COLOR_INDEX, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA.
	GLenum iType;			// GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP,	GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, and GL_FLOAT.
	GLvoid *iPixels;
	GLint iZSlice;	  // Slice number in case 3D

protected:
private:
};