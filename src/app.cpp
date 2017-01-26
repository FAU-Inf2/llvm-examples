#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"

#include "lib/instruction_counter.h"

using namespace llvm;

int main(int argc, char **argv) {
  if (argc < 2) {
    errs() << "USAGE: " << argv[0] << " <input file>\n";
    return 1;
  }

  SMDiagnostic diag;
  LLVMContext context;

  std::unique_ptr<Module> module(parseIRFile(argv[1], diag, context));

  if (!module) {
    diag.print(argv[0], errs());
    return 1;
  }

  legacy::PassManager passManager;
  passManager.add(new InstructionCounter());
  passManager.run(*module);

  return 0;
}
