#include <bits/stdc++.h>
using namespace std;

#define byte_size 4
#define block_size 64

#define cache_size 32768 // 32KB
#define cache_blocks 512 // 32768 / 64
#define cache_set_size 64 // 512 / 8

#define INVALID 0
#define MISSPENDING 1
#define VALID 2
int64_t MOD = 1000000;
struct decoded {
    int64_t tag;
    int index;
    int offset;
    int64_t real;
};
int64_t total = 0;
int64_t hit = 0;
//---------------------------------------------------------------------CACHE------------------------------------------
class CACHE {
private:
    vector<vector<int64_t>> cache;
    vector<int64_t> status;
    vector<int64_t> modify;
    vector<int64_t> tag;
    vector<vector<decoded>> waiting;

public:
    CACHE()
        : cache(cache_blocks, vector<int64_t>(16, 0)),
          status(cache_blocks, 0),
          modify(cache_blocks, 0),
          tag(cache_blocks, 0),
          waiting(cache_blocks)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrb(0,255);
        for(auto &block:cache)
        {   
            for(auto &byte:block)
            {
               //byte = distrb(gen);
            }
        }
        for(auto &t:tag)
        {
            t = distrb(gen);
        }
        for (auto& index : status) {
            index = INVALID;
        }
    }
    vector<int64_t >memory_request()
    {   
        vector<int64_t>ret(16);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrb(0,255);
        cout<<"BLOCK:PULLED INTO CACHE ";
        for(auto &val:ret)
        {
            val = distrb(gen);
            cout<<val<<" ";
        }
        cout<<endl;
        return ret;
    }
    int evict(int t, int i, int o, int r,int li,int ri,int read)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrb(0,255);
        for(int j=li;j<=ri;j++)
        {
            if(status[j]==INVALID)
            {
                status[j] = MISSPENDING;
                tag[j] = t;
                cache[j] = memory_request();
                status[j] = VALID;
                if(read)
                {
                    return cache[j][o];
                }
                else
                {
                    cout<<"PREVIOUS VALUE: "<<cache[j][o]<<endl;
                    cache[j][o] = distrb(gen);
                    cout << "UPDATED VALUE: " << cache[j][o] << endl;
                    return cache[j][o];
                }
            }
        }
        return -1;
    }
    int replacment(int t,int i,int o,int r,int li,int ri,int read)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrb(0,255);
        int victim = li+(random()%7);
        cout<<"victim"<<" "<<victim<<endl;
        status[victim] = MISSPENDING;
        tag[victim] = t;
        cache[victim] = memory_request();
        status[victim] = VALID;
        if(read)
        {
            return cache[victim][o];
        }
        else
        {
            cout<<"PREVIOUS VALUE: "<<cache[victim][o]<<endl;
            cache[victim][o] = distrb(gen);
            cout << "UPDATED VALUE: " << cache[victim][o] << endl;
            return cache[victim][o];
        }

    }
    void cache_read(int t, int i, int o, int64_t r) {
        int li = i * 8, ri = i * 8 + 7;
        cout<<li<<" "<<ri<<endl;
        for (int j = li; j <= ri; j++) {
            if (tag[j] == t && status[j] == VALID) 
            {
                cout << "CACHE HIT: " << o << endl;
                hit++;
                cout << "VALUE: " << cache[j][o] << endl;
                return;
            } 
            else if (tag[j] == t && status[j] == MISSPENDING) 
            {
                cout << "WAITING FOR MEMORY" << endl;
                decoded node = {t, o, i, r};
                waiting[j].push_back(node);
                return;
            }
        }
        cout<<"CACHE MISS"<<endl;
        cout << "REQUEST TO MAIN MEMORY" << endl;
        int res = evict(t,i,o,r,li,ri,1);
        if(res!=-1)
        {
            cout<<res<<endl;
            return;
        }
        cout<<"REPLACMENT"<<endl;
        cout<<replacment(t,i,o,r,li,ri,1)<<endl;
        
    }
    void cache_write(int t, int i, int o, int64_t r) 
    {
        int li = i * 8, ri = i * 8 + 7;
        //cout<<li<<" "<<ri<<endl;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrb(0,255);
        for (int j = li; j <= ri; j++) {
            if (tag[j] == t && status[j] == VALID) 
            {
                cout << "CACHE HIT: " << o << endl;
                hit++;
                cout<<"PREVIOUS VALUE: "<<cache[j][o]<<endl;
                cache[j][o] = distrb(gen);
                cout << "UPDATED VALUE: " << cache[j][o] << endl;
                return;
            } 
            else if (tag[j] == t && status[j] == MISSPENDING) 
            {
                cout << "WAITING FOR MEMORY" << endl;
                decoded node = {t, o, i, r};
                waiting[j].push_back(node);
                return;
            }
        }
        cout<<"CACHE MISS"<<endl;
        cout << "REQUEST TO MAIN MEMORY" << endl;
        int res = evict(t,i,o,r,li,ri,0);
        if(res!=-1)
        {
            cout<<res<<endl;
            return;
        }
        cout<<"REPLACMENT"<<endl;
        cout<<replacment(t,i,o,r,li,ri,0)<<endl;
    }
    void print() {
        int ind = 502;
        for (int i = ind;i<512;i++) 
        {
            if (ind == 512) break;
            cout<<status[ind]<<" - "<<tag[ind]<<" - ";
            for(int j = 0;j<16;j++)
            {
                cout << cache[i][j] << " ";
            }
            ind++;
            cout << endl;
        }
    }
};
CACHE cache;

