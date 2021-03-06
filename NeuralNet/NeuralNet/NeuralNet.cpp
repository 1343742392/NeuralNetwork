// NeuralNet.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NeuralNet.h"
#include <fstream>


float expects[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

float expect = 0;

class Mnist
{
	int imgNum = 60000;
	int imgSize = 784;

	
	



public:

	char *allLab = new char[imgNum];
	char *allImg = new char[imgNum * imgSize];

	Mnist(const char* imgName = "img", const char* labName = "lab")
	{
		std::ifstream labIn(labName, std::ios::binary);
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

	void getImg(int index, char* buffer)
	{
		setArr(imgSize, buffer, &allImg[index * imgSize]);
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
	for (int bigF = 0; bigF < 10000; bigF++)
	{
		std::vector<std::vector<std::vector<float>>> netW;
		//mni.setExpect(expect, bigF);
		//printf("图片数字为%d", *mni.getLab(bigF));
		if (*mni.getLab(bigF) != 0 && *mni.getLab(bigF + 1) != 0)
		{
			continue;
		}
		if (*mni.getLab(bigF) == 0)
		{
			expect = 1;
		}
		else
		{
			expect = 0.5f;
		}
		char* buffer = new char[mni.getImgSize()];
		
		mni.getImg(bigF, buffer);
		for (int f = 0; f < 784; f++)
		{
			float value = buffer[f];
			if (value < 0) value = value * (-1) + 128;
			floatBuffer[f] = value / 255;
		}
		delete[] buffer;
		//创建网络  
		Net n1(floatBuffer);
		n1.addLay(Lay(16, mni.getImgSize()));
		n1.addLay(Lay(16, 16));
		n1.addLay(Lay(1, 16));
		netW = n1.getNetWs();


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
		
		
		std::ifstream wfs("w", std::ios::binary);
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
			n1.setNetWs(netW);

			free(wBuffer);
		}
		wfs.close();


		float* reslut = new float[1];
		
		n1.run(reslut);
		float oldCost = cost(&expect, reslut);

	
			printf(" %d张  \n", bigF);
			printfArr(1, reslut);
			printf("图片数字为%d", *mni.getLab(bigF));
			printf("       %f\n", oldCost);
			printf("\n");
		
		
		//如果结果不满意




		if (oldCost > 0.05f)
		{
			
			float* allDerivatives = new float[allWNum];
			char* allW = new char[allWNum * 20];
			memset(allW, 0, allWNum);
	
			do
			{
				//计算偏导数 调整权重  
				int index2 = 0;
				float* oldReslut = reslut;
				float nowReslut[10];
				for (int f2 = 0; f2 < netW.size(); f2++)//层数
				{
					for (int f1 = 0; f1 < netW[f2].size(); f1++)//神经元数
					{
						for (int f = 0; f < netW[f2][f1].size(); f++)//w数
						{
							float addValue = netW[f2][f1][f] * 0.5;
							float oldW = netW[f2][f1][f];
							//if(expect == 1)
							netW[f2][f1][f] = oldW + addValue;
							/*else
								netW[f2][f1][f] = netW[f2][f1][f] - addValue;*/
							n1.setNetWs(netW);
							n1.run(nowReslut);
							float newCost = cost(&expect, nowReslut);
							allDerivatives[index2] = (newCost - oldCost) / addValue;
							//printf("%f\n", allDerivatives[index2]);

							/*if (allDerivatives[index2] >= netW[f2][f1][f])
								netW[f2][f1][f] = 0;
							else*/
								netW[f2][f1][f] = oldW - allDerivatives[index2] * oldW;
							floatToChar(netW[f2][f1][f], &allW[index2 * 20]);

							index2++;
						}
					}
				}

				n1.setNetWs(netW);
				n1.run(reslut);
				printf("调整后结果为：\n");
				printfArr(1, reslut);
				printf("       %f\n", cost(&expect, reslut));
				printf("\n");


				std::ofstream o("w", std::ios::binary);
				if (!o.is_open())
				{
					printf("没打开w");
					return 0;
				}
				o.write(allW, allWNum * 20);
				o.close();
				break;



			
			} while (cost(&expect, reslut) > 0.1f);

			/*std::ofstream* o = new std::ofstream("w");
			if (!(*o).is_open())
			{
				printf("没打开w");
				return 0;
			}
			(*o).write(allW, allWNum * 20);
			(*o).close();*/
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