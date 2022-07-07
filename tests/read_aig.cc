
/// @file read_aig.cc
/// @brief read_aig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"


BEGIN_NAMESPACE_YM

int
read_aig(
  int argc,
  char** argv
)
{
  for ( SizeType i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    Aig aig;
    aig.read_aig(filename);
    aig.write_aig(cout);
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
