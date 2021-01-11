#ifndef __INCLUDE_INSTRUCTION_COUNTER
#define __INCLUDE_INSTRUCTION_COUNTER

#include "llvm/Pass.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <map>

using namespace llvm;

struct InstructionCounter : public FunctionPass {

  static char ID;
  std::map<std::string, int> counter;

  InstructionCounter() : FunctionPass(ID) {}

  bool runOnFunction(Function &function) override;
  void print(llvm::raw_ostream &stream, const Module *module) const override;
  void getAnalysisUsage(AnalysisUsage &Info) const override;

};

#endif
