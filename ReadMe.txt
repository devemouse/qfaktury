QFaktury to darmowy system fakturujący pracujący w systemach Windows, Linux i Mac OS.
Umożliwia on drukowanie faktur, faktur pro forma i korekt. Umożliwia zarządzanie bazą faktur, towarów 
i kontrahentów.


Wymagania:
Dowolny Linux, Windows, MacOSX z zainstalowanym >=Qt4.4 

Kompilacja:
Do skompilowania wystarczy wydać dwa polecenia: 
qmake
make

Instalacja
Program można zainstalować przy użyciu polecenia:
sudo make install

Powoduje to skopiowanie wszystkich potrzebnych plików do:
qfaktury - /usr/local/bin
grafiki - /usr/local/share/qfaktury/icons
css - /usr/local/share/qfaktury/templates
jezyki - /usr/local/share/qfaktury


Upgrade do wersji 0.6.2
Przy upgradzie do wersji 0.6.2 konieczne jest dodanie nowej formy płatności np. terminowa. W przeciwnym
wypadku po wybraniu ostatniej formy płatności pojawi się okienko do wystawiania faktur zaliczkowych. 


Ograniczenia!!

1. Duplikat wystawiony nie jest przechowywany w programie. 
2. W przypadku faktur brutto, założenie jest, ze w kartotece towarów ceny są brutto.
3. Do rachunku nie wystawimy duplikatu, ani korekty.




