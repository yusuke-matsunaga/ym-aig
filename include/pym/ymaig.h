#ifndef PYM_YMAIG_H
#define PYM_YMAIG_H

/// @file ymaig.h
/// @brief ymaig の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymaig モジュールの初期化関数
extern "C" PyObject* PyInit_ymaig();

END_NAMESPACE_YM

#endif // PYM_YMAIG_H
