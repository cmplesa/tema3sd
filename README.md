**Echipa tema 3 SD:**
- matei.hutu - **Nume: Hutu Matei ALexandru**
- marian_cosmin.plesa - **Nume: Plesa Marian Cosmin**
**Grupă: 311CA**

## Tema 3:

## Task 1 - Friends ##
- Pentru a salva legaturile dintre prieteni, am folosit o matrice de adiacenta,
 de dimensiunea MAX_PEOPLE, in care indicele unui nume e dat de functia
get_user_id().
- Pentru sugestii, se trece prin matricea de adiacenta si se salveaza toti
userii care ar trebui sa apara intr-un array. Acest array este sortat, si
unicizat in cazul in care un user este recomandat de mai multi prieteni.
- Distanta intre 2 userii este determinata printr-un algoritm BFS incepand de
la primul user, si se termina la prima aparitie a celui de-al doilea.
- Asemenea sugestiilor, prietenii comuni sunt salvati intr-un array care este
sortat si unicizat, si apoi afisat.
- Cel mai popular user este determinat astfel: se calculeaza numarul de
prieteni al fiecarui prieten al user-ului si se determina maximul dintre ele.
 Apoi se afiseaza userul cu numarul maxim.

## Task 2 - Posts ##
- Pentru a retine postarile si repostarile am implementat urmatoarele
structuri: pentru un post/repost, un nod ce contine pointeri la copii ei
(reposturile ei) si numarul de copii, id-ul nodului, id-ul user-ului care a
facut postarea, id-ul nodului parine, titlul postarii, numarul de like-uri si
array-ul cu userii care i-au dat like; arborele unei postarii ce contine un
pointer la root si structura tuturor postarilor ce contine: un array de arbori
de postari, numarul de postari, si numarul total(de postari si repost-uri).
- La creearea unei postari se creste dimensiunea array-ului de arbori, iar la
noul arbore creeat se pune in root nodul noii postari.
- In cazul repostarii, se adauga un copil nou post-ului sau repost-ului la care
se face repost(postarea originala se cauta dupa id-ul fiecarui root al
arborilor).
- La primul repost comun !!!TO DO!!!
- La comanda de like, se cauta post-ul sau repost-ul dat, si in functie de caz
ii da like sau unlike, si modifica array-ul de like-uri al postarii.
- In cazul comenzii ratio, se cauta postarea dupa id, se trece prin toii copiii
ei, si se retine numarul maxim de like-uri. Daca id-ul postarii cu numarul
maxim de like-uri este cea originala se afiseaza mesajul dorit, altfel ea a
fost "ratio'd", si se afiseaza asta.
- Comanda delete in fuctie de caz face astfel: daca se da delete la o postare,
 se elimina tot arborele din array, si i se da free, iar daca se da delete la un
repost, i se trece id-ul pe -1 intrucat el va fi eliberat la finalizare(daca
id-ul unui nod este pe -1, toate celalte comenzi il vor ignora pe el si
descendentii lui).
- Afisare like-urilor cauta postarea si printeaza numarul de like-uri din
structura nodului.
- La afisarea repost-urilor, se cauta postarea dorita, se afiseaza comanda
dorita, iar apoi se afiseaza reposturile ei: recursiv se trece prin toti
copiii acelui nod si se afiseaza in ordinea trecerii.

## Task 3 - Feed ##
- La feed, se trece invers prin lista de postari si se afiseaza daca este a
userului sau a unui prieten(si se decrementeaza acel feed_size).
- Afisarea profilului se face astfel: se trece in ordine prin lista de postari,
 si daca este a userului se afiseaza. Se trece din nou prin lista, iar la
fiecare postare se trece recursiv prin arborele de repostari si se afiseaza
cele ale utilizatorului.
- Pentru afisarea prietenilor care au repostat, se cauta postarea si se trece
recursiv prin arborele ei de repost-uri, iar in cazul in care un repost a fost
pus de un prieten se afiseaza.
- In cazul clicai, se cauta cel mai mare subgraf complet ce contine user-ul
dat. Se umple un array cu toti prietenii si user-ul dat. Se trece prin lista si
se cauta prietenul cu cele mai putine conexiuni. Daca acesta are mai putine de
numarul de prieteni ramasi in array minus 1, inseamna ca nu este complet si se
elimina din array. Daca nu s-a eliminat un prieten, atunci s-a gasit grupul si
se afiseaza.
