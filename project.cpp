#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

//linux: not exe but ((space))
//binary to int pros: comparing tag w/ "==" 
class block{
    public:
    int index; //LSB
    int tag;
    int nru_bit;
};

int bi_to_de(char* bi, int n){
    int sum = 0;
    for (int i=0; i<n ; i++){
        if (bi[i] == '1'){ 
            int b; //pow( float x, float y )	回傳 x 的 y 次方	float
            b = int(pow(2, n-1-i)) * 1;
            sum = sum + b;
        }
    }
    return sum;
}

int num_bits(int n){
    n = log2(n);
    return n;
}

int indexInDe(char* addr, int offsetBits, int indexBits, int n){
    char Id[indexBits];
    for(int i=0; i<indexBits; i++){
        Id[indexBits-1-i] = addr[n-1-offsetBits-i];
    }
    int index = bi_to_de(Id, indexBits);
    return index;
}

int tagInDe(char* addr, int offsetBits, int indexBits, int n){
    int rest = offsetBits + indexBits;
    int tagBits = n - rest;

    char Tag[tagBits];
    for(int i=0; i<tagBits; i++){
        Tag[tagBits-1-i] = addr[n-1-rest-i];
    }
    int tag = bi_to_de(Tag, tagBits);
    return tag;
}


int main(int argc, char *argv[3]){
    //read from cache#.org
    int addressBit, blockSize, cacheSet, associativity = 0;
    string x; // those we don't need
    ifstream doc_cache (argv[1]);
    ofstream output_file (argv[3]);
    output_file.clear();
    if (doc_cache.is_open())
    {
        /*
        Address_bits: 8
        Block_size: 4
        Cache_sets: 8
        Associativity: 1
        */
        doc_cache >> x >> addressBit;
        output_file << x << " " << addressBit << endl; 
        doc_cache >> x >> blockSize;
        output_file << x << " " << blockSize << endl;
        doc_cache >> x >> cacheSet;
        output_file << x << " " << cacheSet << endl;
        doc_cache >> x >> associativity;
        output_file << x << " " << associativity << endl;
        output_file << endl;
        /*
        cout << addrBits << endl;
        cout << blockSize << endl;
        cout << cacheSets << endl;
        cout << associativity << endl;
        */
        doc_cache.close();

        
    }
    else cout << "open doc_cache failed";

    int tagBit, indexBit, offsetBit = 0;
    offsetBit = num_bits(blockSize);
    indexBit = num_bits(cacheSet);
    tagBit = addressBit - offsetBit - indexBit;
    output_file << "Offset bit count: " << offsetBit << endl;
    output_file << "Indexing bit count: " << indexBit << endl;
    output_file << "Indexing bits:";
    for (int i=0; i<indexBit; i++){
        output_file << " " << offsetBit+indexBit-1-i ;
    }
    output_file << endl;

    //read from reference#.lst
    ifstream doc_ref (argv[2]);
    if (doc_ref.is_open())
    {
        /*
        .benchmark testcase1
        00101100
        00100000
        00101100
        00100000
        .end
        */
        output_file << endl;
        doc_ref >> x ;
        output_file << x ;
        doc_ref >> x ;
        output_file << " " << x << endl; 
        
    }
    else cout << "open doc_ref failed";

    //initialize the cache
    block cache[cacheSet][associativity];

    for (int i=0; i<cacheSet; i++){
        for (int j=0; j<associativity; j++){
            cache[i][j].index = -1;
            cache[i][j].tag = -1;
            cache[i][j].nru_bit = 1; // turn to 0 when use;
        }
    }
/*
    for (int i=0; i<cacheSets; i++){
        for (int j=0; j<associativity; j++){
            cout << cache[i][j].index;
            cout << cache[i][j].tag;
            cout << cache[i][j].nru_bit;
        }
        cout << endl;
    }
*/  
    vector<bool> record;
    while (1){
        char query[addressBit];
        doc_ref >> query;
        cout << "read: " << query ;
        if (query[0] == '.' && query[1] == 'e'){ //if it is ".end"
            output_file << query << endl;
            output_file << endl;
            break;
        }
        else{
            int I = indexInDe(query, offsetBit, indexBit, addressBit);//if an binary address
            int T = tagInDe(query, offsetBit, indexBit, addressBit);
            bool hit = false;
            for (int i=0; i< associativity; i++){
                if (cache[I][i].tag == T){ //if hit
                    cache[I][i].nru_bit = 0;
                    record.push_back(0); //hit
                    hit = true;
                    cout << "hit: " << I << " "<< T;
                    break;
                }
            }
            if (hit==false){
                bool space = false;
                record.push_back(1); //miss
                for (int i=0; i< associativity; i++){ // if there is a space
                    if (cache[I][i].nru_bit == 1){
                        cache[I][i].index = I;
                        cache[I][i].tag = T;
                        cache[I][i].nru_bit = 0;
                        space = true;
                        break; 
                    }
                }
                if (space==false){ //NRU
                    for (int i = 0; i < associativity; i++){
                        cache[I][i].nru_bit = 1;
                    }
                    for (int i=0; i< associativity; i++){ // if there is a space
                        if (cache[I][i].nru_bit == 1){
                            cache[I][i].index = I;
                            cache[I][i].tag = T;
                            cache[I][i].nru_bit = 0;
                            space = true;
                            break; 
                        }
                    }
                }
            }
            cout << endl;
            output_file << query;
            if (hit)
                output_file << " hit" << endl;
            else
                output_file << " miss" << endl;
        }
    }
    
    int missCount = 0;
    for (int i=0; i < record.size(); i++){
        if (record[i] == 1)
            missCount++;
    }
    output_file << "Total cache miss count: " << missCount << endl;
    cout << "miss count: "<< missCount << endl;
    doc_ref.close();// danger!! extreme case
    output_file.close(); 
    //}
    return 0;
    

}