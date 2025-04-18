# Microsoft Developer Studio Project File - Name="lry_comm_static_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lry_comm_static_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lry_comm_static_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lry_comm_static_lib.mak" CFG="lry_comm_static_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lry_comm_static_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "lry_comm_static_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lry_comm_static_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "e:\obj\original_daq_module\lry_com_static_lib\release"
# PROP Intermediate_Dir "e:\obj\original_daq_module\lry_com_static_lib\release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "F:\cvs_sandbox\software\original_srt_data_acquisition_module\code\lry_comm" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "lry_comm_static_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "e:\obj\alpine_trainer\lry_com_static_lib\debug"
# PROP Intermediate_Dir "e:\obj\alpine_trainer\lry_com_static_lib\debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "f:\lry_comm" /I "f:\srt_timer_test" /I "F:\cvs_sandbox\software\original_srt_data_acquisition_module\code\lry_comm" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "lry_comm_static_lib - Win32 Release"
# Name "lry_comm_static_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lry_comm\backgrnd.c
# End Source File
# Begin Source File

SOURCE=..\lry_comm\commbufs.c
# End Source File
# Begin Source File

SOURCE=..\lry_comm\commport.c
# End Source File
# Begin Source File

SOURCE=..\lry_comm\errors.c
# End Source File
# Begin Source File

SOURCE=..\lry_comm\fifo.c
# End Source File
# Begin Source File

SOURCE=..\lry_comm\winfile.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\lry_comm\backgrnd.h
# End Source File
# Begin Source File

SOURCE=..\lry_comm\commbufs.h
# End Source File
# Begin Source File

SOURCE=..\lry_comm\commport.h
# End Source File
# Begin Source File

SOURCE=..\lry_comm\errors.h
# End Source File
# Begin Source File

SOURCE=..\lry_comm\fifo.h
# End Source File
# Begin Source File

SOURCE=..\lry_comm\winfile.h
# End Source File
# End Group
# End Target
# End Project
