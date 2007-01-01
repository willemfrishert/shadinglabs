#include "CShaderEffect.h"

CShaderEffect::CShaderEffect( CMyRenderer* aRenderer )
{
	iRenderer = aRenderer;

	iTextures.clear();
	iShaderProgram.clear();
	iVertexShader.clear();
	iFragmentShader.clear();
};


CShaderEffect::~CShaderEffect()
{
	for (int i=0, max=static_cast<int>(iTextures.size());i<max;i++)
	{
		delete iTextures.at(i);
	}
	for (int i=0, max=static_cast<int>(iVertexShader.size());i<max;i++)
	{
		delete iVertexShader.at(i);
	}	
	for (int i=0, max=static_cast<int>(iFragmentShader.size());i<max;i++)
	{
		delete iFragmentShader.at(i);
	}	
	for (int i=0, max=static_cast<int>(iShaderProgram.size());i<max;i++)
	{
		delete iShaderProgram.at(i);
	}
};

void CShaderEffect::RenderSceneOnQuad( TTexture & aTexture )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( aTexture.iTarget );
	glBindTexture( aTexture.iTarget, aTexture.iId );
	if ( aTexture.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture.iTarget);
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
	glDisable( aTexture.iTarget );

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}

void CShaderEffect::RenderSceneOnQuad( TTexture& aTexture0, TTexture& aTexture1 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( aTexture0.iTarget );
	glBindTexture( aTexture0.iTarget, aTexture0.iId );
	if ( aTexture0.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture0.iTarget);
	}

	glActiveTexture( GL_TEXTURE1 );
	glEnable( aTexture1.iTarget );
	glBindTexture( aTexture1.iTarget, aTexture1.iId );
	if ( aTexture1.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture1.iTarget);
	}

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
	glDisable( aTexture0.iTarget );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( aTexture1.iTarget );

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}


void CShaderEffect::RenderSceneOnQuad( TTexture& aTexture0, TTexture& aTexture1, TTexture& aTexture2 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( aTexture0.iTarget );
	glBindTexture( aTexture0.iTarget, aTexture0.iId );
	if ( aTexture0.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture0.iTarget);
	}

	glActiveTexture( GL_TEXTURE1 );
	glEnable( aTexture1.iTarget );
	glBindTexture( aTexture1.iTarget, aTexture1.iId );
	if ( aTexture1.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture1.iTarget);
	}

	glActiveTexture( GL_TEXTURE2 );
	glEnable( aTexture2.iTarget );
	glBindTexture( aTexture2.iTarget, aTexture2.iId );
	if ( aTexture2.iGenerateMipMaps )
	{
		glGenerateMipmapEXT(aTexture2.iTarget);
	}

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
	glDisable( aTexture0.iTarget );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( aTexture1.iTarget );

	glActiveTexture( GL_TEXTURE2 );
	glDisable( aTexture2.iTarget );	

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}


void CShaderEffect::RenderSceneOnQuad( TTexture& aTexture0, TTexture& aTexture1, TTexture& aTexture2, TTexture& aTexture3 )
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glActiveTexture( GL_TEXTURE0 );
	glEnable( aTexture0.iTarget );
	glBindTexture( aTexture0.iTarget, aTexture0.iId );
	if ( aTexture0.iGenerateMipMaps )
	{
		glGenerateMipmapEXT( aTexture0.iTarget );
	}

	glActiveTexture( GL_TEXTURE1 );
	glEnable( aTexture1.iTarget );
	glBindTexture( aTexture1.iTarget, aTexture1.iId );
	if ( aTexture1.iGenerateMipMaps )
	{
		glGenerateMipmapEXT( aTexture1.iTarget );
	}

	glActiveTexture( GL_TEXTURE2 );
	glEnable( aTexture2.iTarget );
	glBindTexture( aTexture2.iTarget, aTexture2.iId );
	if ( aTexture2.iGenerateMipMaps )
	{
		glGenerateMipmapEXT( aTexture2.iTarget );
	}

	glActiveTexture( GL_TEXTURE3 );
	glEnable( aTexture3.iTarget );
	glBindTexture( aTexture3.iTarget, aTexture3.iId );
	if ( aTexture3.iGenerateMipMaps )
	{
		glGenerateMipmapEXT( aTexture3.iTarget );
	}

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
	glDisable( aTexture0.iTarget );

	glActiveTexture( GL_TEXTURE1 );
	glDisable( aTexture1.iTarget );

	glActiveTexture( GL_TEXTURE2 );
	glDisable( aTexture2.iTarget );	

	glActiveTexture( GL_TEXTURE3 );
	glDisable( aTexture3.iTarget );		

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
}
