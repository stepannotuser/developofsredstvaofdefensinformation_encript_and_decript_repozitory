#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "Alghorutm.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

void print_file_content(const string& filepath) {
    ifstream in(filepath, std::ios::binary);
    if (!in.is_open()) {
        cerr << "Не удалось открыть файл: " << filepath << endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    std::cout << "Содержимое файла " << filepath << ":\n";
    std::cout << content << std::endl;  
}

void process_folder(const string& folder, const string& password, bool show_content, bool decrypt_mode) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().string().find("data") != string::npos) {
            string filepath = entry.path().string();

            if (show_content) {
                cout << "До алгоритма:\n";
                print_file_content(filepath);
            }

            ifstream in(filepath, std::ios::binary);
            vector<unsigned char> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            vector<unsigned char> result;
            
            if (decrypt_mode) {
                result = Encryptor::getInstance().decrypt(data, password);  
            }
            else {
                result = Encryptor::getInstance().encrypt(data, password); 
            }
          
            ofstream out(filepath, std::ios::binary | std::ios::trunc);
            out.write((char*)result.data(), result.size());
            out.close();

            if (show_content) {
                cout << endl;
                cout << "После алгоритма:\n";
                print_file_content(filepath);
                cout << endl;
            } 
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        cout << "Неправильно заданы параметры\n";
        return 1;
    }

    string folder = argv[1];
    string password = argv[2];
    string mode = argv[3];
    bool decrypt_mode = (mode == "decrypt");
    bool show_content = (argc == 5 && string(argv[4]) == "--show");

    if (mode != "encrypt" && mode != "decrypt") {
        cout << "Ошибка: <режим> должен быть encrypt или decrypt\n";
        return 1;
    }

    process_folder(folder, password, show_content, decrypt_mode);
    if (decrypt_mode) {
        cout << "Дешифрование завершено\n";
    }
    else {
        cout << "Шифрование завершено\n";
    }
    return 0;
}
