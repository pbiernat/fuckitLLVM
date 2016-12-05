#include<llvm/IR/InstIterator.h>
#include<llvm/IR/InstVisitor.h>
#include<llvm/Support/raw_ostream.h>

using namespace llvm;

struct fuckit_exitcode_exit : public BasicBlockPass
{
	static char ID;
	int changed;

	fuckit_exitcode_exit() : BasicBlockPass(ID)
	{
		changed = 0;
	}

	virtual bool runOnBasicBlock(BasicBlock &BB)
	{
		CallInst* call;
		BasicBlock::iterator I;

		changed = 0;
		for (I = BB.begin(); I != BB.end(); ++I) {
			if ((call = dyn_cast<CallInst>(&*I))
					&& call->getCalledFunction()
					&& call->getCalledFunction()->getName() == "exit") {
				errs() << "Setting argument of \"" << *call << "\" to be 0\n";
				call->setOperand(0,
						ConstantInt::get(Type::getInt32Ty(BB.getContext()), 0));
				changed = 1;
			}
		}

		return changed;
	}
};

struct fuckit_exitcode_return : public ModulePass
{
	static char ID;
	fuckit_exitcode_return() : ModulePass(ID) {}

	virtual bool runOnModule(Module &M)
	{
		inst_iterator I;
		ReturnInst* ret;

		Function *main = M.getFunction("main");
		for (I = inst_begin(main); I != inst_end(main); I++) {
			if ((ret = dyn_cast<ReturnInst>(&*I))) {
				errs() << "Setting value of \"" << *ret << "\" to be 0\n";
				/* Assume return codes are 32 bit */
				/* This is true on every platform I know about */
				ret->setOperand(0,
						ConstantInt::get(Type::getInt32Ty(M.getContext()), 0));
			}
		}

		return true;
	}
};

char fuckit_exitcode_exit::ID = 0;
char fuckit_exitcode_return::ID = 0;
static RegisterPass<fuckit_exitcode_exit>
		fuckit_exitcode_exit("fuckit-exitcode-exit",
		"Improve reliability by always exiting 0",
		false,
		false);
static RegisterPass<fuckit_exitcode_return>
		fuckit_exitcode_return("fuckit-exitcode-return",
		"Improve reliability by always returning 0",
		false,
		false);
