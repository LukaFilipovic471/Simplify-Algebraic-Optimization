#include <llvm/Pass.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/Constants.h>

using namespace llvm;

namespace {

    class AddMulPass final : public FunctionPass {
    public:
        static char ID;

        AddMulPass() : FunctionPass(ID) {}

        template<class DataType>
        static bool handleAddInstruction(BasicBlock &B, Instruction &I) {
            Value *IdentityValue = nullptr;
            bool changed = false;
            if (auto *CI1 = dyn_cast<DataType>(I.getOperand(0))) {
                if (CI1->isZero()) {
                    IdentityValue = I.getOperand(1);
                }
            } else if (auto *CI2 = dyn_cast<DataType>(I.getOperand(1))) {
                if (CI2->isZero()) {
                    IdentityValue = I.getOperand(0);
                }
            }
            if (IdentityValue) {
                I.replaceAllUsesWith(IdentityValue);
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleIntMulInstruction(BasicBlock &B, Instruction &I) {
            Value *IdentityValue = nullptr;
            bool changed = false;
            auto *CI1 = dyn_cast<ConstantInt>(I.getOperand(0));
            auto *CI2 = dyn_cast<ConstantInt>(I.getOperand(1));
            if (CI1) {
                if (CI1->isOne()) {
                    IdentityValue = I.getOperand(1);
                }
            } else if (CI2) {
                if (CI2->isOne()) {
                    IdentityValue = I.getOperand(0);
                }
            }
            if (CI1) {
                if (CI1->isZero()) {
                    IdentityValue = ConstantInt::get(dyn_cast<IntegerType>(CI1->getType()), 0);
                }
            } else if (CI2) {
                if (CI2->isZero()) {
                    IdentityValue = ConstantInt::get(dyn_cast<IntegerType>(CI2->getType()), 0);
                }
            }
            if (IdentityValue) {
                I.replaceAllUsesWith(IdentityValue);
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleFMulInstruction(BasicBlock &B, Instruction &I) {
            Value *IdentityValue = nullptr;
            bool changed = false;

            auto *CI1 = dyn_cast<ConstantFP>(I.getOperand(0));
            auto *CI2 = dyn_cast<ConstantFP>(I.getOperand(1));
            if (CI1) {
                if (CI1->isOneValue()) {
                    IdentityValue = I.getOperand(1);
                }
            } else if (CI2) {
                if (CI2->isOneValue()) {
                    IdentityValue = I.getOperand(0);
                }
            }
            if (CI1) {
                if (CI1->isZero()) {
                    IdentityValue = ConstantInt::get(dyn_cast<IntegerType>(I.getType()), 0);
                }
            } else if (CI2) {
                if (CI2->isZero()) {
                    IdentityValue = ConstantInt::get(dyn_cast<IntegerType>(I.getType()), 0);
                }
            }
            if (IdentityValue) {
                I.replaceAllUsesWith(IdentityValue);
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        bool runOnFunction(Function &F) override {
            bool changed = false;
            for (BasicBlock &B: F) {
                for (BasicBlock::iterator DI = B.begin(); DI != B.end();) {
                    Instruction &I = *DI;
                    ++DI;
                    if (I.getOpcode() == Instruction::Add) {
                        changed |= handleAddInstruction<ConstantInt>(B, I);
                    } else if (I.getOpcode() == Instruction::FAdd) {
                        changed |= handleAddInstruction<ConstantFP>(B, I);
                    } else if (I.getOpcode() == Instruction::Mul) {
                        changed |= handleIntMulInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::FMul) {
                        changed |= handleFMulInstruction(B, I);
                    }
                }
            }
            return changed;
        }
    }; // class AddMulPass

    char AddMulPass::ID = 0;
    RegisterPass<AddMulPass> X("add-mul-pass", "Add Mul Pass", false, false);

}
