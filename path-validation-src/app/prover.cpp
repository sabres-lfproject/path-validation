#include <iostream>
#include <fstream>
#include <iomanip>
#include "sha256.h"
#include "zkbpp_core.h"

using namespace std;

int main(int argc, char *argv[]) {

    ifstream ifile;
    ofstream ofile;
    string tmp_str = "";
    string secret_filename = "";
    string hash_filename = "";
    string proof_filename = "";
    const int input_byte_number = ZKBOO_HASH_BYTES;
    const int output_byte_number = ZKBOO_HASH_BYTES;
    const int random_tape_len_in_bytes = sha256_random_tape_len_in_bytes(input_byte_number, output_byte_number);

    if (argc != 4) {
        printf("prover should input secret filename, hash filename and proof filename \n");
        exit(1);
    }
    secret_filename = argv[1];
    hash_filename = argv[2];
    proof_filename = argv[3];

    uint8_t * input = new uint8_t[input_byte_number];
    ifile.open(secret_filename, ios::in);
    if (!ifile.is_open()) {
        cout << "Could not open the file" << endl;
        return 0;
    }
    // printf("secret input of hop_%d: \n", hop_id);
    while(getline(ifile, tmp_str)) {
        int j = 0;
        for(int i=0; i < (int)tmp_str.length(); i+=2) {
            string byte = tmp_str.substr(i, 2);
            input[j] = strtol(byte.c_str(), NULL, 16);
            // printf("%02X ",input[j]);
            j++;
        }
    }
    ifile.close();

    uint32_t res[8]; // 256 bits
    memset(res, 0, 8 * sizeof(uint32_t)); // initial the buffer

    ifile.open(hash_filename, ios::in);
    if (!ifile.is_open()) {
        cout << "Could not open the file" << endl;
        return 0;
    }
    // read the hash from file
    // printf("hash value of hop_%d: \n", hop_id);
    while(getline(ifile, tmp_str)) {
        int j = 0;
        for(int i=0; i < (int)tmp_str.length(); i+=8) {
            string byte = tmp_str.substr(i, 8);
            res[j] = strtol(byte.c_str(), NULL, 16);
            // printf("%08X ",res[j]);
            j++;
        }
    }
    // printf("\n");
    ifile.close();

    vector<string> z_all;
    string proof_commit_full;
    const char * commit_input = (char *) input;
    proof_commit_full = zkbpp_prove_commit<uint32_t>(z_all, "sha256", commit_input, strlen(commit_input),
                    NULL, 0,
                    (char *)res, output_byte_number,
                    random_tape_len_in_bytes + input_byte_number * sizeof(uint32_t), sha256);

    // printf("Generated proof-commit-full with length %d bytes,  Z_ALL  length is %lld bytes.\n", (int) proof_commit_full.size(), get_vector_string_size(z_all));

    // the challenger will generate the challenge
    unsigned char hash_p[ZKBOO_HASH_BYTES];
    GenChallengeROM_from_single_proof(hash_p, proof_commit_full);
    ofile.open(proof_filename, ios::out);
    for (int input_byte_idx = 0; input_byte_idx < (int)ZKBOO_HASH_BYTES; input_byte_idx++){
        ofile << setfill('0') << setw(2) << hex << (int)hash_p[input_byte_idx];
    }
    ofile << "\n\n";

    vector<string> z;
    string proof_commit_updated; // after challenge is known

    z = zkbpp_prove_response(z_all, hash_p);
    for (const auto &str: z) {
        for (const auto &ch: str) {
            ofile << setfill('0') << setw(2) << hex << (int)(unsigned char)ch;
        }
        ofile << "\n";
    }
    ofile << "\n";

    proof_commit_updated = zkbpp_update_proof_commitment_string(proof_commit_full, hash_p);
    for(int proof_idx = 0; proof_idx < (int)proof_commit_updated.length(); proof_idx++) {
        ofile << setfill('0') << setw(2) << hex << (uint)(unsigned char)(proof_commit_updated[proof_idx]);
    }
    ofile.close();

}