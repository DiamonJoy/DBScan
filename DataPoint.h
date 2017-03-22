 /*
 作者: XuBin&&Jason
 邮箱: 190662513@qq.com
 维护: 2014.3.8
 */
#include <vector>

using namespace std;

const int DIME_NUM = 4;        //数据维度，全局常量

//数据点类型
class DataPoint
{
private:
	unsigned long dpID;                //数据点ID
	double dimension[DIME_NUM];        //维度数据
	long clusterId;                    //所属聚类ID
	bool isKey;                        //是否核心对象
	bool visited;                    //是否已访问
	vector<unsigned long> arrivalPoints;    //领域数据点id列表
public:
	DataPoint();                                                    //默认构造函数
	DataPoint(unsigned long dpID,double* dimension , bool isKey);    //构造函数
	
	unsigned long GetDpId();                //GetDpId方法
	void SetDpId(unsigned long dpID);        //SetDpId方法
	double* GetDimension();                    //GetDimension方法
	void SetDimension(double* dimension);    //SetDimension方法
	bool IsKey();                            //GetIsKey方法
	void SetKey(bool isKey);                //SetKey方法
	bool isVisited();                        //GetIsVisited方法
	void SetVisited(bool visited);            //SetIsVisited方法
	long GetClusterId();                    //GetClusterId方法
	void SetClusterId(long classId);        //SetClusterId方法
	vector<unsigned long>& GetArrivalPoints();    //GetArrivalPoints方法
};

//默认构造函数
DataPoint::DataPoint()
{
}

//构造函数
DataPoint::DataPoint(unsigned long dpID,double* dimension , bool isKey):isKey(isKey),dpID(dpID)
{
	//传递每维的维度数据
	for(int i=0; i<DIME_NUM;i++)
	{
		this->dimension[i]=dimension[i];
	}
}

//设置维度数据
void DataPoint::SetDimension(double* dimension)
{
	for(int i=0; i<DIME_NUM;i++)
	{
		this->dimension[i]=dimension[i];
	}
}

//获取维度数据
double* DataPoint::GetDimension()
{
	return this->dimension;
}

//获取是否为核心对象
bool DataPoint::IsKey()
{
	return this->isKey;
}

//设置核心对象标志
void DataPoint::SetKey(bool isKey)
{
	this->isKey = isKey;
}

//获取DpId方法
unsigned long DataPoint::GetDpId()
{
	return this->dpID;
}

//设置DpId方法
void DataPoint::SetDpId(unsigned long dpID)
{
	this->dpID = dpID;
}

//GetIsVisited方法
bool DataPoint::isVisited()
{
	return this->visited;
}


//SetIsVisited方法
void DataPoint::SetVisited( bool visited )
{
	this->visited = visited;
}

//GetClusterId方法
long DataPoint::GetClusterId()
{
	return this->clusterId;
}

//GetClusterId方法
void DataPoint::SetClusterId( long clusterId )
{
	this->clusterId = clusterId;
}

//GetArrivalPoints方法
vector<unsigned long>& DataPoint::GetArrivalPoints()
{
	return arrivalPoints;
}

