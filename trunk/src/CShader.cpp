#include "CShader.h"

CShader::CShader()
{
	iCompiled = false;
	iShaderId = 0;
	iShaderSource = 0;
	iMemoryAllocated = false;
	
}

CShader::~CShader()
{
	if (iShaderSource!=0)   
	{
		if (iMemoryAllocated)
		{
			delete[] iShaderSource;  // free ASCII Source
		}
	}
	
	if (iCompiled)
	{ 
		glDeleteShader( iShaderId );
		CHECK_GL_ERROR();
	}
}

unsigned long getFileLength(ifstream& file)
{
	if(!file.good()) return 0;
	
	file.seekg(0,ios::end);
	unsigned long len = file.tellg();
	file.seekg(ios::beg);
	
	return len;
}

int CShader::LoadFromFile(char* filename)
{
	ifstream file;
	file.open(filename, ios::in);
	if(!file) return -1;
	
	unsigned long len = getFileLength(file);
	
	if (len==0) return -2;   // "Empty File" 
	
	if (iShaderSource!=0)    // there is already a source loaded, free it!
    {
		if (iMemoryAllocated)
		{
			delete[] iShaderSource;
		}
    }
	
	iShaderSource = (GLubyte*) new char[len+1];
	if (iShaderSource == 0) return -3;   // can't reserve memory
	iMemoryAllocated = true;
	
	
	iShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
													// it is important to 0-terminate the real length later, len is just max possible value...
	
	unsigned int i=0;
	while (file.good())
    {
		iShaderSource[i++] = file.get();       // get character from file
		if (i>len) i=len;   // coding guidelines...
    }
	
	iShaderSource[i] = 0;  // 0 terminate it.
	
	file.close();
	
	return 0;
}

void CShader::LoadFromMemory(const char* program)
{
	if (iShaderSource!=0)    // there is already a source loaded, free it!
    {
		if (iMemoryAllocated)
			delete[] iShaderSource;
    }
	iMemoryAllocated = false;
	iShaderSource = (GLubyte*) program;
	
}

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
	return ( GL_FALSE == status )? false:true;
}

bool CShader::Compile()
{
	
	bool status = true;
	
	if (iShaderSource==0) return false;

	GLint length = (GLint) strlen((const char*)iShaderSource);
	glShaderSource(iShaderId, 1, (const GLchar **)&iShaderSource, &length);
	CHECK_GL_ERROR();

	glCompileShader(iShaderId); 
	CHECK_GL_ERROR();

	status = CompilerLog();
	
	if(status)
	{
		iCompiled = true;
	}
	return status;
}
