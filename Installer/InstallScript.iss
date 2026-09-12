; Quack Kitchen 1.0.0 installer, based on the DigiPen TCR sample used by GAM300.

#define GameName "Quack Kitchen"
#define GameVersion "1.0.0"
#define GameExecutable "Quack Kitchen.exe"

[Setup]
AppId={{7BBDAB3A-55D4-4DE7-8889-96BBB475245B}
AppName={#GameName}
AppVersion={#GameVersion}
AppVerName={#GameName} {#GameVersion}
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=https://www.digipen.edu/
AppSupportURL=https://www.digipen.edu/
VersionInfoVersion={#GameVersion}.0
VersionInfoCompany=DigiPen Institute of Technology
VersionInfoDescription={#GameName} installer
VersionInfoProductName={#GameName}
VersionInfoProductVersion={#GameVersion}
DefaultDirName={autopf}\DigiPen\{#GameName}
DefaultGroupName=DigiPen\{#GameName}
UninstallDisplayIcon={app}\{#GameExecutable}
OutputDir=INSTALLER
OutputBaseFilename=Quack_Kitchen_1.0.0_Setup
LicenseFile=INSTALLERFILES\DigiPen_EULA.txt
SetupIconFile=..\Project\DuckEngine\Resources\GameIcon.ico
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
CloseApplications=yes
RestartApplications=no
RestartIfNeededByRun=no
SetupLogging=yes
MinVersion=10.0.17763

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "GAMEDIRECTORY\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "REDIST\VC_redist.x64.exe"; DestDir: "{tmp}"; Flags: ignoreversion deleteafterinstall
Source: "INSTALLERFILES\DigiPen_EULA.txt"; DestDir: "{app}\Licenses"; Flags: ignoreversion

[Icons]
Name: "{group}\{#GameName}"; Filename: "{app}\{#GameExecutable}"; WorkingDir: "{app}"
Name: "{group}\{cm:UninstallProgram,{#GameName}}"; Filename: "{uninstallexe}"; WorkingDir: "{app}"
Name: "{commondesktop}\{#GameName}"; Filename: "{app}\{#GameExecutable}"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{tmp}\VC_redist.x64.exe"; Parameters: "/install /quiet /norestart"; StatusMsg: "Installing Microsoft Visual C++ Runtime..."; Flags: waituntilterminated runhidden
Filename: "{app}\{#GameExecutable}"; Description: "{cm:LaunchProgram,{#GameName}}"; WorkingDir: "{app}"; Flags: nowait postinstall skipifsilent
