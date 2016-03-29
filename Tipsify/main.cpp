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

void loadData(objLoader * testLoader,char * objFolder)
{
	int i = 0; char buffer[10];
	char filePath[100];
	strcpy(filePath, objFolder);
	strcat(filePath, "frame");
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
	int characterId = 1; int aniId = 0;
	char Character[5][20] = { "Ganfaul_M_Aure", "Kachujin_G_Rosales", "Maw_J_Laygo", "Nightshade" };
	char Animation[6][40] = { "dancing_maraschino_step", "Standing_2H_Cast_Spell", "Standing_2H_Magic_Area_Attack", "Standing_Jump", "Standing_React_Death_Backward", "Standing_React_Large_From_Back" };
	
	char objFolder[150];
	strcpy(objFolder, "D:/TriangleOrdering/VF/Obj/");
	strcat(objFolder, Character[characterId]);
	strcat(objFolder, "/");
	strcat(objFolder, Animation[aniId]);
	strcat(objFolder, "/subdiv2/");

	objLoader *testLoader = new objLoader();
	loadData(testLoader,objFolder);

	printf("Number of Vertex: %i \n", testLoader->vertexCount);
	printf("Number of faces: %i\n", testLoader->faceCount); 
	
	int vertexCount = testLoader->vertexCount;
	int faceCount = testLoader->faceCount;
	int *piIndexBuffer = (int *)malloc(faceCount * 3 * sizeof(int));
	int *piIndexBufferOut = (int *)malloc(faceCount * 3 * sizeof(int));
	int *piClustersIn = (int *)malloc(faceCount * sizeof(int));
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

	FanVertOptimizeVCacheOnly(piIndexBuffer,piIndexBufferOut,vertexCount,faceCount,iCacheSize,piScratch,piClustersIn,&iNumClusters);
	printf("finish vertex cache algorithm\n");
	printf("num of clusters : %i \n", iNumClusters);
	
	printf("last face %u\n", piClustersIn[iNumClusters]);
	
	float lamda = 0.85;
	int iNumClustersOut = 0;
	FanVertOptimizeClusterOnly(piIndexBufferOut, vertexCount, faceCount, iCacheSize, lamda, piClustersIn, iNumClusters, piClustersOut, &iNumClustersOut, piScratch);
	printf("finish linear clustering\n");
	printf("num of clusters : %i \n", iNumClustersOut);



	getchar();
}