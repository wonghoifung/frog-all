#include <iostream>
#include <string>

using namespace std;

class CompanyA {
public:
  void sendCleartext(const string& msg) {cout<<"CompanyA::sendCleartext: "<<msg<<endl;}
  void sendEncrypted(const string& msg) {cout<<"CompanyA::sendEncrypted: "<<msg<<endl;}
};

class CompanyB {
public:
  void sendCleartext(const string& msg) {cout<<"CompanyB::sendCleartext: "<<msg<<endl;}
  void sendEncrypted(const string& msg) {cout<<"CompanyB::sendEncrypted: "<<msg<<endl;}
};

class CompanyZ {
public:
  void sendEncrypted(const string& msg) {cout<<"CompanyZ::sendEncrypted: "<<msg<<endl;}
};

struct MsgInfo {
  string info;
};

template <typename Company>
class MsgSender {
public:
  void sendClear(const MsgInfo& info) {
    string msg = string("[") + info.info + string("]");
    Company c;
    c.sendCleartext(msg);
  }
  void sendSecret(const MsgInfo& info) {
    string msg = string("{") + info.info + string("}");
    Company c;
    c.sendEncrypted(msg);
  }
};

template <>
class MsgSender<CompanyZ> {
public:
  void sendSecret(const MsgInfo& info) {
    string msg = string("{") + info.info + string("}");
    CompanyZ c;
    c.sendEncrypted(msg);
  }
};

template <typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
  //using MsgSender<Company>::sendClear;
  void sendClearMsg(const MsgInfo& info) {
    cout << "log begin" << endl;
    //sendClear(info);
    this->sendClear(info);
    //MsgSender<Company>::sendClear(info);
    cout << "log end" << endl;
  }
};

int main() {
  //LoggingMsgSender<CompanyZ> aMsgSender;
  LoggingMsgSender<CompanyA> aMsgSender;
  MsgInfo msgData; 
  msgData.info = "helloworld";
  aMsgSender.sendClearMsg(msgData);
}

