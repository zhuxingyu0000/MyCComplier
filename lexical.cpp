#include <iostream>
#include "lexical.h"

#include <regex>
#include <vector>

#include <cassert>

using namespace std;

const char* regexsid[]={
        "^if[ \t\v\f\r\\(]","^else[ \t\v\f\r\\{]","^for[ \t\v\f\r\\(]","^while[ \t\v\f\r\\(]","^do[ \t\v\f\r\\{]","^return[ \t\v\f\r]","^unsigned[ \t\v\f\r]","^signed[ \t\v\f\r]",
        "^extern[ \t\v\f\r]","^const[ \t\v\f\r]","^typedef[ \t\v\f\r]","^break[ \t\v\f\r;]","^continue[ \t\v\f\r;]","^auto[ \t\v\f\r]","^static[ \t\v\f\r]",
        "^goto[ \t\v\f\r]","^switch[ \t\v\f\r\\(]","^volatile[ \t\v\f\r]","^inline[ \t\v\f\r]","^enum[ \t\v\f\r\\{]","^struct[ \t\v\f\r\\{]","^union[ \t\v\f\r\\{]",
        "^case[ \t\v\f\r]","^int[ \t\v\f\r\\(]","^float[ \t\v\f\r\\(]","^double[ \t\v\f\r\\(]","^long[ \t\v\f\r\\(]","^short[ \t\v\f\r\\(]","^char[ \t\v\f\r\\(]",
        "^bool[ \t\v\f\r\\(]", nullptr
};

const char* regexop[]={
        "^\\+{1}","^-{1}","^\\*","^/","^=", nullptr
};

const char* regexbracket[]={
        "^\\(","^\\)","^\\{","^\\}","^;", nullptr
};

const char* regexuser[]={
        "^([a-zA-Z]|_)([a-zA-z0-9]|_)*", nullptr
};

const char* regexint[]={
        "^[0-9]+", nullptr
};

const char* regexdouble[]={
        "^(([0-9]+\\.[0-9]*)|(\\.[0-9]+))", nullptr
};

const char* regexchar[]={
        "^(\'[0-9A-Za-z`~!@#\\$%\\^&\\*\\(\\)\\{\\}\\[\\]:;<>,\\.\\?/ \\|\\+-_=]\')",
        "^\'\\\\[abfnrtv\\\\\"0]\'",
        nullptr
};

vector<regex*> compid;
vector<regex*> compop;
vector<regex*> compbracket;
vector<regex*> compuser;
vector<regex*> compint;
vector<regex*> compdouble;
vector<regex*> compchar;

void lex::lexregex_init()
{
    int rv=0;
    const char** ppartten;
    for(ppartten=regexsid;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compid.push_back(reg);
    }
    for(ppartten=regexop;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compop.push_back(reg);
    }
    for(ppartten=regexbracket;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compbracket.push_back(reg);
    }
    for(ppartten=regexuser;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compuser.push_back(reg);
    }
    for(ppartten=regexint;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compint.push_back(reg);
    }
    for(ppartten=regexdouble;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compdouble.push_back(reg);
    }
    for(ppartten=regexchar;*ppartten!= nullptr;ppartten++) {
        regex* reg=new regex(*ppartten);
        compchar.push_back(reg);
    }
}

void lex::lexregex_clear() {
    for(auto it=compid.begin();it!=compid.end();it++) delete *it;
    for(auto it=compop.begin();it!=compop.end();it++) delete *it;
    for(auto it=compbracket.begin();it!=compbracket.end();it++) delete *it;
    for(auto it=compuser.begin();it!=compuser.end();it++) delete *it;
    for(auto it=compint.begin();it!=compint.end();it++) delete *it;
    for(auto it=compdouble.begin();it!=compdouble.end();it++) delete *it;
    for(auto it=compchar.begin();it!=compchar.end();it++) delete *it;
}

void lex::stringscanner::nextnchar(int n){index+=n;}

char lex::stringscanner::nextchar() {
    if(index>=str.length())return EOF;
    else return str[index++];
}

void lex::stringscanner::updateendl() {
    int offset=0;
    while(str[index+offset]!='\n'&&(index+offset)<str.length()){offset++;}
    lineend=index+offset;
}

std::string lex::stringscanner::getstringendl() {
    return str.substr(index,lineend-index);
}

