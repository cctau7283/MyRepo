#include <iostream>
#include<sstream>
#include <list>
using namespace std;


/* run this program using the console pauser or add your own getch, system("pause") or input loop */



struct Node{
	Node(float a, int b);
	float coefficient;
	int degree;
};

Node::Node(float a,int b)
{
	coefficient=a;
	degree=b;
}

class poly{
	private:
		list<Node> L;
//		int numofsect;
		
		
	public:	
		void input(string a);
		void print();
		void simplify();
//		poly();
//		~poly();
		poly operator+(poly &as);
		poly operator*(poly &as);
		poly operator-(poly &as);
		poly operator/(poly &as);
	};
	
	
	poly poly::operator+(poly &as)
	{
		
	}
	
	enum CASE{ET_COEFFICIENT,ET_X,
	ET_CARET,ET_DEGREE,ET_PM,FAIL,STORE
	};
	
	void poly::input(string a)
	{
		istringstream s;
		s>>a;//声明流类 
		
		CASE current_state=ET_PM;//声明各个变量
		int deg=0;
		double cof=0;
		bool neg=0,flag_no_cof=false,flag_need_sign=false,stop_flag=false; 
		
		while(!stop_flag)
		{
			switch(current_state)
			{
			case FAIL:
				stop_flag=true;
				break;
			case STORE:
				{
					if(neg==true)
						cof=-cof;
					L.push_back(Node(cof, deg));
					deg = 0;
					cof = 0;
					neg = false;
					flag_no_cof = false;
					flag_need_sign = true;
					current_state=ET_PM;
					break;
				}
			case ET_PM:
				{
					if(s.peek()=='+'||s.eof())
						{
							s.get();
							break;
						}
					else if(s.peek()=='-')
						{
							neg=true;
							s.get();
							break;
						}
					else if(s.eof())
						break;
					else if(s.peek()!='+'&&s.peek()!='-'
					&&flag_need_sign==true)
						current_state=FAIL;
						break;
						
				}
			
			case ET_COEFFICIENT:
				{
					double co;
					s>>co;
					if(!s.fail())
						co=cof;
					else
						flag_no_cof=true;
					current_state=ET_X;
					break;
				}
			
			case ET_X:
				{
					if(s.peek()=='x')
						{
							current_state=ET_CARET;
							s.get();
							break;
						}
					else if(s.peek()!='x'&&flag_no_cof==true)
						{
							current_state=FAIL;
							break;
						}
					else if(s.peek()!='x'&&flag_no_cof==false)
						deg=0;
						current_state=STORE;
						s.get();
						break;
				}
			case ET_CARET:
				{
					if(s.peek()=='^')
					{
						current_state=ET_DEGREE;
						s.get();
						break;
					}
					else if(s.peek()!='^'&&deg==1)
					{
						current_state=STORE;
						s.get();
						break;
					}
					else
						current_state=FAIL;
						break;
				}
			case ET_DEGREE:
				{
					int de;
					s>>de;
					if(!s.fail()&&de>0)
					{
						deg=de;
						current_state=STORE;
						break;
					}
					else
						current_state=FAIL;
						break;
				}
			
			}
			
		}
		
	}
	
	void printp(Node a)
	{
		if(a.coefficient>0)
			cout<<"+"<<a.coefficient<<"x^"<<a.degree;
		if(a.coefficient==0)
			return;
		if(a.coefficient<0)
			cout<<a.coefficient<<"x^"<<a.degree;
	}
	
	void poly::print()
	{
		list<Node>::iterator it=L.begin();
		for(;it!=L.end();++it)
		{
			Node &n=*it;
			
			if(it==L.begin())
			{
				cout<<n.coefficient<<"x^"<<n.degree;
			}
			else
				printp(n);
			
		}
		cout<<endl;
	}
	
	
	
	void poly::simplify()
	{
		list<Node>::iterator it=L.begin();
		list<Node>::iterator it2=++it;
		for(;it!=L.end();++it)
		{
			Node&n=*it;
			for(;it2!=L.end();++it2)
			{
				Node&m=*it2;
				if(n.degree==m.degree)
				{
					n.coefficient+=m.coefficient;
					
					L.erase(it2);
				}
				
			}
		}
	}
	
	
	
	
	
	int main(int argc, char** argv) {
	
	struct Node;
//	class poly;
	cout<<"Please type in the initial polynomial:";
	string y;
	cin>>y;
	poly poly1;
	poly1.input(y);
	poly1.print();//测试是否读取成功 
	
	return 0;
}
	
	
	
	
	
	
