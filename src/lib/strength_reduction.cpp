#include "lib/strength_reduction.h"

#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

char StrengthReduction::ID = 0;

static int isConst2(const BinaryOperator *binaryOperator, const int operand) {
  ConstantInt *constant;
  return ((constant = dyn_cast<ConstantInt>(binaryOperator->getOperand(operand))) &&
          (constant->getZExtValue() == 2));
}

static int operandToShift(const BinaryOperator *binaryOperator) {
  assert (binaryOperator->getOpcode() == Instruction::Mul);

  if (isConst2(binaryOperator, 0)) {
    return 1;
  }

  if (isConst2(binaryOperator, 1)) {
    return 0;
  }

  return -1;
}

bool StrengthReduction::runOnFunction(Function &function) {
  bool modified = false;

  for (BasicBlock &basicBlock : function) {
    for (auto itInsn = basicBlock.begin(); itInsn != basicBlock.end(); ) {
      Instruction *instruction = &*itInsn++;
      BinaryOperator *binaryOperator;

      if ((binaryOperator = dyn_cast<BinaryOperator>(instruction)) &&
          (binaryOperator->getOpcode() == Instruction::Mul)) {

        int _operandToShift = operandToShift(binaryOperator);
        if (_operandToShift != -1) {
          BinaryOperator *shl = BinaryOperator::CreateShl(
            binaryOperator->getOperand(_operandToShift),
            ConstantInt::getSigned(binaryOperator->getType(), 1),
            "", binaryOperator);
          shl->takeName(binaryOperator);
          binaryOperator->replaceAllUsesWith(shl);
          binaryOperator->eraseFromParent();

          modified = true;
        }
      }
    }
  }

  return modified;
}

void StrengthReduction::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
}

static RegisterPass<StrengthReduction>
  strengthReduction("strength-reduction", "performs a very simple strength reduction");
