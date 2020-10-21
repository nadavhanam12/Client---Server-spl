#include <vector>
#include <string>
#include <STOMPProtocol.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
using namespace std;

STOMPProtocol::STOMPProtocol(clientInfo * client): reciept_id(0), subscription_id(0), topicToId () , recieptIdToAction(), BorrowOrder () , activeclient (client), borrow ("") ,login_name(""), login_pass (""){}

string STOMPProtocol::KeyboardInputToStomp (string input) {
    vector <string> inputs;
    boost::split(inputs,input,boost::is_any_of(" "));
    string sender = activeclient->getClientName();
    if (inputs[0].compare("login") == 0) {
        login_name = inputs [2];
        login_pass = inputs [3];
        return "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + inputs[2] + "\npasscode:" + inputs[3] +
               "\n\n";
    }

    if (inputs[0].compare("join")==0) {
        if (activeclient->isJanerExist(inputs[1])) {
            cout << "already subscribed to " + inputs[1] << endl;
            return "";
        }
        reciept_id ++;
        subscription_id ++;
        topicToId.push_back(inputs[1]);
        recieptIdToAction.insert(pair<int,string>(reciept_id,"join " + inputs[1]));
        return "SUBSCRIBE\ndestination:"+inputs[1]+"\nid:"+to_string(subscription_id)+"\nreceipt:"+to_string(reciept_id)+"\n\n";
    }

    if (inputs[0].compare("exit")==0) {
        if (!(activeclient->isJanerExist(inputs[1]))) {
            cout << "not subscribed to " + inputs[1] << endl;
            return "";
        }
        reciept_id++;
        recieptIdToAction.insert(pair<int,string>(reciept_id,"exit " + inputs[1]));
        int size = topicToId.size();
        for (int i=0;i<size;i++)
            if (topicToId[i]==inputs[1])
                return "UNSUBSCRIBE\nid:"+to_string(i+1)+"\nreceipt:"+to_string(reciept_id)+"\n\n";
    }

    if (inputs[0].compare("add")==0) {
        if (activeclient->isBookExist(inputs[2])) {
            cout << inputs[2] + " book is already in the inventory!" << endl;
            return "";
        }
        return "SEND\ndestination:" + inputs[1] + "\n\n" + sender + " has added the book " + inputs[2] + "\n";
    }

    if (inputs[0].compare("borrow")==0) {
        if (activeclient->isBookExist(inputs[2])) {
            cout << inputs[2] + "book is already in the inventory, no need to borrow!" << endl;
            return "";
        }
        if (activeclient->isOnWishList(inputs[2])) {
            cout << inputs[2] + " is already in your wishlist!" << endl;
            return "";
        }
        activeclient->addToWishlist(inputs[2]);
        return "SEND\ndestination:" + inputs[1] + "\n\n" + sender + " wish to borrow " + inputs[2] + "\n";
    }

    if (inputs[0].compare("return")==0) {
        if (!(activeclient->isBookExist(inputs[2]))) {
            cout << inputs[2] + " is not in your inventory!" << endl;
            return "";
        }
        string backto = BorrowOrder.find(inputs[2])->second;
        if (backto.compare(activeclient->getClientName())==0) {
            cout << inputs[2] + " is owned by this user!" << endl;
            return "";
        }
        if (!(activeclient->isBookExist(inputs[2]))) {
            cout << inputs[2] + " is not in the inventory!" << endl;
            return "";
        }
        activeclient->removeBook(inputs[2]);
        return "SEND\ndestination:" + inputs[1] + "\n\nReturning " + inputs[2] + " to " + backto + "\n";
    }

    if (inputs[0].compare("status")==0) {
        return "SEND\ndestination:" + inputs[1] + "\n\nbook status\n";
    }

    if (inputs[0].compare("logout")==0) {
        reciept_id++;
        recieptIdToAction.insert(pair<int,string>(reciept_id,"disconnect"));
        string rec_id = to_string(reciept_id);
        return "DISCONNECT\nreceipt:" + rec_id + "\n\n";
    }
    return "";
}

