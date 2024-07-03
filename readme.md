# QSyncUi

## 编译环境
1. 准备`Qt6.7.2 MSVC2019`, `Visual Studio 2022`编译环境

2. 配置`aws-sdk-cpp`环境

    1. 使用管理员权限打开`x64 Native Tools Command Prompt for VS 2022 Preview`
    2. 下载`aws-sdk-cpp`源码
    ```shell
    git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
    ```
    3. 切换到目录
    ```shell
    cd aws-sdk-cpp
    mkdir build
    cd build
    ```
    4. 编译并安装
    ```shell
    cmake ..  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_INSTALL_PREFIX="C:/Program Files (x86)/aws" -DBUILD_ONLY="s3"
    cmake --build . --config=Debug
    cmake --install . --config=Debug
    ```

3. 编译本项目
    
    1. 使用`Visual Studio 2022`打开`CMakeLists.txt`
    2. 配置Qt插件
    3. 编译

    或

    1. 使用Qt Creator打开`CMakeLists.txt`
