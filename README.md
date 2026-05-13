# TestPlayStay
簡単なUnityテストリポジトリ、他人のコードを引いてきたりする。

## OpenGL サンプル
GLFW + GLAD を使って、色付き三角形を描画するサンプルです。

### 必要なもの
- GLFW
- GLAD

### vcpkg (MinGW) での導入
```
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg.exe install glfw3 glad --triplet x64-mingw-dynamic
```

### ビルド例 (MinGW g++)
```
g++ Test.cpp -o Test -std=c++17 -IC:/vcpkg/installed/x64-mingw-dynamic/include -LC:/vcpkg/installed/x64-mingw-dynamic/lib -lglfw3 -lglad -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm -lws2_32
```

### 実行時の注意
vcpkg の DLL を PATH に通してください。
```
set PATH=C:\vcpkg\installed\x64-mingw-dynamic\bin;%PATH%
```

### 備考
- VS でビルドする場合は、CMake + vcpkg の構成が楽です。