void lex::stringscanner::prevchar() {
    index--;
}

inline char makecharfromrow(string& s)
{
    if(s[1]!='\\') return s[1];
    else{
        switch(s[2]){
            case 'n':
                return '\n';
            case 'a':
                return '\a';
            case 'b':
                return '\b';
            case 'f':
                return '\f';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            case 'v':
                return '\v';
            case '\\':
                return '\\';
            case '\'':
                return '\'';
            case '\"':
                return '\"';
            case '0':
                return '\0';
            default:
                assert(1==2);
        }
    }
}

pair<lex::TOKEN,lex::VAR> lex::lexanalyzer::nextscan(){
    char next;
    int cnt=0;
    while(isspace(next=scan.nextchar())){
        if(next=='\n') scan.updateendl();
    }
    if(next==EOF){
        VAR v;
        return make_pair(TOKEN_END,v);
    }
    scan.prevchar();
    for(auto it=compid.begin();it!=compid.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        t.push_back(' ');
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            v.id=(ID)cnt;
            string pat;
            scan.nextnchar(sm[0].length()-1);
            pat=sm[0].str();
            pat.pop_back();
            cout<<"id:"<<pat<<endl;
            return make_pair(TOKEN_IDENTIFIER,v);
        }
    }
    cnt=0;
    for(auto it=compop.begin();it!=compop.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            v.op=(OP)cnt;
            scan.nextnchar(sm[0].length());
            cout<<"op:"<<sm[0]<<endl;
            return make_pair(TOKEN_OPERATOR,v);
        }
    }
    cnt=0;
    for(auto it=compbracket.begin();it!=compbracket.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            string sss=sm[0];
            switch(sss[0]){
                case '(':
                    v.op=OP_LSMALL_BRAK;
                    break;
                case ')':
                    v.op=OP_RSMALL_BRAK;
                    break;
                case '[':
                    v.op=OP_LMID_BRAK;
                    break;
                case ']':
                    v.op=OP_RMID_BRAK;
                    break;
                case '{':
                    v.op=OP_LLARGE_BRAK;
                    break;
                case '}':
                    v.op=OP_RLARGE_BRAK;
                    break;
                case ';':
                    v.op=OP_ENDL;
                    break;
                default:
                    v.op=(OP)0;
            }
            scan.nextnchar(sm[0].length());
            cout<<"bra:"<<sm[0]<<endl;
            return make_pair(TOKEN_BRACKET,v);
        }
    }
    cnt=0;
    for(auto it=compdouble.begin();it!=compdouble.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            scan.nextnchar(sm[0].length());
            v.f=atof(sm[0].str().c_str());
            cout<<"fl:"<<sm[0]<<endl;
            return make_pair(TOKEN_DOUBLE,v);
        }
    }
    cnt=0;
    for(auto it=compint.begin();it!=compint.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            scan.nextnchar(sm[0].length());
            v.l=atoll(sm[0].str().c_str());
            cout<<"int:"<<sm[0]<<endl;
            return make_pair(TOKEN_INT,v);
        }
    }
    cnt=0;
    for(auto it=compchar.begin();it!=compchar.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            string sss=sm[0].str();
            scan.nextnchar(sm[0].length());
            v.c=makecharfromrow(sss);
            cout<<"char:"<<sm[0]<<endl;
            return make_pair(TOKEN_CHAR,v);
        }
    }
    cnt=0;
    for(auto it=compuser.begin();it!=compuser.end();it++,cnt++)
    {
        string t=scan.getstringendl();
        smatch sm;
        if(true==regex_search(t,sm,**it)){
            VAR v;
            v.s=new char[sm[0].length()+1];
            strcpy(v.s,sm[0].str().c_str());
            scan.nextnchar(sm[0].length());
            cout<<"user:"<<sm[0]<<endl;
            return make_pair(TOKEN_USER,v);
        }
    }
    VAR v;
    return make_pair(TOKEN_END,v);
}

int main()
{
    pair<lex::TOKEN,lex::VAR> p;
    p.first=lex::TOKEN_INT;
    lex::lexregex_init();
    string r("int a()\n{x = 1;return (x + 1)  ;}");
    lex::stringscanner s(r);
    lex::lexanalyzer a(s);
    while(p.first!=lex::TOKEN_END)
        p=a.nextscan();
    lex::lexregex_clear();
    return 0;
}
