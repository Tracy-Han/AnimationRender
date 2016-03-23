#include <stdio.h>
#include <string>

#include "Tipsify.h"
#include "objLoader\ObjLoader.h"

void printVector(obj_vector *v)
{
	printf("%.2f,", v->v[0]);
	printf("%.2f,", v->v[1]);
	printf("%.2f  \n", v->v[2]);
}

void loadData(objLoader * testLoader)
{
	int i = 0; char buffer[10];
	char filePath[100];
	strcpy(filePath, "subdiv1/frame");
	itoa(i + 1, buffer, 10);
	strcat(filePath, buffer);

	strcat(filePath, ".obj");
	printf(filePath); printf("\n");
	testLoader->load(filePath);
}
void initialBuffer(int * piIndexBuffer, float *pfVertexPosition,
				  objLoader *testLoader, int faceCount, int vertexCount)
{
	int count = 0;
	for (int i = 0; i < vertexCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pfVertexPosition[count] = testLoader->vertexList[i]->v[j];
			count++;
		}
	}
	
	count = 0;
	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			piIndexBuffer[count] = testLoader->faceList[i]->vertex_index[j];
			count++;
		}
	}
}
void main()
{
	objLoader *testLoader = new objLoader();
	loadData(testLoader);

	printf("Number of Vertex: %i \n", testLoader->vertexCount);
	printf("Number of faces: %i\n", testLoader->faceCount); 
	
	int vertexCount = testLoader->vertexCount;
	int faceCount = testLoader->faceCount;
	int *piIndexBuffer = (int *)malloc(faceCount * 3 * sizeof(int));
	int *piIndexBufferOut = (int *)malloc(faceCount * 3 * sizeof(int));
	int *piClustersOut = (int *)malloc(faceCount * sizeof(int));
	float *pfVertexPosition = (float *)malloc(vertexCount * 3 * sizeof(int));
	int iCacheSize = 20; int iNumClusters; int *piScratch = NULL; int *piRemap = NULL;

	initialBuffer(piIndexBuffer,pfVertexPosition,testLoader, faceCount, vertexCount);
	printf("vertex 1 position x: %.2f ,y: %.2f ,z: %.2f  \n", 
		pfVertexPosition[0], pfVertexPosition[1], pfVertexPosition[2]);
	printf("vertex %i position x: %.2f ,y: %.2f ,z: %.2f \n",
		vertexCount, pfVertexPosition[vertexCount * 3 - 3], pfVertexPosition[vertexCount * 3 - 2], pfVertexPosition[vertexCount * 3 - 1]);

	printf("face 1 index x: %u ,y: %u ,z: %u  \n",
		piIndexBuffer[0], piIndexBuffer[1], piIndexBuffer[2]);
	printf("face %i index x: %u ,y: %u ,z: %u  \n", 
		faceCount, piIndexBuffer[faceCount * 3 - 3], piIndexBuffer[faceCount * 3 - 2], piIndexBuffer[faceCount * 3 - 1]);

	FanVertOptimizeVCacheOnly(piIndexBuffer,piIndexBufferOut,vertexCount,faceCount,iCacheSize,piScratch,piClustersOut,&iNumClusters);
	printf("finish vertex cache algorithm");

	// what is piClustersOut
	// get the entire optimization
	// get the reordered cache-optimized order
	// how to read the ACMR
		// ACMR is only related to the cache and the triangle order
		// In paper, if the ACMR drops the thread hold (!)
	// how to get he OVR

	getchar();
}