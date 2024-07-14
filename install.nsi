; 此脚本文件用于NSIS自动生成安装包程序 created at 2022-3-2 by jeady
; 此脚本一般不做修改，只需要在配套的nsh配置文件中修改相关项即可

; 导入配置文件，命令相关头文件
!include config.nsh
!include "FileFunc.nsh"
!include "WordFunc.nsh"

; 定义一些可选常量
!define PRODUCT_PUBLISHER "珞珈云"
!define PRODUCT_WEB_SITE "https://sync.snakekiss.com/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_ROOT_KEY "HKLM"

SetCompressor lzma
Unicode true

!include "MUI.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "${INSTALLER_ICON}"
!define MUI_UNICON "${INSTALLER_ICON}"

!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; 欢迎页面， 如果需要，可将以下两行取消注释；
; !define MUI_WELCOMEPAGE_TEXT ${UPDATE_LOG}
; !insertmacro MUI_PAGE_WELCOME
; 许可协议页面； 如有需要可取消注释
; !define MUI_LICENSEPAGE_CHECKBOX
; !define MUI_LICENSEPAGE_TEXT_TOP "程序更新日志"
; !insertmacro MUI_PAGE_LICENSE "${EXEC_APP_DIR}\license.txt"
; 组件选择页面； 如需要可自定义
; Page components 

; 安装目录选择页面；如不需要，可以注释掉 / 执行静默安装模式
!define MUI_DIRECTORYPAGE_TEXT_TOP "安装程序将把 ${INSTALLER_NAME} ${PRODUCT_VERSION} 安装到以下目录。请不要更换安装目录。点击[安装(I)]开始安装。"
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面； 如不要可以注释掉
; 添加是否显示更新日志选项
!define MUI_FINISHPAGE_SHOWREADME  $INSTDIR\${README_FILE}
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!define MUI_FINISHPAGE_SHOWREADME_TEXT "显示软件更新日志"
; 添加启动选项
!define MUI_FINISHPAGE_RUN $INSTDIR\${EXEC_APP_NAME}
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "English"


; 安装预释放文件
!insertmacro MUI_RESERVEFILE_LANGDLL
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

; 安装界面的标题文本
Name "${DISPLAY_NAME} ${PRODUCT_VERSION}"
; 最终输出的安装包的名称
OutFile "${INSTALLER_NAME}.exe"
; 默认的安装路径
InstallDir "${DEFAULT_INSTALL_PATH}"
; 卸载信息要写入的注册表路径
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
; 是否执行静默安装，即不显示安装界面
SilentInstall ${INSTALL_TYPE}

; 展示安装细节
ShowInstDetails show
; 展示卸载细节
ShowUnInstDetails show
; 展示在安装界面上的公司名称
BrandingText "珞珈云"



Var isNewer ; 0 相同版本 1 已装新版 2 此为新版
; 在
Section MainSection
; 设置安装包的释放目录，即安装路径
  SetOutPath "$INSTDIR"
  ${If} $isNewer == 2
    SetOverwrite ifnewer
    DetailPrint "Set overwrite ifnewer"
  ${Else}
    SetOverwrite ifdiff
    DetailPrint "Set overwrite ifdiff"
  ${EndIf}
  File /r "${EXEC_APP_DIR}\*"

  CreateDirectory "$SMPROGRAMS\${DISPLAY_NAME}"
  CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\${DISPLAY_NAME}.lnk" "$INSTDIR\${EXEC_APP_NAME}"
  CreateShortCut "$DESKTOP\${DISPLAY_NAME}.lnk" "$INSTDIR\${EXEC_APP_NAME}"
  StrCmp ${BOOT_START} 'true' 0 +2
    CreateShortCut "$SMSTARTUP\${DISPLAY_NAME}.lnk" "$INSTDIR\${EXEC_APP_NAME}"
SectionEnd

; 添加卸载程序，添加网站信息
Section -AdditionalInfo
  WriteUninstaller "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
  StrCmp ${GENERATE_WEBSITE} "true" 0 +3
    WriteIniStr "$INSTDIR\${PRODUCT_PUBLISHER}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
    CreateShortCut "$SMPROGRAMS\${DISPLAY_NAME}\Website.lnk" "$INSTDIR\${PRODUCT_PUBLISHER}.url"
SectionEnd

