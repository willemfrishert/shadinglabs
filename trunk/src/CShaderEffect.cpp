#include "CShaderEffect.h"

CShaderEffect::CShaderEffect(CMyRenderer* aRenderer)
: iRenderer(aRenderer)
{

}
CShaderEffect::~CShaderEffect()
{
	iRenderer = NULL;
}

void CShaderEffect::RenderSceneOnQuad( GLuint aColorMapId, bool aGenerateMipMap )
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


void CShaderEffect::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1 )
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


void CShaderEffect::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2 )
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


void CShaderEffect::RenderSceneOnQuad( GLuint aColorMapId0, GLuint aColorMapId1, GLuint aColorMapId2, GLuint aColorMapId3 )
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