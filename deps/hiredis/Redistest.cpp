#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <queue>
#include <fstream>
#include "hiredis.h"

using namespace std;
#define WARM_FILE "/home/wujianfeng/YCSB-tracegen/warm.txt"
#define TEST_FILE "/home/wujianfeng/YCSB-tracegen/test.txt"

typedef struct node{
    string key;
    string val;
    char op;
} node;

size_t sum = 10000000;
size_t num;
int point;

vector<node> warmQueue;
vector<node> testQueue;
redisContext *c;
redisReply *reply;

vector<string> Split(string str){
    vector<string> result;
    string temp("");
    str+=' ';
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] == ' '){
            result.push_back(temp);
            temp = "";
        }
        else{
            temp += str[i];
        }
    }
    return result;
}

void Load(bool isWarm){
    num = 0;
    point = 1;
    cout<<"Load for "<<(isWarm?"warm":"test")<<" start."<<endl;
    string req = "", value = "";
    vector<string> res;

    fstream f;
    f.open((isWarm?WARM_FILE:TEST_FILE), ios::in);
    if(!f){
        cout<<"Load file open fail!\n";
        exit(-1);
    }
    while(!f.eof()){
        getline(f, req);
        if(req.empty()) continue;
        node nd;
        res = Split(req);
        if(isWarm) {// for warm
            nd.op = res[0][0];
            nd.key = res[1];
            nd.val = res[2];
            warmQueue.push_back(nd);
        }
        else {// for test
            nd.op = res[0][0];
            nd.key = res[1];
            if(nd.op == 'I' || nd.op == 'U')
                nd.val = res[2];
            testQueue.push_back(nd);
        }
        num++;
        if((num*10)/sum == point){
            cout<<"Process "<<point++<<"0%."<<endl;
        }
    }
    f.close();

    cout<<"Load for "<<(isWarm?"warm":"test")<<" end."<<endl;
}

void Warm_Exec(){
    num = 0;
    point = 1;
    cout<<"Warm start.\n";
    for(auto it : warmQueue){
        reply = (redisReply *)redisCommand(c,"SET %s %s", it.key.c_str(), it.val.c_str());
        freeReplyObject(reply);
        num++;
        if((num*10)/sum == point){
            cout<<"Process "<<point++<<"0%."<<endl;
        }
    }
    cout<<"Warm end.\n";
}

void Test_Exec(){
    num = 0;
    point = 1;
    cout<<"Test start.\n";
    string val;
    for(auto it : testQueue){
        switch (it.op) {
            case 'I':{
                reply = (redisReply *)redisCommand(c,"SET %s %s", it.key.c_str(), it.val.c_str());
                freeReplyObject(reply);
                break;
            }
            case 'U':{
                reply = (redisReply *)redisCommand(c,"SET %s %s", it.key.c_str(), it.val.c_str());
                freeReplyObject(reply);
                break;
            }
            case 'R':{
                reply = (redisReply *)redisCommand(c,"GET %s", it.key.c_str());
                freeReplyObject(reply);
                break;
            }
            case 'D':{
                reply = (redisReply *)redisCommand(c,"DEL %s", it.key.c_str());
                freeReplyObject(reply);
                break;
            }
            default:break;
        }
        num++;
        if((num*10)/sum == point){
            cout<<"Process "<<point++<<"0%."<<endl;
        }
    }
    cout<<"Test end.\n";
}

// 计时框架
int main() {
    timespec t1, t2, t3, t4;
    uint64_t deltaT;

    Load(true); // load for warm
    Load(false); // load for test
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

    c = redisConnectWithTimeout((char*)"127.0.0.1", 6379, timeout);
    if (c->err) {
        printf("Connection error: %s\n", c->errstr);
        exit(1);
    }
    else{
        printf("Connection ok\n");
    }
    
    clock_gettime(CLOCK_REALTIME, &t1);
    Warm_Exec();
    clock_gettime(CLOCK_REALTIME, &t2);
    deltaT = (t2.tv_sec - t1.tv_sec) * 1000000000 + t2.tv_nsec - t1.tv_nsec; //纳秒
    cout<<"Warm time : "<<deltaT<<" ns"<<endl;

    clock_gettime(CLOCK_REALTIME, &t3);
    Test_Exec();
    clock_gettime(CLOCK_REALTIME, &t4);
    deltaT = (t4.tv_sec - t3.tv_sec) * 1000000000 + t4.tv_nsec - t3.tv_nsec; //纳秒
    cout<<"Test time : "<<deltaT<<" ns"<<endl;

    return 0;
}