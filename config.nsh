# 本配置文件/nsis头文件用于NSIS自动生成安装包程序 created at 2022-2-15 by jeady
; 源程序信息
!define EXEC_APP_DIR ".\build"  ; 要打包的目录，执行文件前应删除目录下的非必需文件，如日志等
!define EXEC_APP_NAME "QSyncUi.exe"     ; 可执行程序相对于打包目录的路经
;!define PRODUCT_VERSION "v1.0.0"       ; 应用程序的版本号
!define UPDATE_LOG "测试打包用的可执行文件"     ; 本次更新的简要描述

; 安装目标信息
!define DEFAULT_INSTALL_PATH  "D:\LogiCloud"  ; 默认安装目录； 字符串最后不带反斜杠
!define PRODUCT_NAME "QSyncUi"    ; 可运行程序的的名字； 将用于注册表和进程检测
!define DISPLAY_NAME "珞珈云"   ; 开始菜单中/桌面快捷方式/开机启动项要显示的名字
!define README_FILE "UpdateLog.txt" ; 安装完成后要显示的更新日志


; 安装包文件信息
!define INSTALLER_NAME "LogiCloud" ; 安装包的前缀名字
!define INSTALLER_ICON ".\icon.ico"    ; 安装包的图标; 如需替换，请修改文件路径

; 其他选项
!define BOOT_START "true"          ; 是否创建软件开机自启动链接
!define INSTALL_TYPE "normal"      ; 安装方式 normal | silent
!define NEED_REBOOT "false"         ; 安装完成后是否需要提示重新启动计算机
!define GENERATE_WEBSITE "false"    ; 安装目录下是否创建公司官网链接
!define USER_DEFINED_CMD "null" ; 安装完成后要执行的自定义指令，不执行则为空null

