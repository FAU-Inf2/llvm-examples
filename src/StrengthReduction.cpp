#include "StrengthReduction.hpp"
#include <llvm/IR/InstrTypes.h>
#include <llvm/Support/Casting.h>
using namespace llvm;
static bool isConst2(const BinaryOperator &op, int operandNum) {
  ConstantInt *ci = dyn_cast<ConstantInt>(op.getOperand(operandNum));
  return ci && ci->getSExtValue() == 2;
}
static int operandToShift(const BinaryOperator &op) {
  return isConst2(op, 0) ? 1 : isConst2(op, 1) ? 0 : -1;
}

PreservedAnalyses StrengthReductionPass::run(Function &F,
                                             FunctionAnalysisManager &fam) {
  bool modifiedFunc = false;
  errs() << "running on " << F.getName() << "\n";
  for (BasicBlock &bb : F) {
    for (auto it = bb.begin(); it != bb.end();) {
      Instruction &instr = *(it++);
      if (BinaryOperator *binOp = dyn_cast<BinaryOperator>(&instr)) {
        int toShift = operandToShift(*binOp);
        if (binOp->getOpcode() == BinaryOperator::Mul && toShift != -1) {
          BinaryOperator *shift = BinaryOperator::CreateShl(
              binOp->getOperand(toShift), ConstantInt::get(binOp->getType(), 1),
              "", binOp);
          binOp->replaceAllUsesWith(shift);
          binOp->eraseFromParent();
          modifiedFunc = true;
        }
      }
    }
  }
  return modifiedFunc ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

llvm::PassPluginLibraryInfo getCustomPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "strength-reduction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            // this one is needed for opt
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  FPM.addPass(StrengthReductionPass());
                  return true;
                });
            // this one is needed for clang
            PB.registerPipelineEarlySimplificationEPCallback(
                [&](ModulePassManager &MPM, auto, auto) {
                  MPM.addPass(createModuleToFunctionPassAdaptor(
                      StrengthReductionPass()));
                  return true;
                });
          }};
}
extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getCustomPluginInfo();
}
