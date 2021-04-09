//
//  STDM.cpp
//  LAB4
//
//  Created by 钱依柔 on 6/30/19.
//  Copyright © 2019 钱依柔. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <deque>
#include <stdlib.h>

using namespace std;

typedef struct _PairInfo{
    int src;
    int val;
}PairInfo, *PPairInfo;

class STDM{
public:
    STDM();
    ~STDM();
    
    vector<std::string> SplitStr(const string& strSrc, const string& strSplit);
    int getLastTime(const vector<string>& frames);
    void ReadFile(const char* szFilePath);
    map< int,vector<int> > inputBuffer();
    
    void outputBuffer(map< int,vector<int> >& inBuf);
private:
    int duration;
    double inputRate;
    map< char,vector<std::string> > mpInfo;
};

STDM::STDM(){
    duration = 0;
    inputRate = 0.0f;
}

STDM::~STDM()
{
}

vector<string> STDM::SplitStr(const string& strSrc, const string& strSplit){
    vector<string> vsRet;
    string::size_type pos1, pos2;
    pos2 = strSrc.find(strSplit);
    pos1 = 0;
    while(string::npos != pos2){
        vsRet.push_back(strSrc.substr(pos1, pos2-pos1));
        
        pos1 = pos2 + strSplit.size();
        pos2 = strSrc.find(strSplit, pos1);
    }
    if(pos1 != strSrc.length()){
        vsRet.push_back(strSrc.substr(pos1));
    }
    return vsRet;
}

// get last time from file
int STDM::getLastTime(const vector<string>& frames){
    string lastFrame = frames[frames.size() - 1];
    return lastFrame.at(2) - '0';
}

//read in file
void STDM::ReadFile(const char* szFilePath){
    mpInfo.clear();
    ifstream fileCsv(szFilePath);
    if (!fileCsv.is_open()){
        return;
    }
    
    while (!fileCsv.eof()){
        string strLine;
        getline(*static_cast<istream*>(&fileCsv),strLine);
        if(strLine.length() == 0){
            continue;
        }
        
        vector<string> cmps = SplitStr(strLine, ":");
        if(cmps.size() < 2){
            continue;
        }
        
        char srcName = cmps[0].at(6);
        string srcItem = cmps[1];
        vector<string> frame = SplitStr(srcItem, ",");
        duration = max(duration, getLastTime(frame));
        
        mpInfo[srcName] = frame;
    }
}

//get information from file
map< int,vector<int> > STDM::inputBuffer(){
    map< int,vector<int> > inBuf ;//= new int[numOfSrc][duration + 1];
    
    map< char,vector<string> >::iterator it = mpInfo.begin();
    for (; it != mpInfo.end(); it++){
        int srcNum = it->first - 'A';
        vector<string>& vsValue = it->second;
        for (vector<string>::iterator itValue = vsValue.begin(); itValue != vsValue.end(); itValue++){
            vector<string> f = SplitStr(*itValue, " ");
            int startTime = atoi(f[0].c_str());
            int endTime = atoi(f[1].c_str());
            int block = f[2].at(1) - '0';
            inBuf[srcNum].resize(duration + 1);
            for (int i = startTime; i < endTime; i++){
                inBuf[srcNum][i] = block;
            }
            inputRate += ((endTime - startTime)) * 1.0 / duration;
        }
    }
    return inBuf;
}

string intToStr(int nValue){
    char szRet[15] = {0};
    sprintf(szRet, "%d", nValue);
    return szRet;
}

string toBinary(int num, int len){
    string res;
    for (int i = 0; i < len; i++){
        res.insert(0, intToStr(num % 2));
        num /= 2;
    }
    return res;
}

//output the result
void STDM::outputBuffer(map< int,vector<int> >& inBuf){
    string startFlag = "1", endFlag = "1";
    int outputRate = (int) ceil(inputRate);
    //int addrlen = (int) ceil(log(inBuf.size()) / log(2));
    deque<PairInfo> buffer;
    //Queue<Pair<Integer, Integer>> buffer = new ArrayDeque<>();
    
    float tBegin = 0, tEnd, tOffset = 0.5;
    for (int t = 0; t < duration || !buffer.empty(); t++){
        string frame(startFlag);
        int ct = 0;
        //inBuf
        for (int i = 0; i < inBuf.size(); i++){
            int curr = inBuf[i][t];
            if (curr != 0){
                PairInfo pairInfo = {i, curr};
                buffer.push_back(pairInfo);
            }
        }
        
        //tOffset = (buffer.size() > 1 ? 0.5 : 1);
        tBegin = t + 1;
        tEnd = tBegin + tOffset;
        cout << "SF" << endl;
        while (ct++ < outputRate){
            if (!buffer.empty()){
                PairInfo last = buffer.front();
                buffer.pop_front();
                //output
                cout << last.src << "," << tBegin << " " << tEnd << " " << char(65 + last.src) << last.val << endl;
                tBegin = tEnd;
                tEnd = tBegin + tOffset;
            }
        }
        cout << "EF" << endl;
    }
}

//main function
int main(int argc, char *argv[]){
    if (argc != 2){
        cout << "param is error" << endl;
        return 0;
    }
    
    STDM s;
    s.ReadFile(argv[1]);
    map<int, vector<int> > inBuffer = s.inputBuffer();
    s.outputBuffer(inBuffer);
    
    return 0;
}
