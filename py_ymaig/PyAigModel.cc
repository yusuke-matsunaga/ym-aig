
/// @file PyAigModel.cc
/// @brief Python AigModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigModel.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct AigModelObject
{
  PyObject_HEAD
  AigModel* mPtr;
};

// Python 用のタイプ定義
PyTypeObject AigModelType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
AigModel_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'AigModel' is disabled");
  return nullptr;
}

// 終了関数
void
AigModel_dealloc(
  PyObject* self
)
{
  auto aig_obj = reinterpret_cast<AigModelObject*>(self);
  delete aig_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
AigModel_read_aag(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto aig_model = AigModel::read_aag(filename);
    auto obj = AigModelType.tp_alloc(&AigModelType, 0);
    auto aig_obj = reinterpret_cast<AigModelObject*>(obj);
    aig_obj->mPtr = new AigModel{std::move(aig_model)};
    return obj;
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_aag(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
AigModel_read_aig(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto aig_model = AigModel::read_aig(filename);
    auto obj = AigModelType.tp_alloc(&AigModelType, 0);
    auto aig_obj = reinterpret_cast<AigModelObject*>(obj);
    aig_obj->mPtr = new AigModel{std::move(aig_model)};
    return obj;
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_aig(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
AigModel_input(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos = 0;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  auto& aig = PyAigModel::Get(self);
  auto id = aig.input(pos);
  auto symbol = aig.input_symbol(pos);
  return Py_BuildValue("(ks)", id, symbol.c_str());
}

PyObject*
AigModel_latch(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos = 0;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  auto& aig = PyAigModel::Get(self);
  auto id = aig.latch(pos);
  auto src_id = aig.latch_src(pos);
  auto symbol = aig.latch_symbol(pos);
  return Py_BuildValue("(ksk)", id, symbol.c_str(), src_id);
}

PyObject*
AigModel_output(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos = 0;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  auto& aig = PyAigModel::Get(self);
  auto src_id = aig.output_src(pos);
  auto symbol = aig.output_symbol(pos);
  return Py_BuildValue("(sk)", symbol.c_str(), src_id);
}

PyObject*
AigModel_and(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos = 0;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  auto& aig = PyAigModel::Get(self);
  auto id = aig.and_node(pos);
  auto src1_id = aig.and_src1(pos);
  auto src2_id = aig.and_src2(pos);
  return Py_BuildValue("(kkk)", id, src1_id, src2_id);
}

PyObject*
AigModel_print(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "file",
    nullptr
  };
  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  auto& aig = PyAigModel::Get(self);
  if ( filename == nullptr ) {
    aig.print(cout);
  }
  else {
    ofstream fout{filename};
    if ( fout ) {
      aig.print(fout);
    }
    else {
      ostringstream buff;
      buff << filename << ": Could not open file";
      PyErr_SetString(PyExc_ValueError, buff.str().c_str());
      return nullptr;
    }
  }
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef AigModel_methods[] = {
  {"read_aag", reinterpret_cast<PyCFunction>(AigModel_read_aag),
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("read 'aag' file")},
  {"read_aig", reinterpret_cast<PyCFunction>(AigModel_read_aig),
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("read 'aig' file")},
  {"input", AigModel_input,
   METH_VARARGS,
   PyDoc_STR("return input's information")},
  {"latch", AigModel_latch,
   METH_VARARGS,
   PyDoc_STR("return latch's information")},
  {"output", AigModel_output,
   METH_VARARGS,
   PyDoc_STR("return output's information")},
  {"and_node", AigModel_and,
   METH_VARARGS,
   PyDoc_STR("return AND node's information")},
  {"print", reinterpret_cast<PyCFunction>(AigModel_print),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write contents")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AigModel_M(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& aig = PyAigModel::Get(self);
  int val = aig.M();
  return Py_BuildValue("i", val);
}

PyObject*
AigModel_I(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& aig = PyAigModel::Get(self);
  int val = aig.I();
  return Py_BuildValue("i", val);
}

PyObject*
AigModel_L(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& aig = PyAigModel::Get(self);
  int val = aig.L();
  return Py_BuildValue("i", val);
}

PyObject*
AigModel_O(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& aig = PyAigModel::Get(self);
  int val = aig.O();
  return Py_BuildValue("i", val);
}

PyObject*
AigModel_A(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& aig = PyAigModel::Get(self);
  int val = aig.A();
  return Py_BuildValue("i", val);
}

// getsetter 定義
PyGetSetDef AigModel_getsetters[] = {
  {"M", AigModel_M, nullptr, PyDoc_STR("total node num"), nullptr},
  {"I", AigModel_I, nullptr, PyDoc_STR("input num"), nullptr},
  {"L", AigModel_L, nullptr, PyDoc_STR("latch num"), nullptr},
  {"O", AigModel_O, nullptr, PyDoc_STR("output num"), nullptr},
  {"A", AigModel_A, nullptr, PyDoc_STR("AND gate num"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'AigModel' オブジェクトを使用可能にする．
bool
PyAigModel::init(
  PyObject* m
)
{
  AigModelType.tp_name = "AigModel";
  AigModelType.tp_basicsize = sizeof(AigModelObject);
  AigModelType.tp_itemsize = 0;
  AigModelType.tp_dealloc = AigModel_dealloc;
  AigModelType.tp_flags = Py_TPFLAGS_DEFAULT;
  AigModelType.tp_doc = PyDoc_STR("AigModel object");
  AigModelType.tp_methods = AigModel_methods;
  AigModelType.tp_getset = AigModel_getsetters;
  AigModelType.tp_new = AigModel_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "AigModel", &AigModelType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief AigModel を PyObject に変換する．
PyObject*
PyAigModel::ToPyObject(
  const AigModel& val
)
{
  auto obj = AigModelType.tp_alloc(&AigModelType, 0);
  auto aig_obj = reinterpret_cast<AigModelObject*>(obj);
  aig_obj->mPtr = new AigModel{val};
  return obj;
}

// @brief PyObject が AigModel タイプか調べる．
bool
PyAigModel::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief AigModel を表す PyObject から AigModel を取り出す．
const AigModel&
PyAigModel::Get(
  PyObject* obj
)
{
  auto aig_obj = reinterpret_cast<AigModelObject*>(obj);
  return *aig_obj->mPtr;
}

// @brief AigModel を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigModel::_typeobject()
{
  return &AigModelType;
}

END_NAMESPACE_YM
