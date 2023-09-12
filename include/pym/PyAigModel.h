#ifndef PYAIGMODEL_H
#define PYAIGMODEL_H

/// @file PyAigModel.h
/// @brief PyAigModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/AigModel.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyAigModel PyAigModel.h "PyAigModel.h"
/// @brief Python 用の AigModel 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyAigModel
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief AigModel を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AigModel& val ///< [in] 値
  );

  /// @brief PyObject が AigModel タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief AigModel を表す PyObject から AigModel を取り出す．
  /// @return AigModel を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  const AigModel&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief AigModel を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYAIGMODEL_H
