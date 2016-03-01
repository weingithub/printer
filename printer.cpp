#include "printer.h"
#include <map>
#include <stdlib.h>

map<string, vector<Preferential*> > mapPrefer;
map<string, Goods> mapGoods;



double Buy2Handsel1::pay(Item & it)
{
    double num = it.getNum();   //get the number of goods
    double price = it.getPrice();   //get the price of goods

    int lessNum = (num)/3;  //buy 2 then handsel 1,so the Dividend is 3        
    return (num - lessNum)*price;
}
	
int Buy2Handsel1::saveNumber(Item & it)
{
	return (int)(it.getNum()/3);
}

double Discounts::pay(Item & it)
{
        int num = it.getNum();   //get the number of goods
        int price = it.getPrice();   //get the price of goods

        return (num*price*percent);
}
	
void Discounts::print(double money, double savemoney)
{
		cout<<"小计: "<<money<<"(元)"<<"，节省"<<savemoney<<"（元）"<<endl;
}

void parseJson(string json, vector<string> & vecresult);

void printTicket(string inputdata)
{
	vector<string> vec;
	parseJson(inputdata, vec);
	
	map<string, double> itemnum;
	map<string, double>::iterator iter;
	
	string code;
	double num = 0;
	
	for(int i = 0; i < vec.size(); ++i)
	{
		//获取条形码
		int pos = vec[i].find('-');
		if(string::npos != pos) //条形码-数量形式
		{
			code = vec[i].substr(0, pos);
			string strnum = vec[i].substr(pos+1);
			num = atof(strnum.c_str());
		}
		else
		{
			code = vec[i];
			num = 1;
		}
		
		if (itemnum.end() == (iter = itemnum.find(code)))  //条形码之前不存在
		{
			itemnum.insert(make_pair<string, int>(code, num));
		}
		else
		{
			iter->second = iter->second + num;
		}
	}
	
	//根据map，迭代创建商品对象
	vector<Item *> vecItem;
	//开始输出第一部分
	cout<<"***<没钱赚商店>购物清单"<<endl;
	
	for(iter = itemnum.begin(); iter != itemnum.end(); ++iter)
	{
		Item * pItem = new Item(mapPrefer.find(iter->first)->second, mapGoods.find(iter->first)->second, iter->second);
		pItem->calculate();
		pItem->print();
		vecItem.push_back(pItem);
	}
	
	cout<<"----------------------------"<<endl;
	//开始输出第二部分，是否有买二赠一商品
	//顺便统计总算以及节省的钱数
	bool isFirst = true;
	double paymoney = 0, savemoney = 0;
	for (vector<Item *>::iterator it = vecItem.begin(); it != vecItem.end(); ++it)
	{
		Item * pIt = *it;
		paymoney += pIt->getSum();
		savemoney += pIt->getSaveMoney();
		
		if (0 != pIt->getSaveNum())
		{
			if (isFirst)
			{
				cout<<"买二赠一商品："<<endl;
				isFirst = false;
			}
			
			pIt->printSaveNum();
		}
	}
	
	if (!isFirst)
	{
		cout<<"----------------------"<<endl;
	}
	
	cout<<"总计："<<paymoney<<"（元）"<<endl;
	
	if ( savemoney > -0.0000001 && savemoney < 0.000001)  //double与0比较，近似等于0，说明没有节省
	{
		;
	}	
	else
	{
		cout<<"节省："<<savemoney<<"（元）"<<endl;
	}
	
	cout<<"**************************************"<<endl;
	
	//释放对象
	for (vector<Item *>::iterator it = vecItem.begin(); it != vecItem.end(); ++it)
	{
		Item * pIt = *it;
		delete pIt;
	}
}

void parseJson(string json, vector<string> & vecresult)
{
	if ('[' == json[0])
	{
		int i = 1;
		bool start = false;
		string str;
		while(json[i] != ']')
		{
			if ('\'' == json[i] && false == start) //如何区分字符串的开始或者结束呢？加1个变量控制
			{
				//字符串开始
				start = true; //标识下一个是写入字符串中的字符
			}
			else if('\'' != json[i] && start)
			{
				str.push_back(json[i]);
			}
			else if('\'' == json[i] && start)  //字符串的结束
			{
				vecresult.push_back(str);
				str.clear();
				start = false;
			}
			++i;
		}
	}
}

int main(int argc, char ** argv)
{
    //设置优惠信息
	Preferential*  pSend = new Buy2Handsel1(40);
	Preferential*  pDis = new Discounts(0.95, 30);
		
	
	//设置条形码和优惠之间的关系
	vector<Preferential* > vecTemp;
	vecTemp.push_back(pSend);
	vecTemp.push_back(pDis);
	mapPrefer.insert(make_pair<string, vector<Preferential*> >("ITEM000001", vecTemp));
	
	vecTemp.clear();
	vecTemp.push_back(pDis);
	mapPrefer.insert(make_pair<string, vector<Preferential*> >("ITEM000002", vecTemp));
	
	vecTemp.clear();
	vecTemp.push_back(pSend);
	mapPrefer.insert(make_pair<string, vector<Preferential*> >("ITEM000003", vecTemp));
	
	vecTemp.clear();
	mapPrefer.insert(make_pair<string, vector<Preferential*> >("ITEM000004", vecTemp));
	
	//设置条形码与商品之间的关系
	
	//可口可乐
	Goods cole = Goods("可口可乐", "瓶", 3, "ITEM000001");
	//羽毛球
	Goods tenis = Goods("羽毛球", "个", 1, "ITEM000002");
	//苹果
	Goods apple = Goods("苹果", "斤", 5.5, "ITEM000003");
	//书籍
	Goods fight = Goods("奋斗", "本", 28, "ITEM000004");
	
	mapGoods.insert(make_pair<string, Goods>("ITEM000001", cole));
	mapGoods.insert(make_pair<string, Goods>("ITEM000002", tenis));
	mapGoods.insert(make_pair<string, Goods>("ITEM000003", apple));
	mapGoods.insert(make_pair<string, Goods>("ITEM000004", fight));
	
	//根据数据解析打印
	printTicket(argv[1]);
}
