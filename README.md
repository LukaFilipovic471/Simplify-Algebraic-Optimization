# LLVM Algebraic Optimization Passes

These LLVM passes implement several algebraic optimizations to enhance performance by simplifying mathematical expressions during compilation. The following optimizations are included:

1. **AddMulPass**: This optimization simplify expressions involving addition and multiplication by eliminating redundant operations. For example:
   - x + 0 = x
   - x * 1 = x

2. **CommonSubexprPass**: This pass identify and factorize common subexpressions to reduce redundancy in calculations. For example:
   - x * y * z + x * a * z = x * z * (y + a)

3. **PowerOptimizationPass**: These optimization transform exponentiation into multiplication for specific cases, improving performance. For example:
   - x^4 = x * x * x * x

## Preparing the Environment

To get started, download and set up the LLVM repository using the following commands:

wget "http://www.prevodioci.matf.bg.ac.rs/kk/2023/vezbe/llvm-project.zip"
unzip -d llvm-project llvm-project.zip

Inside the extracted directory, you will find a script named `make_llvm.sh` to build the project. Follow these steps:

1. Make the script executable:
   chmod +x make_llvm.sh

2. Run the script to build LLVM:
   ./make_llvm.sh

This will compile the LLVM project.

## Adding the Pass You Want to Test

1. Clone the repository.
2. Move the entire directory of the pass you want to test to the `llvmproject/llvm/lib/Transforms/` directory.

3. Update the `llvmproject/llvm/lib/Transforms/CMakeLists.txt` and add the following line:
   add_subdirectory(PassYouWantToTest)

## Running the Optimization

To compile and run your code with these optimizations:

1. Place the files from the `Test` directory in the `llvmproject/build/` directory.

2. From the `llvmproject/build/` directory, execute the following commands as an example:
   ./bin/clang -S -emit-llvm test.c
   ./bin/opt -S -load lib/PassYpuWantToTest.so -enable-new-pm=0 -pass-test test.ll -o output.ll

3. The optimized code will be available in `output.ll`.

## Members:

- Luka Filipović - [@LukaFilipovic471](https://github.com/LukaFilipovic471)

- Sara Kalinić - [@sarakalinic24](https://github.com/sarakalinic24)
