// ANTreal.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//TSP问题，旅行商问题：假设一个旅行商人要拜访n个城市，
//他必须选择所有要走的路径，路径的限制室每个城市拜访一次
//而且最后返回出发的城市。选择的路径具有最小值

//蚁群算法是一种基于种群，通过模拟蚁群采集食物的过程的
//启发式搜索算法。蚂蚁通过释放信息素来记录陆军，并趋向于
//走信息素量比较多的路径来寻找最佳路径

//1.信息素的总量：蚂蚁拥有的信息素量是一定的，在一定距离后
//信息素用尽
//2.观测范围：蚂蚁具有一定的观测范围，选择信息素较多的路径
//3.出错概率：蚂蚁有一定的出错概率，会不忘信息素最多的方向走
//4.消减速度：信息素会以一定的速度消减，一些很少选择的路径上
//的信息素会消减，减少影响
//5.记忆能力：蚂蚁记得走过的路，不会选择走过的路


//信息素的更新公式
//Phe [ i ][ j ] = Dis* Phe[i][j]  +  DPhe[i][j]
//Phe 表示 每条边上的信息素浓度，Dis为信息素消逝的速率，
//DPhe为本次循环内的边ij的信息素增加量。
//DPhe的值为  每个蚂蚁在本次循环留在路径ij上的信息素量KDPhe的总和，
//对于KDPhe有三种模型，Ant-Circle System ， 
//Ant-Quantity System 和 Ant-Density System.
//Ant-Circle System    ： KDPhe = Q / LK   或0（当蚂蚁未经过此路径时。Q为一个常量，LK为这个循环中，此蚂蚁的总路程）
//Ant-Quantity System  ： KDPhe = Q / D[i][j] 或0
//Ant-Density System  .： KDPhe = Q   或 0

using namespace std;
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include<fstream>
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
int out;//输出时使用的变量
double Max;//用来选取最多信息素的边
bool Passed[NMax];//用来判断城市是否已经经过，是否可以选取
int main()
{
	//读入数据
	ifstream readFile("yiqunshuru.txt");
	readFile >> n;//读入城市数目
	if (n > NMax){//超过界限，结束
		printf("数据规模超出界限，程序结束\n");
		return 0;
	}
	//读入每条边的距离
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			readFile >> D[i][j];
		}
	}	
	//读入起点
	readFile >> start;
	if (start > n - 1)
	{
		printf("输入非法起点，程序结束\n");
		return 0;
	}
	readFile.close();
	//对称轴上距离置为0
	for (i = 0; i < n; i++){
		D[i][i] = 0;
	}
	//初始化每条边上的信息素浓度
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++)
		{
			Phe[i][j] = 1;
		}
	}
	
	//蚂蚁数量
	m = 999;
	//设置每只蚂蚁的出发点
	for (i = 0; i < m; i++)
	{
		Path[i][0] = start;
	}
	//开始循环，总数1000
	for (k = 0; k < K; k++)
	{
		//每次循环开始，进行上一次信息素的消逝
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				Phe[i][j] *= Dis;
			}
		}

		srand((unsigned)time(NULL));

		for (i = 0; i < m; i++)
		{//对于每只蚂蚁，进行一次循环	
			ant = start;
			//设置当前蚂蚁的起点
			//设置未访问和起点
			for (j = 0; j < n; j++)
			{
				Passed[j] = false;
			}
			Passed[ant] = true;

			for (j = 1; j < n; j++)
			{//每只蚂蚁选n-1次边
				Max = 0;//信息素最多值
				sameNum = 0;//同样信息素浓度数目
				bugNum = 0;//出错下的选择
				//寻找周围边上信息素的最大值记录在max里
				for (p = 0; p < n; p++){
					if (!Passed[p])
						Max = Max > Phe[ant][p] ? Max : Phe[ant][p];
				}
				//扫描所有可能选择的边，
				//最大值的边和其他边分别记录下来
				for (p = 0; p < n; p++){
					if (Max == Phe[ant][p]){
						if (!Passed[p])
							samePhe[sameNum++] = p;
					}else{
						if (!Passed[p])
							bugTry[bugNum++] = p;
					}
					
				}
				//下一步的选择，取决于随机比例和出错概率的比较值
				if ((double)rand() / 32765 < bugP  || !bugNum){
					//最大值边中选择
					ant = samePhe[rand() % sameNum];
				}	
				else{
					//出错状况
					ant = bugTry[rand() % bugNum];
				}
				Passed[ant] = true;//路径经过
				Path[i][j] = ant;//保存路径
			}
		}

		//完成对所有蚂蚁的操作后，
		//进行增加信息素的操作，使用Ant-Circle System 

		for (i = 0; i < m; i++)//一次循环，表示的是第i只蚂蚁的n-1次选择
		{
			LK = 0;
			for (j = 0; j < n - 1; j++)//n-1次选择，分别是第j次选择的城市
			{
				LK += D[Path[i][j]][Path[i][j + 1]];//Path记录的是城市
			}
			LK += D[Path[i][j]][Path[i][0]];//从最后一步回到初始点
			for (j = 0; j < n - 1; j++)
			{
				Phe[Path[i][j]][Path[i][j + 1]] += Q / LK;//所有走过的路径phe值都加了						
			}
			Phe[Path[i][j]][Path[i][0]] += Q / LK;//初始点
		}
	}
	//蚁群算法已经结束，下面输出路径
	p = 0xfffff;

	for (i = 0; i < m; i++)
	{
		LK = 0;
		for (j = 0; j < n - 1; j++){
			LK += D[Path[i][j]][Path[i][j + 1]];//计算一次循环中蚂蚁的总路程
		}	
		LK += D[Path[i][j]][Path[i][0]];//回到初始点
		//更新最小值
		if (LK < p)
		{
			p = LK;
			out = i;
		}
	}
	printf("1000次蚁群算法运行之后的最短路径输出：\n");
	//输出最短路径，通过最后的路径输出
	for (i = 0; i < n; i++){
		cout << Path[out][i] << "--->";
	}
	//输出返回初始点
	cout << Path[out][0] << endl;

	//输出所有信息素的值
	/*for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%lf   ", Phe[i][j]);
		}
		printf("\n");
	}*/

	//通过最后的信息素输出
	printf("最短路径输出(根据信息素矩阵计算输出)：\n");
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


