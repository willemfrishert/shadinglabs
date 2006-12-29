#pragma once

class TTextFileHandler
	{
	public:
		static char* TextFileRead(char *aFileName);
		static int   TextFileWrite(char *aFileName, char *aDataString);
	};

