#include "CVertexShader.h"

CVertexShader::CVertexShader()
{
	iShaderId = glCreateShader( GL_VERTEX_SHADER );
	CHECK_GL_ERROR();
}

CVertexShader::~CVertexShader()
{
}
