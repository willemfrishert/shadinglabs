/*
 *  CShader.cpp
 *  repellingforce
 *
 *  Created by Willem Frishert on 2006-09-29.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "CShader.h"


void CMyRenderer::SetShaders( int& CShaderProgramId, char* CVertexShader, char* CFragmentShader )
{
	cout << "Setting shaders: " << aVertexShader << " and " << aFragmentShader <<endl;
	//INIT NAMES AND STORAGE
	GLuint vertexShaderId, fragmentShaderId;
	char   *vertexShaderStore = NULL, *fragmentShaderStore = NULL;
	
	vertexShaderId		= glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderId	= glCreateShader(GL_FRAGMENT_SHADER);
	
	//LOAD SHADERS
	vertexShaderStore    = TTextFileHandler::TextFileRead( aVertexShader );
	fragmentShaderStore = TTextFileHandler::TextFileRead( aFragmentShader );
	
	//CREATE SOURCE HOLDERS
	const char* vertexShaderStoreC		= vertexShaderStore;
	const char* fragmentShaderStoreC	= fragmentShaderStore;
	glShaderSource(vertexShaderId,   1, &vertexShaderStoreC, NULL);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderStoreC, NULL);
	
	free(vertexShaderStore);
	free(fragmentShaderStore);
	
	//COMPILE
	glCompileShader( vertexShaderId );
	glCompileShader( fragmentShaderId );
	
	//LINK PROGRAM
	CShaderProgramId = glCreateProgram();
	glAttachShader( aShaderProgramId, fragmentShaderId );
	glAttachShader( aShaderProgramId, vertexShaderId );
	glLinkProgram( aShaderProgramId );
	
	//Check the statuses:
	bool status(true);
	status &= VerifyShaderCompilation( vertexShaderId );
	status &= VerifyShaderCompilation( fragmentShaderId );
	status &= VerifyShaderProgram( aShaderProgramId );
	if ( !status ){ exit(-1); }
}
bool CMyRenderer::VerifyShaderCompilation( int aShaderId )
{
	GLint status;
	GLint type;
	glGetShaderiv( aShaderId, GL_COMPILE_STATUS, &status );
	glGetShaderiv( aShaderId, GL_SHADER_TYPE, &type );
	
	string typeString = (GL_VERTEX_SHADER==type)?"Vertex":"Fragment";
	
	if( GL_FALSE == status )
		{
		const int KLengthMax=500;
		char infoLog[KLengthMax];
		
		glGetShaderInfoLog( aShaderId, KLengthMax, NULL, infoLog);
		cout << " - "<< typeString <<" Shader compilation FAILED:\n" << infoLog << endl;
		
		//	exit(-1);
		}
	else
		{
		cout << " - "<< typeString <<" Shader compilation OK.\n";
		}
	return (0==status)? false:true;
}


bool CMyRenderer::VerifyShaderProgram( int CShaderProgramId )
{
	GLint status;
	glGetProgramiv( aShaderProgramId, GL_LINK_STATUS, &status);
	
	if( GL_FALSE == status )
		{
		const int KLengthMax=500;
		char infoLog[KLengthMax];
		glGetProgramInfoLog( aShaderProgramId, KLengthMax, NULL, infoLog);
		cout << " - Program Linking FAILED:\n" << infoLog << endl;
		}
	else
		{
		cout << " - Program Linking OK.\n";
		}
	return (0==status)? false:true;
}