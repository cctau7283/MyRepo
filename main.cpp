#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <cmath>
using namespace std;

/* run this program using the console pauser or add your own getch,
 * system("pause") or input loop */

struct Node {
  Node(float a, int b);
  float coefficient;
  int degree;
};

Node::Node(float a, int b) {
  coefficient = a;
  degree = b;
}

class poly {
 private:
  list<Node> L;

 public:
  poly();
  poly(poly&);
  void input(string a);
  void print();
  void simplify();
  void enlarge();
  int countsector();

  poly operator+(poly &as);
  poly operator*(poly &as);
  poly operator-(poly &as);
  poly operator/(poly &as);
};

poly::poly()
{
	;
}

poly::poly(poly &a)
{
	list<Node>::iterator it1 = L.begin();
	list<Node>::iterator it2 = a.L.begin();
	
	for(;it2!=a.L.end();++it2)
	{
		Node&n2=*it2;
		L.push_back(Node(n2.coefficient,n2.degree));
	}
}

poly poly::operator+(poly &as) {
  list<Node>::iterator it1 = L.begin();
  list<Node>::iterator it2 = as.L.begin();
  
  poly poly3;
  for (; it2 != as.L.end();++it2) 
  {
    Node &n2 = *it2;
    poly3.L.push_back(Node(n2.coefficient,n2.degree));
  }
  
    for(;it1!=L.end();++it1)
    {
    	Node &n1 = *it1;
    	poly3.L.push_back(Node(n1.coefficient,n1.degree));
	}
	
	poly3.simplify();
  	return poly3;
}

poly poly::operator*(poly &as) {
  list<Node>::iterator it1 = L.begin();
  list<Node>::iterator it2 = as.L.begin();
  poly result;
  for (; it1 != L.end(); ++it1) {
    Node &n1 = *it1;
    for (it2=as.L.begin(); it2 != as.L.end(); ++it2) {
      Node &n2 = *it2;
      result.L.push_back(Node(n1.coefficient*n2.coefficient, n1.degree + n2.degree));
      result.print();
    }
  }
  result.simplify();
  return result;
}

poly poly::operator-(poly &as) {
 list<Node>::iterator it1 = L.begin();
  list<Node>::iterator it2 = as.L.begin();
  
  poly poly3;
  for (; it2 != as.L.end();++it2) 
  {
    Node &n2 = *it2;
    poly3.L.push_back(Node(n2.coefficient,n2.degree));
  }
  
    for(;it1!=L.end();++it1)
    {
    	Node &n1 = *it1;
    	poly3.L.push_back(Node(n1.coefficient,n1.degree));
	}
	
	poly3.simplify();
  	return poly3;
}

poly poly::operator/(poly &as)
{
	this->enlarge();
	poly middle;
	poly remain((*this));
	cout<<"*";
	remain.print();
	poly polynew,blank;
	
	list<Node>::iterator it1 = L.begin();
    list<Node>::iterator it2 = as.L.begin();
    list<Node>::iterator it3 = remain.L.begin();
    
    if ((*it1).degree<(*it2).degree)
    {
    	cout<<"Wrong input!"<<endl;
    	return blank;
	}
	
	for(it1=L.begin();it2!=as.L.begin();++it2)
	{
		Node&n1=*it1;
		Node&n2=*it2;
		Node&n3=(remain.L.front());
		if(n2.degree>=n1.degree)
		{
			polynew.L.push_back(Node(n3.coefficient/n1.coefficient,n3.degree-n1.degree));
			for(it1=L.begin();it1!=L.end();++it1)
			{
				float cof=polynew.L.back().coefficient;
				int deg=polynew.L.back().degree;
				polynew.print();
				middle.L.push_back(Node(n1.coefficient*cof,n1.degree+deg));
				//middle.print();
			}
			remain=remain-middle;
			middle.L.clear();
			//remain.print();
		}
		else
			break;
	}
	
	bool success=true;
	for(;it3!=remain.L.end();++it3)
	{
		Node&n=*it3;
		if(n.coefficient||n.degree)
			success=false;
	}
	
	if(success==true)
	{
		return polynew;
	}
	else
	{
		cout<<"This division cannot be realised."<<endl;
		return polynew;
	}
}


enum CASE { ET_COEFFICIENT, ET_X, ET_CARET, ET_DEGREE, ET_PM, FAIL, STORE };

