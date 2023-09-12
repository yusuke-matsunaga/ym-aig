
/// @file read_aig.cc
/// @brief read_aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigModel.h"


BEGIN_NAMESPACE_YM

int
read_aig(
  int argc,
  char** argv
)
{
  for ( SizeType i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    auto aig = AigModel::read_aig(filename);
    aig.print(cout);
  }
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_aig(argc, argv);
}
