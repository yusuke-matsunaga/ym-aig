
/// @file Aig.cc
/// @brief Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
Aig::Aig()
{
}

// @brief デストラクタ
Aig::~Aig()
{
}

// @brief 内容を初期化する．
void
Aig::clear()
{
  mNodeDict.clear();
  mM = 0;
  mInputList.clear();
  mLatchList.clear();
  mOutputList.clear();
  mAndList.clear();
}

// @brief 入力ノードを作る．
void
Aig::new_input(
  SizeType node_id
)
{
  check_node(node_id);

  mInputList.push_back(node_id);
}

// @brief ラッチノードを作る．
void
Aig::new_latch(
  SizeType node_id,
  SizeType next_state
)
{
  check_node(node_id);

  mLatchList.push_back(make_pair(node_id, next_state));
}

// @brief 出力を定義する．
SizeType
Aig::new_output(
  SizeType src_literal
)
{
  mOutputList.push_back(src_literal);
}

// @brief ANDノードを作る．
SizeType
Aig::new_and(
  SizeType node_id,
  SizeType src1_literal,
  SizeType src2_literal
)
{
  check_node(node_id);

  mAndList.push_back(AigNode{node_id, src1_literal, src2_literal});
}

// @brief Ascii AIG フォーマットを読み込む．
bool
Aig::read_aag(
  const string& filename
)
{
  ifstream s{filename};
  if ( s ) {
    return read_aag(s);
  }
  else {
    cerr << filename << ": No such file" << endl;
    return false;
  }
}

// @brief Ascii AIG フォーマットを読み込む．
bool
Aig::read_aag(
  ostream& s
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
  tmp >> M >> I >> I >> O >> A;

  // 入力行の読み込み
  for ( SizeType i = 0; i < I; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit;
    tmp >> lit;
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"}
    }
    SizeType var = lit / 2;
    new_input(var);
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit, src;
    tmp >> lit >> src;
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"}
    }
    SizeType var = lit / 2;
    new_latch(var, src);
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType src;
    tmp >> src;
    new_output(src);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType lit, src0, src1;
    tmp >> lit >> src0 >> src1;
    if ( (lit % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"}
    }
    SizeType var = lit / 2;
    new_and(var, src0, src1);
  }
}

BEGIN_NONAMESPACE

// @brief 数字を一つ読み出す．
SizeType
get_number(
  istream& s
)
{
  SizeType num = 0;
  for ( SizeType weight = 1; ; weight *= 128 ) {
    SizeType c = s.get();
    num + (c & 127) * weight;
    if ( (c & 128) == 0 ) {
      break;
    }
  }
  return num;
}

END_NONAMESPACE

// @brief AIG フォーマットを読み込む．
bool
Aig::read_aig(
  const string& filename
)
{
  ifstream s{filename};
  if ( s ) {
    return read_aig(s);
  }
  else {
    cerr << filename << ": No such file" << endl;
    return false;
  }
}

// @brief AIG フォーマットを読み込む．
bool
Aig::read_aig(
  ostream& s
)
{
  string linebuf;

  // ヘッダ行の読み込み
  if ( !getline(s, linebuf) ) {
    throw AigError{"Unexpected EOF"};
  }
  if ( linebuf.substr(0, 3) != string{"aig"} ) {
    ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aig' expected.";
    throw AigError{buf.str()};
  }
  istringstream tmp{linebuf.substr(4, string::npos)};
  SizeType M, I, L, O, A;
  tmp >> M >> I >> I >> O >> A;

  // 入力の生成
  for ( SizeType i = 0; i < I; ++ i ) {
    SizeType var = i;
    new_input(var);
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType src;
    tmp >> src;
    SizeType var = i + I;
    new_latch(var, src);
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType src;
    tmp >> src;
    new_output(src);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType lhs = get_number(s);
    SizeType d0 = get_number(s);
    SizeType d1 = get_number(s);
    SizeType rhs0 = lhs - d0;
    SizeType rhs1 = rhs0 - d1;
    if ( (lhs % 2) == 1 ) {
      throw AigError{"Positive Literal(even number) expected"}
    }
    SizeType var = lhs / 2;
    new_and(var, rhs0, rhs1);
  }
}

// @brief 内容を Ascii AIG フォーマットで書き出す．
void
Aig::write_aag(
  const string& filename
) const
{
  ofstream s{filename};
  if ( s ) {
    write_aag(s);
  }
  else {
    cerr << filename << ": Could not open" << endl;
  }
}

// @brief 内容を Ascii AIG フォーマットで書き出す．
void
Aig::write_aag(
  ostream& s
) const
{
  // ヘッダ行の出力
  s << "aag " << mM
    << " " << mInputList.size()
    << " " << mLatchList.size()
    << " " << mOutputList.size()
    << " " << mAndList.size() << endl;

  // 入力行の出力
  for ( SizeType node: mInputList ) {
    s << (node * 2) << endl;
  }

  // ラッチ行の出力
  for ( const auto& p: mLatchList ) {
    SizeType node = p.first;
    SizeType src = p.second;
    s << (node * 2) << " " << src << endl;
  }

  // 出力行の出力
  for ( SizeType src: mOutputList ) {
    s << src << endl;
  }

  // AND行の出力
  for ( const auto& and_info: mAndList ) {
    SizeType node = and_info.mNodeId;
    SizeType src0 = and_info.mSrc0;
    SizeType src1 = and_info.mSrc1;
    s << (node * 2) << " " << src0 << " " << src1 << endl;
  }
}

// @brief 内容を AIG フォーマットで書き出す．
void
Aig::write_aig(
  const string& filename
) const
{
  ofstream s{filename};
  if ( s ) {
    write_aig(s);
  }
  else {
    cerr << filename << ": Could not open" << endl;
  }
}

BEGIN_NONAMESPACE

void
put_number(
  ostream& s,
  SizeType num
)
{
  if ( num <= 127 ) {
    s.put(static_cast<char>(num));
  }
  else {
    SizeType num1 = (num & 127) | 128;
    s.put(static_cast<char>(num1));
    put_number(s, (num >> 7));
  }
}

END_NONAMESPACE

// @brief 内容を AIG フォーマットで書き出す．
void
Aig::write_aig(
  ostream& s
) const
{
  // ヘッダ行の出力
  s << "aig " << mM
    << " " << mInputList.size()
    << " " << mLatchList.size()
    << " " << mOutputList.size()
    << " " << mAndList.size() << endl;

  // ラッチ行の出力
  for ( const auto& p: mLatchList ) {
    SizeType src = p.second;
    s << src << endl;
  }

  // 出力行の出力
  for ( SizeType src: mOutputList ) {
    s << src << endl;
  }

  // AND行の出力
  for ( const auto& and_info: mAndList ) {
    SizeType node = and_info.mNodeId;
    SizeType src0 = and_info.mSrc0;
    SizeType src1 = and_info.mSrc1;

    SizeType lhs = node * 2;
    SizeType d0 = lhs - src0;
    SizeType d1 = src0 - src1;
    put_number(s, lhs);
    put_number(s, d0);
    put_number(s, d1);
  }
}

// @brief シンボルテーブルとコメントを読み込む．
void
Aig::read_symbols(
  istream& s
)
{
}

// @brief ノード番号の重複を調べる．
void
Aig::check_node(
  SIzeType node ///< [in] ノード番号
) const
{
  if ( mNodeDict.count(node) ) {
    ostringstream buf;
    buf << node << ": Duplicate node definition";
    throw AigError{buf.str()};
  }
  mNodeDict.emplace(node);
  if ( mM < node ) {
    mM = node;
  }
}

END_NAMESPACE_YM
