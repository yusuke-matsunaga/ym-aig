
/// @file Aig.cc
/// @brief Aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"
#include "Aag.h"
#include "AigHandle.h"
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

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
Aig::initialize(
  SizeType I,
  SizeType L,
  SizeType O
)
{
  mInputList.clear();
  mInputList.resize(I);
  mInputSymbols.clear();
  mInputSymbols.resize(I);
  for ( SizeType i = 0; i < I; ++ i ) {
    mInputList[i] = new AigNode(i + 1);
  }

  mLatchList.clear();
  mLatchList.resize(L);
  mLatchSrcList.clear();
  mLatchSrcList.resize(L);
  mLatchSymbols.clear();
  mLatchSymbols.resize(L);
  for ( SizeType i = 0; i < L; ++ i ) {
    mLatchList[i] = new AigNode(i + I + 1);
  }

  mOutputList.clear();
  mOutputList.resize(O);
  mOutputSymbols.clear();
  mOutputSymbols.resize(O);

  mAndList.clear();

  mComment = string{};
}

// @brief ANDノードを生成する．
// @return 生成したノードのハンドルを返す．
AigHandle
Aig::new_and(
  AigHandle src1,
  AigHandle src2
)
{
  SizeType id = mAndList.size() + I() + L() + 1;
  auto node = new AigNode{id, src1, src2};
  mAndList.push_back(node);
  return AigHandle{node, false};
}

// @brief ラッチのソースハンドルを設定する．
void
Aig::set_latch_src(
  SizeType pos, ///< [in] ラッチ番号 ( 0 <= pos < L() )
  AigHandle src ///< [in] ソースのハンドル
)
{
  ASSERT_COND( 0 <= pos && pos < L() );
  mLatchSrcList[pos] = src;
}

// @brief 出力のソースハンドルを設定する．
void
Aig::set_output_src(
  SizeType pos,
  AigHandle src
)
{
  ASSERT_COND( 0 <= pos && pos < O() );
  mOutputList[pos] = src;
}

// @brief ANDノードのソースハンドルを設定する．
void
Aig::set_and_src(
  SizeType pos,
  AigHandle src1,
  AigHandle src2
)
{
  ASSERT_COND( 0 <= pos && pos < A() );
  auto node = mAndList[pos];
  node->set_src(src1, src2);
}

// @brief 入力のシンボルを設定する．
void
Aig::set_input_symbol(
  SizeType pos,
  const string& name
)
{
  ASSERT_COND( 0 <= pos && pos < I() );
  mInputSymbols[pos] = name;
}

// @brief ラッチのシンボルを設定する．
void
Aig::set_latch_symbol(
  SizeType pos,
  const string& name
)
{
  ASSERT_COND( 0 <= pos && pos < L() );
  mLatchSymbols[pos] = name;
}

// @brief 出力のシンボルを設定する．
void
Aig::set_output_symbol(
  SizeType pos,
  const string& name
)
{
  ASSERT_COND( 0 <= pos && pos < O() );
  mOutputSymbols[pos] = name;
}

// @brief 入力数を得る．
SizeType
Aig::I() const
{
  return mInputList.size();
}

// @brief ラッチ数を得る．
SizeType
Aig::L() const
{
  return mLatchList.size();
}

// @brief 出力数を得る．
SizeType
Aig::O() const
{
  return mOutputList.size();
}

// @brief ANDノード数を返す．
SizeType
Aig::A() const
{
  return mAndList.size();
}

// @brief 入力ノードのハンドルを得る．
AigHandle
Aig::input(
  SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
) const
{
  ASSERT_COND( 0 <= pos && pos < I() );
  return AigHandle{mInputList[pos], false};
}

// @brief ラッチノードのハンドルを得る．
AigHandle
Aig::latch(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < L() );
  return AigHandle{mLatchList[pos], false};
}

// @brief ラッチのソースハンドルを得る．
AigHandle
Aig::latch_src(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < L() );
  return mLatchSrcList[pos];
}

// @brief 出力のソースハンドルを得る．
AigHandle
Aig::output_src(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < O() );
  return mOutputList[pos];
}

