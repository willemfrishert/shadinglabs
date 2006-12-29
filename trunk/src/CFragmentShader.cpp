#include "CFragmentShader.h"

CFragmentShader::CFragmentShader()
{
	iShaderId = glCreateShader( GL_FRAGMENT_SHADER ); 
	CHECK_GL_ERROR();
}

CFragmentShader::~CFragmentShader()
{
}
