
/// @file ymaig_module.cc
/// @brief Python 用の aig モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyAigModel.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymaig_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymaig_module = {
  PyModuleDef_HEAD_INIT,
  "ymaig",
  PyDoc_STR("ymaig: Extension module for aig"),
  -1,
  ymaig_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymaig()
{
  auto m = PyModule::init(&ymaig_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyAigModel::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