// @brief ANDノードのハンドルを得る．
AigHandle
Aig::and_node(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < A() );
  return AigHandle{mAndList[pos], false};
}

// @brief 入力のシンボルを得る．
const string&
Aig::input_symbol(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < I() );
  return mInputSymbols[pos];
}

// @brief ラッチのシンボルを得る．
const string&
Aig::latch_symbol(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < L() );
  return mLatchSymbols[pos];
}

// @brief 出力のシンボルを得る．
const string&
Aig::output_symbol(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < O() );
  return mOutputSymbols[pos];
}

// @brief コメントを得る．
string
Aig::comment() const
{
  return mComment;
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
  istream& s
)
{
  // aag の本体部分は別クラスで読み込む．
  Aag aag;
  aag.read(s);

  SizeType M = aag.M();
  SizeType I = aag.I();
  SizeType L = aag.L();
  SizeType O = aag.O();
  SizeType A = aag.A();

  initialize(I, L, O);

  // aag 上のノード番号をキーにハンドルを収めるテーブル
  vector<AigHandle> node_map(M * 2 + 2);
  // 入力ノードの対応付を行う．
  for ( SizeType i = 0; i < I; ++ i ) {
    auto h = input(i);
    SizeType pos = aag.input(i);
    node_map[pos * 2 + 0] = h;
    node_map[pos * 2 + 1] = ~h;
  }
  // ラッチノードの対応付を行う．
  for ( SizeType i = 0; i < L; ++ i ) {
    auto h = latch(i);
    SizeType pos = aag.latch(i);
    node_map[pos * 2 + 0] = h;
    node_map[pos * 2 + 1] = ~h;
  }
  // ANDノードの生成と対応付を行う．
  // この時点ではファンインのノードが存在するとは限らない
  // ので入力は空にしておく．
  for ( SizeType i = 0; i < A; ++ i ) {
    auto h = new_and(AigHandle::make_zero(),
		     AigHandle::make_zero());
    SizeType pos = aag.and_node(i);
    node_map[pos * 2 + 0] = h;
    node_map[pos * 2 + 1] = ~h;
  }
  // ラッチの入力の設定を行う．
  for ( SizeType i = 0; i < L; ++ i ) {
    SizeType src_pos = aag.latch_src(i);
    auto h = node_map[src_pos];
    set_latch_src(i, h);
  }
  // 出力ノードの入力の設定を行う．
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src_pos = aag.output_src(i);
    auto h = node_map[src_pos];
    set_output_src(i, h);
  }
  // ANDノードの入力の設定を行う．
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType src1_pos = aag.and_src1(i);
    auto h1 = node_map[src1_pos];
    SizeType src2_pos = aag.and_src2(i);
    auto h2 = node_map[src2_pos];
    set_and_src(i, h1, h2);
  }

  // シンボルテーブルとコメントの読み込みを行う．
  read_symbols(s);

  // 入力からのトポロジカル順になるようにID番号を振り直す．
  reordering();

  // ANDゲートのファンインを並べ替える．
  for ( auto node: mAndList ) {
    node->arrange_fanin();
  }

  return true;
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
    num += (c & 127) * weight;
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
  istream& s
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
  tmp >> M >> I >> L >> O >> A;

  ASSERT_COND( M == I + L + A );

  if ( debug ) {
    cout << "aig " << M
	 << " " << I
	 << " " << L
	 << " " << O
	 << " " << A << endl;
  }

  // M と A は捨てる．
  initialize(I, L, O);

  // ラッチ行の読み込み
  vector<SizeType> latch_src(L);
  for ( SizeType i = 0; i < L; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw AigError{"Unexpected EOF"};
    }
    istringstream tmp{linebuf};
    SizeType src;
    tmp >> src;
    if ( debug ) {
      cout << "L#" << i << ": " << src << endl;
    }
    latch_src[i] = src;
  }

  // 出力行の読み込み
  vector<SizeType> output_src(O);
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
    output_src[i] = src;
  }

  vector<AigHandle> node_list(M * 2 + 2);
  node_list[0] = AigHandle::make_zero();
  node_list[1] = AigHandle::make_one();
  for ( SizeType i = 0; i < I; ++ i ) {
    auto h = input(i);
    node_list[i * 2 + 2] = h;
    node_list[i * 2 + 3] = ~h;
  }
  for ( SizeType i = 0; i < L; ++ i ) {
    auto h = latch(i);
    node_list[(i + I + 1) * 2 + 0] = h;
    node_list[(i + I + 1) * 2 + 1] = ~h;
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType d0 = get_number(s);
    SizeType d1 = get_number(s);
    SizeType pos = i + I + L + 1;
    SizeType lhs = pos * 2;
    SizeType rhs0 = lhs - d0;
    SizeType rhs1 = rhs0 - d1;
    if ( debug ) {
      cout << "A#" << i << ": "
	   << d0 << " " << d1
	   << " -> " << rhs0 << " " << rhs1 << endl;
    }
    auto src1 = node_list[rhs0];
    auto src2 = node_list[rhs1];
    auto h = new_and(src1, src2);
    node_list[lhs + 0] = h;
    node_list[lhs + 1] = ~h;
  }

  // ラッチのソースの設定
  for ( SizeType i = 0; i < L; ++ i ) {
    auto src = node_list[latch_src[i]];
    set_latch_src(i, src);
  }

  // 出力のソースの設定
  for ( SizeType i = 0; i < O; ++ i ) {
    auto src = node_list[output_src[i]];
    set_output_src(i, src);
  }

  // シンボルの読み込み
  read_symbols(s);

  return true;
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
  s << "aag " << (I() + L() + A())
    << " " << I()
    << " " << L()
    << " " << O()
    << " " << A() << endl;

  // 入力行の出力
  for ( SizeType i = 0; i < I(); ++ i ) {
    s << (i + 1) * 2 << endl;
  }

  // ラッチ行の出力
  for ( SizeType i = 0; i < L(); ++ i ) {
    SizeType node_id = i + I() + 1;
    auto src = mLatchSrcList[i];
    s << (node_id * 2) << " " << src.lit() << endl;
  }

  // 出力行の出力
  for ( SizeType i = 0; i < O(); ++ i ) {
    auto src = mOutputList[i];
    s << src.lit() << endl;
  }

  // AND行の出力
  for ( SizeType i = 0; i < A(); ++ i ) {
    auto node = mAndList[i];
    auto src0 = node->src1();
    auto src1 = node->src2();
    s << node->lit() << " "
      << src0.lit() << " "
      << src1.lit() << endl;
  }

  // シンボルテーブルとコメントの出力
  write_symbols(s);
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
  s << "aig " << (I() + L() + A())
    << " " << I()
    << " " << L()
    << " " << O()
    << " " << A() << endl;

  // ラッチ行の出力
  for ( auto src: mLatchSrcList ) {
    s << src.lit() << endl;
  }

  // 出力行の出力
  for ( auto src: mOutputList ) {
    s << src.lit() << endl;
  }

  // AND行の出力
  for ( auto node: mAndList ) {
    SizeType src0 = node->src1().lit();
    SizeType src1 = node->src2().lit();
    SizeType lhs = node->lit();
    SizeType d0 = lhs - src0;
    SizeType d1 = src0 - src1;
    put_number(s, d0);
    put_number(s, d1);
  }

  // シンボルテーブルとコメントの出力
  write_symbols(s);
}

