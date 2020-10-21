#ifndef BOOST_ECHO_CLIENT_READFROMSOCKET_H
#define BOOST_ECHO_CLIENT_READFROMSOCKET_H
#include <string>
#include <STOMPProtocol.h>

using namespace std;

class readFromSocket {
private:
    ConnectionHandler * reciever;
    STOMPProtocol * decoder;
    string answer;
    bool connect;

    void clear();

    void copy(const ConnectionHandler* _reciever,
              const STOMPProtocol * _decoder,
              const string &_answer,
              const bool &_connect);

public:
    readFromSocket(ConnectionHandler * c, STOMPProtocol * S);

    void operator () ();

    void reconnect ();

    // Destructor
    virtual ~readFromSocket();

    //Copy Consturctor
    readFromSocket(const readFromSocket &other);

    // Move Constructor
    readFromSocket(readFromSocket &&other);

    // Copy Assignment
    readFromSocket& operator=(const readFromSocket &other);

    // Move Assignment
    readFromSocket& operator=(readFromSocket &&other);

};



#endif //BOOST_ECHO_CLIENT_READFROMSOCKET_H
