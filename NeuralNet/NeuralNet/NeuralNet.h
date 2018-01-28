#pragma once
#ifndef NEURAL_NET
#define NEURAL_NET
#include"MyTools.h"
#include<vector>
/*
std::ifstream in("bitmap.bmp");
if (!in.is_open())
return 0;
int fileLength = getFileLength(in);
char* buffer = new char[fileLength];
in.read(buffer,fileLength);
float* floatBuffer = new float[fileLength];
for (int f = 0; f < fileLength; f++)
{
floatBuffer[f] = buffer[f] / 255;
}
Net* n1 = new Net(floatBuffer, fileLength);
n1->addLay(Lay(16, fileLength));
n1->addLay(Lay(16, 16));
n1->addLay(Lay(16, 16));
n1->addLay(Lay(1, 16));
printf("%f\n", *n1->run());
printf("%f", *n1->run());

反复运行结果不会变
*/

class Neure
{
	int inputNum;
	std::vector<float> w;
	float *input = 0;


public:
	Neure(int inputLen)
	{
		inputNum = inputLen;
		input = new float[inputLen];

		for (int f = 0; f < inputLen; f++)
		{
			w.push_back(rand() % 100 * 0.001 + 0.05 );
		}
		
	}

	void setInput(float *Arrinput)
	{
		setArr(inputNum, input, Arrinput);
		
	}


	std::vector<float> getW()
	{
		return w;
	}

	void setW(std::vector<float> &w)
	{
		this->w = w;
	}

	float *getInput()
	{
		return input;
	}

	float out()
	{
		float value = 0;
		for (int f = 0; f < inputNum; f++)
		{
			value = value + w[f] * input[f];
		}
		float f = sigmoid(value);

		return f;
	}

	int getInputNum()
	{
		return inputNum;
	}

	~Neure()
	{

	}
};

class Lay
{
private:
	std::vector<Neure> ys;
	std::vector<std::vector<float>> LWs;
public:

	//神经元数    输入个数
	Lay(int Ylength, int Ilength)
	{
		for (int f = 0; f < Ylength; f++)
		{
			Neure  y(Ilength);
			ys.push_back(y);
		}
	}


	std::vector<Neure> getYuan()
	{
		return ys;
	}

	void  out(float* lout)
	{
		for (int f = 0; f < ys.size(); f++)
		{
			lout[f] = ys[f].out();
			
		}
	}
	//输入数据   
	void setInput(float *data)
	{
		for (int f = 0; f < ys.size(); f++)
		{
			ys[f].setInput(data);
		}
	}

	int getYuanLength()
	{
		return ys.size();
	}

	std::vector<std::vector<float>> getLWs()
	{
		//遍历这一层所有神经元
		for (int f = 0; f < ys.size(); f++)
		{
			std::vector<float> ws;
			//遍历这个神经元的输入个数
			for (int f1 = 0; f1 < ys[f].getInputNum(); f1++)
			{
				ws.push_back(ys[f].getW()[f1]);
			}
			LWs.push_back(ws);
		}
		return LWs;
	}

	void  setWs(std::vector<std::vector<float>> &layW)
	{
		//遍历这一层所有神经元
		for (int f = 0; f < ys.size(); f++)
		{
			ys[f].setW(layW[f]);
		}
	}

};

class Net
{
	std::vector<Lay> ls;
	std::vector<float> ws;
	float *input = 0;
	std::vector<std::vector<std::vector<float>>> netW;
public:
	//整个网络的输入   输入多少个数据
	Net( float *input)
	{
		this->input = input;
	}

	void addLay(Lay lay)
	{
		ls.push_back(lay);
	
	}

	void  run(float &reslut)
	{

		float * lOut = new float[ls[0].getYuanLength()];
		ls[0].setInput(input);
		ls[0].out(lOut);
		for (int f = 1; f < ls.size(); f++)
		{
	
			ls[f].setInput(lOut);
			ls[f].out(lOut);
		}
		reslut = *lOut;
		delete[] lOut;
	}

	std::vector<std::vector<std::vector<float>>> getNetWs()
	{
		for (int f = 0; f < ls.size(); f++)
		{
			netW.push_back(ls[f].getLWs());
		}
		return netW;
	}

	 void setNetWs(std::vector<std::vector<std::vector<float>>> &netWs)
	{
		for (int f = 0; f < ls.size(); f++)
		{
			ls[f].setWs(netWs[f]);
		}
	}
};
#endif 