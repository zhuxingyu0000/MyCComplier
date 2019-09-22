//
// Created by zhuxingyu on 2019/9/22.
//

#ifndef MYC_LEXICAL_H
#define MYC_LEXICAL_H

#include <utility>

#include <cstring>

namespace lex {

    extern void lexregex_init();
    extern void lexregex_clear();

    typedef enum{
        TOKEN_IDENTIFIER,
        TOKEN_NUM,
        TOKEN_DOUBLE,
        TOKEN_USER,
        TOKEN_STRING,
        TOKEN_CHAR,
        TOKEN_OPERATOR,
        TOKEN_INT,
        TOKEN_BRACKET,
        TOKEN_END
    }TOKEN;

    typedef enum{
        IDEN_IF,
        IDEN_ELSE,
        IDEN_FOR,
        IDEN_WHILE,
        IDEN_DO,
        IDEN_RETURN,
        IDEN_UNSIGNED,
        IDEN_SIGNED,
        IDEN_EXTERN,
        IDEN_CONST,
        IDEN_TYPEDEF,
        IDEN_BREAK,
        IDEN_CONTINUE,
        IDEN_AUTO,
        IDEN_STATIC,
        IDEN_GOTO,
        IDEN_SWITCH,
        IDEN_VOLATILE,
        IDEN_INLINE,
        IDEN_ENUM,
        IDEN_STRUCT,
        IDEN_UNION,
        IDEN_CASE,
        IDEN_INT,
        IDEN_FLOAT,
        IDEN_DOUBLE,
        IDEN_LONG,
        IDEN_SHORT,
        IDEN_CHAR,
        IDEN_BOOL
    }ID;

    typedef enum{
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_EQL,
        OP_LSMALL_BRAK,
        OP_RSMALL_BRAK,
        OP_LMID_BRAK,
        OP_RMID_BRAK,
        OP_LLARGE_BRAK,
        OP_RLARGE_BRAK,
        OP_ENDL
    }OP;

    typedef union{
        ID id;
        OP op;
        char c;
        char* s;
        double f;
        long long int l;
    }VAR;

    class scanner{
    public:
        virtual void nextnchar(int n)=0;
        virtual void prevchar()=0;
        virtual char nextchar()=0;
        virtual void updateendl()=0;
        virtual std::string getstringendl()=0;
    };

    class stringscanner:public scanner{
    public:
        stringscanner(std::string& s):str(s),index(0),lineend(0){updateendl();}
        void nextnchar(int n);
        char nextchar();
        void updateendl();
        void prevchar();
        std::string getstringendl();
    private:
        std::string str;
        int index;
        int lineend;
    };

    class lexanalyzer {
    public:
        lexanalyzer(scanner& s):scan(s){}
        std::pair<TOKEN,VAR> nextscan();

    private:
        scanner& scan;
    };
}

#endif //MYC_LEXICAL_H
