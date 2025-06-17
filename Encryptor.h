#include <vector>
#include <string>

using std::vector;
using std::string;

class Encryptor {
private:
    Encryptor(); // Свойство паттерна Одиночка (Singleton)
    Encryptor(const Encryptor&) = delete; // Свойство паттерна Одиночка
    Encryptor& operator=(const Encryptor&) = delete; // Свойство паттерна Одиночка
    vector<unsigned char> generateKey(const string& password, size_t length);
    

public:
    static Encryptor& getInstance(); // Паттерн Одиночка
    vector<unsigned char> encrypt(const vector<unsigned char>& data, const string& password);
    vector<unsigned char> decrypt(const vector<unsigned char>& data, const string& password);
};
