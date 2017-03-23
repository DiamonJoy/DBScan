 /*
 作者: DiamonJoy
 维护: 2014.3.8
 */
#include <iostream>
#include <cmath>
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <vector>
#include "DataPoint.h"

using namespace std;

//聚类分析类型
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;        //数据集合
	unsigned int dimNum;			//维度
	double radius;                    //半径
	unsigned int dataNum;            //数据数量
	unsigned int minPTs;            //邻域最小数据个数
	
	double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //距离函数
	void SetArrivalPoints(DataPoint& dp);                                //设置数据点的领域点列表
	void KeyPointCluster( unsigned long i, unsigned long clusterId );    //对数据点领域内的点执行聚类操作
public:
	
	ClusterAnalysis(){}                    //默认构造函数
	bool Init(char* fileName, double radius, int minPTs);    //初始化操作
	bool DoDBSCANRecursive();            //DBSCAN递归算法
	bool WriteToFile(char* fileName);    //将聚类结果写入文件
};



/*
函数：聚类初始化操作
说明：将数据文件名，半径，领域最小数据个数信息写入聚类算法类，读取文件，把数据信息读入写进算法类数据集合中
参数：
char* fileName;    //文件名
double radius;    //半径
int minPTs;        //领域最小数据个数  
返回值： true;    */
bool ClusterAnalysis::Init(char* fileName, double radius, int minPTs)
{
	this->radius = radius;        //设置半径
	this->minPTs = minPTs;        //设置领域最小数据个数
	this->dimNum = DIME_NUM;    //设置数据维度
	ifstream ifs(fileName);        //打开文件
	if (! ifs.is_open())                //若文件已经被打开，报错误信息
	{
		cout << "Error opening file\n";    //输出错误信息
		exit (-1);                        //程序退出
	}
	
	unsigned long i=0;            //数据个数统计
	while (! ifs.eof() )                //从文件中读取POI信息，将POI信息写入POI列表中
	{
		DataPoint tempDP;                //临时数据点对象
		double tempDimData[DIME_NUM];    //临时数据点维度信息
		for(int j=0; j<DIME_NUM; j++)    //读文件，读取每一维数据
		{
			ifs>>tempDimData[j];
		}
		tempDP.SetDimension(tempDimData);    //将维度信息存入数据点对象内
		tempDP.SetDpId(i);                    //将数据点对象ID设置为i
		tempDP.SetVisited(false);            //数据点对象isVisited设置为false
		tempDP.SetClusterId(-1);            //设置默认簇ID为-1
		dadaSets.push_back(tempDP);            //将对象压入数据集合容器
		i++;        //计数+1
	}
	ifs.close();        //关闭文件流
	dataNum =i;            //设置数据对象集合大小为i
	for(unsigned long k=0; k<dataNum;k++)
	{
		SetArrivalPoints(dadaSets[k]);            //计算数据点领域内对象
	}
	return true;    //返回
}

/*
函数：将已经过聚类算法处理的数据集合写回文件
说明：将已经过聚类结果写回文件
参数：
char* fileName;    //要写入的文件名
返回值： true    */
bool ClusterAnalysis::WriteToFile(char* fileName )
{
	ofstream of1(fileName);//初始化文件输出流
	for(int d=1; d<=DIME_NUM ; d++)						//将维度信息写入文件
	{
		of1<<"数据"<<d<<'\t';
		if (d==DIME_NUM) of1<<"所属簇ID"<<'\t'<<endl;
	}
	for(unsigned long i=0; i<dataNum;i++)                //对处理过的每个数据点写入文件
	{
		for(int d=0; d<DIME_NUM ; d++)                    //将维度信息写入文件
			of1<<dadaSets[i].GetDimension()[d]<<'\t';
		if(dadaSets[i].GetClusterId() != -1) of1 << dadaSets[i].GetClusterId() <<endl;        //将所属簇ID写入文件
		else of1 <<"噪声点"<<endl;
	}
	of1.close();    //关闭输出文件流
	return true;    //返回
}

