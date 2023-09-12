
/// @file py_ymaig.cc
/// @brief py_ymaig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "pym/ymaig.h"


BEGIN_NAMESPACE_YM

static struct _inittab init_table[] = {
  {"ymaig", PyInit_ymaig},
  {nullptr, nullptr}
};

void
ymaig_init()
{
  PyImport_ExtendInittab(init_table);
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::ymaig_init();

  return Py_BytesMain(argc, argv);
}
