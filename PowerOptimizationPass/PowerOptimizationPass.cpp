
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
    struct PowerOptimizationPass : public FunctionPass {
        static char ID;
        PowerOptimizationPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            bool changed = false;
            for (BasicBlock &B : F) {
                for (BasicBlock::iterator DI = B.begin(); DI != B.end();) {
                    Instruction &I = *DI++;
                    
                    if (auto *Call = dyn_cast<CallInst>(&I)) {
                        Function *Callee = Call->getCalledFunction();
                        if (Callee && Callee->getName() == "pow") {
                            if (auto *Exp = dyn_cast<ConstantFP>(Call->getArgOperand(1))) {
                                double expValue = Exp->getValueAPF().convertToDouble();
                                if (expValue >= 2.0 && expValue <= 5.0) {
                                    IRBuilder<> Builder(Call);
                                    Value *Base = Call->getArgOperand(0);
                                    Value *Result = Base;

                                    for (int i = 1; i < expValue; ++i) {
                                        Result = Builder.CreateFMul(Result, Base, "pow_opt");
                                    }
                                    Call->replaceAllUsesWith(Result);
                                    Call->eraseFromParent();
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
            return changed;
        }
    };
}

char PowerOptimizationPass::ID = 0;
static RegisterPass<PowerOptimizationPass> X("power-opt-pass", "Power Optimization Pass", false, false);
