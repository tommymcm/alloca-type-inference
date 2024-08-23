#include "llvm/Pass.h"

#include "llvm/IR/PassManager.h"

class TypeInferencePass : public llvm::PassInfoMixin<TypeInferencePass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
};
