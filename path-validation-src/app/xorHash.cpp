#include <iostream>
#include <fstream>
#include <iomanip>
#include "sha256.h"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream ifile;
    string tmp_str = "";
    uint8_t * secret;
    secret = new uint8_t[ZKBOO_HASH_BYTES];
    int fileNum;

    fileNum = argc - 1;
    // std::cout<<fileNum<<std::endl;
    if (fileNum != 1 && fileNum != 2) {
        printf("The tool takes one or two argument\n");
        exit(1);
    }

    for (int idx = 0; idx < fileNum; idx++) {
        ifile.open(argv[idx+1], ios::in);
        if (!ifile.is_open()) {
            cout << "Could not open the secret file" << endl;
            exit(1);
        }
        while(getline(ifile, tmp_str)) {
            for(int i=0; i < (int)tmp_str.length(); i+=2) {
                string byte = tmp_str.substr(i, 2);
                secret[i/2] = !idx ? (int)strtol(byte.c_str(), NULL, 16) : secret[i/2] ^ (int)strtol(byte.c_str(), NULL, 16);
            }
            tmp_str.clear();
        }
        ifile.close();
    }
    // std::cout << "new secret: ";
    // for (int i = 0; i < ZKBOO_HASH_BYTES; i++) {
    //     printf("%02X", secret[i]);
    // }
    // std::cout << std::endl;
    ofstream ofile;
    ofile.open("../data/new.secret", ios::out);
    for (int i = 0; i < ZKBOO_HASH_BYTES; i++) {
        ofile << setfill('0') << setw(2) << hex << (int)secret[i];
    }
    ofile.close();

    // std::cout << "hash value: ";
    uint32_t hash_of_secret[8]; // 256 bits
    memset(hash_of_secret, 0, 8 * sizeof(uint32_t)); // initial the buffer
    sha256<uint8_t, uint32_t>(secret, ZKBOO_HASH_BYTES, NULL, 0, hash_of_secret, 8);
    
    string fileName = argv[1];
    string purefileName = fileName.substr(0, fileName.rfind("."));
    {
        ofstream ofile;
        ofile.open(purefileName + ".hash", ios::out);
        // show the result of SHA256(secret)

        for(int hash_word_idx=0; hash_word_idx < 8; hash_word_idx++){
            // printf("%08X ", hash_of_secret[hash_word_idx]);
            ofile << setfill('0') << setw(8) << hex << (int)hash_of_secret[hash_word_idx];
        }
        // printf("\n");
        ofile.close();
    }
    return 0;
}