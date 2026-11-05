#include <llvm/Passes/PassBuilder.h>
#include <llvm/Plugins/PassPlugin.h>
struct StrengthReductionPass
    : public llvm::PassInfoMixin<StrengthReductionPass> {
  // this is the function that will be invoked for each defined function of the
  // module
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &fam);
};
