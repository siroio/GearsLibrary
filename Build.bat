@echo off
@chcp 65001 > nul

set BUILD_DIR=Build
set IS_PAUSE=true

if not exist %BUILD_DIR%\nul (
  echo %BUILD_DIR%フォルダが存在しないため、作成します。
  mkdir %BUILD_DIR%
)

echo CMakeのビルドを開始します...
cmake -S . -B %BUILD_DIR%
if errorlevel 1 (
  echo CMakeのビルドに失敗しました。
  pause
  exit /b 1
)

echo makeを開始します...
cmake --build %BUILD_DIR%
if errorlevel 1 (
  echo ビルドに失敗しました。
  pause
  exit /b 1
)

echo ビルドが正常に終了しました。
if %IS_PAUSE% == true (
  pause
)