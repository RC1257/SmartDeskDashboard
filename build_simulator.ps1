$ProjectRoot = $PSScriptRoot

cmake `
  -S "$ProjectRoot\simulator" `
  -B "$ProjectRoot\simulator\build" `
  -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="$ProjectRoot\tools\vcpkg\scripts\buildsystems\vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  -DSDL2_DIR="$ProjectRoot\tools\vcpkg\installed\x64-windows\share\sdl2"

cmake --build "$ProjectRoot\simulator\build" --config Debug
