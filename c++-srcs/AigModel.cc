
/// @file AigModel.cc
/// @brief AigModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigModel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigModel::AigModel(
) : mImpl{new ModelImpl}
{
}

// @brief コピーコンストラクタ
AigModel::AigModel(
  const AigModel& src
) : mImpl{new ModelImpl{*src.mImpl}}
{
}

// @brief ムーブコンストラクタ
AigModel::AigModel(
  AigModel&& src
) : mImpl{src.mImpl}
{
  src.mImpl = nullptr;
}

// @brief コピー代入文
AigModel&
AigModel::operator=(
  const AigModel& src
)
{
  if ( this != &src ) {
    delete mImpl;
    mImpl = new ModelImpl{*src.mImpl};
  }
  return *this;
}

// @brief ムーブ代入文
AigModel&
AigModel::operator=(
  AigModel&& src
)
{
  mImpl = src.mImpl;
  src.mImpl = nullptr;
  return *this;
}

// @brief デストラクタ
AigModel::~AigModel()
{
  delete mImpl;
}

// @brief Ascii AIG フォーマットを読み込む．
AigModel
AigModel::read_aag(
  const string& filename
)
{
  ifstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << "AigModel::read_aag: Could not open file "
	<< filename;
    throw std::invalid_argument{buf.str()};
  }
  return read_aag(s);
}

// @brief Ascii AIG フォーマットを読み込む．
AigModel
AigModel::read_aag(
  istream& s
)
{
  AigModel aig;
  aig.mImpl->read_aag(s);
  return aig;
}

// @brief AIG フォーマットを読み込む．
AigModel
AigModel::read_aig(
  const string& filename
)
{
  ifstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << "AigModel::read_aag: Could not open file "
	<< filename;
    throw std::invalid_argument{buf.str()};
  }
  return read_aig(s);
}

// @brief AIG フォーマットを読み込む．
AigModel
AigModel::read_aig(
  istream& s
)
{
  AigModel aig;
  aig.mImpl->read_aig(s);
  return aig;
}

// @brief 変数番号の最大値を返す．
SizeType
AigModel::M() const
{
  return mImpl->M();
}

// @brief 入力数を得る．
SizeType
AigModel::I() const
{
  return mImpl->I();
}

// @brief ラッチ数を得る．
SizeType
AigModel::L() const
{
  return mImpl->L();
}

// @brief 出力数を得る．
SizeType
AigModel::O() const
{
  return mImpl->O();
}

// @brief ANDノード数を返す．
SizeType
AigModel::A() const
{
  return mImpl->A();
}

// @brief 入力ノードのリテラルを得る．
SizeType
AigModel::input(
  SizeType pos
) const
{
  return mImpl->input(pos);
}

// @brief ラッチノードのリテラルを得る．
SizeType
AigModel::latch(
  SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
) const
{
  return mImpl->latch(pos);
}

// @brief ラッチのソースリテラルを得る．
SizeType
AigModel::latch_src(
  SizeType pos
) const
{
  return mImpl->latch_src(pos);
}

// @brief 出力のソースリテラルを得る．
SizeType
AigModel::output_src(
  SizeType pos
) const
{
  return mImpl->output_src(pos);
}

// @brief ANDノードのリテラルを得る．
SizeType
AigModel::and_node(
  SizeType pos
) const
{
  return mImpl->and_node(pos);
}

// @brief ANDノードのソース1のリテラルを得る．
SizeType
AigModel::and_src1(
  SizeType pos
) const
{
  return mImpl->and_src1(pos);
}

// @brief ANDノードのソース2のリテラルを得る．
SizeType
AigModel::and_src2(
  SizeType pos
) const
{
  return mImpl->and_src2(pos);
}

// @brief 入力のシンボルを得る．
const string&
AigModel::input_symbol(
  SizeType pos
) const
{
  return mImpl->input_symbol(pos);
}

// @brief ラッチのシンボルを得る．
const string&
AigModel::latch_symbol(
  SizeType pos
) const
{
  return mImpl->latch_symbol(pos);
}

// @brief 出力のシンボルを得る．
const string&
AigModel::output_symbol(
  SizeType pos
) const
{
  return mImpl->output_symbol(pos);
}

// @brief コメントを得る．
string
AigModel::comment() const
{
  return mImpl->comment();
}

// @brief 内容を出力する．
void
AigModel::print(
  ostream& s
) const
{
  mImpl->print(s);
}

END_NAMESPACE_YM_AIG
