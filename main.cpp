#include <iostream>

using namespace std;


// Constants for hashtable size and encryption shift
const int hashtable_size = 100;
const int caeserShift = 3;

// User structure
struct User {
    string username;
    string password;
};

// Node structure for hashtable
struct Node {
    User user;
    Node* next{};
};

// Now class for hashtable
class Hashtable {
private:
    Node* table [hashtable_size]{};

public:
    Hashtable() {
        // this is range-based for loop
        for (auto & i : table)
            i = nullptr;

    }

    // Hash function
    int hash(string key) { // also can be static and const string& key
        int hashValue = 0;
        for (char c : key)
            // horner's rule
            hashValue = (hashValue * 31 + c) % hashtable_size; // to ensure it remains within the range of table
        return hashValue;
    }

    // function to add user
    void addUser(User user) {
        int index = hash(user.username);
        Node* newNode = new Node;
        newNode->user = user;
        newNode->next = table[index];
        table[index] = newNode;
    }
};







int main() {
    return 0;
}
// test1


/*Ref
 * https://en.cppreference.com/w/cpp/language/range-for
 * https://www.geeksforgeeks.org/transform-c-stl-perform-operation-elements/
 * https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/
 *
 * */