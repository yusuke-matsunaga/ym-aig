#ifndef AAG_H
#define AAG_H

/// @file Aag.h
/// @brief Aag のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class Aag Aag.h "Aag.h"
/// @brief aag 形式のファイルの内容を表すクラス
//////////////////////////////////////////////////////////////////////
class Aag
{
public:

  /// @brief コンストラクタ
  Aag();

  /// @brief デストラクタ
  ~Aag();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大の変数番号を返す．
  SizeType
  M() const;

  /// @brief 入力数を返す．
  SizeType
  I() const;

  /// @brief ラッチ数を返す．
  SizeType
  L() const;

  /// @brief 出力数を返す．
  SizeType
  O() const;

  /// @brief AND数を返す．
  SizeType
  A() const;

  /// @brief 入力のノード番号を返す．
  SizeType
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < I() )
  ) const;

  /// @brief ラッチのノード番号を返す．
  SizeType
  latch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < L() )
  ) const;

  /// @brief ラッチのソースリテラルを返す．
  SizeType
  latch_src(
    SizeType pos ///< [in] 位置 ( 0 <= pos < L() )
  ) const;

  /// @brief 出力のソースリテラルを返す．
  SizeType
  output_src(
    SizeType pos ///< [in] 位置 ( 0 <= pos < O() )
  ) const;

  /// @brief ANDノードのノード番号を返す．
  SizeType
  and_node(
    SizeType pos ///< [in] 位置 ( 0 <= pos < A() )
  ) const;

  /// @brief ANDノードのソースリテラル1を返す．
  SizeType
  and_src1(
    SizeType pos ///< [in] 位置 ( 0 <= pos < A() )
  ) const;

  /// @brief ANDノードのソースリテラル2を返す．
  SizeType
  and_src2(
    SizeType pos ///< [in] 位置 ( 0 <= pos < A() )
  ) const;

  /// @brief aag 形式のファイルを読み込む．
  void
  read(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aag 形式の入力ストリームを読み込む．
  void
  read(
    istream& s ///< [in] 入力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を登録する．
  void
  reg_node(
    SizeType id ///< [in] ノード番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義済みノード番号の辞書
  vector<bool> mNodeDict;

  // 入力ノード番号のリスト
  vector<SizeType> mInputList;

  // ラッチノード番号とソースリテラルのリスト
  vector<pair<SizeType, SizeType>> mLatchList;

  // 出力のソースリテラルのリスト
  vector<SizeType> mOutputList;

  // ANDノード番号とソースリテラルのリスト
  vector<tuple<SizeType, SizeType, SizeType>> mAndList;

};

END_NAMESPACE_YM_AIG

#endif // AAG_H
