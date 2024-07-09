REM 设置构建类型，这里假设您已经通过某种方式设置了BUILD_TYPE环境变量
set BUILD_TYPE=Release

REM 运行CMake配置
cmake -B build -S . -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH=%QTDIR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

REM 构建项目
cmake --build build --config %BUILD_TYPE% --parallel 32

REM 使用windeployqt
windeployqt.exe build/%BUILD_TYPE%/QSyncUi.exe

