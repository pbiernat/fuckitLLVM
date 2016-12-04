#include<llvm/IR/InstVisitor.h>
#include<llvm/Support/raw_ostream.h>

using namespace llvm;

struct fuckit : public ModulePass
{
	static char ID;
	fuckit() : ModulePass(ID) {}

	virtual bool
	runOnModule(Module &M)
	{
		errs() << "Pass started\n";
		return true;
	}
};

char fuckit::ID = 0;
static RegisterPass<fuckit> cats("fuckit", "Improve reliability", false, false);
