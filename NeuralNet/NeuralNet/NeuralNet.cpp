// NeuralNet.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NeuralNet.h"
#include <fstream>

float expect = 0;

std::vector<std::vector<std::vector<float>>> netW;



int main()
{

	//读取图片
	for (int bigF = 0; bigF < 20; bigF++)
	{
		char bitmapIndex[2];
		intToChar(bigF, bitmapIndex);
		char name[] = "bitmap";
		std::ifstream in(strcat(name, bitmapIndex));
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
		int allWNum = 27120;
		std::ifstream wfs("w");
		if (wfs.is_open())
		{
			char* wBuffer = new char[getFileLength(wfs)];
			wfs.read(wBuffer, getFileLength(wfs));
			wfs.close();

			for (int f2 = 0; f2 < netW.size(); f2++)
			{
				for (int f1 = 0; f1 < netW[f2].size(); f1++)
				{
					for (int f = 0; f < netW[f2][f1].size(); f++)
					{
						charToFloat(&wBuffer[allWNum * 20], netW[f2][f1][f]);
						printf("%f", netW[f2][f1][f]);
						allWNum++;
					}
				}
			}
			n1->setNetWs(netW);

		}

		float reslut;
		n1->run(reslut);
		printf(" 保存的权重结果为：%f\n", reslut);

		//如果结果不满意
		if (cost(expect, reslut) < 0.2f) return 0;

		std::ofstream out("w");
		float* allDerivatives = new float[allWNum];
		char* allW = new char[allWNum * 20];

		do
		{
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
						if (expect == 1)
							netW[f2][f1][f] = allDerivatives[index2];
						else
							netW[f2][f1][f] = -allDerivatives[index2];

						floatToChar(netW[f2][f1][f], &allW[index2 * 20]);
					}
				}
			}

			n1->setNetWs(netW);
			n1->run(reslut);
			printf("调整后结果为： %f\n", reslut);


		} while (cost(expect, reslut) > 0.2f);

		out.write(allW, allWNum * 20);
		out.close();
	}
	

	return 0;
}



/*

int main()
{
	//读取图片
	



	float f = -0.43045220f;
	char  c [] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	float d = 0;
	floatToChar(f, c);
	charToFloat(c, d);
	printf("%f", d);



	std::ofstream out("test");
	if (!out.is_open())
		return 0;
	out.write(c, 20);
	out.close();

	std::ifstream in("test");
	in.read(c, 20);
	in.close();
	
	printf("%f", f);
	return 0;
}*/