# Microsoft Developer Studio Project File - Name="xrick" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=xrick - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xrick.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xrick.mak" CFG="xrick - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xrick - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xrick - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xrick - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "xrick - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "xrick - Win32 Release"
# Name "xrick - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\control.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_ents.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_maps.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_picsPC.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_picsST.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_screens.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_snd.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_spritesPC.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_spritesST.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_tilesPC.c
# End Source File
# Begin Source File

SOURCE=.\src\dat_tilesST.c
# End Source File
# Begin Source File

SOURCE=.\src\data.c
# End Source File
# Begin Source File

SOURCE=.\src\devtools.c
# End Source File
# Begin Source File

SOURCE=.\src\draw.c
# End Source File
# Begin Source File

SOURCE=.\src\e_bomb.c
# End Source File
# Begin Source File

SOURCE=.\src\e_bonus.c
# End Source File
# Begin Source File

SOURCE=.\src\e_box.c
# End Source File
# Begin Source File

SOURCE=.\src\e_bullet.c
# End Source File
# Begin Source File

SOURCE=.\src\e_rick.c
# End Source File
# Begin Source File

SOURCE=.\src\e_sbonus.c
# End Source File
# Begin Source File

SOURCE=.\src\e_them.c
# End Source File
# Begin Source File

SOURCE=.\src\ents.c
# End Source File
# Begin Source File

SOURCE=.\src\game.c
# End Source File
# Begin Source File

SOURCE=.\src\maps.c
# End Source File
# Begin Source File

SOURCE=.\src\rects.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_gameover.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_getname.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_imain.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_imap.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_pause.c
# End Source File
# Begin Source File

SOURCE=.\src\scr_xrick.c
# End Source File
# Begin Source File

SOURCE=.\src\scroller.c
# End Source File
# Begin Source File

SOURCE=.\src\sysarg.c
# End Source File
# Begin Source File

SOURCE=.\src\sysevt.c
# End Source File
# Begin Source File

SOURCE=.\src\sysjoy.c
# End Source File
# Begin Source File

SOURCE=.\src\syskbd.c
# End Source File
# Begin Source File

SOURCE=.\src\syssnd.c
# End Source File
# Begin Source File

SOURCE=.\src\system.c
# End Source File
# Begin Source File

SOURCE=.\src\sysvid.c
# End Source File
# Begin Source File

SOURCE=.\src\unzip.c
# End Source File
# Begin Source File

SOURCE=.\src\util.c
# End Source File
# Begin Source File

SOURCE=.\src\xrick.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\config.h
# End Source File
# Begin Source File

SOURCE=.\include\control.h
# End Source File
# Begin Source File

SOURCE=.\include\data.h
# End Source File
# Begin Source File

SOURCE=.\include\debug.h
# End Source File
# Begin Source File

SOURCE=.\include\devtools.h
# End Source File
# Begin Source File

SOURCE=.\include\draw.h
# End Source File
# Begin Source File

SOURCE=.\include\e_bomb.h
# End Source File
# Begin Source File

SOURCE=.\include\e_bonus.h
# End Source File
# Begin Source File

SOURCE=.\include\e_box.h
# End Source File
# Begin Source File

SOURCE=.\include\e_bullet.h
# End Source File
# Begin Source File

SOURCE=.\include\e_rick.h
# End Source File
# Begin Source File

SOURCE=.\include\e_sbonus.h
# End Source File
# Begin Source File

SOURCE=.\include\e_them.h
# End Source File
# Begin Source File

SOURCE=.\include\ents.h
# End Source File
# Begin Source File

SOURCE=.\include\game.h
# End Source File
# Begin Source File

SOURCE=.\include\img.h
# End Source File
# Begin Source File

SOURCE=.\include\maps.h
# End Source File
# Begin Source File

SOURCE=.\include\pics.h
# End Source File
# Begin Source File

SOURCE=.\include\rects.h
# End Source File
# Begin Source File

SOURCE=.\include\screens.h
# End Source File
# Begin Source File

SOURCE=.\include\scroller.h
# End Source File
# Begin Source File

SOURCE=.\include\sprites.h
# End Source File
# Begin Source File

SOURCE=.\include\syssnd.h
# End Source File
# Begin Source File

SOURCE=.\include\system.h
# End Source File
# Begin Source File

SOURCE=.\include\sysvid.h
# End Source File
# Begin Source File

SOURCE=.\include\tiles.h
# End Source File
# Begin Source File

SOURCE=.\include\unzip.h
# End Source File
# Begin Source File

SOURCE=.\include\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\xrick.rc
# End Source File
# Begin Source File

SOURCE=.\src\xrickPC.ico
# End Source File
# Begin Source File

SOURCE=.\src\xrickST.ico
# End Source File
# End Group
# End Target
# End Project
