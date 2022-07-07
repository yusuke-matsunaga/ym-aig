#ifndef AIGHANDLE_H
#define AIGHANDLE_H

/// @file AigHandle.h
/// @brief AigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigNode;

//////////////////////////////////////////////////////////////////////
/// @class AigHandle AigHandle.h "AigHandle.h"
/// @brief AigNode へのポインタと極性を組み合わせたもの
//////////////////////////////////////////////////////////////////////
class AigHandle
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 定数0となる．
  AigHandle() : mBody{0UL}
  {
  }

  /// @brief ノードと極性を指定したコンストラクタ
  AigHandle(
    AigNode* node, ///< [in] ノード
    bool inv       ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv)}
  {
  }

  /// @brief 定数0のハンドルを返す．
  static
  AigHandle
  make_zero()
  {
    return AigHandle{nullptr, false};
  }

  /// @brief 定数1のハンドルを返す．
  static
  AigHandle
  make_one()
  {
    return AigHandle{nullptr, true};
  }

  /// @brief デストラクタ
  ~AigHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数ハンドルの時 true を返す．
  bool
  is_const() const
  {
    return node() == nullptr;
  }

  /// @brief 定数0ハンドルの時 true を返す．
  bool
  is_zero() const
  {
    return mBody == 0UL;
  }

  /// @brief 定数1ハンドルの時 true を返す．
  bool
  is_one() const
  {
    return mBody == 1UL;
  }

  /// @brief ノードを返す．
  AigNode*
  node() const
  {
    return reinterpret_cast<AigNode*>(mBody & ~1UL);
  }

  /// @brief 極性を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1);
  }

  /// @brief 極性を反転したハンドルを返す．
  AigHandle
  operator~() const
  {
    return AigHandle{mBody ^ 1UL};
  }

  /// @brief 自身を表すリテラルを返す．
  SizeType
  lit() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  AigHandle(
    ympuint body
  ) : mBody{body}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  ympuint mBody;

};

END_NAMESPACE_YM_AIG

#endif // AIGHANDLE_H
