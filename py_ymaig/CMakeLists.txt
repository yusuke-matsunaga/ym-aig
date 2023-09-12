# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( py_ymaig
  ymaig_module.cc
  PyAigModel.cc
  )

add_library ( py_ymaiglib
  SHARED
  $<TARGET_OBJECTS:py_ymaig_obj>
  $<TARGET_OBJECTS:ym_aig_obj>
  )

set_target_properties( py_ymaiglib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "ymaig"
  )

target_link_libraries( py_ymaiglib
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable ( py_ymaig
  py_ymaig.cc
  $<TARGET_OBJECTS:py_ymaig_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options( py_ymaig
  PRIVATE "-g"
  )

target_link_libraries ( py_ymaig
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
