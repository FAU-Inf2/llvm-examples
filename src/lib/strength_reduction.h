#ifndef __INCLUDE_STRENGTH_REDUCTION
#define __INCLUDE_STRENGTH_REDUCTION

#include "llvm/Pass.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <map>

using namespace llvm;

struct StrengthReduction : public FunctionPass {

  static char ID;

  StrengthReduction() : FunctionPass(ID) {}

  bool runOnFunction(Function &function) override;
  void getAnalysisUsage(AnalysisUsage &Info) const override;

};

#endif
