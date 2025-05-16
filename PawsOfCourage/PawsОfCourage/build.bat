@echo off
setlocal enabledelayedexpansion

set FILES=
for %%f in (src\*.cpp) do (
    set FILES=!FILES! %%f
)

emcc %FILES% -o index.html ^
 -std=c++14 -Wall -Os -DPLATFORM_WEB -D_DEFAULT_SOURCE -Wunused-result -Wno-missing-braces ^
 -I. -I C:/raylib/raylib/src -I C:/raylib/raylib/src/external ^
 -L C:/raylib/raylib/src C:/raylib/raylib/src/web/libraylib.a ^
 -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 ^
 --preload-file "Sprites" --shell-file Web/custom_shell.html ^
 -s "EXPORTED_FUNCTIONS=['_main','_malloc','_free']" ^
 -s "EXPORTED_RUNTIME_METHODS=['ccall']"
