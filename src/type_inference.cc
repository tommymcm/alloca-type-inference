#include <optional>

#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

#include <llvm/Support/raw_ostream.h>

#include "type_inference.hh"

using namespace llvm;

llvm::PreservedAnalyses TypeInferencePass::run(
    llvm::Function &F,
    llvm::FunctionAnalysisManager &FAM) {

  // Track modifications.
  bool modified = false;

  // Get the LLVM Module and DataLayout.
  auto *module = F.getParent();
  auto &data_layout = module->getDataLayout();

  // For each alloca instruction in the function.
  for (auto &BB : F) {
    for (auto &I : BB) {

      auto *alloca = dyn_cast<AllocaInst>(&I);

      // Skip non-alloca instructions.
      if (not alloca) {
        continue;
      }

      llvm::errs() << I << "\n";

      // Get the allocation size.
      auto maybe_size = alloca->getAllocationSize(data_layout);

      // If the allocation doesnt have a known size, skip it.
      if (not maybe_size.has_value()) {
        continue;
      }

      auto size = *maybe_size;

      llvm::errs() << "  alloca (size = " << std::to_string(size) << ")\n";

      // For each use of the alloca:
      Type *inferred_type = NULL;
      for (auto &use : alloca->uses()) {

        // Unpack the use.
        auto *user = use.getUser();

        llvm::errs() << "  user: " << *user << "\n";

        if (auto *store = dyn_cast<StoreInst>(user)) {
          auto *stored_type = store->getPointerOperandType();
          auto store_size = data_layout.getTypeStoreSize(stored_type);

          llvm::errs() << "  store of " << *stored_type
                       << " (size = " << std::to_string(store_size) << ")\n";

          // Check for type conflicts.
          if (inferred_type and stored_type != inferred_type) {
            llvm::errs() << "  type conflict!\n";
            inferred_type = NULL;
            break;
          }

          // If the store size matches the alloca size, set the inferred type.
          if (store_size == size) {
            inferred_type = stored_type;
          }
          
        } else if (auto *load = dyn_cast<LoadInst>(user)) {
          auto *loaded_type = load->getPointerOperandType();
          auto load_size = data_layout.getTypeStoreSize(loaded_type);

          llvm::errs() << "  load of " << *loaded_type
                       << " (size = " << std::to_string(load_size) << ")\n";

          // Check for type conflicts.
          if (inferred_type and loaded_type != inferred_type) {
            llvm::errs() << "  type conflict!\n";
            inferred_type = NULL;
            break;
          }

          // If the store size matches the alloca size, set the inferred type.
          if (load_size == size) {
            inferred_type = loaded_type;
          }
        }
      }

      // If we could not infer a type for this alloca, skip it.
      if (not inferred_type) {
        continue;
      }

      // If we were able to infer a type for this alloca, change its type to it.
      alloca->setAllocatedType(inferred_type);

      // Mark the function as modified.
      modified = true;
    }
  }

  return modified ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all();
}
