// ************************************************************************
// Shader Program : Manage Shader (Vertex/Fragment)
// ************************************************************************
#include "CShader.h"

char* aGLSLErrorString[] = {
	"(e0000) GLSL not enabled",
	"(e0001) not a valid program object",
	"(e0002) not a valid object",
	"(e0003) out of memory",
	"(e0004) unknown compiler error"};
//----------------------------------------------------------------------------- 

CShader::CShader()
{
	compiler_log = 0;
	is_compiled = false;
	iShaderId = 0;
	ShaderSource = 0;
	_memalloc = false;
	
}

//----------------------------------------------------------------------------- 

CShader::~CShader()
{
	if (compiler_log!=0) free(compiler_log);
	if (ShaderSource!=0)   
		{
		if (_memalloc)
			delete[] ShaderSource;  // free ASCII Source
		}
	
	if (is_compiled)
		{ 
		glDeleteShader( iShaderId );
		CHECK_GL_ERROR();
		}
}

//----------------------------------------------------------------------------- 
unsigned long getFileLength(ifstream& file)
{
	if(!file.good()) return 0;
	
	file.seekg(0,ios::end);
	unsigned long len = file.tellg();
	file.seekg(ios::beg);
	
	return len;
}


//----------------------------------------------------------------------------- 

int CShader::LoadFromFile(char* filename)
{
	ifstream file;
	file.open(filename, ios::in);
	if(!file) return -1;
	
	unsigned long len = getFileLength(file);
	
	if (len==0) return -2;   // "Empty File" 
	
	if (ShaderSource!=0)    // there is already a source loaded, free it!
    {
		if (_memalloc)
			delete[] ShaderSource;
    }
	
	ShaderSource = (GLubyte*) new char[len+1];
	if (ShaderSource == 0) return -3;   // can't reserve memory
	_memalloc = true;
	
	
	ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
													// it is important to 0-terminate the real length later, len is just max possible value...
	
	unsigned int i=0;
	while (file.good())
    {
		ShaderSource[i++] = file.get();       // get character from file
		if (i>len) i=len;   // coding guidelines...
    }
	
	ShaderSource[i] = 0;  // 0 terminate it.
	
	file.close();
	
	return 0;
}

//----------------------------------------------------------------------------- 

void CShader::LoadFromMemory(const char* program)
{
	if (ShaderSource!=0)    // there is already a source loaded, free it!
    {
		if (_memalloc)
			delete[] ShaderSource;
    }
	_memalloc = false;
	ShaderSource = (GLubyte*) program;
	
}


// ----------------------------------------------------
// Compiler Log: Ausgabe der Compiler Meldungen in String

bool CShader::CompilerLog(void)
{    	
	GLint status=0;
	GLint type=0;
	glGetShaderiv( iShaderId, GL_COMPILE_STATUS, &status );
	glGetShaderiv( iShaderId, GL_SHADER_TYPE, &type );
	
	string typeString = (GL_VERTEX_SHADER==type)?"Vertex":"Fragment";
	
	if( GL_FALSE == status )
		{
		const int KLengthMax=500;
		char infoLog[KLengthMax];
		
		glGetShaderInfoLog( iShaderId, KLengthMax, NULL, infoLog);
		cout << " - "<< typeString <<" Shader compilation FAILED:\n" << infoLog << endl;
		
		//	exit(-1);
		}
	else
		{
		cout << " - "<< typeString <<" Shader compilation OK.\n";
		}
	return (0==status)? false:true;
}

// ----------------------------------------------------

bool CShader::Compile()
{
	
	bool status = true;
	
  if (ShaderSource==0) return false;
	
  GLint	length = (GLint) strlen((const char*)ShaderSource);
  glShaderSource(iShaderId, 1, (const GLchar **)&ShaderSource, &length);
  CHECK_GL_ERROR();
	
  glCompileShader(iShaderId); 
  CHECK_GL_ERROR();

	status = CompilerLog();
	
	return status;
}
