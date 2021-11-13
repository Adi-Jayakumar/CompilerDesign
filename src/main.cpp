#include "astnodes.h"
#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "semanticanalyser.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

// //===----------------------------------------------------------------------===//
// // Code Generation
// //===----------------------------------------------------------------------===//

// static llvm::LLVMContext TheContext;
// static llvm::IRBuilder<> Builder(TheContext);
// static std::unique_ptr<llvm::Module> TheModule;

//===----------------------------------------------------------------------===//
// AST Printer
//===----------------------------------------------------------------------===//

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                                     const Expr &ast)
{
    os << ast.to_string();
    return os;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main(int argc, char **argv)
{

    //--------------------ARGUMENT PARSING--------------------//
    if (argc != 2)
    {
        std::cerr << "Usage: ./code InputFile" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file '" << argv[1] << std::endl;
        return 1;
    }

    //--------------------PRINTING LEXED PROGRAM--------------------//

    Lexer lex(file);

    Token cur = lex.GetToken();
    while (cur.type != EOF_TOK)
    {
        std::cout << "Token: " << cur.lexeme << " with type " << cur.type << std::endl;
        cur = lex.GetToken();
    }
    std::cout << "Lexing finished\n\n\n\n"
              << std::endl;

    //--------------------PRINTING PARSED PROGRAM--------------------//

    std::ifstream new_file(argv[1]);
    Parser q(new_file);

    std::vector<SP<Stmt>> parse = q.ParseProgram();

    std::cout << "Abstract Syntax Tree:\n\n"
              << std::endl;

    for (auto &decl : parse)
        std::cout << decl->to_string() << std::endl;

    std::cout << "Parsing finished\n\n\n\n"
              << std::endl;

    //--------------------SEMANTICALLY ANALYSING PROGRAM--------------------//
    std::cout << "Semantically analysing program\n\n"
              << std::endl;

    SemanticAnalyser sa;
    sa.AnalyseProgram(parse);

    std::cout << "Printing semantically analysed program\n\n"
              << std::endl;

    for (auto &decl : parse)
        std::cout << decl->to_string() << std::endl;

    std::cout << "\n\n"
              << std::endl;

    //--------------------CODE GENERATION--------------------//

    Compiler c;
    for (auto &decl : parse)
        decl->Compile(c);

    std::cout << "\n\n\nCode Gen:"
              << std::endl;
    c.PrintCode();
    c.WriteCode();
    return 0;
}
