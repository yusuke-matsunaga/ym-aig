
/// @file read_aag.cc
/// @brief read_aag の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Aig.h"


BEGIN_NAMESPACE_YM

int
read_aag(
  int argc,
  char** argv
)
{
  for ( SizeType i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    Aig aig;
    aig.read_aag(filename);
    aig.write_aag(stdout);
  }
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_aag(argc, argv);
}
