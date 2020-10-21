#ifndef BOOST_ECHO_CLIENT_BOOKDETAILS_H
#define BOOST_ECHO_CLIENT_BOOKDETAILS_H
#include <string>
using namespace std;

class BookDetails {
private:
    string janer;
    string name;

public:
    BookDetails(string _janer, string _name);

    string getJaner ();

    string getName ();

};



#endif //BOOST_ECHO_CLIENT_BOOKDETAILS_H
