#ifndef BOOST_ECHO_CLIENT_CLIENTINFO_H
#define BOOST_ECHO_CLIENT_CLIENTINFO_H
#include <string>
#include <map>
#include <vector>
#include <list>
#include <bookDetails.h>

using namespace std;

class clientInfo {
private:
    string name;
    string password;
    list <BookDetails> inventory;
    list <string> janers;
    list <string> wishlist;

    void clear();

    void copy(const string &_name,
              const string &_pasword,
              const list <BookDetails> &_inventory,
              const list <string> &_janers,
              const list <string> &_wishlist);

public:
    clientInfo(string _name, string _password);

    string getClientName();

    void addJaner (string janer);

    void removeJaner (string janer);

    bool isJanerExist (string janer);

    bool isBookExist(string book_name);

    void addBook(BookDetails book);

    void removeBook(string book_name);

    void addToWishlist (string book_name);

    void removeFromWishlist (string book_name);

    bool isOnWishList (string book_name);

    string getBooksListByJaner (string janer);

    void setClientName (string name);

    void setClientPassword(string password);

    void clearDetails();

    // Destructor
    virtual ~clientInfo();

    //Copy Consturctor
    clientInfo(const clientInfo &other);

    // Move Constructor
    clientInfo(clientInfo &&other);

    // Copy Assignment
    clientInfo& operator=(const clientInfo &other);

    // Move Assignment
    clientInfo& operator=(clientInfo &&other);

};


#endif

