// NeuralNet.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NeuralNet.h"
#include <fstream>

float expect = 1;

std::vector<std::vector<std::vector<float>>> netW;



int main()
{
	//读取权重
	std::ifstream wfs("w");
	char* wBuffer = new char[getFileLength(wfs)];
	if (wfs.is_open())
	{
		
		wfs.read(wBuffer, getFileLength(wfs));
		wfs.close();

	}
	//读取图片
	std::ifstream in("bitmap.bmp");
	if (!in.is_open())
		return 0;
	int fileLength = getFileLength(in);
	char* buffer = new char[fileLength];
	in.read(buffer, fileLength);
	float* floatBuffer = new float[fileLength];
	for (int f = 0; f < fileLength; f++)
	{
		floatBuffer[f] = buffer[f] / 255;
	}

	//创建网络
	Net* n1 = new Net(floatBuffer, fileLength);
	n1->addLay(Lay(16, fileLength));
	n1->addLay(Lay(16, 16));
	n1->addLay(Lay(16, 16));
	n1->addLay(Lay(1, 16));
	
	netW = n1->getNetWs();
	



	//设置保存的权重
	int allWNum = 0;
	for (int f2 = 0; f2 < netW.size(); f2++)
	{
		for (int f1 = 0; f1 < netW[f2].size(); f1++)
		{
			for (int f = 0; f < netW[f2][f1].size(); f++)
			{
				netW[f2][f1][f ] = (float)wBuffer[allWNum];
				netW[f2][f1][f] = netW[f2][f1][f] / 255;
				allWNum++;
			}
		}
	}
	n1->setNetWs(netW);
	float reslut;
	n1->run(reslut);
	printf(" 保存的权重结果为：%f\n", reslut);
	
	std::ofstream out("w");
	float* allDerivatives = new float[allWNum];
	int a = allWNum * 8;
	char* allW = new char[allWNum * 8];
	//如果结果不满意
	while (cost(expect, reslut) > 0.2f)
	{
		//打开权重保存文件


		//计算偏导数 调整权重  
		int index2 = 0;
		for (int f2 = 0; f2 < netW.size(); f2++)
		{
			for (int f1 = 0; f1 < netW[f2].size(); f1++)
			{
				for (int f = 0; f < netW[f2][f1].size(); f++)
				{
					index2++;
					netW[f2][f1][f] = netW[f2][f1][f] + 0.5f;
					n1->setNetWs(netW);
					float nowReslut;
					n1->run(nowReslut);
					allDerivatives[index2] = (nowReslut - reslut) / 0.5f;

					if(expect == 1)
						netW[f2][f1][f] = allDerivatives[index2];
					else
						netW[f2][f1][f] = -allDerivatives[index2];
					printf("%f\n", netW[f2][f1][f]);
					floatToChar(netW[f2][f1][f], &allW[index2 * 8]);
				}
			}
		}

		n1->setNetWs(netW);
		n1->run(reslut);
		printf("调整后结果为： %f\n", reslut);

		

		
	}
	out.write(allW, allWNum * 8);
	out.close();

	return 0;
}




/*
int main()
{
	//读取图片
	



	float f = 1.2345648646f;
	char  c [] ={1,2,3,4,5,6,7,8,9};

	floatToChar(f, c);
	



	std::ofstream out("test");
	if (!out.is_open())
		return 0;
	out.write(c, 8);
	out.close();

	std::ifstream in("test");
	in.read(c, 8);
	in.close();
	charToFloat(c, f);
	printf("%f", f);
	return 0;
}*/