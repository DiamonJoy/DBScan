 /*
 ����: XuBin&&Jason
 ����: 190662513@qq.com
 ά��: 2014.3.8
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
	cout<<"����뾶radius:"; cin>>radius;
	cout<<"������������С���ݵ����minPTs:"; cin>>minPTs;
	cout<<"�뽫���������ļ�ת����.txt�ļ�\n���������ļ���ַ:\n����C:\\Users\\XuBin&&Jason\\Desktop\\data.txt"<< endl;
	cin>>datafile;
	ClusterAnalysis myClusterAnalysis;                        //�����㷨��������
	myClusterAnalysis.Init(datafile,radius,minPTs);			  //ָ���뾶����������С���ݵ����
	myClusterAnalysis.DoDBSCANRecursive();                    //ִ�о����㷨
	cout<<"\n�������ļ������ַ\n����C:\\Users\\XuBin&&Jason\\Desktop\\redata.txt��,�����н�.txt�ļ�ת���������ļ�:"<< endl;
	cin>>resultfile;
	myClusterAnalysis.WriteToFile(resultfile);//дִ�к�Ľ��д���ļ�
	cout<<"д��ɹ�"<< endl;
	
	system("pause");    //��ʾ���
	return 0;            //����
}