add_llvm_library( AddMulPass MODULE BUILDTREE_ONLY
  AddMulPass.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )

