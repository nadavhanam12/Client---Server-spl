#include <STOMPProtocol.h>
#include <connectionHandler.h>
#include <clientInfo.h>
#include <readFromSocket.h>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <boost/algorithm/string.hpp>

using namespace std;

int main () {
    bool connected = false;
    int count = 0;
    string login;
    getline(cin, login);
    vector <string> inputs;
    boost::split(inputs, login, boost::is_any_of(" "));
    vector <string> hostport;
    boost::split(hostport, inputs[1], boost::is_any_of(":"));
    string host = hostport[0];
    short port = stoi(hostport[1]);
    ConnectionHandler connectionHandler(host, port);
    connected = connectionHandler.connect();
    if (!connected) {
        cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 0;
    }
    clientInfo client(inputs[2], inputs[3]);
    STOMPProtocol stompProtocol(&client);
    readFromSocket taskSocket(&connectionHandler, &stompProtocol);
    thread thSocket(ref(taskSocket));
    while (connected) {
        string input;
        string toSend;
        if (count==0) {
            toSend = stompProtocol.KeyboardInputToStomp(login);
            count++;
        }
        else {
            getline (cin, input);
            toSend = stompProtocol.KeyboardInputToStomp(input);
        }
        if (toSend.compare("")!=0) {
            if (!connectionHandler.sendFrameAscii(toSend, '\0')) {
                connectionHandler.connect();
                taskSocket.reconnect();
                connectionHandler.sendFrameAscii(toSend, '\0');
            }
        }
    }
}