#include "TTexture.h"

TTexture::TTexture(  GLint aMipMapLevel,bool aGenerateMipMaps,GLint aComponents, GLfloat aMaginificationFilter,
				   GLfloat aMinificatationFilter, GLfloat aTextureWrapS, GLfloat aTextureWrapR, GLfloat aEnvMode,
				   GLsizei aHeight,GLsizei aWidth, GLenum aFormat,GLenum aType, GLenum aTarget /*= GL_TEXTURE_2D*/,
				   GLint aZSlice /*= 0*/, GLint aBorder /*= 0*/, GLvoid *aPixels /*= NULL*/ )
: iTarget(aTarget)
, iMipMapLevel(aMipMapLevel)
, iGenerateMipMaps(aGenerateMipMaps)
, iComponents(aComponents)
, iMaginificationFilter(aMaginificationFilter)
, iMinificatationFilter(aMinificatationFilter)
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
	glBindTexture( GL_TEXTURE_2D, iId );

	// set texture parameters
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iMinificatationFilter );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iMaginificationFilter );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, iTextureWrapS );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, iTextureWrapR );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, iEnvMode );

	// set up the texture size, type, format,...
	glTexImage2D( iTarget, iMipMapLevel, iComponents, iWidth, iHeight, iBorder, iFormat, iType, iPixels );

	if (iGenerateMipMaps )
	{
		glGenerateMipmapEXT( GL_TEXTURE_2D );
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