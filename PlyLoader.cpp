#include "stdafx.h"
#include "PLYLoader.h"

CPLYLoader::CPLYLoader()
{
	this->m_totalConnectedQuads = 0;
	this->m_totalConnectedPoints = 0;
	m_ModelData.iTotalConnectedTriangles = 0;
}

int CPLYLoader::LoadModel(char* filename)
{
	//Loading hint
	printf("Loading %s...\n", filename);
	char* pch = strstr(filename, ".ply");

	//if file isn's null, go on reading
	if (pch != NULL)
	{
		FILE* file = fopen(filename, "r");
		if (!file)
		{
			printf("load PLY file %s failed\n", filename);
			return false;
		}
		//comfirm the size of the file
		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		try
		{
			mp_vertexXYZ = (float*)malloc(ftell(file));
			mp_vertexNorm = (float*)malloc(ftell(file));
			mp_vertexRGB = (float*)malloc(ftell(file));
		}
		catch (char*)
		{
			return -1;
		}
		if (mp_vertexXYZ == NULL) return -1;
		if (mp_vertexNorm == NULL) return -2;
		if (mp_vertexRGB == NULL) return -3;

		//go to the begining of the file
		fseek(file, 0, SEEK_SET);

		if (file)
		{
			int i = 0;
			int temp = 0;
			int quads_index = 0;
			int triangle_index = 0;
			int normal_index = 0;
			int colorIndex = 0;
			char buffer[1000];

			//read a line once
			fgets(buffer, 300, file);


			// READ HEADER
			// -----------------

			// Find number of vertexes
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				//printf("%s\n", buffer);

				fgets(buffer, 300, file);
			}
			strcpy(buffer, buffer + strlen("element vertex"));

			//the second parament is like regular expression.
			//%i can automately translate the octonary number into the decimal number.
			sscanf(buffer, "%i", &this->m_totalConnectedPoints);
			fclose(file);
			printf("%s Loaded!\n", filename);

		}

		else
		{
			printf("File can't be opened\n");
		}
	}
	else
	{
		printf("File does not have a .PLY extension. ");
	}

	return 0;
}

int CPLYLoader::AllPointNums()
{
	return m_totalConnectedPoints;

}


