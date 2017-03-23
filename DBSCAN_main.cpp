 /*
 作者: DiamonJoy
 维护: 2014.3.8
 */

#include "ClusterAnalysis.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
	char datafile[50], resultfile[50];
	double radius;
	int minPTs;
	cout<<"输入半径radius:"; cin>>radius;
	cout<<"输入领域内最小数据点个数minPTs:"; cin>>minPTs;
	cout<<"请将数据样本文件转换成.txt文件\n输入样本文件地址:\n例如C:\\Users\\Desktop\\data.txt"<< endl;
	cin>>datafile;
	ClusterAnalysis myClusterAnalysis;                        //聚类算法对象声明
	myClusterAnalysis.Init(datafile,radius,minPTs);		  //指定半径和领域内最小数据点个数
	myClusterAnalysis.DoDBSCANRecursive();                    //执行聚类算法
	cout<<"\n输入结果文件储存地址\n例如C:\\Users\\Desktop\\redata.txt）,再自行将.txt文件转化成样本文件:"<< endl;
	cin>>resultfile;
	myClusterAnalysis.WriteToFile(resultfile); //写执行后的结果写入文件
	cout<<"写入成功"<< endl;
	
	system("pause");    //显示结果
	return 0;           //返回
}
