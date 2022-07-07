#ifndef AIGNODE_H
#define AIGNODE_H

/// @file AigNode.h
/// @brief AigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"
#include "AigHandle.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNode AigNode.h "AigNode.h"
/// @brief Aig のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class AigNode
{
public:

  /// @brief 入力ノード用のコンストラクタ
  AigNode(
    SizeType id ///< [in] ID番号
  ) : mId{id}
  {
  }

  /// @brief ANDノード用のコンストラクタ
  AigNode(
    SizeType id,    ///< [in] ID番号
    AigHandle src1, ///< [in] 入力1
    AigHandle src2  ///< [in] 入力2
  ) : mId{id},
      mSrc1{src1},
      mSrc2{src2}
  {
  }

  /// @brief デストラクタ
  ~AigNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const
  {
    return mSrc1.is_const() && mSrc2.is_const();
  }

  /// @brief ANDノードの時 true を返す．
  bool
  is_and() const
  {
    return !is_input();
  }

  /// @brief 入力1を得る．
  AigHandle
  src1() const
  {
    return mSrc1;
  }

  /// @brief 入力2を得る．
  AigHandle
  src2() const
  {
    return mSrc2;
  }

  /// @brief 自身を表すリテラルを返す．
  SizeType
  lit() const
  {
    return id() * 2;
  }

  /// @brief ID番号を設定する．
  void
  set_id(
    SizeType id
  )
  {
    mId = id;
  }

  /// @brief 入力を設定する．
  void
  set_src(
    AigHandle src1,
    AigHandle src2
  )
  {
    mSrc1 = src1;
    mSrc2 = src2;
  }

  /// @brief ファンインを整列させる．
  void
  arrange_fanin()
  {
    if ( mSrc1.node() == nullptr ) {
      if ( mSrc2.node() != nullptr ) {
	swap(mSrc1, mSrc2);
      }
    }
    else if ( mSrc2.node() != nullptr ) {
      if ( mSrc1.node()->id() < mSrc2.node()->id() ) {
	swap(mSrc1, mSrc2);
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // 入力1
  AigHandle mSrc1;

  // 入力2
  AigHandle mSrc2;

};

// @brief 自身を表すリテラルを返す．
inline
SizeType
AigHandle::lit() const
{
  return node()->lit() + static_cast<SizeType>(inv());
}

END_NAMESPACE_YM_AIG

#endif // AIGNODE_H