// @brief シンボルテーブルとコメントを読み込む．
void
Aig::read_symbols(
  istream& s
)
{
  string linebuf;
  bool symbol_mode = true;
  while ( getline(s, linebuf) ) {
    if ( symbol_mode ) {
      if ( linebuf == "c" ) {
	symbol_mode = false;
      }
      else {
	auto p = linebuf.find_first_of(" ");
	auto pos_str = linebuf.substr(0, p);
	auto name = linebuf.substr(p + 1, string::npos);
	SizeType pos = atoi(pos_str.substr(1, string::npos).c_str());
	char prefix = pos_str[0];
	if ( prefix == 'i' ) {
	  mInputSymbols[pos] = name;
	}
	else if ( prefix == 'l' ) {
	  mLatchSymbols[pos] = name;
	}
	else if ( prefix == 'o' ) {
	  mOutputSymbols[pos] = name;
	}
	else {
	  ASSERT_NOT_REACHED;
	}
      }
    }
    else {
      mComment += linebuf + '\n';
    }
  }
}

// @brief シンボルテーブルとコメントを出力する．
void
Aig::write_symbols(
  ostream& s ///< [in] 出力ストリーム
) const
{

  // 入力のシンボルテーブルの出力
  for ( SizeType i = 0; i < I(); ++ i ) {
    const auto& name = input_symbol(i);
    if ( name != string{} ) {
      s << "i" << i << " " << name << endl;
    }
  }

  // ラッチのシンボルテーブルの出力
  for ( SizeType i = 0; i < L(); ++ i ) {
    const auto& name = latch_symbol(i);
    if ( name != string{} ) {
      s << "l" << i << " " << name << endl;
    }
  }

  // 出力のシンボルテーブルの出力
  for ( SizeType i = 0; i < O(); ++ i ) {
    const auto& name = output_symbol(i);
    if ( name != string{} ) {
      s << "o" << i << " " << name << endl;
    }
  }

  // コメントの出力
  if ( comment() != string{} ) {
    s << "c" << endl;
    s << comment();
  }
}

