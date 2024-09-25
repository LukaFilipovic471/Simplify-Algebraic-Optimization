#include <llvm/Pass.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace {

    class CommonSubexprPass final : public FunctionPass {
    public:
        static char ID;

        CommonSubexprPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            cout << "function\n";
            bool changed = false;

            for (BasicBlock &B : F) {
                for (auto DI = B.begin(); DI != B.end();) {
                    Instruction &I = *DI++;
                    
                    
                    if (I.getOpcode() == Instruction::Add) {
                        std::cout << "add\n";
                        Value *Op1 = I.getOperand(0);
                        Value *Op2 = I.getOperand(1);

                        
                        if (auto *Mul1 = dyn_cast<Instruction>(Op1)) {
                            std::cout << "op1\n";
                            if (Mul1->getOpcode() == Instruction::FMul || Mul1->getOpcode() == Instruction::Mul) {
                                std::cout << "mul1\n";
                                if (auto *Mul2 = dyn_cast<Instruction>(Op2)) {
                                    cout << "op2\n";
                                    if (Mul2->getOpcode() == Instruction::FMul || Mul2->getOpcode() == Instruction::Mul) {
                                        std::cout << "mul2\n";
                                        Value *X1 = nullptr;
                                        Value *Z1 = nullptr;
                                        Value *Y = nullptr, *A = nullptr;
                                        
                                        if(Mul1->getOperand(0)==nullptr){
                                            std::cout<<"nullptr\n";
                                        }

                                        
                                        if (areEquivalent(Mul1->getOperand(0), Mul2->getOperand(0))) {
                                            std::cout << "eqX\n";
                                            X1 = Mul1->getOperand(0);
                                            if (areEquivalent(Mul1->getOperand(2), Mul2->getOperand(2))) {
                                                std::cout << "eqZ\n";
                                                Z1 = Mul1->getOperand(2);
                                                Y = Mul1->getOperand(1);
                                                A = Mul2->getOperand(1);
                                                std::cout << X1->getValue() << " " << Z1->getValue() << " " << Y->getValue() << " " << A->getValue() << "\n";
                                            }
                                        }

                                        
                                        if (X1 && Z1 && Y && A) {
                                            std::cout << "builder\n";
                                            
                                            IRBuilder<> Builder(&I);
                                            Value *YAddA = Builder.CreateAdd(Y, A);
                                            Value *NewMul = Builder.CreateMul(X1, YAddA);
                                            Value *Optimized = Builder.CreateMul(NewMul, Z1);

                                            
                                            I.replaceAllUsesWith(Optimized);
                                            I.eraseFromParent(); 
                                            changed = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return changed;
        }
        
        bool areEquivalent(Value *V1, Value *V2) {
    	    
    	    if (V1 == V2) return true;


            if (!V1 || !V2) return false;


    	    if (auto *C1 = dyn_cast<Constant>(V1)) {
                if (auto *C2 = dyn_cast<Constant>(V2)) {
                    return C1->getType() == C2->getType() && C1->getUniqueInteger() == C2->getUniqueInteger();
                }
                return false;
            }


    	    if (auto *I1 = dyn_cast<Instruction>(V1)) {
        	if (auto *I2 = dyn_cast<Instruction>(V2)) {

            	    if (I1->getOpcode() != I2->getOpcode()) return false;


            	    if (I1->getNumOperands() != I2->getNumOperands()) return false;


            	    for (unsigned i = 0, e = I1->getNumOperands(); i != e; ++i) {
                        if (!areEquivalent(I1->getOperand(i), I2->getOperand(i))) {
                            return false;
                        }
                    }

                    return true;
                }
                return false;
            }


    	    if (isa<Argument>(V1) && isa<Argument>(V2)) {
                return V1 == V2;
    	    }

    	    if (isa<GlobalValue>(V1) && isa<GlobalValue>(V2)) {
                return V1 == V2; 
    	    }


    	    return false;
    	}
    };

    char CommonSubexprPass::ID = 0;
    static RegisterPass<CommonSubexprPass> X("common-subexpr-pass", "Common Subexpression Pass", false, false);

} 

