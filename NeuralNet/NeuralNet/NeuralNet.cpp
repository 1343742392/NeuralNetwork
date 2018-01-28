// NeuralNet.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "NeuralNet.h"
#include <fstream>


float expect = 0;



class Mnist
{
	int imgNum = 10000;
	int imgSize = 784;

	char *allLab = new char[imgNum];
	char *allImg = new char[imgNum * imgSize];
	



public:

	Mnist(const char* imgName = "img", const char* labName = "lab")
	{
		std::ifstream labIn(labName);
		if (!labIn.is_open())
			return;
		labIn.read(allLab, imgNum);
		labIn.close();

		std::ifstream imgIn(imgName);
		if (!imgIn.is_open())
			return;
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
};

int main()
{

	//读取图片
	Mnist mni;

	for (int bigF = 0; bigF < 100; bigF++)
	{
		std::vector<std::vector<std::vector<float>>> netW;
		if (*mni.getLab(bigF) == 2)
			expect = 0;
		else
			expect = 1;

		char* buffer;
		float* floatBuffer = new float[mni.getImgSize()];
		buffer = mni.getImg(bigF);
		for (int f = 0; f < 784; f++)
		{
			float value = buffer[f];
			if (value < 0) value = value * (-1) + 128;
			floatBuffer[f] = value / 255;
		}
		printf("input");
		printfArr(10, floatBuffer, 200);           /////////////////////////
		//创建网络  错误60万个w
		Net* n1 = new Net(floatBuffer);
		n1->addLay(Lay(16, mni.getImgSize()));
		n1->addLay(Lay(16, 16));
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
						index1++;
					}
				}
			}
			n1->setNetWs(netW);

			free(wBuffer);
		}
		wfs.close();


		float reslut;
		n1->run(reslut);
		printf(" %d保存的权重结果为：%f\n", bigF, reslut);
	
		//如果结果不满意
		if (cost(expect, reslut) > 0.1f)
		{
			
			float* allDerivatives = new float[allWNum];
			char* allW = new char[allWNum * 20];
			memset(allW, 0, allWNum);
	
			do
			{
				//计算偏导数 调整权重  
				int index2 = 0;
				float oldReslut = reslut;
				for (int f2 = 0; f2 < netW.size(); f2++)
				{
					for (int f1 = 0; f1 < netW[f2].size(); f1++)
					{
						for (int f = 0; f < netW[f2][f1].size(); f++)
						{
						
							if(expect == 1)
								netW[f2][f1][f] = netW[f2][f1][f] + netW[f2][f1][f] * 0.5;
							else
								netW[f2][f1][f] = netW[f2][f1][f] - netW[f2][f1][f] * 0.5;
							n1->setNetWs(netW);
							float nowReslut;
							n1->run(nowReslut);
							allDerivatives[index2] = (nowReslut - reslut) / 0.5f;


							if (allDerivatives[index2] >= netW[f2][f1][f]) netW[f2][f1][f] = 0;
							else netW[f2][f1][f] = netW[f2][f1][f] - allDerivatives[index2];
							floatToChar(netW[f2][f1][f], &allW[index2 * 20]);
							index2++;
						}
					}
				}

				n1->setNetWs(netW);
				n1->run(reslut);
				printf("调整后结果为： %f\n", reslut);


				std::ofstream* o = new std::ofstream("w");
				if (!(*o).is_open())
				{
					printf("没打开w");
					return 0;
				}
				(*o).write(allW, allWNum * 20);
				(*o).close();
				break;

				if (expect)
				{
					//调向1如果变小就停止调整
					if ((oldReslut - reslut > 0.005) |( oldReslut == reslut)) break;
				}
				else
				{
					//调向0如果变大就停止调整
					if ((oldReslut - reslut < 0.005) | (oldReslut == reslut)) break;
				}
			} while (cost(expect, reslut) > 0.1f);

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
		delete[] floatBuffer;

	}
	

	return 0;
}



/*
int main()
{
	//读取图片
	printf("%f\n", sigmoid(8.024844648015f));
	return 0;
}*/