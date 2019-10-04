set(LLVM_LINK_COMPONENTS
  Core
  ExecutionEngine
  Interpreter
  MC
  Support
  nativecodegen
  )

add_llvm_example(ParallelJIT
  ParallelJIT.cpp
  )

<<<<<<< HEAD
target_link_libraries(ParallelJIT PRIVATE ${LLVM_PTHREAD_LIB})
=======
target_link_libraries(ParallelJIT ${LLVM_PTHREAD_LIB})
>>>>>>> origin/release/4.x
