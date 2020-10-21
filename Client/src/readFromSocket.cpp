#include <string>
#include <connectionHandler.h>
#include <readFromSocket.h>

readFromSocket::readFromSocket(ConnectionHandler * c, STOMPProtocol * S) : reciever(c), decoder (S), answer ("") , connect(true) {}

void readFromSocket::operator()() {
    while (true) {
        if (connect) {
            reciever->getFrameAscii(answer, '\0');
            string output = decoder->StompToOtput(answer);
            cout << output << endl;
            if ((output.compare("disconnected!") == 0)|(output.compare("Wrong password") == 0)) {
                reciever->close();
                connect = false;
            }
            string s = decoder->getBorrow();
            if (s.compare("") != 0) {
                reciever->sendFrameAscii(s, '\0');
                decoder->clearBorrow();
            }
            answer = "";
        }
    }
}

void readFromSocket::reconnect() {
    connect = true;
}

// Destructor
readFromSocket::~readFromSocket() {
    clear();
}

void readFromSocket::clear() {
    reciever = nullptr;
    decoder = nullptr;
}

// Copy Constructor
readFromSocket::readFromSocket(const readFromSocket &other) :reciever(other.reciever) , decoder (other.decoder) , answer(other.answer) , connect (other.connect)  {}

// Copy Assignment
readFromSocket& readFromSocket::operator=(const readFromSocket &other) {
    if (this != &other)
    {   clear();
        copy(other.reciever,
             other.decoder,
             other.answer,
             other.connect); }
    return *this;
}


void readFromSocket::copy(const ConnectionHandler* _reciever,
                          const STOMPProtocol * _decoder,
                          const string &_answer,
                          const bool &_connect) {
    reciever = const_cast<ConnectionHandler *>(_reciever);
    decoder = const_cast<STOMPProtocol *>(_decoder);
    answer = _answer;
    connect = _connect;

}

// Move Constructor
readFromSocket::readFromSocket(readFromSocket&& other)
        : reciever (other.reciever),
          decoder(other.decoder),
          answer (other.answer),
          connect(other.connect) {
    other.reciever = nullptr;
    other.decoder = nullptr;
}

// Move Assignment
readFromSocket& readFromSocket::operator=(readFromSocket &&other)
{
    if (this != &other)
    {
        clear();
        reciever = other.reciever;
        decoder = other.decoder;
        answer = other.answer;
        connect = other.connect;
        other.clear();
    }
    return *this;
}











