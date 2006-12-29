/*
 *  CShader.h
 *  repellingforce
 *
 *  Created by Willem Frishert on 2006-09-29.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class Shader
	{
	public:
		Shader(GLenum aShaderType, string aShaderFileName);
		~Shader();
		
		FileName(string aShaderFileName);
		bool LoadShader( int ShaderProgramId);
		ReloadShader();
		DisableShader();
		EnableShader();
		
		ShaderId();
					 
	private:
		int iShaderId;
		int iShaderProgramId;
		string iShaderFileName;
		GLenum iShaderType;
	}