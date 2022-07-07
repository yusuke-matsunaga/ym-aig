
/// @file Aag.cc
/// @brief Aag の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Aag.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE
bool debug = false;
END_NONAMESPACE

// @brief コンストラクタ
Aag::Aag()
{
}

// @brief デストラクタ
Aag::~Aag()
{
}

// @brief 最大の変数番号を返す．
SizeType
Aag::M() const
{
  return mNodeDict.size();
}

// @brief 入力数を返す．
SizeType
Aag::I() const
{
  return mInputList.size();
}

// @brief ラッチ数を返す．
SizeType
Aag::L() const
{
  return mLatchList.size();
}

// @brief 出力数を返す．
SizeType
Aag::O() const
{
  return mOutputList.size();
}

// @brief AND数を返す．
SizeType
Aag::A() const
{
  return mAndList.size();
}

// @brief 入力のノード番号を返す．
SizeType
Aag::input(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < I() );
  return mInputList[pos];
}

// @brief ラッチのノード番号を返す．
SizeType
Aag::latch(
  SizeType pos ///< [in] 位置 ( 0 <= pos < L() )
) const
{
  ASSERT_COND( 0 <= pos && pos < L() );
  return mLatchList[pos].first;
}

// @brief ラッチのソースリテラルを返す．
SizeType
Aag::latch_src(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < L() );
  return mLatchList[pos].second;
}

// @brief 出力のソースリテラルを返す．
SizeType
Aag::output_src(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < O() );
  return mOutputList[pos];
}

// @brief ANDノードのノード番号を返す．
SizeType
Aag::and_node(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < A() );
  return std::get<0>(mAndList[pos]);
}

// @brief ANDノードのソースリテラル1を返す．
SizeType
Aag::and_src1(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < A() );
  return std::get<1>(mAndList[pos]);
}

// @brief ANDノードのソースリテラル2を返す．
SizeType
Aag::and_src2(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < A() );
  return std::get<2>(mAndList[pos]);
}

// @brief aag 形式のファイルを読み込む．
void
Aag::read(
  const string& filename
)
{
  ifstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << filename << ": No such file.";
    throw AigError{buf.str()};
  }
  read(s);
}

// @brief aag 形式の入力ストリームを読み込む．
void
Aag::read(
  istream& s
)
{
  string linebuf;

  // ヘッダ行の読み込み
  if ( !getline(s, linebuf) ) {
    throw AigError{"Unexpected EOF"};
  }
  if ( linebuf.substr(0, 3) != string{"aag"} ) {
    ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aag' expected.";
    throw AigError{buf.str()};
  }
  istringstream tmp{linebuf.substr(4, string::npos)};
  SizeType M, I, L, O, A;
  tmp >> M >> I >> L >> O >> A;

  if ( debug ) {
    cout << "aag " << M
	 << " " << I
	 << " " << L
	 << " " << O
	 << " " << A << endl;
  }

  mNodeDict.clear();
  mNodeDict.resize(M + 1, false);
  mInputList.clear();
  mLatchList.clear();
  mOutputList.clear();
  mAndList.clear();

  // 入力行の読み込み
  for ( SizeType i = 0; i < I; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit;
    tmp >> lit;
    if ( debug ) {
      cout << "I#" << i << ": " << lit << endl;
    }
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"};
    }
    SizeType var = lit / 2;
    mInputList.push_back(var);
    reg_node(var);
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit, src;
    tmp >> lit >> src;
    if ( debug ) {
      cout << "L#" << i << ": " << lit << " " << src << endl;
    }
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"};
    }
    SizeType var = lit / 2;
    mLatchList.push_back({var, lit});
    reg_node(var);
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType src;
    tmp >> src;
    if ( debug ) {
      cout << "O#" << i << ": " << src << endl;
    }
    mOutputList.push_back(src);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit, src0, src1;
    tmp >> lit >> src0 >> src1;
    if ( debug ) {
      cout << "A#" << i << ": " << lit << " " << src0 << " " << src1 << endl;
    }
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"};
    }
    SizeType var = lit / 2;
    mAndList.push_back({var, src0, src1});
    reg_node(var);
  }
}

// @brief ノード番号を登録する．
void
Aag::reg_node(
  SizeType id
)
{
  if ( mNodeDict[id] ) {
    ostringstream buf;
    buf << "Node#" << id << ": Defined more than once.";
    throw AigError{buf.str()};
  }
  mNodeDict[id] = true;
}

END_NAMESPACE_YM_AIG
