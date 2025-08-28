#include "StrengthReduction.cpp"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/SourceMgr.h>
int main(int argc, char **argv) {
  using namespace llvm;
  if (argc < 2) { /* error handling */ }
  SMDiagnostic diag;
  LLVMContext context;
  std ::unique_ptr<Module> module = parseIRFile(argv[1], diag, context);
  if (!module) {
    diag.print(argv[0], errs());
    return 1;
  }
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;
  PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
  ModulePassManager MPM = PB.buildO0DefaultPipeline(OptimizationLevel::O0);
  MPM.addPass(createModuleToFunctionPassAdaptor(StrengthReductionPass()));
  MPM.run(*module, MAM);
  errs() << *module << "\n";
}
