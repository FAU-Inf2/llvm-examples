#include "instruction_counter.h"

char InstructionCounter::ID = 0;

bool InstructionCounter::runOnFunction(Function &function) {
  this->counter.clear();

  for (auto basicBlock = function.begin(); basicBlock != function.end(); ++basicBlock) {
    for (auto instruction = basicBlock->begin(); instruction != basicBlock->end(); ++instruction) {
      this->counter[instruction->getOpcodeName()] += 1;
    }
  }

  return false;
}

void InstructionCounter::print(llvm::raw_ostream &stream, const Module *module) const {
  for (auto iterator = this->counter.begin(); iterator != this->counter.end(); ++iterator) {
    stream << iterator->first << ": " << iterator->second << "\n";
  }
}

static bool MODIFIES_CFG = false;
static bool ANALYSIS_ONLY = true;

static RegisterPass<InstructionCounter>
  instructionCounter("instruction-counter", "counts how often each instruction is used",
                     MODIFIES_CFG, ANALYSIS_ONLY);