//-----------------------------------------------------------CPU-----------------------------------------------------------------
class CPU {
public:
    int64_t generate_address() {
    random_device rd;
    mt19937_64 gen(rd());

    double mean = (1LL << 40) / 8;  
    double stddev = (1LL << 40) / 16;  
    normal_distribution<double> dist(mean, stddev);
    int64_t address = static_cast<int64_t>(dist(gen));
    address = (address / 4) * 4;
    return address%MOD;
    }
};
//---------------------------------------------------------------DECODER--------------------------------

class decoder {
public:
    decoded decode(int64_t r) {
        bitset<40> binary(r); 
        int offset = r & ((1 << 6) - 1); 
        offset = (offset>>2);          
        int index = (r >> 6) & ((1 << 6) - 1);    
        int64_t tag = (r >> 12) & ((1LL << 28) - 1); 

        cout << "Binary: " << binary << endl;
        cout << "Tag: " << tag << ", Index: " << index << ", Offset: " << offset << endl;

        return decoded{tag, index, offset,r};
    }
};
decoder d;

int main() {
    CPU cpu;
    int x = 1000000;
   
    while (x--)
        {
            int64_t val = cpu.generate_address();
            cout <<"ADDRESS: "<<val << endl;
            decoded d1 = d.decode(val);
            int type = random()%2;
            if(type == 1)
            {
                cout<<"READ"<<endl;
                cache.cache_read(d1.tag,d1.index,d1.offset,d1.real);
            }
            else
            {
                cout<<"WRITE"<<endl;
                cache.cache_write(d1.tag,d1.index,d1.offset,d1.real);
            }
            cache.print();
            total++;
            cout<<"hit: "<<hit<<" "<<"total: "<<total<<endl;
            cout<<(1.0*hit)/total<<endl;
        }
    cout<<"hit: "<<hit<<" "<<"total: "<<total<<endl;
    cout << (1.0*hit) / total << endl;

    return 0;
    /*
    decoder d;
    
    vector<int64_t> addresses;
    for (int i = 0; i < 10; i++) {
        int64_t tag = rand() % (1LL << 28);
        int64_t index = 63;  
        int64_t address = (tag << 12) | (index << 6);
        addresses.push_back(address);
        cout << "Generated Address for Set 7: " << address << endl;
        decoded d1 = d.decode(address);
        cout << "Tag: " << d1.tag << ", Index: " << d1.index << ", Offset: " << d1.offset << ", Address: " << d1.real << endl;
        int x = random()%2;
        if(x == 1)
        {
            cout<<"READ"<<endl;
            cache.cache_read(d1.tag,d1.index,d1.offset,d1.real);
        }
        else
        {
            cout<<"WRITE"<<endl;
            cache.cache_write(d1.tag,d1.index,d1.offset,d1.real);
        }
        cache.print();
        sleep(1);
    }
    */
}
