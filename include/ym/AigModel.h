#ifndef AIGMODEL_H
#define AIGMODEL_H

/// @file AigModel.h
/// @brief AigModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig_nsdef.h"


BEGIN_NAMESPACE_YM_AIG

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class AigModel AigModel.h "ym/AigModel.h"
/// @brief AIG 形式のファイルを読み込んだ結果を表すクラス
///
/// 実際の処理は ModelImpl が行う．
//////////////////////////////////////////////////////////////////////
class AigModel
{
private:

  /// @brief コンストラクタ
  AigModel();


public:

  /// @brief コピーコンストラクタ
  AigModel(
    const AigModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  AigModel(
    AigModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief コピー代入文
  AigModel&
  operator=(
    const AigModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入文
  AigModel&
  operator=(
    AigModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~AigModel();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ファイル入力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief Ascii AIG フォーマットを読み込む．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigModel
  read_aag(
    const string& filename ///< [in] ファイル名
  );

  /// @brief Ascii AIG フォーマットを読み込む．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigModel
  read_aag(
    istream& s ///< [in] 入力ストリーム
  );

  /// @brief AIG フォーマットを読み込む．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigModel
  read_aig(
    const string& filename ///< [in] ファイル名
  );

  /// @brief AIG フォーマットを読み込む．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigModel
  read_aig(
    istream& s ///< [in] 入力ストリーム
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号の最大値を返す．
  SizeType
  M() const;

  /// @brief 入力数を得る．
  SizeType
  I() const;

  /// @brief ラッチ数を得る．
  SizeType
  L() const;

  /// @brief 出力数を得る．
  SizeType
  O() const;

  /// @brief ANDノード数を返す．
  SizeType
  A() const;

  /// @brief 入力ノードのリテラルを得る．
  SizeType
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
  ) const;

  /// @brief ラッチノードのリテラルを得る．
  SizeType
  latch(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const;

  /// @brief ラッチのソースリテラルを得る．
  SizeType
  latch_src(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const;

  /// @brief 出力のソースリテラルを得る．
  SizeType
  output_src(
    SizeType pos ///< [in] 出力番号 ( 0 <= pos < O() )
  ) const;

  /// @brief ANDノードのリテラルを得る．
  SizeType
  and_node(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const;

  /// @brief ANDノードのソース1のリテラルを得る．
  SizeType
  and_src1(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const;

  /// @brief ANDノードのソース2のリテラルを得る．
  SizeType
  and_src2(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const;

  /// @brief 入力のシンボルを得る．
  const string&
  input_symbol(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
  ) const;

  /// @brief ラッチのシンボルを得る．
  const string&
  latch_symbol(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const;

  /// @brief 出力のシンボルを得る．
  const string&
  output_symbol(
    SizeType pos ///< [in] 出力番号 ( 0 <= pos < O() )
  ) const;

  /// @brief コメントを得る．
  string
  comment() const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  ModelImpl* mImpl;

};

END_NAMESPACE_YM_AIG

#endif // AIGMODEL_H