string STOMPProtocol::StompToOtput(string input) {
    vector<string> inputs;
    boost::split(inputs, input, boost::is_any_of("\n"));
    if (inputs[0].compare("CONNECTED") == 0) {
        activeclient->clearDetails();
        activeclient->setClientName(login_name);
        activeclient->setClientPassword(login_pass);
        return "Login successful";
    }

    if (inputs[0].compare("ERROR") == 0) {
        return (inputs[2]);
    }

    if (inputs[0].compare("RECEIPT") == 0) {
        int id = stoi(inputs[1].substr(11));
        string action = recieptIdToAction.find(id)->second;
        if (action.compare("disconnect")==0) {
            activeclient->clearDetails();
            return "disconnected!";
        }
        vector <string> split;
        boost::split(split, action, boost::is_any_of(" "));
        string janer = split[1];
        if (action.substr(0,4).compare("join")==0) {
            activeclient->addJaner(janer);
            return "Joined club " + janer;
        }
        if (action.substr(0,4).compare("exit")==0) {
            activeclient->removeJaner(janer);
            return "Exited club " + janer;
        }
    }

    if (inputs[0].compare("MESSAGE") == 0) {
        string janer = inputs[3].substr(12);
        string msg = inputs[5];
        vector<string> action;
        boost::split(action, msg, boost::is_any_of(" "));
        if (action.size() == 2) { //send status message
            if (activeclient->isJanerExist(janer)) {
                string booklist = activeclient->getBooksListByJaner(janer);
                borrow = "SEND\n" + inputs[3] + "\n\n" + activeclient->getClientName() + ":" + booklist + "\n";
            }
        }
        if (action.size() == 3) { //someone has a book
            if (activeclient->isOnWishList(action[2])) {
                activeclient->removeFromWishlist(action[2]);
                BorrowOrder.insert(pair <string,string>(action[2],action[0]));
                BookDetails toAdd(janer, action[2]);
                activeclient->addBook(toAdd);
                borrow = "SEND\n" + inputs[3] + "\n\n" + "Taking " + action[2] + " from " + action[0] +"\n";
            }
        }
        if (action.size() == 4) { //returning or taking a book
            if (action[0].compare("Taking")==0) {
                if (action[3].compare(activeclient->getClientName()) == 0) {
                    activeclient->removeBook(action[1]);
                }
            }
            if (action[0].compare("Returning")==0) {
                if (action[3].compare(activeclient->getClientName())==0) {
                    BookDetails toAdd(janer, action[1]);
                    activeclient->addBook(toAdd);
                }
            }
        }
        if (action.size() == 5) { //wish to borrow
            if (activeclient->isBookExist(action[4])) {
                borrow = "SEND\n" + inputs[3] + "\n\n" + activeclient->getClientName() + " has " + action[4] + "\n";
            }
        }
        if (action.size() == 6) { //book is added
            if (activeclient->getClientName().compare(action[0])==0) {
                BookDetails toAdd(janer, action[5]);
                activeclient->addBook(toAdd);
                BorrowOrder.insert(pair<string,string>(action[5],activeclient->getClientName()));
            }
        }
        return janer+":"+msg;
    }
    return "error";
}

string STOMPProtocol::getBorrow() {
    return borrow;
}

void STOMPProtocol::clearBorrow() {
    borrow = "";
}

// Destructor
STOMPProtocol::~STOMPProtocol() {
    clear();
}

void STOMPProtocol::clear() {
    topicToId.clear();
    recieptIdToAction.clear();
    BorrowOrder.clear();
    activeclient->clearDetails();
    activeclient = nullptr;
}

// Copy Constructor
STOMPProtocol::STOMPProtocol(const STOMPProtocol &other) :
        reciept_id(other.reciept_id),
        subscription_id(other.subscription_id),
        topicToId(other.topicToId),
        recieptIdToAction(other.recieptIdToAction),
        BorrowOrder(other.BorrowOrder),
        activeclient(other.activeclient),
        borrow(other.borrow),
        login_name(other.login_name),
        login_pass(other.login_pass)
{}

// Copy Assignment
STOMPProtocol& STOMPProtocol::operator=(const STOMPProtocol &other)
{
    if (this != &other)
    {   clear();
        copy(other.reciept_id,
        other.subscription_id,
        other.topicToId,
        other.recieptIdToAction,
        other.BorrowOrder,
        other.activeclient,
        other.borrow,
        other.login_name,
        other.login_pass); }

    return *this;
}

void STOMPProtocol::copy(const int &_reciept_id, const int &_subscription_id, const vector<string> &_topicToId,
                         const map<int, string> &_recieptIdToAction, const map<string, string> &_BorrowOrder,
                         const clientInfo *_activeclient, const string &_borrow, const string &_login_name,
                         const string &_login_pass) {
    reciept_id = _reciept_id;
    subscription_id = _subscription_id;
    topicToId = _topicToId;
    recieptIdToAction = _recieptIdToAction;
    BorrowOrder =_BorrowOrder;
    activeclient = const_cast<clientInfo *>(_activeclient);
    borrow  = _borrow;
    login_name = _login_name;
    login_pass = _login_pass;
}

// Move Constructor
STOMPProtocol::STOMPProtocol(STOMPProtocol&& other)
        : reciept_id (other.reciept_id),
          subscription_id(other.subscription_id),
          topicToId (other.topicToId),
          recieptIdToAction(other.recieptIdToAction),
          BorrowOrder(other.BorrowOrder),
          activeclient(other.activeclient),
          borrow(other.borrow),
          login_name(other.login_name),
          login_pass(other.login_pass) {
          other.topicToId.clear();
          other.recieptIdToAction.clear();
          other.BorrowOrder.clear();
          other.activeclient = nullptr;
}

// Move Assignment
STOMPProtocol& STOMPProtocol::operator=(STOMPProtocol &&other)
{
    if (this != &other)
    {
        clear();
        reciept_id = other.reciept_id;
        subscription_id = other.subscription_id;
        topicToId = other.topicToId;
        recieptIdToAction = other.recieptIdToAction;
        BorrowOrder =other.BorrowOrder;
        activeclient = other.activeclient;
        borrow  = other.borrow;
        login_name = other.login_name;
        login_pass = other.login_pass;
        other.clear();
    }
    return *this;
}




















