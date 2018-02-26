#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
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
  void input(string a);
  void print();
  void simplify();

  poly operator+(poly &as);
  poly operator*(poly &as);
  poly operator-(poly &as);
  poly operator/(poly &as);
};

poly poly::operator+(poly &as) {
  list<Node>::iterator it1 = L.begin();
  list<Node>::iterator it2 = as.L.begin();

  poly poly3;
  for (; it2 != as.L.end();) {

    Node &n1 = *it1;
    Node &n2 = *it2;
    if (n2.degree < n1.degree) {
      poly3.L.push_back(Node(n1.coefficient, n1.degree));
      ++it1;
    }

    else if (n1.degree < n2.degree) {
      poly3.L.push_back(Node(n2.coefficient, n2.degree));
      ++it2;
    }

    else if (n1.degree == n2.degree) {
      poly3.L.push_back(Node((n2.coefficient + n1.coefficient), n1.degree));
      ++it1;
      ++it2;
    }
  }

  for (; it1 != L.end();) {
    Node &n1 = *it1;
    Node &n2 = *it2;
    if (n1.degree == n2.degree) {
      poly3.L.push_back(Node((n2.coefficient + n1.coefficient), n1.degree));
    } else if (n1.degree != n2.degree) {
      poly3.L.push_back(Node(n1.coefficient, n1.degree));
      ++it1;
    }
  }

  Node &n1 = *it1;
  Node &n2 = *it2;
  if (n1.degree == n2.degree) {
    poly3.L.push_back(Node((n2.coefficient + n1.coefficient), n1.degree));
  } else if (n1.degree != n2.degree) {
    poly3.L.push_back(Node(n1.coefficient, n1.degree));
    poly3.L.push_back(Node(n2.coefficient, n2.degree));
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
    for (; it2 != as.L.end(); ++it2) {
      Node &n2 = *it2;
      result.L.push_back(
          Node(n1.coefficient * n2.coefficient, n1.degree + n2.degree));
    }
  }

  result.simplify();
  return result;
}

poly poly::operator-(poly &as) {
  list<Node>::iterator it1 = L.begin();
  list<Node>::iterator it2 = as.L.begin();

  poly poly3;
  for (; it2 != as.L.end();) {

    Node &n1 = *it1;
    Node &n2 = *it2;
    if (n2.degree < n1.degree) {
      poly3.L.push_back(Node(n1.coefficient, n1.degree));
      ++it1;
    }

    else if (n1.degree < n2.degree) {
      poly3.L.push_back(Node(n2.coefficient, n2.degree));
      ++it2;
    }

    else if (n1.degree == n2.degree) {
      poly3.L.push_back(Node((n2.coefficient - n1.coefficient), n1.degree));
      ++it1;
      ++it2;
    }
  }

  return poly3;
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
      if (neg == true)
        cof = -cof;
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

void printp(Node n) {
  if (n.coefficient > 0) {
    if (n.coefficient == 1 && n.degree != 1) {
      cout << "+x^" << n.degree;
    } else if (n.coefficient == 1 && n.degree == 1) {
      cout << "+x";
    } else if (n.coefficient != 1 && n.degree == 1) {
      cout << "+" << n.coefficient << "x";
    } else if (n.coefficient != 1 && n.degree != 1)
      cout << "+" << n.coefficient << "x^" << n.degree;
    else if (n.degree == 0) {
      cout << "+" << n.coefficient;
    }
  }

  if (n.coefficient == 0)
    return;
  if (n.coefficient < 0) {
    if (n.degree > 1) {
      cout << n.coefficient << "x^" << n.degree;
    } else if (n.coefficient != -1 && n.degree == 1) {
      cout << n.coefficient << "x";
    } else if (n.coefficient == -1 && n.degree == 1) {
      cout << "-x";
    } else if (n.degree == 0) {
      cout << n.coefficient;
    }
  };
}

void poly::print() {
  list<Node>::iterator it = L.begin();
  for (; it != L.end(); ++it) {
    Node &n = *it;

    if (it == L.begin()) {
      if (n.coefficient == 1 && n.degree != 1) {
        cout << "x^" << n.degree;
      } else if (n.coefficient == 1 && n.degree == 1) {
        cout << "x";
      } else if (n.coefficient != 1 && n.degree == 1) {
        cout << n.coefficient << "x";
      } else if (n.coefficient != 1 && n.degree != 1)
        cout << n.coefficient << "x^" << n.degree;
      else if (n.degree == 0) {
        cout << n.coefficient;
      }

    } else
      printp(n);
  }
  cout << endl;
}

void poly::simplify() {
  struct {
    bool operator()(Node a, Node b) { return a.degree > b.degree; }
  } sortdeg;

  L.sort(sortdeg);

  for (list<Node>::iterator it = L.begin(); it != L.end(); ++it) {
    Node &n = *it;
    Node &m = *std::next(it, 1);
    if (m.degree == n.degree) {
      n.coefficient += m.coefficient;
      it = L.erase(it);
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
  poly1.print();
  cout << "Please type in another polynomial:";
  string x;
  cin >> x;
  poly poly2, poly3;
  poly2.input(x);
  poly2.print();
  poly3 = poly2 + poly1;
  poly3.print();

  return 0;
}
