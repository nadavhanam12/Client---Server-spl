#include <string>
#include <map>
#include <vector>
#include <stack>
#include <clientInfo.h>
#ifndef BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
using namespace std;

class STOMPProtocol {
private:
    int reciept_id;
    int subscription_id;
    vector <string> topicToId;
    map <int,string> recieptIdToAction;
    map <string,string> BorrowOrder;
    clientInfo * activeclient;
    string borrow;
    string login_name;
    string login_pass;

    void clear();

    void copy(const int &_reciept_id,
              const int &_subscription_id,
              const vector <string> &_topicToId,
              const map <int,string> &_recieptIdToAction,
              const  map <string,string> &_BorrowOrder,
              const clientInfo * _activeclient,
              const string &_borrow,
              const string &_login_name,
              const string &_login_pass);

public:
    STOMPProtocol(clientInfo * client);

    string KeyboardInputToStomp(string input);

    string StompToOtput (string input);

    string getBorrow();

    void clearBorrow ();

    // Destructor
    virtual ~STOMPProtocol();

    //Copy Consturctor
    STOMPProtocol(const STOMPProtocol &other);

    // Move Constructor
    STOMPProtocol(STOMPProtocol &&other);

    // Copy Assignment
    STOMPProtocol& operator=(const STOMPProtocol &other);

    // Move Assignment
    STOMPProtocol& operator=(STOMPProtocol &&other);

    };

#endif //BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
