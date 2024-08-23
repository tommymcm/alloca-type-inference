# Alloca Type Inference
I have realized a few times that some compiler front-ends and middle-end passes will use byte-array types for their alloca instructions.

This seems alright, but it blocks a good amount of middle-end optimizations from occuring because of type mismatches at memory operations.

This pass fixes those.
