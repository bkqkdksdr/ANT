// ANTreal.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


using namespace std;
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include<fstream>
//aaa,太难了
//不知道说啥
//使用10个蚂蚁，进行10个城市的TSP问题求解。

const int MMax = 9999;//蚂蚁数量,蚂蚁数量根据城市数量决定。

const int NMax = 500;//城市数量最大数量，超过出错

int m, n;//蚂蚁数量与城市数量

const double Q = 999;//常量

const int K = 1000;//循环总次数

int D[NMax][NMax];//路径长度

double Phe[NMax][NMax];//边对应的信息素浓度

int LK;//蚂蚁此循环中的总路径长度

int Path[MMax][NMax];//记录蚂蚁的路径，用于防止走重复的路。记录的是点

int ant;//蚂蚁当前所在点

int i, j, k, p;//循环使用

double Dis = 0.1;//每次信息素 消失的速率

int sameNum, samePhe[NMax];//每次去寻找信息素最多的边，如初始情况，信息素量都相同时，要

						   //随机的去从中选取边。

int bugNum, bugTry[NMax];//出错情况下进行的选择

double bugP = 0.90;//每一次操作的出错概率

				   //后来发现，出错概率要结合蚂蚁数与城市数进行判断，而定值Q为估计距离

int start;//出发点，城市编号从0 - n-1.

double Max;//用来选取最多信息素的边

bool Passed[NMax];//用来判断城市是否已经经过，是否可以选取

int main()

{

	//载入数据



	ifstream readFile("yiqunshuru.txt");


	readFile >> n;
	if (n > NMax)

		return 0;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			readFile >> D[i][j];
		}
	}



	readFile.close();




	for (i = 0; i < n; i++)

		D[i][i] = 0;

	start = 2;

	if (start > n - 1)
	{
		return 0;
	}




	for (i = 0; i < n; i++)

		for (j = 0; j < n; j++)
		{
			Phe[i][j] = 1;//初始化每条边上的信息素浓度
		}


	m = 999;
	for (i = 0; i< m; i++)
	{
		Path[i][0] = start;//每只蚂蚁的出发点都固定
	}




	for (k = 0; k < K; k++)
	{

		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				Phe[i][j] *= Dis;
			}
		}//每次循环后，进行信息素的消逝

		srand((unsigned)time(NULL));

		for (i = 0; i < m; i++)
		{//对于每只蚂蚁，进行一次循环	

			ant = start;

			for (j = 0; j < n; j++)
			{
				Passed[j] = false;
			}

			Passed[ant] = true;

			for (j = 1; j < n; j++)
			{//每只蚂蚁选n-1次边

				Max = 0;

				sameNum = 0;

				bugNum = 0;

				for (p = 0; p < n; p++)

					if (!Passed[p])

						Max = Max > Phe[ant][p] ? Max : Phe[ant][p];//寻找周围边上信息素的最大值记录在max里

				for (p = 0; p < n; p++)

					if (Max == Phe[ant][p])

						if (!Passed[p])

							samePhe[sameNum++] = p;//!!将所有max的点都扫一遍，看看有几个重复的最大值点

				for (p = 0; p < n; p++)

					if (!Passed[p])

						bugTry[bugNum++] = p;

				if ((double)rand() / 32765 < bugP)

					ant = samePhe[rand() % sameNum];//用随机数，从中选中一条边

				else

					ant = bugTry[rand() % bugNum];//出错情况下，随机选一条边

				Passed[ant] = true;//路径经过

				Path[i][j] = ant;//保存路径//！！！！第i个蚂蚁的第j次选择的到达点是ant（当前走过的点位）

			}

		}

		//完成对每一个蚂蚁的操作后，进行增加信息素的操作，使用Ant-Circle System 

		for (i = 0; i < m; i++)
		{

			LK = 0;

			for (j = 0; j < n - 1; j++)
			{
				LK += D[Path[i][j]][Path[i][j + 1]];//计算一次循环中蚂蚁的总路程
			}

			LK += D[Path[i][j]][Path[i][0]];//回到初始点

			for (j = 0; j < n - 1; j++)
			{
				Phe[Path[i][j]][Path[i][j + 1]] += Q / LK;//所有走过的路径phe值都加了
														  // printf("danq: %d.%d\n",Path[i][j],Path[i][j+1]);
														  // if(Path[i][j]==Path[i][j+1])
														  // {
														  // 	scanf("%d",&LK);
														  // }
			}
			//printf("\n");

			Phe[Path[i][j]][Path[i][0]] += Q / LK;//初始点

		}

	}

	p = 0xfffff;//虽然已经完成操作，但我们要直观的从所有现有路径中找出最短的路径。

	for (i = 0; i < m; i++)
	{

		LK = 0;

		for (j = 0; j < n - 1; j++)

			LK += D[Path[i][j]][Path[i][j + 1]];//计算一次循环中蚂蚁的总路程

		LK += D[Path[i][j]][Path[i][0]];//回到初始点

		if (LK < p)
		{

			p = LK;

			start = i;

		}

	}

	for (i = 0; i < n; i++)

		cout << Path[start][i] << "->";

	cout << Path[start][0] << endl;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%lf   ", Phe[i][j]);
		}
		printf("\n");
	}


	int kaishi = start;
	int biaoji;
	double maxx;
	printf("%d", start);
	for (i = 0; i<n; i++)
	{
		maxx = -1.0;
		biaoji = -1;
		for (j = 0; j<n; j++)
		{

			if (Phe[kaishi][j]>maxx)
			{
				maxx = Phe[kaishi][j];
				biaoji = j;
			}
		}
		printf("--->%d", biaoji);
		kaishi = biaoji;


	}



	return 0;

}


