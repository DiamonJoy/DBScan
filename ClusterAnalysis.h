 /*
 ����: XuBin&&Jason
 ����: 190662513@qq.com
 ά��: 2014.3.8
 */
#include <iostream>
#include <cmath>
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <vector>
#include "DataPoint.h"

using namespace std;

//�����������
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;        //���ݼ���
	unsigned int dimNum;			//ά��
	double radius;                    //�뾶
	unsigned int dataNum;            //��������
	unsigned int minPTs;            //������С���ݸ���
	
	double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //���뺯��
	void SetArrivalPoints(DataPoint& dp);                                //�������ݵ��������б�
	void KeyPointCluster( unsigned long i, unsigned long clusterId );    //�����ݵ������ڵĵ�ִ�о������
public:
	
	ClusterAnalysis(){}                    //Ĭ�Ϲ��캯��
	bool Init(char* fileName, double radius, int minPTs);    //��ʼ������
	bool DoDBSCANRecursive();            //DBSCAN�ݹ��㷨
	bool WriteToFile(char* fileName);    //��������д���ļ�
};



/*
�����������ʼ������
˵�����������ļ������뾶��������С���ݸ�����Ϣд������㷨�࣬��ȡ�ļ�����������Ϣ����д���㷨�����ݼ�����
������
char* fileName;    //�ļ���
double radius;    //�뾶
int minPTs;        //������С���ݸ���  
����ֵ�� true;    */
bool ClusterAnalysis::Init(char* fileName, double radius, int minPTs)
{
	this->radius = radius;        //���ð뾶
	this->minPTs = minPTs;        //����������С���ݸ���
	this->dimNum = DIME_NUM;    //��������ά��
	ifstream ifs(fileName);        //���ļ�
	if (! ifs.is_open())                //���ļ��Ѿ����򿪣���������Ϣ
	{
		cout << "Error opening file\n";    //���������Ϣ
		exit (-1);                        //�����˳�
	}
	
	unsigned long i=0;            //���ݸ���ͳ��
	while (! ifs.eof() )                //���ļ��ж�ȡPOI��Ϣ����POI��Ϣд��POI�б���
	{
		DataPoint tempDP;                //��ʱ���ݵ����
		double tempDimData[DIME_NUM];    //��ʱ���ݵ�ά����Ϣ
		for(int j=0; j<DIME_NUM; j++)    //���ļ�����ȡÿһά����
		{
			ifs>>tempDimData[j];
		}
		tempDP.SetDimension(tempDimData);    //��ά����Ϣ�������ݵ������
		tempDP.SetDpId(i);                    //�����ݵ����ID����Ϊi
		tempDP.SetVisited(false);            //���ݵ����isVisited����Ϊfalse
		tempDP.SetClusterId(-1);            //����Ĭ�ϴ�IDΪ-1
		dadaSets.push_back(tempDP);            //������ѹ�����ݼ�������
		i++;        //����+1
	}
	ifs.close();        //�ر��ļ���
	dataNum =i;            //�������ݶ��󼯺ϴ�СΪi
	for(unsigned long k=0; k<dataNum;k++)
	{
		SetArrivalPoints(dadaSets[k]);            //�������ݵ������ڶ���
	}
	return true;    //����
}

/*
���������Ѿ��������㷨��������ݼ���д���ļ�
˵�������Ѿ���������д���ļ�
������
char* fileName;    //Ҫд����ļ���
����ֵ�� true    */
bool ClusterAnalysis::WriteToFile(char* fileName )
{
	ofstream of1(fileName);//��ʼ���ļ������
	for(int d=1; d<=DIME_NUM ; d++)						//��ά����Ϣд���ļ�
	{
		of1<<"����"<<d<<'\t';
		if (d==DIME_NUM) of1<<"������ID"<<'\t'<<endl;
	}
	for(unsigned long i=0; i<dataNum;i++)                //�Դ������ÿ�����ݵ�д���ļ�
	{
		for(int d=0; d<DIME_NUM ; d++)                    //��ά����Ϣд���ļ�
			of1<<dadaSets[i].GetDimension()[d]<<'\t';
		if(dadaSets[i].GetClusterId() != -1) of1 << dadaSets[i].GetClusterId() <<endl;        //��������IDд���ļ�
		else of1 <<"������"<<endl;
	}
	of1.close();    //�ر�����ļ���
	return true;    //����
}

