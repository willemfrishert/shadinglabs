#include "TTexture.h"

TTexture::TTexture(  GLuint aId )
: iId(aId)
{
};

TTexture::TTexture(  GLint aMipMapLevel,bool aGenerateMipMaps,GLint aComponents, GLfloat aMaginificationFilter,
				   GLfloat aMinificatationFilter, GLfloat aTextureWrapS, GLfloat aTextureWrapR, GLfloat aEnvMode,
				   GLsizei aHeight,GLsizei aWidth, GLenum aFormat,GLenum aType, GLenum aTarget /*= GL_TEXTURE_2D*/,
				   GLint aZSlice /*= 0*/, GLint aBorder /*= 0*/, GLvoid *aPixels /*= NULL*/ )
: iTarget(aTarget)
, iMipMapLevel(aMipMapLevel)
, iGenerateMipMaps(aGenerateMipMaps)
, iComponents(aComponents)
, iMaginificationFilter(aMaginificationFilter)
, iMinificationFilter(aMinificatationFilter)
, iTextureWrapS(aTextureWrapS)
, iTextureWrapR(aTextureWrapR)
, iEnvMode(aEnvMode)
, iHeight(aHeight)
, iWidth(aWidth)
, iBorder(aBorder)
, iFormat(aFormat)
, iType(aType)
, iPixels(aPixels)
, iZSlice(aZSlice)
{
	glGenTextures( 1, &iId );

	glBindTexture( iTarget, iId );

	// set texture parameters
	glTexParameterf( iTarget, GL_TEXTURE_MIN_FILTER, iMinificationFilter );
	glTexParameterf( iTarget, GL_TEXTURE_MAG_FILTER, iMaginificationFilter );
	glTexParameterf( iTarget, GL_TEXTURE_WRAP_S, iTextureWrapS );
	glTexParameterf( iTarget, GL_TEXTURE_WRAP_T, iTextureWrapR );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, iEnvMode );

	switch( iTarget )
	{
	case GL_TEXTURE_2D:
		{
			glTexImage2D( iTarget, iMipMapLevel, iComponents, iWidth, iHeight, iBorder, iFormat, iType, iPixels );
			break;
		}
	case GL_TEXTURE_1D:
		{
				glTexImage1D (iTarget, iMipMapLevel, iComponents, iWidth, iHeight, iFormat , iType, iPixels);
			break;
		}

	}
	// set up the texture size, type, format,...


	if (iGenerateMipMaps )
	{
		glGenerateMipmapEXT( iTarget );
	}
}

TTexture::~TTexture()
{
	glDeleteTextures(1, &iId);
	if (NULL != iPixels)
	{
		delete iPixels;
	}
}