#include <iostream>

using namespace std;

class Airport {

};

class Airplane {
public:
  virtual void fly(const Airport& destination) = 0;
};

void Airplane::fly(const Airport& destination) {
  cout << "default Airplane::fly called" << endl;
}

class ModelA: public Airplane {
public:
  virtual void fly(const Airport& destination) {
    Airplane::fly(destination);
  }
};

class ModelB: public Airplane {
public:
  virtual void fly(const Airport& destination) {
    Airplane::fly(destination);
  }
};

class ModelC: public Airplane {
public:
  virtual void fly(const Airport& destination) {
    cout << "ModelC::fly called" << endl;
  }
};

int main() {
  Airplane* a = new ModelA();
  Airplane* b = new ModelB();
  Airplane* c = new ModelC();
  Airport ap;
  a->fly(ap);
  b->fly(ap);
  c->fly(ap);
}

