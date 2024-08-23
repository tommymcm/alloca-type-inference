#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

#include <llvm/Support/CommandLine.h>

#include "type_inference.hh"

// Register the passes and pipelines with the new pass manager
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return { LLVM_PLUGIN_API_VERSION,
           "alloca-type-inference",
           LLVM_VERSION_STRING,
           [](llvm::PassBuilder &PB) {
             // Register transforation passes.
             PB.registerPipelineParsingCallback(
                 [](llvm::StringRef name,
                    llvm::FunctionPassManager &FPM,
                    llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                   // LowerFold require some addition simplification to be run
                   // after it so it doesn't break other passes in the pipeline.
                   if (name == "alloca-type-infer") {
                     FPM.addPass(TypeInferencePass());
                     return true;
                   }

                   return false;
                 });
           } };
}
