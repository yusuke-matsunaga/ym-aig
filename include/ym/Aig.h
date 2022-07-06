#ifndef AIG_H
#define AIG_H

/// @file Aig.h
/// @brief Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

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

  /// @brief 内容を初期化する．
  void
  clear();

  /// @brief 入力ノードを作る．
  void
  new_input(
    SizeType node_id ///< [in] ノード番号
  );

  /// @brief ラッチノードを作る．
  void
  new_latch(
    SizeType node_id,   ///< [in] ノード番号
    SizeType next_state ///< [in] next_state リテラル
  );

  /// @brief 出力を定義する．
  void
  new_output(
    SizeType src_literal ///< [in] ソースリテラル
  );

  /// @brief ANDノードを作る．
  void
  new_and(
    SizeType node_id,      ///< [in] ノード番号
    SizeType src1_literal, ///< [in] ソース1のリテラル
    SizeType src2_literal  ///< [in] ソース2のリテラル
  );


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
    ostream& s ///< [in] 入力ストリーム
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
    ostream& s ///< [in] 入力ストリーム
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

  /// @brief ノード番号の重複を調べる．
  ///
  /// 重複していた場合には AigError を送出する．
  void
  check_node(
    SIzeType node ///< [in] ノード番号
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  struct AigNode
  {
    SizeType mNodeId;
    SizeType mSrc0;
    SizeType mSrc1;
  };

  // 定義済みノード番号の辞書
  unordered_set<SizeType> mNodeDict;

  // ノード番号の最大値
  SizeType mM{0};

  // 入力ノード番号のリスト
  vector<SizeType> mInputList;

  // ラッチノード番号とラッチ入力リテラルのリスト
  vector<pair<SizeType, SizeType>> mLatchList;

  // 出力のソースリテラルのリスト
  vector<SizeType> mOutputList;

  // ANDノードのリスト
  vector<AigNode> mAndList;

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

END_NAMESPACE_YM

#endif // AIG_H
