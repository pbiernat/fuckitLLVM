#include <llvm/IR/InstVisitor.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

struct sigpass : public FunctionPass
{
	static char ID;
	sigpass() : FunctionPass(ID) {}

    int insert_sighandlers(Function * F) {
        // Get the Context
        LLVMContext& ctx = F->getContext();
        Constant * sigFunc = F->getParent()->getOrInsertFunction(
                                        "register_sighandlers", Type::getVoidTy(ctx),
                                        NULL);
       
        IRBuilder<> builder(F->getEntryBlock().getFirstNonPHI());
        builder.CreateCall(sigFunc);

        errs() << "Created Builder!\n";
        return 0;
    }

    int insert_callmarkers(Function * F) {

        for (Function::iterator bb=F->begin(), bb_end=F->end(); bb != bb_end; ++bb) {
            for (BasicBlock::iterator inst=bb->begin(), end=bb->end(); inst != end; ++inst) {
                if (isa<CallInst>(*inst)) {
                    errs() << "Found Call Instruction: Placing marker...\n";
                    BasicBlock::iterator itt(*inst);
                    itt++;
                    IRBuilder<> builder(&(*itt));

                    Constant* beginConstAddress = ConstantInt::get(Type::getInt64Ty(F->getContext()), (int64_t)0x41414141);
                    Value* ConstPtr = ConstantExpr::getIntToPtr(
                    beginConstAddress , PointerType::getUnqual(Type::getInt64Ty(F->getContext()))); 

                    Value * lol = builder.CreateLoad(ConstPtr);
                    builder.CreateStore(lol, ConstPtr);
                }
            }
        }
        return 0;
    }

	virtual bool
	runOnFunction(Function &F)
	{

        insert_callmarkers(&F);
        
        if (F.getName() == "main") {
            errs() << "Identified Main\n";
            insert_sighandlers(&F);
        }
		return true;
	}
};

char sigpass::ID = 0;
static RegisterPass<sigpass> cats("sigpass", "Improve reliability", false, false);
