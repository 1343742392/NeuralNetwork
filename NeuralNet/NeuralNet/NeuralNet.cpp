// NeuralNet.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NeuralNet.h"
#include <fstream>


float expect[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };



class Mnist
{
	int imgNum = 1000;
	int imgSize = 784;

	char *allLab = new char[imgNum];
	char *allImg = new char[imgNum * imgSize];
	



public:

	Mnist(const char* imgName = "img", const char* labName = "lab")
	{
		std::ifstream labIn(labName);
		if (!labIn.is_open())
			return;
		labIn.seekg(8, std::ios::beg);
		labIn.read(allLab, imgNum);
		labIn.close();




		std::ifstream imgIn(imgName, std::ios::binary);
		if (!imgIn.is_open())
			return;
		imgIn.seekg(16, std::ios::beg);
		imgIn.read(allImg, imgNum * imgSize);
		imgIn.close();
	}

	~Mnist()
	{
		free(allLab);
		free(allImg);
	}

	int getImgSize()
	{
		return imgSize;
	}

	char* getImg(int index)
	{
		return &allImg[index * imgSize];
	}

	char* getLab(int index)
	{
		return &allLab[index];
	}

	void setExpect(float *expect, int index)
	{
		for (int f = 0; f < 10; f++)
		{
			if (*getLab(index) == f)
			{
				expect[f] = 1;
				break;
			}
			else
			{
				expect[f] = 0;
			}
		}	
	}
};

int main()
{

	//读取图片
	Mnist mni;
	float* floatBuffer = new float[mni.getImgSize()];
	for (int bigF = 0; bigF < 1000; bigF++)
	{
		std::vector<std::vector<std::vector<float>>> netW;
		mni.setExpect(expect, bigF);

		char* buffer;
		
		buffer = mni.getImg(bigF);
		for (int f = 0; f < 784; f++)
		{
			float value = buffer[f];
			if (value < 0) value = value * (-1) + 128;
			floatBuffer[f] = value / 255;
		}
	
		//创建网络  
		Net* n1 = new Net(floatBuffer);
		n1->addLay(Lay(16, mni.getImgSize()));

		n1->addLay(Lay(1, 16));
		netW = n1->getNetWs();


		int allWNum = 0;
		for (int f2 = 0; f2 < netW.size(); f2++)
		{
			for (int f1 = 0; f1 < netW[f2].size(); f1++)
			{
				for (int f = 0; f < netW[f2][f1].size(); f++)
				{

					allWNum++;
				}
			}
		}

		//设置保存的权重
		
		
		std::ifstream wfs("w");
		if (wfs.is_open())
		{
			char* wBuffer = new char[getFileLength(wfs)];
			wfs.read(wBuffer, getFileLength(wfs));
			

			int index1 = 0;
			for (int f2 = 0; f2 < netW.size(); f2++)
			{
				for (int f1 = 0; f1 < netW[f2].size(); f1++)
				{
					for (int f = 0; f < netW[f2][f1].size(); f++)
					{
						charToFloat(&wBuffer[index1 * 20], netW[f2][f1][f]);
						//printf("%f\n", netW[f2][f1][f]);
						index1++;
					}
				}
			}
			n1->setNetWs(netW);

			free(wBuffer);
		}
		wfs.close();


		float* reslut = new float[10];
		
		n1->run(reslut);
		float oldCost = cost(expect, reslut);
		printf(" %d张  \n", bigF);
		printfArr(10, reslut);
		printf("       %f\n", oldCost);
		printf("\n");
		//如果结果不满意
		
		if (oldCost > 2)
		{
			
			float* allDerivatives = new float[allWNum];
			char* allW = new char[allWNum * 20];
			memset(allW, 0, allWNum);
	
			do
			{
				//计算偏导数 调整权重  
				int index2 = 0;
				float* oldReslut = reslut;
				for (int f2 = 0; f2 < netW.size(); f2++)
				{
					for (int f1 = 0; f1 < netW[f2].size(); f1++)
					{
						for (int f = 0; f < netW[f2][f1].size(); f++)
						{
							float addValue = netW[f2][f1][f] * 0.5;
							float nowReslut[10];
							float oldW = netW[f2][f1][f];
							//if(expect == 1)
							netW[f2][f1][f] = oldW + addValue;
							/*else
								netW[f2][f1][f] = netW[f2][f1][f] - addValue;*/
							n1->setNetWs(netW);
							n1->run(nowReslut);
							float newCost = cost(expect, nowReslut);
							allDerivatives[index2] = (newCost - oldCost) / addValue;


							/*if (allDerivatives[index2] >= netW[f2][f1][f])
								netW[f2][f1][f] = 0;
							else*/
								netW[f2][f1][f] = oldW - allDerivatives[index2] * oldW;
							floatToChar(netW[f2][f1][f], &allW[index2 * 20]);

							index2++;
						}
					}
				}

				n1->setNetWs(netW);
				n1->run(reslut);
				printf("调整后结果为：\n");
				printfArr(10, reslut);
				printf("       %f\n", cost(expect, reslut));
				printf("\n");


				/*std::ofstream* o = new std::ofstream("w");
				if (!(*o).is_open())
				{
					printf("没打开w");
					return 0;
				}
				(*o).write(allW, allWNum * 20);
				(*o).close();
				break;*/



			
			} while (cost(expect, reslut) > 2);

			std::ofstream* o = new std::ofstream("w");
			if (!(*o).is_open())
			{
				printf("没打开w");
				return 0;
			}
			(*o).write(allW, allWNum * 20);
			(*o).close();
			delete[] allDerivatives;
			delete[] allW;
		

		}
	}
	
	delete[] floatBuffer;
	return 0;
}



/*
int main()
{
	//读取图片
	printf("%f\n", sigmoid(8.024844648015f));
	return 0;
}*/