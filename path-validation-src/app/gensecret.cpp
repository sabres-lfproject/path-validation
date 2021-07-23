#include <iostream>
#include <openssl/rand.h>
#include "sha256.h"
#include "zkbpp_core.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>

using namespace std;

long long get_vector_string_size(const vector<string> &v){
    long long size = 0;
    for(unsigned int i=0; i<v.size(); i++){
        size += v[i].size();
    }
    return size;
}

int main(int argc, char *argv[]){

    // int hop_number = 1;
    string fileName = "";
    if (argc != 2) {
        printf("initializer should take one argument\n");
        exit(1);
    } else if (argc == 2) {
        // hop_number = atoi(argv[1]);
        // if (hop_number < 1) {
        //     printf("hop number should larger than zero\n");
        //     exit(1);
        // }
    }
    fileName = argv[1];
    // printf("hop number: %d\n", hop_number);

    string fpath = "../data/"; // Segmentation fault will happen, if there is no data folder. Should mkdir at the beginning.
    // input string test: 00000000000000000000000000000000
    uint8_t* input = new uint8_t[32]{0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
    // vector<uint8_t *> inputs;

    // for (int i = 0; i < ZKBOO_HASH_BYTES; i++) {
    //     inputs.push_back(new uint8_t[ZKBOO_HASH_BYTES]);
    // }
    const int input_byte_number = 32;
    ofstream file;

    // clock_t start, end;
    // double init_time_used = 0;

    // generate a 32 bytes random number
    // for (int hop_id = hop_number; hop_id > 0; hop_id--) {

        // start = clock();

        // RAND_bytes(inputs[hop_id], input_byte_number);
        RAND_bytes(input, input_byte_number);
        file.open(fpath + fileName + ".secret", ios::out);
        // show the random number
        // printf("A random secret of %d: \n", hop_id);
        for (int input_byte_idx = 0; input_byte_idx < input_byte_number; input_byte_idx++){
            file << setfill('0') << setw(2) << hex << (int)input[input_byte_idx];

            // if(hop_id < hop_number) {
            //     input[input_byte_idx] = input[input_byte_idx] ^ inputs[hop_id+1][input_byte_idx];
            // }
            // printf("%02X", input[input_byte_idx]);
        }
        // printf("\n");
        file.close();

        // uint32_t res[8]; // 256 bits
        // memset(res, 0, 8 * sizeof(uint32_t)); // initial the buffer
        // sha256<uint8_t, uint32_t>(inputs[hop_id], input_byte_number, NULL, 0, res, 8);

        // file.open(fpath + to_string(hop_id) + ".hash", ios::out);
        // // show the result of SHA256(secret)
        // printf("hash value of hop_%d: \n", hop_id);
        // for(int hash_word_idx=0; hash_word_idx < 8; hash_word_idx++){
        //     printf("%08X ", res[hash_word_idx]);
        //     file << setfill('0') << setw(8) << hex << (int)res[hash_word_idx];
        // }
        // printf("\n");
        // file.close();

        // end = clock();
        // init_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;


        // printf("Computed proof-commit-for-verifier with length %d bytes.\n", (int) proof_commit_updated.size());
        // printf("Computed response (without proof_commit), length %lld bytes.\n", get_vector_string_size(z));
    // }
    // std::cout << "init_time_used: " << init_time_used << std::endl; 
    return 0;
}
