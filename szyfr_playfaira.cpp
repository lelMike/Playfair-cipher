#include <iostream>
#include <fstream>
#include <string>

using namespace std;

char CHARACTERS[36] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                       'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0',
                       '1', '2', '3', '4', '5', '6', '7', '8', '9'};

string makeUnique(const string& key){

    //Tworzenie unikatowego ciagu klucza
    string unique;
    bool seen[256] = {false};
    for (int i = 0; i < key.length(); i++) {
        if (!seen[key[i]]) {
            unique += key[i];
            seen[key[i]] = true;
        }
    }

    return unique;
}

int search(const char oneDimTable[36], char elem){
    for (int i = 0; i < 36; i++){
        if (elem == oneDimTable[i]){
            return i;
        }
    }
}

int main() {

    //Deklaracja klucza i nazwy pliku
    string key, file_location; char table[6][6]; char oneDimTable[36]; bool mode;
    cout << "Enter desired key: (A-Z, 0-9)" << endl;
    cin >> key;
    cout << "Enter file name: " << endl;
    cin >> file_location;
    cout << "Do you want to cipher (1) or decypher (0)?" << endl;
    cin >> mode;

    ofstream file_overwrite((file_location + ".out"), ios::trunc);
    file_overwrite.close();

    //Szyfrowanie + deszyfrowanie
    string line;
    ifstream file_read(file_location);
    if (file_read.is_open()) {
        while (getline(file_read, line)){
            key = makeUnique(key);

            //Tworzenie tabelki do szyfru
            string copy_unique = key;
            for (int i = 0; i < 6; i++){
                for (int j = 0; j < 6; j++){
                    if (!copy_unique.empty()){
                        table[i][j] = copy_unique[0];
                        copy_unique = copy_unique.substr(1, copy_unique.length());
                    }
                }
            }

            int iter = 0; int temper = key.length() % 6;

            //Generuje macierz do szyforwania 6x6
            if(key.length() < 36) {
                for (int i = key.length() / 6; i < 6; i++) {
                    for (int j = temper; j < 6; j++) {
                        int temp = 0;
                        while (temp < key.length()){
                            if (CHARACTERS[iter] == key[temp]){
                                temp = 0; iter++;
                            }
                            else{
                                temp++;
                            }
                        }
                        table[i][j] = CHARACTERS[iter]; iter++;
                    }
                    temper = 0;
                }
            }

            for (int i = 0; i < 6; i++){
                for (int x = 0; x < 6; x++){
                    oneDimTable[(i*6) + x] = table[i][x];
                }
            }

            for (int i = 0; i < 6; i++){
                for (int x = 0; x < 6; x++){
                    cout << table[i][x] << " ";
                }
                cout << endl;
            }

            string coded; string ciphered;
            //Wypelnianie znakami specjalnymi (Q)
            for (int i = 0; i < line.length() - 1; i += 2){
                if (line[i] == line[i + 1]){
                    line = line.insert(i + 1, "Q");
                }
            }
            if (line.length() % 2){
                line += "Q";
            }
            cout << "Entered: " << line << endl;
            //Szyfrowanie po tabelce
            for (int i = 0; i < line.length(); i += 2){
                int flat_index1 = search(oneDimTable, line[i]); int flat_index2 = search(oneDimTable, line[i + 1]);
                int y1 = flat_index1 / 6; int x1 = flat_index1 % 6;
                int y2 = flat_index2 / 6; int x2 = flat_index2 % 6;

                if (y1 == y2){
                    if (mode) {
                        coded += table[y1][(x1 + 1) % 6];
                        coded += table[y2][(x2 + 1) % 6];
                    }
                    else{
                        coded += table[y1][(x1 + 5) % 6];
                        coded += table[y2][(x2 + 5) % 6];
                    }
                }
                else if (x1 == x2){
                    if (mode) {
                        coded += table[(y1 + 1) % 6][x1];
                        coded += table[(y2 + 1) % 6][x2];
                    }
                    else{
                        coded += table[(y1 + 5) % 6][x1];
                        coded += table[(y2 + 5) % 6][x2];
                    }
                }
                else{
                    coded += table[y1][x2];
                    coded += table[y2][x1];
                }
            }
            cout << "Output: " <<coded << endl << endl;

            if (mode){
                ofstream file_export((file_location.substr(0, file_location.length() - 4) + ".out"), ios::app);
                file_export << coded << endl;
                file_export.close();
            }
            else{
                ofstream file_export((file_location + ".out"), ios::app);
                file_export << coded << endl;
                file_export.close();
            }

        }
        file_read.close();
    }
    else{
        cout << "Unable to open file" << endl;
    }

    return 0;
}