/*
函数：设置数据点的领域点列表
说明：设置数据点的领域点列表
参数：
返回值： true;    */
void ClusterAnalysis::SetArrivalPoints(DataPoint& dp)
{
	for(unsigned long i=0; i<dataNum; i++)                //对每个数据点执行
	{
		double distance =GetDistance(dadaSets[i], dp);//获取与特定点之间的距离
		//cout << dp.GetDpId()<<"to"<<i<<"is"<<distance<< endl;
		if(distance <= radius && i!=dp.GetDpId())        //若距离小于半径，并且特定点的id与dp的id不同执行
			dp.GetArrivalPoints().push_back(i);            //将特定点id压力dp的领域列表中
	}
	if(dp.GetArrivalPoints().size() >= minPTs)            //若dp领域内数据点数据量> minPTs执行
	{
		dp.SetKey(true);    //将dp核心对象标志位设为true
		return;                //返回
	}
	dp.SetKey(false);    //若非核心对象，则将dp核心对象标志位设为false
}


/*
函数：执行聚类操作
说明：执行聚类操作
参数：
返回值： true;    */
bool ClusterAnalysis::DoDBSCANRecursive()
{
	unsigned long clusterId=0, k = 0;                        //聚类id计数，初始化为0
	for(unsigned long i=0; i<dataNum;i++)            //对每一个数据点执行
	{
		DataPoint& dp=dadaSets[i];                    //取到第i个数据点对象
		if(!dp.isVisited() && dp.IsKey())            //若对象没被访问过，并且是核心对象执行
		{
			dp.SetClusterId(clusterId);                //设置该对象所属簇ID为clusterId
			dp.SetVisited(true);                    //设置该对象已被访问过
			KeyPointCluster(i,clusterId);            //对该对象领域内点进行聚类
			clusterId++;                            //clusterId自增1
		}
		/*if(!dp.IsKey()) {
			cout <<"噪声点"<<i<< endl;
			k++;
		}*/
	}
	for(unsigned long j=0; j<dataNum;j++)  
	{
		DataPoint& dptemp=dadaSets[j];
		if(dptemp.GetClusterId()!=-1) 
			cout << "数据点"<< dptemp.GetDpId()<< " 聚类ID为"<< dptemp.GetClusterId() << endl;
		else 
		{
			cout <<"噪声点"<<j<< endl;
			k++;
		}
	}

	cout <<"\n共聚类" <<clusterId<<"个"<< endl;       //算法完成后，输出聚类个数    
    cout <<"噪声点" <<k<<"个"<< endl; 
	return true;    //返回
}

/*
函数：对数据点领域内的点执行聚类操作
说明：采用递归的方法，深度优先聚类数据
参数：
unsigned long dpID;            //数据点id
unsigned long clusterId;    //数据点所属簇id
返回值： void;    */
void ClusterAnalysis::KeyPointCluster(unsigned long dpID, unsigned long clusterId )
{
	DataPoint& srcDp = dadaSets[dpID];        //获取数据点对象
	if(!srcDp.IsKey())    return;
	vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //获取对象领域内点ID列表
	for(unsigned long i=0; i<arrvalPoints.size(); i++)
	{
		DataPoint& desDp = dadaSets[arrvalPoints[i]];    //获取领域内点数据点
		if(!desDp.isVisited())                            //若该对象没有被访问过执行
		{
			//cout << "数据点"<< desDp.GetDpId()<<" 聚类ID为" <<clusterId << endl;
			desDp.SetClusterId(clusterId);        //设置该对象所属簇的ID为clusterId，即将该对象吸入簇中
			desDp.SetVisited(true);                //设置该对象已被访问
			if(desDp.IsKey())                    //若该对象是核心对象
			{
				KeyPointCluster(desDp.GetDpId(),clusterId);    //递归地对该领域点数据的领域内的点执行聚类操作，采用深度优先方法
			}
		}
	}
}

//两数据点之间距离
/*
函数：获取两数据点之间距离
说明：获取两数据点之间的欧式距离
参数：
DataPoint& dp1;        //数据点1
DataPoint& dp2;        //数据点2
返回值： double;    //两点之间的距离        */
double ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance =0;        //初始化距离为0
	for(int i=0; i<DIME_NUM;i++)    //对数据每一维数据执行
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i],2);    //距离+每一维差的平方
	}
	return pow(distance,0.5);        //开方并返回距离
}


