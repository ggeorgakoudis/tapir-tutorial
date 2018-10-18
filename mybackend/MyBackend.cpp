#include "llvm/Transforms/Tapir/MyBackend.h"

using namespace llvm;

llvm::MyBackend::MyBackend(){}

void llvm::MyBackend::preProcessFunction(Function &F) {}
void llvm::MyBackend::postProcessFunction(Function &F) {}
void llvm::MyBackend::postProcessHelper(Function &F) {}
bool llvm::MyBackend::processMain(Function &F) {
  return false;
}
bool llvm::MyBackend::processLoop(LoopSpawningHints LSH, LoopInfo &LI, ScalarEvolution &SE, DominatorTree &DT,
               AssumptionCache &AC, OptimizationRemarkEmitter &ORE) {
  return false;
}

//Process instruction statement into runtime calls
Function *llvm::MyBackend::createDetach(DetachInst &detach,
                                  ValueToValueMapTy &DetachCtxToStackFrame,
                                  DominatorTree &DT, AssumptionCache &AC) {
  auto VoidTy = Type::getVoidTy(detach.getContext());
  auto Int8Ty = Type::getInt8Ty(detach.getContext());
  auto Int8PtrTy = PointerType::getUnqual(Int8Ty);
  auto M = detach.getParent()->getParent()->getParent();
  BasicBlock *detB = detach.getParent();
  BasicBlock *Spawned  = detach.getDetached();
  BasicBlock *Continue = detach.getContinue();


  CallInst *cal = nullptr;
  Function *extracted = extractDetachBodyToFunction(detach, DT, AC, &cal, ".bnd");
  assert(extracted && "could not extract detach body to function");

  // Unlink the detached CFG in the original function.  The heavy lifting of
  // removing the outlined detached-CFG is left to subsequent DCE.

  // Replace the detach with a branch to the continuation.
  BranchInst *ContinueBr = BranchInst::Create(Continue);
  ReplaceInstWithInst(&detach, ContinueBr);

  // Rewrite phis in the detached block.
  {
    BasicBlock::iterator BI = Spawned->begin();
    while (PHINode *P = dyn_cast<PHINode>(BI)) {
      P->removeIncomingValue(detB);
      ++BI;
    }
  }

  IRBuilder<> builder(cal);
  std::vector<Value *> Args = {builder.CreatePointerCast(extracted, Int8PtrTy)};
  for(unsigned i=0; i<cal->getNumArgOperands(); i++) {
    Args.push_back(cal->getArgOperand(i));
  }
  Type *TypeParams[] = {Int8PtrTy};
  FunctionType *FnTy = FunctionType::get(VoidTy, TypeParams, /*isVarArg*/true);
  CallInst *runtimecall = CallInst::Create(M->getOrInsertFunction("mybackend_detach", FnTy), Args);

  ReplaceInstWithInst(cal, runtimecall);

  return extracted;
}

//Process sync instruction into runtime calls
void llvm::MyBackend::createSync(SyncInst &SI, ValueToValueMapTy &DetachCtxToStackFrame) {
  auto M = SI.getParent()->getParent()->getParent();
  auto VoidTy = Type::getVoidTy(SI.getContext());
  auto Int8Ty = Type::getInt8Ty(SI.getContext());
  auto Int8PtrTy = PointerType::getUnqual(Int8Ty);
  IRBuilder<> builder(&SI);
  std::vector<Value *> Args = {builder.CreatePointerCast(SI.getParent()->getParent(), Int8PtrTy)};
  Type *TypeParams[] = {Int8PtrTy};
  FunctionType *FnTy = FunctionType::get(VoidTy, TypeParams, /*isVarArg*/false);
  CallInst *call = builder.CreateCall(M->getOrInsertFunction("mybackend_sync", FnTy), Args);

  // Replace the detach with a branch to the continuation.
  BranchInst *PostSync = BranchInst::Create(SI.getSuccessor(0));
  ReplaceInstWithInst(&SI, PostSync);
}

//Get number of workers * 8
Value* llvm::MyBackend::GetOrCreateWorker8(Function &F) {
  auto M = F.getParent();
  auto Int32Ty = Type::getInt32Ty(F.getContext());
  IRBuilder<> builder(F.getEntryBlock().getFirstNonPHIOrDbgOrLifetime());
  std::vector<Value *> Args = {};
  FunctionType *FnTy = FunctionType::get(Int32Ty, /*isVarArg*/false);
  CallInst *call = builder.CreateCall(M->getOrInsertFunction("get_num_workers", FnTy), Args);
  Value *P8 = builder.CreateMul(call, ConstantInt::get(Int32Ty, 8));
  return P8;
}