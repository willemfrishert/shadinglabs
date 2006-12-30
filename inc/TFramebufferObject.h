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

#pragma once
#include "Basic.h"

class TFramebufferObject
{
public:
	TFramebufferObject();
	virtual ~TFramebufferObject();

	void Enable( bool aStatus,  GLenum aBuffer, const int aViewportWidth, const int aViewportHeight );

	virtual void AttachTexture( GLenum aAttachment, GLenum aTextureType, GLuint texId,int aMipmapLevel = 0, int aZSlice = 0);
	virtual void AttachRenderBuffer( GLenum aAttachment, GLuint buffId );
	void Unattach( GLenum aAttachment );
	bool IsValid();

	/// Is attached type GL_RENDERBUFFER_EXT or GL_TEXTURE?
	GLenum GetAttachedType( GLenum aAttachment );

	/// What is the Id of Render buffer/texture currently 
	/// attached to "attachment?"
	GLuint GetAttachedId( GLenum aAttachment );

	/// Which mipmap level is currently attached to "attachment?"
	GLint  GetAttachedMipLevel( GLenum aAttachment );

	/// Which cube face is currently attached to "attachment?"
	GLint  GetAttachedCubeFace( GLenum aAttachment );

	/// Which z-slice is currently attached to "attachment?"
	GLint  GetAttachedZSlice( GLenum aAttachment );

	/// Return number of color attachments permitted
	static GLint GetMaxColorAttachments();

private:
	GLuint iFramebufferObjectId;
	void  FramebufferAttachTexture( GLenum aAttachment, GLenum aTextureType, GLuint texId, int aMipmapLevel, int aZSlice );
};