/*
�������������ݵ��������б�
˵�����������ݵ��������б�
������
����ֵ�� true;    */
void ClusterAnalysis::SetArrivalPoints(DataPoint& dp)
{
	for(unsigned long i=0; i<dataNum; i++)                //��ÿ�����ݵ�ִ��
	{
		double distance =GetDistance(dadaSets[i], dp);//��ȡ���ض���֮��ľ���
		//cout << dp.GetDpId()<<"to"<<i<<"is"<<distance<< endl;
		if(distance <= radius && i!=dp.GetDpId())        //������С�ڰ뾶�������ض����id��dp��id��ִͬ��
			dp.GetArrivalPoints().push_back(i);            //���ض���idѹ��dp�������б���
	}
	if(dp.GetArrivalPoints().size() >= minPTs)            //��dp���������ݵ�������> minPTsִ��
	{
		dp.SetKey(true);    //��dp���Ķ����־λ��Ϊtrue
		return;                //����
	}
	dp.SetKey(false);    //���Ǻ��Ķ�����dp���Ķ����־λ��Ϊfalse
}


/*
������ִ�о������
˵����ִ�о������
������
����ֵ�� true;    */
bool ClusterAnalysis::DoDBSCANRecursive()
{
	unsigned long clusterId=0, k = 0;                        //����id��������ʼ��Ϊ0
	for(unsigned long i=0; i<dataNum;i++)            //��ÿһ�����ݵ�ִ��
	{
		DataPoint& dp=dadaSets[i];                    //ȡ����i�����ݵ����
		if(!dp.isVisited() && dp.IsKey())            //������û�����ʹ��������Ǻ��Ķ���ִ��
		{
			dp.SetClusterId(clusterId);                //���øö���������IDΪclusterId
			dp.SetVisited(true);                    //���øö����ѱ����ʹ�
			KeyPointCluster(i,clusterId);            //�Ըö��������ڵ���о���
			clusterId++;                            //clusterId����1
		}
		/*if(!dp.IsKey()) {
			cout <<"������"<<i<< endl;
			k++;
		}*/
	}
	for(unsigned long j=0; j<dataNum;j++)  
	{
		DataPoint& dptemp=dadaSets[j];
		if(dptemp.GetClusterId()!=-1) 
			cout << "���ݵ�"<< dptemp.GetDpId()<< " ����IDΪ"<< dptemp.GetClusterId() << endl;
		else 
		{
			cout <<"������"<<j<< endl;
			k++;
		}
	}

	cout <<"\n������" <<clusterId<<"��"<< endl;       //�㷨��ɺ�����������    
    cout <<"������" <<k<<"��"<< endl; 
	return true;    //����
}

/*
�����������ݵ������ڵĵ�ִ�о������
˵�������õݹ�ķ�����������Ⱦ�������
������
unsigned long dpID;            //���ݵ�id
unsigned long clusterId;    //���ݵ�������id
����ֵ�� void;    */
void ClusterAnalysis::KeyPointCluster(unsigned long dpID, unsigned long clusterId )
{
	DataPoint& srcDp = dadaSets[dpID];        //��ȡ���ݵ����
	if(!srcDp.IsKey())    return;
	vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //��ȡ���������ڵ�ID�б�
	for(unsigned long i=0; i<arrvalPoints.size(); i++)
	{
		DataPoint& desDp = dadaSets[arrvalPoints[i]];    //��ȡ�����ڵ����ݵ�
		if(!desDp.isVisited())                            //���ö���û�б����ʹ�ִ��
		{
			//cout << "���ݵ�"<< desDp.GetDpId()<<" ����IDΪ" <<clusterId << endl;
			desDp.SetClusterId(clusterId);        //���øö��������ص�IDΪclusterId�������ö����������
			desDp.SetVisited(true);                //���øö����ѱ�����
			if(desDp.IsKey())                    //���ö����Ǻ��Ķ���
			{
				KeyPointCluster(desDp.GetDpId(),clusterId);    //�ݹ�ضԸ���������ݵ������ڵĵ�ִ�о������������������ȷ���
			}
		}
	}
}

//�����ݵ�֮�����
/*
��������ȡ�����ݵ�֮�����
˵������ȡ�����ݵ�֮���ŷʽ����
������
DataPoint& dp1;        //���ݵ�1
DataPoint& dp2;        //���ݵ�2
����ֵ�� double;    //����֮��ľ���        */
double ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance =0;        //��ʼ������Ϊ0
	for(int i=0; i<DIME_NUM;i++)    //������ÿһά����ִ��
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i],2);    //����+ÿһά���ƽ��
	}
	return pow(distance,0.5);        //���������ؾ���
}


