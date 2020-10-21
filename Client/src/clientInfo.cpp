#include <clientInfo.h>
#include <list>
#include <string>

clientInfo::clientInfo(string _name, string _password): name(_name), password(_password), inventory() ,janers() , wishlist()  {}

string clientInfo::getClientName() {
    return name;
}

bool clientInfo::isBookExist(string book_name) {
    list<BookDetails>::iterator it;
    for (it = inventory.begin(); it != inventory.end(); it++)
        if (it->getName().compare(book_name)==0)
            return true;
    return false;
}



void clientInfo::addBook(BookDetails toAdd) {
        inventory.push_back(toAdd);
}

void clientInfo::removeBook(string book_name) {
    list<BookDetails>::iterator it;
    list<BookDetails>::iterator it1;
    for (it = inventory.begin(); it != inventory.end(); it++) {
        if (it->getName().compare(book_name) == 0)
            it1 = it;
    }
        inventory.erase(it1);
}

void clientInfo::addJaner(string janer) {
    janers.push_back(janer);
}

bool clientInfo::isJanerExist(string janer) {
        list<string>::iterator it;
           for(it=janers.begin();it!=janers.end();it++)
               if (janer.compare(*it)==0)
                   return true;
    return false;
}

void clientInfo::removeJaner(string janer) {
    if (!isJanerExist(janer))
        return;
    janers.remove(janer);
}

void clientInfo::addToWishlist(string book_name) {
    wishlist.push_back(book_name);
}

void clientInfo::removeFromWishlist(string book_name) {
    wishlist.remove(book_name);
}

bool clientInfo::isOnWishList(string book_name) {
    list<string>::iterator it;
    for(it=wishlist.begin();it!=wishlist.end();it++)
        if (book_name.compare(*it)==0)
            return true;
    return false;
}

string clientInfo::getBooksListByJaner(string janer) {
    string output = "";
    list<BookDetails>::iterator it;
    for (it = inventory.begin(); it != inventory.end(); it++) {
        if (it->getJaner().compare(janer) == 0) {
            output = output + "," + it->getName();
        }
    }
        if ((int)output.size()>0) return output.substr(1);
        else return output;
}

void clientInfo::setClientName(string name) {
    this->name = name;
}

void clientInfo::setClientPassword(string password) {
    this->password = password;

}

void clientInfo::clearDetails() {
    inventory.clear();
    wishlist.clear();
    janers.clear();
}

// Destructor
clientInfo::~clientInfo() {
    clear();
}

void clientInfo::clear() {
    inventory.clear();
    wishlist.clear();
    janers.clear();
}

// Copy Constructor
clientInfo::clientInfo(const clientInfo &other) :name(other.name) ,password(other.password),inventory (other.inventory), janers(other.janers) , wishlist (other.wishlist) {}

// Copy Assignment
clientInfo& clientInfo::operator=(const clientInfo &other) {
    if (this != &other)
    {   clear();
        copy(other.name,
             other.password,
             other.inventory,
             other.janers,
             other.wishlist); }
    return *this;
}


void clientInfo::copy(const string &_name,
                      const string &_pasword,
                      const list <BookDetails> &_inventory,
                      const list <string> &_janers,
                      const list <string> &_wishlist) {
                        name = _name;
                        password = _pasword;
                        inventory = _inventory;
                        janers = _janers;
                        wishlist = _wishlist;
}

// Move Constructor
clientInfo::clientInfo(clientInfo&& other)
        : name (other.name),
          password(other.password),
          inventory (other.inventory),
          janers(other.janers),
          wishlist(other.wishlist) {
    other.inventory.clear();
    other.janers.clear();
    other.wishlist.clear();
}

// Move Assignment
clientInfo& clientInfo::operator=(clientInfo &&other)
{
    if (this != &other)
    {
        clear();
        name = other.name;
        password = other.password;
        inventory = other.inventory;
        janers = other.janers;
        wishlist = other.wishlist;
        other.clear();
    }
    return *this;
}











