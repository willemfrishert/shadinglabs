// ----------------------------------------------------------------------------
// ShaderManager: Easy use of (multiple) Shaders
#include "CShaderManager.h"
#include "CVertexShader.h"
#include "CFragmentShader.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
   // free objects
   vector<CShaderProgram*>::iterator  i=_shaderProgramList.begin();
   while (i!=_shaderProgramList.end()) 
   {
        CShaderProgram* o = *i;
        i=_shaderProgramList.erase(i);
        delete o;
   }
}

// ----------------------------------------------------------------------------

CShaderProgram* CShaderManager::LoadFromFile(char* vertexFile, char* fragmentFile) 
{
   CShaderProgram* o = new CShaderProgram();
  
   CVertexShader* tVertexShader = new CVertexShader;
   CFragmentShader* tFragmentShader = new CFragmentShader;

    // load vertex program
   if (vertexFile!=0)
   if (tVertexShader->LoadFromFile(vertexFile) != 0)
   { 
     cout << "error: can't load vertex shader!\n"; 
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
   }
  
  // Load fragment program
  if (fragmentFile!=0)
  if (tFragmentShader->LoadFromFile(fragmentFile) != 0)
  {
     cout << "error: can't load fragment shader!\n";
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }
  
  // Compile vertex program
  if (vertexFile!=0)
  if (!tVertexShader->Compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->CompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      return 0;
  }
   
  // Compile fragment program 
  if (fragmentFile!=0)
  if (!tFragmentShader->Compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->CompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
     
  }
   
  // Add to object    
  if (vertexFile!=0) o->AddShader(tVertexShader);
  if (fragmentFile!=0) o->AddShader(tFragmentShader); 
  
  // Link 
  if (!o->Link())
  {
     cout << "**LinkER ERROR\n";
     cout << o->LinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }
  
  _shaderProgramList.push_back(o);
  o->ManageMemory();

   return o;
}

// ----------------------------------------------------------------------------

CShaderProgram* CShaderManager::LoadFromMemory(const char* vertexMem, const char* fragmentMem)
{
  CShaderProgram* o = new CShaderProgram();
  
  CVertexShader* tVertexShader = new CVertexShader;
  CFragmentShader* tFragmentShader = new CFragmentShader;

  // get vertex program
  if (vertexMem!=0)
     tVertexShader->LoadFromMemory(vertexMem);
  
  // get fragment program
  if (fragmentMem!=0)
     tFragmentShader->LoadFromMemory(fragmentMem);
 
  // Compile vertex program
  if (vertexMem!=0)
  if (!tVertexShader->Compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->CompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      return 0;
  }
   
  // Compile fragment program 
  if (fragmentMem!=0)
  if (!tFragmentShader->Compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->CompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
     
  }
   
  // Add to object    
  if (vertexMem!=0) o->AddShader(tVertexShader);
  if (fragmentMem!=0) o->AddShader(tFragmentShader); 
  
  // Link 
  if (!o->Link())
  {
     cout << "**LinkER ERROR\n";
     cout << o->LinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }

  _shaderProgramList.push_back(o);
  o->ManageMemory();

   return o;
}

// ----------------------------------------------------------------------------

 bool CShaderManager::Free(CShaderProgram* o)
 {
   vector<CShaderProgram*>::iterator  i=_shaderProgramList.begin();
   while (i!=_shaderProgramList.end()) 
   {
        if ((*i)==o)
        {
            _shaderProgramList.erase(i);
            delete o;
            return true;
        }
        i++;
   }   
   return false;
 }

// ----------------------------------------------------------------------------
    
