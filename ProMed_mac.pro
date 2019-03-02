#-------------------------------------------------
#
# Project created by QtCreator 2010-10-11T16:14:09
#
#-------------------------------------------------

QT       += core widgets printsupport gui sql xml webenginewidgets network

TARGET = ProMed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widget_addpatient.cpp \
    dbconn.cpp \
    obj_patient.cpp \
    widget_listpatients.cpp \
    widget_viewpatient.cpp \
    obj_operation.cpp \
    obj_appointment.cpp \
    obj_setting.cpp \
    dialog_addoperation.cpp \
    dialog_viewoperation.cpp \
    dialog_addappointment.cpp \
    dialog_generaldialog.cpp \
    widget_bigcalendar.cpp \
    widget_statistics.cpp \
    widget_dentalsettings.cpp \
    obj_payment.cpp \
    obj_image.cpp \
    dialog_paymentdlg.cpp \
    dialog_addimage.cpp \
    tool_dropimagetoolbutton.cpp \
    dialog_viewimage.cpp \
    widget_login.cpp \
    dialog_aboutdialog.cpp \
    tool_recompilecrypto.cpp \
    dialog_registerdialog.cpp \
    obj_prescription.cpp \
    dialog_printdialog.cpp \
    tool_backupengine.cpp \
    widget_backupui.cpp \
    widget_internalbrowser.cpp \
    dialog_prescriptiondialog.cpp \
    dialog_patienthistory.cpp \
    obj_patienthistory.cpp \
    tool_registration.cpp \
    dialog_selectdialog.cpp

HEADERS  += mainwindow.h \
    widget_addpatient.h \
    dbconn.h \
    obj_patient.h \
    widget_listpatients.h \
    widget_viewpatient.h \
    obj_operation.h \
    obj_appointment.h \
    obj_setting.h \
    dialog_addoperation.h \
    dialog_viewoperation.h \
    dialog_addappointment.h \
    dialog_generaldialog.h \
    widget_bigcalendar.h \
    widget_statistics.h \
    widget_dentalsettings.h \
    obj_payment.h \
    obj_image.h \
    dialog_paymentdlg.h \
    dialog_addimage.h \
    tool_dropimagetoolbutton.h \
    dialog_viewimage.h \
    widget_login.h \
    dialog_aboutdialog.h \
    tool_recompilecrypto.h \
    dialog_registerdialog.h \
    obj_prescription.h \
    dialog_printdialog.h \
    tool_backupengine.h \
    widget_backupui.h \
    widget_internalbrowser.h \
    dialog_prescriptiondialog.h \
    dialog_patienthistory.h \
    obj_patienthistory.h \
    tool_registration.h \
    dialog_selectdialog.h

FORMS    += mainwindow.ui \
    widget_addpatient.ui \
    widget_listpatients.ui \
    widget_viewpatient.ui \
    dialog_addoperation.ui \
    dialog_viewoperation.ui \
    dialog_addappointment.ui \
    dialog_generaldialog.ui \
    widget_bigcalendar.ui \
    widget_statistics.ui \
    widget_dentalsettings.ui \
    dialog_paymentdlg.ui \
    dialog_addimage.ui \
    dialog_viewimage.ui \
    widget_login.ui \
    dialog_aboutdialog.ui \
    dialog_registerdialog.ui \
    dialog_printdialog.ui \
    widget_backupui.ui \
    widget_internalbrowser.ui \
    dialog_prescriptiondialog.ui \
    dialog_patienthistory.ui \
    dialog_selectdialog.ui

TRANSLATIONS	+= \
    promed_en.ts \
    promed_el.ts \
    promed_fr.ts

CODECFORSRC = UTF-8

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    ProMed.dpdata \
    promed_fr.ts \
    promed_en.ts \
    promed_el.ts

ICON = promed.icns

#QMAKE_MAC_SDK = /Volumes/Shared/Qt/SDK-5.12.1
