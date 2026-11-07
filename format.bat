@echo off
setlocal

set "CLANG_FORMAT=clang-format"
where clang-format >nul 2>&1
if errorlevel 1 (
    set "CLANG_FORMAT="
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
        for /f "usebackq delims=" %%I in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -products * -property installationPath`) do (
            if exist "%%I\VC\Tools\Llvm\bin\clang-format.exe" set "CLANG_FORMAT=%%I\VC\Tools\Llvm\bin\clang-format.exe"
        )
    )
)

if not defined CLANG_FORMAT (
    echo clang-format was not found. Install LLVM or add clang-format to PATH.
    exit /b 1
)

echo Using clang-format: "%CLANG_FORMAT%"

for %%D in ("%~dp0Engine" "%~dp0SFMLTemplate" "%~dp0EngineTest") do (
    pushd "%%~D"
    for /R %%F in (*.cpp) do "%CLANG_FORMAT%" -i "%%F"
    for /R %%F in (*.h) do "%CLANG_FORMAT%" -i "%%F"
    popd
)

echo C++ source files formatted.
