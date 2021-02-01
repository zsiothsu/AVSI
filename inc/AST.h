/*
 * @Author: Chipen Hsiao
 * @Date: 2020-05-01
 * @LastEditTime: 2020-06-02 15:55:23
 * @Description: constructer of abstract syntax tree (AST)
 */
#ifndef ___AST_H___
#define ___AST_H___

#include "Exception.h"
#include "SymbolTable.h"
#include "Token.h"
#include <typeinfo>
#include <vector>

#define __ASSIGN_NAME           "Assign"
#define __BINOP_NAME            "BinOp"
#define __BOOL_NAME             "Boolean"
#define __COMPOUND_NAME         "Compound"
#define __ECHO_NAME             "Echo"
#define __FUNCTIONCALL_NAME     "FunctionCall"
#define __FUNCTIONDECL_NAME     "FunctionDecl"
#define __IF_NAME               "If"
#define __NUM_NAME              "Num"
#define __PARAM_NAME            "Param"
#define __RETURN_NAME           "Return"
#define __UNARYTOP_NAME         "UnaryOp"
#define __VARIABLE_NAME         "Variable"

#define __NONEAST_NAME "NoneAST"

namespace AVSI {
    using std::string;
    using std::vector;

    /*******************************************************
     *                       AST base                      *
     *******************************************************/
    class AST {
    protected:
        Token token;

    public:
        string __AST_name;

        AST(void) {};

        AST(string name) : __AST_name(name) {};

        AST(string name, Token token) : token(token), __AST_name(name) {};

        virtual ~AST() {};

        Token getToken(void);
    };

    /*******************************************************
     *                    derived syntax                   *
     *******************************************************/
    class Assign : public AST {
    private:
    public:
        AST *left;
        AST *right;

        Assign(void) : AST(__ASSIGN_NAME), left(nullptr), right(nullptr) {};

        Assign(AST *left, AST *right)
                : AST(__ASSIGN_NAME, Token(EQUAL, '=')),
                  left(left),
                  right(right) {};

        virtual ~Assign();
    };

    class BinOp : public AST {
    private:
        Token op;

    public:
        AST *left;
        AST *right;

        BinOp(void)
                : AST(__BINOP_NAME),
                  op(emptyToken),
                  left(nullptr),
                  right(nullptr) {};

        BinOp(AST *left, Token op, AST *right)
                : AST(__BINOP_NAME, op), op(op), left(left), right(right) {};

        virtual ~BinOp();

        TokenType getOp(void);
    };

    class Boolean : public AST {
    private:
        any value;
    
    public:
        Boolean(void): AST(__BOOL_NAME) {};

        Boolean(Token token): AST(__BOOL_NAME, token), value((token.getType() == TRUE) ? true : false) {};

        virtual ~Boolean() {};

        any getValue(void);
    };

    class Echo : public AST {
    public:
        AST* content;

        Echo(void): AST(__ECHO_NAME) {};
        Echo(AST* content, Token token)
                : AST(__ECHO_NAME,token), content(content) {};
    };

    class FunctionDecl : public AST {
    public:
        string id;
        AST *paramList;
        AST *compound;

        FunctionDecl(void)
                : AST(__FUNCTIONDECL_NAME), paramList(nullptr), compound(nullptr) {};

        FunctionDecl(string id, AST *paramList, AST *compound, Token token)
                : AST(__FUNCTIONDECL_NAME, token),
                  id(id),
                  paramList(paramList),
                  compound(compound) {};

        virtual ~FunctionDecl();
    };

    class FunctionCall : public AST {
    public:
        string id;
        vector<AST *> paramList;
        Symbol_function *symbol_function;

        FunctionCall(void)
                : AST(__FUNCTIONCALL_NAME), paramList(vector<AST *>()) {};

        FunctionCall(string id, vector<AST *> paramList, Token token)
                : AST(__FUNCTIONCALL_NAME, token), id(id), paramList(paramList) {};

        virtual ~FunctionCall();
    };

    class If : public AST {
    public:
        AST* condition;
        bool noCondition;
        AST* compound;
        AST* next;

        If(void) : AST(__IF_NAME), condition(nullptr), compound(nullptr), next(nullptr) {};
        If(AST *condition, bool noCondition, AST* compound, AST *next, Token token):
                AST(__IF_NAME,token), condition(condition), noCondition(noCondition),
                compound(compound), next(next) {};
        
        virtual ~If();
    };

    class Num : public AST {
    private:
        any value;

    public:
        Num(void) : AST(__NUM_NAME) {};

        Num(Token token) : AST(__NUM_NAME, token), value(token.getValue()) {};

        virtual ~Num() {};

        any getValue(void);
    };

    class UnaryOp : public AST {
    private:
        Token op;

    public:
        AST *right;

        UnaryOp(void) : AST(__UNARYTOP_NAME) {};

        UnaryOp(Token op, AST *right)
                : AST(__UNARYTOP_NAME, op), op(op), right(right) {};

        virtual ~UnaryOp();

        TokenType getOp(void);
    };

    class Variable : public AST {
    public:
        std::string id;

        Variable(void) : AST(__VARIABLE_NAME) {};

        Variable(Token var)
                : AST(__VARIABLE_NAME, var),
                  id(var.getValue().any_cast<std::string>()) {};

        ~Variable() {};
    };

    class Compound : public AST {
    public:
        vector<AST *> child;

        Compound(void)
                : AST(__COMPOUND_NAME, Token(COMPOUND, 0)),
                  child(vector<AST *>()) {};

        virtual ~Compound();
    };

    class Param : public AST {
    public:
        vector<Variable *> paramList;

        Param(void) : AST(__PARAM_NAME), paramList(vector<Variable *>()) {};
    };

    class Return : public AST {
    public:
        AST *ret;

        Return(void) : AST(__RETURN_NAME) {};

        Return(Token token, AST *ret) : AST(__RETURN_NAME, token), ret(ret) {};
    };

    class NoneAST : public AST {
    public:
        NoneAST(void) : AST(__NONEAST_NAME, Token(NONE, 0)) {};

        virtual ~NoneAST() {};
    };

    static AST ASTEmpty = NoneAST();
} // namespace AVSI

#endif