void poly::input(string a) {
  istringstream s(a);

  CASE current_state = ET_PM;
  int deg = 0;
  double cof = 0;
  bool neg = 0, flag_no_cof = false, flag_need_sign = false, stop_flag = false;

  while (!stop_flag) {
    switch (current_state) {
      case FAIL:
        stop_flag = true;
        break;
      case STORE: {
        if (neg == true) cof = -cof;
        L.push_back(Node(cof, deg));
        deg = 0;
        cof = 0;
        neg = false;
        flag_no_cof = false;
        flag_need_sign = true;
        current_state = ET_PM;
        break;
      }
      case ET_PM: {
        if (s.eof()) {
          stop_flag = true;
          break;
        }

        char ch = s.peek();
        if (flag_need_sign) {
          if (ch == '+' || ch == '-') {
            neg = ch == '-';
            s.get();
            current_state = ET_COEFFICIENT;
          } else {
            current_state = FAIL;
          }
        } else
          current_state = ET_COEFFICIENT;
        break;
      }

      case ET_COEFFICIENT: {
        double co;
        s >> co;
        if (!s.fail())
          cof = co;
        else {
          flag_no_cof = true;
          cof = 1.0;
        }
        s.clear();
        current_state = ET_X;
        break;
      }

      case ET_X: {
        if (s.peek() == 'x') {
          current_state = ET_CARET;
          s.get();
          break;
        } else if (s.peek() != 'x' && flag_no_cof == true) {
          current_state = FAIL;
          break;
        } else if (s.peek() != 'x' && flag_no_cof == false)
          deg = 0;
        current_state = STORE;
        break;
      }
      case ET_CARET: {
        if (s.peek() == '^') {
          current_state = ET_DEGREE;
          s.get();
          break;
        } else if (s.peek() != '^') {
          deg = 1;
          current_state = STORE;
          break;
        } else
          current_state = FAIL;
        break;
      }
      case ET_DEGREE: {
        int de;
        s >> de;
        if (!s.fail() && de > 0) {
          deg = de;
          current_state = STORE;
        } else
          current_state = FAIL;
        s.clear();
        break;
      }
    }
  }

  simplify();
}


void poly::print() {
  list<Node>::iterator it = L.begin();
  bool iffirst=true;
  bool ifone1=false,ifone2=false;
  bool ifpositive=true;
  
  for (; it != L.end(); ++it,iffirst=false) 
  {
  	Node&n=*(it);
  	ifpositive=(n.coefficient>0?true:false);
  	ifone1=(n.coefficient==1);
  	
	if(!iffirst&&ifpositive)
		cout<<"+";
	
	if(!ifone1)
		cout<<n.coefficient;
	else if(n.degree==0)
		cout<<"1"; 
		
	if(n.degree==0)
		continue;
		
	cout<<"x";
	
	ifone2=(n.degree==1);
	if(!ifone2)
	{
		cout<<"^"<<n.degree;
	}
	
  }
  cout << endl;
}

void poly::simplify() {
  struct {
    bool operator()(Node a, Node b) { return a.degree > b.degree; }
  } sortdeg;

  L.sort(sortdeg);

  for (list<Node>::iterator it = L.begin(); it != L.end();++it ) {
    Node &n = *it;
    auto nextIt = std::next(it, 1);
    if (nextIt == L.end()) break;
    
    Node &m = *nextIt;
    if (m.degree == n.degree) {
      m.coefficient += n.coefficient;
      it=L.erase(it);
    }
    
  }
  
   L.remove_if([](Node& n) {return n.coefficient == 0;});
}

int poly::countsector()
{
	list<Node>::iterator it = L.begin();
	int a=0;
	for(;it!=L.end();++it)
	{
		++a;
	}
	return a;
}

void poly::enlarge()//用于增加系数为0的项。奇怪，print应该能输出这些项。。。 
{
	list<Node>::iterator it1 = L.begin();
	
	for(;(*it1).degree>0;++it1)
	{
		auto it2 = std::next(it1, 1);
		Node&n1=*it1;
		Node&n2=*it2;
		if(n2.degree-n1.degree>1)
		{
			L.insert(it2,Node(0,n1.degree-1));
		}
	}
}

int main(int argc, char **argv) {
  struct Node;

  cout << "Please type in the initial polynomial:";
  string y;
  cin >> y;
  poly poly1;
  poly1.input(y);
//  poly1.print();
  cout << "Please type in another polynomial:";
  string x;
  cin>> x;
  poly poly2,poly3,poly4,poly5;
  poly2.input(x);
//  poly2.print();
  poly3 = poly2+poly1;
//  cout<<"poly3=poly1+poly2=";
//  poly3.print();
//  cout<<endl;
  poly4=poly1*poly2;
//  cout<<"poly4=poly1*poly2=";
//  poly4.print();
//  poly5=poly1/poly2;
//  cout<<"poly5=poly1/poly2=";
//  poly5.print();
	poly1.enlarge();
	poly poly6(poly1);
	poly6.print();//用于测试enlarge函数 

  return 0;
}
