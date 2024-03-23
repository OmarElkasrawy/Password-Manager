#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <ctime>
#include <cctype>

// push fail 23 mar

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
            // horner's rule for polynomial evaluation
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


// caeser cipher encryption
string encrypt(string text) {
    string encryptedText = text;
    const int charset_size = 94;
    for (char& c : encryptedText) {
        if (isprint(c)) {
            char base = ' ' + 1;
            c = (c - base + caeserShift + charset_size % charset_size + base);
        }
    }
    return encryptedText;
}

// caeser cipher decryption
string decrypt(string text) {
    string decryptedText = text;
    const int charset_size = 94;
    for (char& c : decryptedText) {
        if (isprint(c)) {
            char base = ' ' + 1;
            c = (c - base - caeserShift + charset_size) % charset_size + base;
        }
    }
    return decryptedText;
}

// function generate random password
string generatePassword(int length) {
    string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
    string password;

    srand(time(nullptr));
    for (int i = 0; i < length; ++i)
        password.push_back(charset[rand() % charset.size()]);
    return password;
}


//test push

int main() {
    Hashtable hashtable;
    string choice;

    do {
        cout << "Are you a user? yes(yes)/create a user(create)/generate a password(generate)/retrieve all users(retrieve)/exit(exit): ";
        cin >> choice;
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower); // to transform all capital to small

        if (choice == "yes") {
            string username, password;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            User* user = hashtable.findUser(username);
            if (user != nullptr && decrypt(user->password) == password) {
                cout << "You are authorized!\n";
                string option;
                do {
                    cout << "what would you like to do? (retrieve password(retrieve)/modify password(modify)/exit(exit): ";
                    cin >> option;
                    transform(option.begin(), option.end(), option.begin(), ::tolower);

                    if (option == "retrieve") {
                        cout << "Your password is: " << decrypt(user->password) << endl;
                    } else if (option == "modify") {
                        string newPassword;
                        cout << "Enter your new password: ";
                        cin >> newPassword;

                        newPassword = encrypt(newPassword);

                        user->password = newPassword;
                        cout << "Password modified!\n";

                        ofstream outfile("users.txt");
                        if (outfile.is_open()) {
                            for (int i = 0; i < hashtable_size; ++i) {
                                Node *current = hashtable.getTable()[i];
                                while (current != nullptr) {
                                    outfile << current->user.username << "," << current->user.password << endl;
                                    current = current->next;
                                }
                            }
                            outfile.close();
                        }
                    } else if (option == "exit") {
                        break;
                    } else {
                        cout << "Invalid choice! Choose one of the provided options!";
                    }


                } while (option != "exit");
            } else {
                cout << "Unauthorized access! \n";
            }
        } else if (choice == "create") {
            string username, password;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            // now encrypt the entered password
            password = encrypt(password);

            // check if user already exists
            if (hashtable.findUser(username) != nullptr) {
                cout << "Username already exists! Please choose another one or login!\n";
            } else {
                // now add user
                User newUser = {username, password};
                hashtable.addUser(newUser);
                cout << "User created successfully!\n";

                // now save updated user to users.txt
                hashtable.saveUsersToFile();
            }
            // make it so the user chooses the length
        } else if (choice == "generate") {
            int passwordLength;

            cout << "Enter the length of the password you want to generate: ";
            cin >> passwordLength;
            string generatedPassword = generatePassword(passwordLength);
            cout << "Generated password: " << generatedPassword << endl;
        }
        else if (choice == "retrieve") {
            // retrieve all encrypted users
            Node **table = hashtable.getTable();
            bool found = false; // flag to see if users found
            for (int i = 0; i < hashtable_size; ++i) {
                Node *current = table[i];
                while (current != nullptr) {
                    cout << "Username: " << current->user.username << endl << "Encrypted password: " << current->user.password << endl;
                    current = current->next;
                    found = true;
                }
            }
            if (!found) {
                cout << "Hashtable is empty!! please create a user!" << endl;
            }
        } else if (choice == "exit") {
            break;
        } else {
            cout << "Invalid choice!";
        }
    } while (choice != "exit");

    // save all users to file before exit
    hashtable.saveUsersToFile();

    return 0;
}
// test1


/*Test cases
 * to
 * be
 * tested
 * here
 *
 * Hash Function test for key hello 42 (99247024 % hashtable_size)
 * addUser function for user Kasrawy pass
 * findUser function for existed user Kasrawy pass
 * encryption function pass for password kasrawy1
 * decryption function pass for password kasrawy1
 * password generation length of 10 pass
 * test capitalization for function create "cReAte" pass
 *
 * */

/*Ref
 * https://en.cppreference.com/w/cpp/language/range-for
 * https://www.geeksforgeeks.org/transform-c-stl-perform-operation-elements/
 * https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/
 * https://www.geeksforgeeks.org/horners-method-polynomial-evaluation/
 * */