// @brief ノードのID番号を振り直す．
void
Aig::reordering()
{
  // ノード番号をキーにしてファンアウトリストを保持するリスト
  vector<vector<AigNode*>> fanout_list_array(I() + L() + A() + 1);
  // ANDノードのファンインのファンアウトリストを作る．
  for ( auto node: mAndList ) {
    auto src1 = node->src1();
    auto src1_node = src1.node();
    fanout_list_array[src1_node->id()].push_back(node);
    auto src2 = node->src2();
    auto src2_node = src2.node();
    fanout_list_array[src2_node->id()].push_back(node);
  }
  // 新しいID番号のマップ
  vector<SizeType> id_map(I() + L() + A() + 1, 0);
  // 有効なID番号
  SizeType last_id = 1;
  // ファンインが処理済みとなったノードを入れるキュー
  vector<AigNode*> queue;
  // キューに入っていることを示すフラグ
  vector<bool> in_queue(I() + L() + A() + 1, false);
  for ( auto node: mInputList ) {
    id_map[node->id()] = last_id;
    ++ last_id;
    bool ok = true;
    for ( auto node1: fanout_list_array[node->id()] ) {
      if ( in_queue[node1->id()] ) {
	continue;
      }
      if ( id_map[node1->src1().node()->id()] > 0 &&
	   id_map[node1->src2().node()->id()] > 0 ) {
	// 2つのファンインが処理済み
	queue.push_back(node1);
	in_queue[node1->id()] = true;
      }
    }
  }
  for ( auto node: mLatchList ) {
    id_map[node->id()] = last_id;
    ++ last_id;
    bool ok = true;
    for ( auto node1: fanout_list_array[node->id()] ) {
      if ( in_queue[node1->id()] ) {
	continue;
      }
      if ( id_map[node1->src1().node()->id()] > 0 &&
	   id_map[node1->src2().node()->id()] > 0 ) {
	// 2つのファンインが処理済み
	queue.push_back(node1);
	in_queue[node1->id()] = true;
      }
    }
  }
  for ( SizeType rpos = 0; rpos < queue.size(); ++ rpos ) {
    auto node = queue[rpos];
    id_map[node->id()] = last_id;
    ++ last_id;
    bool ok = true;
    for ( auto node1: fanout_list_array[node->id()] ) {
      if ( in_queue[node1->id()] ) {
	continue;
      }
      if ( id_map[node1->src1().node()->id()] > 0 &&
	   id_map[node1->src2().node()->id()] > 0 ) {
	// 2つのファンインが処理済み
	queue.push_back(node1);
	in_queue[node1->id()] = true;
      }
    }
  }
  // ID番号を付け替える．
  for ( auto node: mInputList ) {
    node->set_id(id_map[node->id()]);
  }
  for ( auto node: mLatchList ) {
    node->set_id(id_map[node->id()]);
  }
  for ( auto node: mAndList ) {
    node->set_id(id_map[node->id()]);
  }
}

END_NAMESPACE_YM_AIG
