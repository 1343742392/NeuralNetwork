#pragma once
#ifndef MY_TOOLS
#define MY_TOOLS	
#include<fstream>
#include<stdio.h>
#include<iostream>
#include<math.h>
long int getFileLength(std::ifstream &f) {
	long int length;
	f.seekg(0, std::ios::end);    // go to the end  
	length = f.tellg();
	f.seekg(0, std::ios::beg);    // go back to the beginning  
	return length;
}



void setArr(int inputLen, float *value, float *setValue, int star = 0)
{
	for (int f = 0; f < inputLen; f++)
	{
		value[f] = setValue[f];
	}
}

float sigmoid(float input, float a = 1)
{
	return 1 / (1 + exp(-a * input));
}

void printfArr(int length, float *arr, int start = 0)
{
	for (int f = 0; f < length; f++)
		printf("%f\n", arr[start + f]);
}

void printfArr(int length, char *arr, int start = 0)
{
	for (int f = 0; f < length; f++)
		printf("%d  %d\n", f, arr[start + f]);
}




float cost(float expect, float reslut)
{

	return pow((reslut - expect), 2);
}

void floatToChar(float &input, char* reslut)
{
	sprintf_s(reslut, 20, "%g", input);
}

void charToFloat(char* input, float &reslut)
{
	sscanf_s(input, "%f", &reslut);
}

void intToChar(int &input, char *out)
{
	sprintf_s(out, 2, "%d", input);
}

#endif