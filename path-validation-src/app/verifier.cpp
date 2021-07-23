#include <iostream>
#include <fstream>
#include "sha256.h"
#include "zkbpp_core.h"

using namespace std;

long long get_vector_string_size(const std::vector<std::string> &v){
	long long size = 0;
	for(unsigned int i=0; i<v.size(); i++){
		size += v[i].size();
	}
	return size;
}

int main(int argc, char *argv[]){

    ifstream file;
    string tmp_str = "";
    string new_str = "";
    string fpath = "../data/";
    string hash_filename = "";
    string proof_filename = "";
    const int input_byte_number = ZKBOO_HASH_BYTES;
    const int output_byte_number = ZKBOO_HASH_BYTES;
    const int random_tape_len_in_bytes = sha256_random_tape_len_in_bytes(input_byte_number, output_byte_number);

    if (argc != 3) {
        printf("verifier should input hash filename and proof filename \n");
        exit(1);
    }
    hash_filename = argv[1];
    proof_filename = argv[2];

    uint32_t res[8]; // 256 bits
    memset(res, 0, 8 * sizeof(uint32_t)); // initial the buffer

    file.open(hash_filename, ios::in);
    if (!file.is_open()) {
        cout << "Could not open the file" << endl;
        return 0;
    }
    // read the hash from file

    while(getline(file, tmp_str)) {
        int j = 0;
        for(int i=0; i < (int)tmp_str.length(); i+=8) {
            string byte = tmp_str.substr(i, 8);
            res[j] = strtol(byte.c_str(), NULL, 16);
            // printf("%08X ",res[j]);
            j++;
        }
    }
    // printf("\n");
    file.close();

    unsigned char hash_p[ZKBOO_HASH_BYTES];
    file.open(proof_filename, ios::in);
    if (!file.is_open()) {
        cout << "Could not open the file" << endl;
        return 0;
    }
    while(getline(file, tmp_str) && !tmp_str.empty()) {
        for(int i=0; i < (int)tmp_str.length(); i+=2) {
            string byte = tmp_str.substr(i, 2);
            char ch = (char)(int)strtol(byte.c_str(), NULL, 16);
            hash_p[i/2] = ch;
        }
        tmp_str.clear();
    }

    vector<string> z;
    while(getline(file, tmp_str) && !tmp_str.empty()) {
        for(int i=0; i < (int)tmp_str.length(); i+=2) {
            string byte = tmp_str.substr(i, 2);
            char ch = (char)(int)strtol(byte.c_str(), NULL, 16);
            new_str.push_back(ch);
        }
        z.push_back(new_str);
        new_str.clear();
        tmp_str.clear();
    }

    string proof_commit_updated = "";
    while(getline(file, tmp_str) && !tmp_str.empty()) {
        for(int i=0; i < (int)tmp_str.length(); i+=2) {
            string byte = tmp_str.substr(i, 2);
            char ch = (char)(int)strtol(byte.c_str(), NULL, 16);
            proof_commit_updated.push_back(ch);
        }
        tmp_str.clear();
    }
    file.close();

    // printf("Computed proof-commit-for-verifier with length %d bytes.\n", (int) proof_commit_updated.size());
    // printf("Computed response (without proof_commit), length %lld bytes.\n", get_vector_string_size(z));

    unsigned char hash_v[32];
    string proof_commit_reconstructed;
    proof_commit_reconstructed = zkbpp_verify<uint32_t>("sha256", hash_p, input_byte_number, NULL, 0,
                        (char *) res, output_byte_number, random_tape_len_in_bytes,
                        sha256, proof_commit_updated, z);

    GenChallengeROM_from_single_proof(hash_v, proof_commit_reconstructed);
    if (proof_commit_reconstructed.length() < 1){
        printf("NOT VERIFIED!");
        return 1;
    }
    assert(memcmp(hash_v, hash_p, ZKBOO_HASH_BYTES) == 0);
    cout << "verify successfully\n";
    return 0;
}
