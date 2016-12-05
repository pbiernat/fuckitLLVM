#include<llvm/IR/IRBuilder.h>
#include<llvm/IR/InstVisitor.h>

using namespace llvm;

struct fuckit_stderrcode_stderr : public ModulePass
{
	static char ID;
	fuckit_stderrcode_stderr() : ModulePass(ID) {}

	virtual bool runOnModule(Module &M)
	{
		Function *main = M.getFunction("main");
        Constant *sigFunc = main->getParent()->getOrInsertFunction(
                "fuckit_sanitize_stderr",
                Type::getVoidTy(main->getContext()),
                NULL);
        IRBuilder<> builder(main->getEntryBlock().getFirstNonPHI());
        builder.CreateCall(sigFunc);

		return true;
	}
};

char fuckit_stderrcode_stderr::ID = 0;
static RegisterPass<fuckit_stderrcode_stderr>
		fuckit_stderrcode_stderr("fuckit-stderr",
		"Improve reliability by discarding stderr",
		false,
		false);
