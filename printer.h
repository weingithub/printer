#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

class Item;

class Preferential
{
public:
	Preferential(int w) : weight(w) {}
	
    bool operator < (Preferential & p)
    {
        return (weight < p.weight);
    }

    virtual double pay(Item & it) = 0;
	
    virtual void print(double money, double savemoney)
    {
        cout<<"小计: "<<money<<"(元)"<<endl;
    }

	virtual int saveNumber(Item & it){return 0;}
private:
    int weight;
};


class Buy2Handsel1 : public Preferential
{
public:
	Buy2Handsel1(int we):Preferential(we){}

    double pay(Item & it);
	int saveNumber(Item & it);
};

class Discounts :  public Preferential
{
public:
    Discounts(double per, int wei):percent(per),Preferential(wei){}
    double pay(Item & it);
	
	virtual void print(double money, double savemoney);
private:
    double percent;
};

class Goods
{
public:
	Goods(char *pName, char *pUnit, double pr, char *pCode): price(pr)
	{
		memset(szName, 0, sizeof(szName));
		strcpy(szName, pName);
		
		memset(szUnit, 0, sizeof(szUnit));
		strcpy(szUnit, pUnit);
		
		memset(szCode, 0, sizeof(szCode));
		strcpy(szCode, pCode);
	}	
public:
    char szName[200];
    char szUnit[40];
    double price;
    char szKind[40];
    char szCode[20];
};

class Item
{
public:
    Item(vector<Preferential *> vec, Goods & gd, double num):prefers(vec),goods(gd), number(num)
	{
		savemoney = 0;
		savenum = 0;
	} 
	
	void calculate()
	{
		if (0 == prefers.size())  //没有优惠
		{
			sum = number*goods.price;
		}
		else if(1 == prefers.size())
		{
			Preferential * pPre = prefers[0];
			sum = pPre->pay(*this);
			savemoney = number*goods.price - sum;
			savenum = pPre->saveNumber(*this);
		}
		else
		{
			Preferential * pMax = prefers[0];
			
			for (int i = 1; i < prefers.size(); ++i)
			{
				if (*pMax < *prefers[i])
				{
					pMax = prefers[i];
				}
			}
			
			sum = pMax->pay(*this);
			savemoney = number*goods.price - sum;
			savenum = pMax->saveNumber(*this);
		}
	}
    
    void print()
    {
        cout<<"名称:"<<goods.szName<<"，数量:"<<number<<goods.szUnit<<"，单价："<<goods.price<<"，";
        
		if (0 == prefers.size())
		{
			cout<<"小计: "<<sum<<"(元)"<<endl;
		}
		else
		{
			Preferential * pPre = prefers[0];
			pPre->print(sum, savemoney);
		}
    }
    
	double getNum() {return number;}
    double getPrice() {return goods.price;}
	
	double getSum() {return sum;}
	double getSaveMoney() {return savemoney;}
	int getSaveNum() {return savenum;}
	
	void printSaveNum()
	{
		cout<<"名称:"<<goods.szName<<"，数量:"<<savenum<<goods.szUnit<<endl;
	}
	
private:
    double number;
	double sum;
	double savemoney;
	int savenum;
    vector<Preferential*>  prefers;
    Goods & goods;
};


#endif
  
