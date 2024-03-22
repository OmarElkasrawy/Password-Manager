#include <iostream>
#include <fstream>
#include <sstream>

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
            loadUsersFromFile(); // it loads users from users.txt when hashtable created
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

    void loadUsersFromFile() {
        ifstream infile("users.txt");
        if(infile.is_open()) {
            string line;
            while (getline(infile, line)) {
                stringstream ss(line);
                string uname, password;
                getline(ss, uname, ',');
                getline(ss, password);
                User user = {
                        uname,
                        password
                };
                addUser(user);
            }
            infile.close();
        } else {
            cerr << "Unable to open file to read" << endl; // out unbuffered to display errors
        }
    }

    void saveUsersToFile() {
        ofstream outfile("users.txt");
        if (outfile.is_open()) {
            for (int i = 0; i < hashtable_size; ++i) {
                Node *current = table[i];
                while (current != nullptr) {
                    outfile << current->user.username << "," << current->user.password << endl;
                    current = current->next;
                }
            }

            outfile.close();
        } else {
            cerr << "Unable to open file to write";
        }
    }

    // function to find user by username
    User* findUser(string username) {
        int index = hash(username);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->user.username == username) {
                return &(current->user);
            }
            current = current->next;
        }
        return nullptr; // user not found
    }
    // return pointer to pointer to a Node
    Node** getTable(){
        return table;
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