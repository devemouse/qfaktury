# #####################################################################
# Automatically generated by qmake (2.01a) Tue Mar 17 19:44:38 2009
# #####################################################################
TEMPLATE = app
TARGET = qfaktury
DEPENDPATH += .
INCLUDEPATH += .
QT += gui xml xmlpatterns sql
CONFIG += debug

# Input
HEADERS += ./src/XmlDataLayer.h \
    ./src/KontrData.h \
    ./src/IDataLayer.h \
    ./src/Const.h \
    ./src/CustomPaymData.h \
    ./src/CustomPayment.h \
    ./src/KorektaBrutto.h \
    ./src/TowaryBruttoLista.h \
    ./src/Rachunek.h \
    ./src/FakturaBrutto.h \
    ./src/Duplikat.h \
    ./src/ConvertAmount.h \
    ./src/config.h \
    ./src/Faktura.h \
    ./src/InvoiceData.h \
    ./src/Kontrahenci.h \
    ./src/KontrahenciLista.h \
    ./src/Korekta.h \
    ./src/old_MainWindow.h \
    ./src/ProductData.h \
    ./src/Settings.h \
    ./src/Towary.h \
    ./src/TowaryLista.h \
    ./src/Ustawienia.h \
    ./src/Uzytkownik.h \
    ./src/ZmienIlosc.h \
    src/MainWindow.h \
    src/InvoicesModel.h \
    src/InvoiceItem.h \
    src/helper_functions.h \
    src/ContractorsModel.h \
    src/DateFilterProxyModelWidget.h \
    src/DateFilterProxyModel.h \
    src/CustomTableView.h \
    src/FilterAwareTableModel.h \
    src/WidgetProxyModel.h \
    src/TextFilterProxyModel.h \
    src/DatabaseManager.h
FORMS += ./ui/CustomPaymentDialog.ui \
    ./ui/Faktura.ui \
    ./ui/Kontrahenci.ui \
    ./ui/KontrahenciLista.ui \
    ./ui/old_MainWindow.ui \
    ./ui/Towary.ui \
    ./ui/TowaryLista.ui \
    ./ui/Ustawienia.ui \
    ./ui/Uzytkownik.ui \
    ./ui/ZmienIlosc.ui \
    ui/MainWindow.ui \
    ui/DateFilterProxyModel.ui \
    ui/DateFilterProxyModelWidget.ui \
    ui/CustomTableView.ui
SOURCES += ./src/XmlDataLayer.cpp \
    ./src/KontrData.cpp \
    ./src/CustomPaymData.cpp \
    ./src/CustomPayment.cpp \
    ./src/KorektaBrutto.cpp \
    ./src/TowaryBruttoLista.cpp \
    ./src/Rachunek.cpp \
    ./src/FakturaBrutto.cpp \
    ./src/Duplikat.cpp \
    ./src/ConvertAmount.cpp \
    ./src/Faktura.cpp \
    ./src/Kontrahenci.cpp \
    ./src/KontrahenciLista.cpp \
    ./src/Korekta.cpp \
    ./src/main.cpp \
    ./src/old_MainWindow.cpp \
    ./src/Towary.cpp \
    ./src/TowaryLista.cpp \
    ./src/Ustawienia.cpp \
    ./src/Uzytkownik.cpp \
    ./src/ZmienIlosc.cpp \
    src/MainWindow.cpp \
    src/InvoicesModel.cpp \
    src/InvoiceItem.cpp \
    src/helper_functions.cpp \
    src/ContractorsModel.cpp \
    src/DateFilterProxyModelWidget.cpp \
    src/DateFilterProxyModel.cpp \
    src/CustomTableView.cpp \
    src/TextFilterProxyModel.cpp \
    src/DatabaseManager.cpp
RESOURCES += qfaktury.qrc
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
RCC_DIR = .rcc
TRANSLATIONS += qfaktury_de.ts \
    qfaktury_en.ts \
    qfaktury_pl.ts
pixmaps.path = /usr/local/share/qfaktury/icons
pixmaps.files = icons/*.png
css.path = /usr/local/share/qfaktury/templates
css.files = templates/*.css
languages.path = /usr/local/share/qfaktury
languages.files = *.qm
desktop.path = /usr/local/share/applications
desktop.files = *.desktop
target.path = /usr/local/bin
INSTALLS += target \
    pixmaps \
    css \
    languages \
    desktop




























