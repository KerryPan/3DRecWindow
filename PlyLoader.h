
#ifndef PLYREADER_H_
#define PLYREADER_H_

#include "stdlib.h"


#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
using namespace std;

struct SModelData
{
	vector <float> vecFaceTriangles; // = face * 9
	vector <float> vecFaceTriangleColors; // = face * 9
	vector <float> vecNormals; // = face * 9
	int iTotalConnectedTriangles;
};

class CPLYLoader
{
public:
	CPLYLoader();
	int LoadModel(char *filename);
	
	int AllPointNums();

private:
	float* mp_vertexXYZ;
	float* mp_vertexNorm;
	float* mp_vertexRGB;
	int m_totalConnectedQuads;
	int m_totalConnectedPoints;
	int m_totalFaces;
	SModelData m_ModelData;

	vector <int> vecFaceIndex;   //¶¥µãË÷Òý
};

#endif