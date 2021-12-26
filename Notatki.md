# Notatki

## Cel

1) Zrozumieć jak działa program i napisać opis teoretyczny
2) Napisać własny aproksymator.c 
3) Integracja naszego aproksymator.c ze splines.c
4) Poprawa nieczytelności już gotowych modułów
5) Zmiana makefile
6) Napisanie testow
7) Porownanie dzialania z akprosykamotor_na_bazie.c
8) Opis wywolania w main'ie
9) Sprawozdanie + wnioski + wycieki pamieci itd.

## Działanie odpowiednich plików

- Points (points.h i points.c) zajmie się przechowywaniem punktów pomiarowych,
- Splines (splines.h i splines.c) będzie odpowiadał za przechowywanie aproksymacji - wykorzystamy do tego funkcje sklejane, czyli wielomiany niskich rzędów zdefiniowane na pod-przedziałach interesującego nas zakresu. Wykorzystując wzór Taylora będziemy reprezentować wielomian w każdym przedziale przez wartość funkcji i jej pochodnych na początku przedziału.
- Main (main.c) będzie odpowiadał za sterowanie - odczytywał intencje użytkownika i wywoływał pozostałe moduły.
- Maker będzie generował aproksymację na podstawie punktów.
- prosta.c - aproksymuje wszystkie punkty za pomocą jednej prostej przechodzącej przez pierwszy i ostatni punkt
- interpolator.c - interpoluje punkty za pomocą wielomianu Lagrange'a

aproksymator_na_bazie.c - aproksymuje punkty przez minimalizację błędu średnio-kwadratowego za pomocą zespołu funkcji bazowych opisanych przed dr. Chwieja (patrz wyżej).

## Materiały

- http://wikidyd.iem.pw.edu.pl/LMP(2f)10.html
- http://galaxy.agh.edu.pl/~chwiej/mn/aproksymacja.pdf
- https://home.agh.edu.pl/~chwiej/mn/interpolacja_2016.pdf
- https://home.agh.edu.pl/~dpawlus/pliki/matlab/MO_interp_apr.pdf
- https://pl.wikipedia.org/wiki/Wielomiany_Czebyszewa
