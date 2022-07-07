#ifndef AIG_H
#define AIG_H

/// @file Aig.h
/// @brief Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief aig 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_AIG			\
  BEGIN_NAMESPACE_YM				\
  BEGIN_NAMESPACE(nsAig)

/// @brief aig 用の名前空間の終了
#define END_NAMESPACE_YM_AIG \
  END_NAMESPACE(nsAig)	     \
  END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_AIG

class AigHandle;
class AigNode;

//////////////////////////////////////////////////////////////////////
/// @class Aig Aig.h "Aig.h"
/// @brief AIG を表すクラス
///
/// このクラスは Aiger フォーマットの入出力用のものであり，
/// このオブジェクト上での最適化処理などは考えていない．
//////////////////////////////////////////////////////////////////////
class Aig
{
public:

  /// @brief コンストラクタ
  Aig();

  /// @brief デストラクタ
  ~Aig();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  initialize(
    SizeType I, ///< [in] 入力数
    SizeType L, ///< [in] ラッチ数
    SizeType O  ///< [in] 出力数
  );

  /// @brief ANDノードを生成する．
  /// @return 生成したノードのハンドルを返す．
  AigHandle
  new_and(
    AigHandle src1, ///< [in] 入力1のハンドル
    AigHandle src2  ///< [in] 入力2のハンドル
  );

  /// @brief ラッチのソースハンドルを設定する．
  void
  set_latch_src(
    SizeType pos, ///< [in] ラッチ番号 ( 0 <= pos < L() )
    AigHandle src ///< [in] ソースのハンドル
  );

  /// @brief 出力のソースハンドルを設定する．
  void
  set_output_src(
    SizeType pos, ///< [in] 出力番号 ( 0 <= pos < O() )
    AigHandle src ///< [in] ソースのハンドル
  );

  /// @brief ANDノードのソースハンドルを設定する．
  void
  set_and_src(
    SizeType pos,   ///< [in] AND番号 ( 0 <= pos < A() )
    AigHandle src1, ///< [in] ソース1のハンドル
    AigHandle src2  ///< [in] ソース1のハンドル
  );

  /// @brief 入力名を設定する．
  void
  set_input_symbol(
    SizeType pos,      ///< [in] 入力番号 ( 0 <= pos < I() )
    const string& name ///< [in] 名前
  );

  /// @brief ラッチ名を設定する．
  void
  set_latch_symbol(
    SizeType pos,      ///< [in] ラッチ番号 ( 0 <= pos < L() )
    const string& name ///< [in] 名前
  );

  /// @brief 出力名を設定する．
  void
  set_output_symbol(
    SizeType pos,      ///< [in] 出力番号 ( 0 <= pos < O() )
    const string& name ///< [in] 名前
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 入力ノードのハンドルを得る．
  AigHandle
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
  ) const;

  /// @brief ラッチノードのハンドルを得る．
  AigHandle
  latch(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const;

  /// @brief ラッチのソースハンドルを得る．
  AigHandle
  latch_src(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const;

  /// @brief 出力のソースハンドルを得る．
  AigHandle
  output_src(
    SizeType pos ///< [in] 出力番号 ( 0 <= pos < O() )
  ) const;

  /// @brief ANDノードのハンドルを得る．
  AigHandle
  and_node(
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


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ファイル入出力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief Ascii AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aag(
    const string& filename ///< [in] ファイル名
  );

  /// @brief Ascii AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aag(
    istream& s ///< [in] 入力ストリーム
  );

  /// @brief AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aig(
    const string& filename ///< [in] ファイル名
  );

  /// @brief AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aig(
    istream& s ///< [in] 入力ストリーム
  );

  /// @brief 内容を Ascii AIG フォーマットで書き出す．
  void
  write_aag(
    const string& filename ///< [in] ファイル名
  ) const;

  /// @brief 内容を Ascii AIG フォーマットで書き出す．
  void
  write_aag(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 内容を AIG フォーマットで書き出す．
  void
  write_aig(
    const string& filename ///< [in] ファイル名
  ) const;

  /// @brief 内容を AIG フォーマットで書き出す．
  void
  write_aig(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルテーブルとコメントを読み込む．
  void
  read_symbols(
    istream& s ///< [in] 入力ストリーム
  );

  /// @brief シンボルテーブルとコメントを出力する．
  void
  write_symbols(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief ノードのID番号を振り直す．
  void
  reordering();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ノードのリスト
  vector<AigNode*> mInputList;

  // ラッチノードのリスト
  vector<AigNode*> mLatchList;

  // ANDノードのリスト
  vector<AigNode*> mAndList;

  // 出力の入力ハンドルのリスト
  vector<AigHandle> mOutputList;

  // ラッチの入力ハンドルのリスト
  vector<AigHandle> mLatchSrcList;

  // 入力のシンボルテーブル
  vector<string> mInputSymbols;

  // ラッチのシンボルテーブル
  vector<string> mLatchSymbols;

  // 出力のシンボルテーブル
  vector<string> mOutputSymbols;

  // コメント
  string mComment;

};


//////////////////////////////////////////////////////////////////////
/// @class AigError AigError.h "AigError.h"
/// @brief Aig 関係のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class AigError :
  public exception
{
public:

  /// @brief コンストラクタ
  AigError(
    const string& msg ///< [in] エラーメッセージ
  ) : mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~AigError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表す文字列を返す．
  const string&
  str() const
  {
    return mMsg;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージ
  string mMsg;

};

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_YM

using nsAig::Aig;
using nsAig::AigError;

END_NAMESPACE_YM

#endif // AIG_H
