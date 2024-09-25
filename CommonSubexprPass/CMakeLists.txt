add_llvm_library( CommonSubexprPass MODULE BUILDTREE_ONLY
  CommonSubexprPass.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )

