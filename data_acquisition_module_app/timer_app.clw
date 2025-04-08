; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTimer_appDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "timer_app.h"

ClassCount=11
Class1=CTimer_appApp
Class2=CTimer_appDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_ADVANCED_SETUP
Class4=configure_dlg
Resource4=IDD_TIMER_APP_DIALOG
Class5=flash_dlg
Resource5=IDD_DIALOG_MESSAGE_BOX
Class6=synch_dlg
Resource6=IDD_DIALOG_SYNCH
Class7=default_dlg
Resource7=IDD_CONFIGURE_DIALOG
Resource8=IDD_DIALOG_INPUT_BOX
Class8=CInputBox
Class9=CAdvancedSetup
Class10=CDialogAdvancedSetup
Class11=c_event_and_heat_dlg
Resource9=IDD_DIALOG_SET_EVENT_AND_HEAT

[CLS:CTimer_appApp]
Type=0
HeaderFile=timer_app.h
ImplementationFile=timer_app.cpp
Filter=N

[CLS:CTimer_appDlg]
Type=0
HeaderFile=timer_appDlg.h
ImplementationFile=timer_appDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTimer_appDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=timer_appDlg.h
ImplementationFile=timer_appDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TIMER_APP_DIALOG]
Type=1
Class=CTimer_appDlg
ControlCount=30
Control1=IDC_EDIT_DATABASE_TEXT_BOX,edit,1350633668
Control2=IDC_BUTTON_CONFIGURE,button,1342242816
Control3=IDC_STATIC_TIME_OF_DAY,static,1342308352
Control4=IDC_BUTTON_SYNCH_TIME,button,1342242816
Control5=IDC_BUTTON_RESET_TIMERS,button,1342242816
Control6=IDC_EDIT_TIME_OF_DAY_STRING,edit,1484783744
Control7=IDC_EDIT_TERMINAL_TEXT_BOX,edit,1350633668
Control8=IDC_EDIT_TEXT_FIELD_DESCRIPTION,edit,1350633600
Control9=IDC_STATIC_TERMINAL,static,1342308352
Control10=IDC_STATIC_DATA_PACKETS,static,1342308352
Control11=IDC_BUTTON_RE_TRANSMIT,button,1342242816
Control12=IDC_BUTTON_EXIT,button,1342242816
Control13=IDC_CHECK_LINK_TO_DATABASE,button,1342242819
Control14=IDC_CHECK_COMMUNICATE_WITH_TIMERS,button,1342242819
Control15=IDC_BUTTON_DELETE_RECORDS,button,1342242816
Control16=IDC_BUTTON_DO_NOTHING,button,1073807361
Control17=IDC_BUTTON_GENERATE_DUMMY_RECORD,button,1342242816
Control18=IDC_BUTTON_TEST,button,1073807360
Control19=IDC_EDIT_EVENT,edit,1484849280
Control20=IDC_STATIC_EVENT,static,1342308352
Control21=IDC_BUTTON_RESET_X,button,1342242816
Control22=IDC_EDIT_VERSION,edit,1476395136
Control23=IDC_EDIT_HEAT,edit,1484849280
Control24=IDC_STATIC_HEAT,static,1342308352
Control25=IDC_BUTTON_SEND_EVENT_AND_HEAT_BUTTON,button,1342242816
Control26=IDC_EDIT_COMM_STATUS,edit,1350633600
Control27=IDC_EDIT_LINK_STATUS,edit,1350633600
Control28=IDC_STATIC_COMM_STATUS,static,1342308352
Control29=IDC_STATIC_LINK_STATUS,static,1342308352
Control30=IDC_BUTTON_DELETE_BACKUP_RECORDS,button,1342242816

[CLS:configure_dlg]
Type=0
HeaderFile=configure_dlg.h
ImplementationFile=configure_dlg.cpp
BaseClass=CDialog
Filter=D
LastObject=configure_dlg
VirtualFilter=dWC

[CLS:flash_dlg]
Type=0
HeaderFile=flash_dlg.h
ImplementationFile=flash_dlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_PERCENT_DONE
VirtualFilter=dWC

[DLG:IDD_CONFIGURE_DIALOG]
Type=1
Class=configure_dlg
ControlCount=26
Control1=IDOK,button,1342242816
Control2=IDC_EDIT_NUMBER_OF_DEVICES,edit,1350631552
Control3=IDC_COMBO_COMM_PORT,combobox,1344339971
Control4=IDC_STATIC_COMM_PORT,static,1342308352
Control5=IDC_STATIC_NUMBER_OF_DEVICES,static,1342308352
Control6=IDC_STATIC_DATABASE_TABLE_NAME,static,1342308352
Control7=IDC_BUTTON_RESTORE_DEFAULT_VALUES,button,1342242816
Control8=IDC_EDIT_CURRENT_WORKING_DIRECTORY,edit,1350633600
Control9=IDC_STATIC_CURRENT_WORKING_DIRECTORY,static,1342308352
Control10=IDC_STATIC_DATABASE_NAME,static,1342308352
Control11=IDC_EDIT_DATABASE_DIRECTORY,edit,1350631552
Control12=IDC_STATIC_DATABASE_DIRECTORY,static,1342308352
Control13=IDC_EDIT_DATABASE_DRIVE,edit,1350631552
Control14=IDC_STATIC_DATABASE_DRIVE,static,1342308352
Control15=IDC_COMBO_DATABASE_TYPE,combobox,1344339971
Control16=IDC_STATIC_DATABASE_TYPE,static,1342308352
Control17=IDC_EDIT_DATABASE_PATH_AND_FILE_NAME,edit,1350633600
Control18=IDC_STATIC_DATABASE_DRIVE2,static,1342308352
Control19=IDC_BUTTON_ERNIES_CANCEL,button,1342242816
Control20=IDC_STATIC_TIMER_TYPE,static,1342308352
Control21=IDC_COMBO_TIMER_TYPE_USED,combobox,1342242819
Control22=IDC_EDIT_DATABASE_TABLE_NAME,edit,1350631552
Control23=IDC_EDIT_DATABASE_NAME,edit,1350631552
Control24=IDC_BUTTON_ADVANCED,button,1342242816
Control25=IDC_STATIC_DATABASE_NAME2,static,1342308352
Control26=IDC_EDIT_DATABASE_EXTENSION,edit,1350631552

[CLS:synch_dlg]
Type=0
HeaderFile=synch_dlg.h
ImplementationFile=synch_dlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_ADJUST_SYNCH_TIME
VirtualFilter=dWC

[DLG:IDD_DIALOG_SYNCH]
Type=1
Class=synch_dlg
ControlCount=6
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BUTTON_SYNCH_TO_SYSTEM,button,1342242816
Control4=IDC_EDIT_USER_SYNCH_TIME,edit,1350631552
Control5=IDC_BUTTON_SYNCH_TO_USER_TIME,button,1342242816
Control6=IDC_BUTTON_ADJUST_SYNCH_TIME,button,1342242816

[CLS:default_dlg]
Type=0
HeaderFile=default_dlg.h
ImplementationFile=default_dlg.cpp
BaseClass=CDialog
Filter=D
LastObject=default_dlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_INPUT_BOX]
Type=1
Class=CInputBox
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_VALUE,edit,1350631552
Control4=IDC_EDIT_MESSAGE,edit,1350631552

[DLG:IDD_DIALOG_MESSAGE_BOX]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT_MESSAGE,edit,1350631552

[CLS:CInputBox]
Type=0
HeaderFile=InputBox.h
ImplementationFile=InputBox.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_MESSAGE

[CLS:CAdvancedSetup]
Type=0
HeaderFile=AdvancedSetup.h
ImplementationFile=AdvancedSetup.cpp
BaseClass=CDialog
Filter=D
LastObject=CAdvancedSetup

[DLG:IDD_DIALOG_ADVANCED_SETUP]
Type=1
Class=CDialogAdvancedSetup
ControlCount=32
Control1=IDOK,button,1342242817
Control2=IDC_EDIT_DEVICE_INITIAL_RECORD1,edit,1350631552
Control3=IDC_EDIT_DEVICE_INITIAL_RECORD2,edit,1350631552
Control4=IDC_EDIT_DEVICE_INITIAL_RECORD3,edit,1350631552
Control5=IDC_EDIT_DEVICE_INITIAL_RECORD4,edit,1350631552
Control6=IDC_EDIT_DEVICE_INITIAL_RECORD5,edit,1350631552
Control7=IDC_EDIT_DEVICE_INITIAL_RECORD6,edit,1350631552
Control8=IDC_EDIT_DEVICE_INITIAL_RECORD7,edit,1350631552
Control9=IDC_EDIT_DEVICE_INITIAL_RECORD8,edit,1350631552
Control10=IDC_EDIT_DEVICE_INITIAL_RECORD9,edit,1350631552
Control11=IDC_STATIC_DEVICE_NUMBER1,static,1342308352
Control12=IDC_STATIC_DEVICE_NUMBER2,static,1342308352
Control13=IDC_STATIC_DEVICE_NUMBER3,static,1342308352
Control14=IDC_STATIC_DEVICE_NUMBER4,static,1342308352
Control15=IDC_STATIC_DEVICE_NUMBER5,static,1342308352
Control16=IDC_STATIC_DEVICE_NUMBER6,static,1342308352
Control17=IDC_STATIC_DEVICE_NUMBER7,static,1342308352
Control18=IDC_STATIC_DEVICE_NUMBER10,static,1342308352
Control19=IDC_STATIC_DEVICE_NUMBER11,static,1342308352
Control20=IDC_EDIT_TALK_TIME,edit,1350631552
Control21=IDC_STATIC_TALK_TIME,static,1342308352
Control22=IDC_EDIT_BAUD_RATE,edit,1350631552
Control23=IDC_STATIC_BAUD_RATE,static,1342308352
Control24=IDC_CHECK_OUTPUT_DIAGNOSTIC_FILES,button,1342242819
Control25=IDC_STATIC_ADVANCED_CONFIGURATION,static,1342308352
Control26=IDC_BUTTON_ERNIES_CANCEL,button,1342242816
Control27=IDC_EDIT_DATABASE_HIGHEST_RECORD_NUMBER,edit,1350631552
Control28=IDC_STATIC_DATABASE,static,1342308352
Control29=IDC_CHECK_USER_DSN_FLAG,button,1342242819
Control30=IDC_EDIT_ODBC_USER_DATA_SOURCE_NAME,edit,1350631552
Control31=IDC_STATIC_ODBC_DATA_SOURSE_NAME,static,1342308352
Control32=IDC_STATIC_HIGHEST_RECORD,static,1342308352

[CLS:CDialogAdvancedSetup]
Type=0
HeaderFile=DialogAdvancedSetup.h
ImplementationFile=DialogAdvancedSetup.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogAdvancedSetup
VirtualFilter=dWC

[DLG:IDD_DIALOG_SET_EVENT_AND_HEAT]
Type=1
Class=c_event_and_heat_dlg
ControlCount=9
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT_DEVICE,edit,1350631552
Control3=IDC_EDIT_EVENT,edit,1350762624
Control4=IDC_EDIT_HEAT,edit,1350631552
Control5=IDC_STATIC_EVENT,static,1342308352
Control6=IDC_STATIC_HEAT,static,1342308352
Control7=IDC_STATIC_DEVICE,static,1342308352
Control8=IDC_BUTTON_SEND_TO_EVERY_DEVICE,button,1342242816
Control9=IDC_BUTTON_SEND_TO_ONE_DEVICE,button,1342242816

[CLS:c_event_and_heat_dlg]
Type=0
HeaderFile=event_and_heat_dlg.h
ImplementationFile=event_and_heat_dlg.cpp
BaseClass=CDialog
LastObject=IDC_BUTTON_SEND_TO_EVERY_DEVICE
Filter=D
VirtualFilter=dWC

