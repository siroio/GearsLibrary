# Visual Studioがインストールされている場合にのみ.editorconfigファイルを生成する
if(MSVC)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/.editorconfig" "
# EditorConfig Settings for Visual Studio

root = true

[*]
indent_style = space
indent_size = 4
end_of_line = lf
charset = utf-8
trim_trailing_whitespace = true
")
endif()