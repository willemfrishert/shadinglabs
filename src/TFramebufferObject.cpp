/*
Copyright (c) 2005, 
	Aaron Lefohn	(lefohn@cs.ucdavis.edu)
	Robert Strzodka (strzodka@caesar.de)
	Adam Moerschell (atmoerschell@ucdavis.edu)
All rights reserved.

This software is licensed under the BSD open-source license. See
http://www.opensource.org/licenses/bsd-license.php for more detail.

*************************************************************
Redistribution and use in source and binary forms, with or 
without modification, are permitted provided that the following 
conditions are met:

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer. 

Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution. 

Neither the name of the University of Californa, Davis nor the names of 
the contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "TFramebufferObject.h"
#include <iostream>
using namespace std;

TFramebufferObject::TFramebufferObject()
	{
	glGenFramebuffersEXT(1, &iFramebufferObjectId);
	}

TFramebufferObject::~TFramebufferObject() 
	{
	glDeleteFramebuffersEXT(1, &iFramebufferObjectId);
	}

void TFramebufferObject::Enable( bool aStatus,  GLenum aBuffer, const int aViewportWidth, const int aViewportHeight ) 
	{
	if (aStatus)
		{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
		}
	else
		{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
//	glDrawBuffer( aBuffer );
	glViewport(0, 0, aViewportWidth, aViewportHeight);
	}

void TFramebufferObject::AttachTexture(GLenum aAttachment, GLenum aTextureTarget, GLuint aTextureId, int aMipmapLevel, int aZSlice)
	{
	GLint previousFrameBufferId;
	// store previous frame buffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
//#ifndef NDEBUG
//	if( GetAttachedId(aAttachment) != aTextureId ) 
//		{
//#endif
		FramebufferAttachTexture( aAttachment, aTextureTarget, aTextureId, aMipmapLevel, aZSlice );
CHECK_GL_ERROR();
//#ifndef NDEBUG
//		}
//	else
//		{
//		cerr << "TFramebufferObject::AttachTexture PERFORMANCE WARNING:\n" << "\tRedundant bind of texture (id = " << aTextureId << ").\n" << "\tHINT : Compile with -DNDEBUG to remove this warning.\n";
//		}
//#endif
	//restore previous frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
}

void TFramebufferObject::AttachRenderBuffer( GLenum aAttachment, GLuint buffId )
  {
	GLint previousFrameBufferId;	
	// store previous frame buffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);

#ifndef NDEBUG
	if( GetAttachedId(aAttachment) != buffId )
    {
#endif

    glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, aAttachment, GL_RENDERBUFFER_EXT, buffId);

#ifndef NDEBUG
    }
	else
    {
		cerr << "TFramebufferObject::AttachRenderBuffer PERFORMANCE WARNING:\n" << "\tRedundant bind of render buffer (id = " << buffId << ")\n" << "\tHINT : Compile with -DNDEBUG to remove this warning.\n";
    }
#endif
	//restore previous frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
  }

void TFramebufferObject::Unattach( GLenum aAttachment )
  {
	GLint previousFrameBufferId;	
	// store previous frame buffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLenum type = GetAttachedType(aAttachment);

	switch(type)
    {
    case GL_NONE:
    break;
    case GL_RENDERBUFFER_EXT:
      AttachRenderBuffer( aAttachment, 0 );
    break;
    case GL_TEXTURE:
      AttachTexture( aAttachment, GL_TEXTURE_2D, 0 );
    break;
    default:
      cerr << "TFramebufferObject::unbind_attachment ERROR: Unknown attached resource type\n";
    }
	
	//restore previous frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
  }

GLint TFramebufferObject::GetMaxColorAttachments()
  {
	GLint maxAttach = 0;
	glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &maxAttach );
	return maxAttach;
  }

void TFramebufferObject::FramebufferAttachTexture(GLenum aAttachment, GLenum aTextureTarget, GLuint aTextureId, int aMipmapLevel, int aZSlice)
  {
	if (aTextureTarget == GL_TEXTURE_1D) 
    {
		glFramebufferTexture1DEXT( GL_FRAMEBUFFER_EXT, aAttachment, GL_TEXTURE_1D, aTextureId, aMipmapLevel );
    }
	else
    {
    if (aTextureTarget == GL_TEXTURE_3D) 
      {
      glFramebufferTexture3DEXT( GL_FRAMEBUFFER_EXT, aAttachment, GL_TEXTURE_3D, aTextureId, aMipmapLevel, aZSlice );
      }
    else
      {
      // Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
      glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, aAttachment, GL_TEXTURE_2D, aTextureId, aMipmapLevel );
      }
    }
  }

bool TFramebufferObject::IsValid()
  {
  bool result = false;
  
  GLenum status;                                            
  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT: // Everything's OK
      result = true;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n";
      break;
//    case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
//      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT\n";
//      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n";
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_UNSUPPORTED_EXT\n";
      break;
//    case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
//      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "GL_FRAMEBUFFER_STATUS_ERROR_EXT\n";
//      break;
    default:
      cout << "glift::CheckFramebufferStatus() ERROR:\n\t" << "Unknown ERROR\n";
    }
  
  return result;
  }

/// Accessors
GLenum TFramebufferObject::GetAttachedType( GLenum aAttachment )
  {
	GLint previousFrameBufferId;
	// store previous frame buffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLint type = 0;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT, &type);
	
	//restore previous frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
	
	return GLenum(type);
  }

GLuint TFramebufferObject::GetAttachedId( GLenum aAttachment )
  {
	GLint previousFrameBufferId;
	// store previous framebuffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLint id = 0;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT, &id);
	
	//restore previous framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
	
	return GLuint(id);
  }

GLint TFramebufferObject::GetAttachedMipLevel( GLenum aAttachment )
  {
	GLint previousFrameBufferId;
	// store previous framebuffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLint level = 0;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT, &level);
	
	//restore previous framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
	
	return level;
  }

GLint TFramebufferObject::GetAttachedCubeFace( GLenum aAttachment )
  {
	GLint previousFrameBufferId;
	// store previous framebuffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLint level = 0;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT, &level);
	
	//restore previous framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
	return level;
  }

GLint TFramebufferObject::GetAttachedZSlice( GLenum aAttachment )
  {
	GLint previousFrameBufferId;
	// store previous framebuffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &previousFrameBufferId );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFramebufferObjectId);
	
	GLint slice = 0;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT, &slice);
	
	//restore previous framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFrameBufferId);
	
	return slice;
  }