; 添加软件相关信息记录到注册表
Section -writeReg
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\${EXEC_APP_NAME}"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_DIR_REGKEY}" "updateLog" "${UPDATE_LOG}"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_DIR_REGKEY}" "installerName" "${INSTALLER_NAME} ${PRODUCT_VERSION}"

  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${DISPLAY_NAME}"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

  WriteRegExpandStr  HKCR "*\shell\${EXEC_APP_NAME}" "" "查看历史版本"
  WriteRegExpandStr  HKCR "*\shell\${EXEC_APP_NAME}" "Icon" "$INSTDIR\${EXEC_APP_NAME}"
  WriteRegExpandStr  HKCR "*\shell\${EXEC_APP_NAME}\command" "" `"$INSTDIR\${EXEC_APP_NAME}" "ver" "%1"`

  ;Now for HKEY_CLASSES_ROOT\Directory\shell
  WriteRegStr HKCR "Directory\shell\Open with ${PRODUCT_NAME}" "" "添加到 ${DISPLAY_NAME} 同步文件夹"
  WriteRegStr HKCR "Directory\shell\Open with ${PRODUCT_NAME}\command" "" `"$INSTDIR\${EXEC_APP_NAME}" "add" "%1"`

  ;Now for HKEY_CLASSES_ROOT\Directory\Background\shell
  WriteRegStr HKCR "Directory\Background\shell\Open with ${PRODUCT_NAME}" "" "添加到 ${DISPLAY_NAME} 同步文件夹"
  WriteRegStr HKCR "Directory\Background\shell\Open with ${PRODUCT_NAME}\command" "" `"$INSTDIR\${EXEC_APP_NAME}" "add" "%1"`

  ;HKEY_CURRENT_USER\Software\Classes\directory\Background\shell
  WriteRegStr HKCU "Software\Classes\directory\Background\shell\Open with ${PRODUCT_NAME}" "" "添加到 ${DISPLAY_NAME} 同步文件夹"
  WriteRegStr HKCU "Software\Classes\directory\Background\shell\Open with ${PRODUCT_NAME}\command" "" `"$INSTDIR\${EXEC_APP_NAME}" "add" "%1"`
  CALL rebootDialog
SectionEnd

Function checkVersion
  Var /GLOBAL lastVersion
  ReadRegStr $lastVersion HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion"
  ${VersionCompare} "$lastVersion" "${PRODUCT_VERSION}" $0
  ${Switch} $0
  ${Case} 0
    MessageBox MB_YESNO "当前版本已经安装了, 是否停止安装?" IDNO continue1
    Quit
    continue1:
    StrCpy $isNewer 0
    Goto finishcheck
  ${Case} 1
    MessageBox MB_YESNO "已安装了新的版本 $lastVersion，是否停止当前版本的安装?" IDNO continue2
    Quit
    continue2:
    StrCpy $isNewer 1
    Goto finishcheck
  ${Case} 2
    StrCpy $isNewer 2
  ${EndSwitch}
  finishcheck:
FunctionEnd
; 提示重启的函数
Function rebootDialog
  StrCmp ${NEED_REBOOT} "true" 0 ret
  MessageBox MB_YESNO "是否立即重启" IDNO ret
  Reboot
  ret:
  DetailPrint "安装完成"
FunctionEnd


Function .onInstSuccess
  StrCmp ${USER_DEFINED_CMD} "null" doneInst 0
  Exec "${USER_DEFINED_CMD}"
  doneInst:
    ${If} ${INSTALL_TYPE} == "silent"
      Exec "$INSTDIR\${EXEC_APP_NAME}"
    ${EndIf}
FunctionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/
Section Uninstall
	; 删除安装目录、开始菜单及桌面快捷方式
  RMDir /r /REBOOTOK "$INSTDIR"
  RMDir /r /REBOOTOK "$SMPROGRAMS\${DISPLAY_NAME}"
  Delete "$DESKTOP\${DISPLAY_NAME}.lnk"
	
	; 删除相关注册表项
  DeleteRegKey ${PRODUCT_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  DeleteRegKey HKCR "*\shell\${EXEC_APP_NAME}"
  DeleteRegKey HKCR "Directory\shell\Open with ${PRODUCT_NAME}"
  DeleteRegKey HKCR "Directory\Background\shell\Open with ${PRODUCT_NAME}"
  SetAutoClose true
SectionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 ${DISPLAY_NAME} ${PRODUCT_VERSION} 及其所有的组件？" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${DISPLAY_NAME} ${PRODUCT_VERSION} 已成功地从您的计算机移除。"
FunctionEnd

