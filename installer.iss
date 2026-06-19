; OS DropZone Inno Setup Installer Script
; For packaging the Windows version of cross-os

[Setup]
AppName=OS DropZone
AppVersion=1.0
AppPublisher=Antigravity
DefaultDirName={userappdata}\OSDropZone
DefaultGroupName=OS DropZone
DisableProgramGroupPage=yes
OutputDir=dist-windows
OutputBaseFilename=OS_DropZone_Windows_Setup
SetupIconFile=icon.ico
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
UninstallDisplayIcon={app}\cross-os.exe

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; The executable
Source: "build-windows\Release\cross-os.exe"; DestDir: "{app}"; Flags: ignoreversion
; Icon
Source: "icon.ico"; DestDir: "{app}"; Flags: ignoreversion
; Bundle everything inside build-windows\Release which was gathered by windeployqt
Source: "build-windows\Release\*"; Excludes: "cross-os.exe,*.obj,*.cpp,*.h,*.rc,*.log"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\OS DropZone"; Filename: "{app}\cross-os.exe"
Name: "{autodesktop}\OS DropZone"; Filename: "{app}\cross-os.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\cross-os.exe"; Description: "{cm:LaunchProgram,OS DropZone}"; Flags: nowait postinstall skipifsilent
