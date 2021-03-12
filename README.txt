***NUME Mandru, PRENUME Cosmina***
****GRUPA 313CB, SERIA CB**********
 
							Readme Tema 1

		*Functia VerifCard va fi apelata in fiecare functie pentru a verifica 
	daca un card este "LOCKED" sau nu.
		*Pointerii SUCCESS si FAIL, prezenti in anumite functii, sunt creati in 
	functie de operatia ce trebuie executa si mai apoi introdusi in history.
		1) Functia add_card: In aceasta functie se verifica unicitatea cardului
	in functie de CARD_NUMBER, apeland functia check_double, in caz afirmativ
	nu se mai realieaza adaugarea si este intors un mesaj de atentionare. Este
	calculata pozitia cardului dupa formula data iar in structura IF sunt
	stocate informatiile cardului ce trebuie adaugat. Lista LC, data ca 
	parametru, este parcursa, numarand cate pozitii/intrari se afla in lista.
	Exista 3 cauzuri pentru inserarea unui card in lista:
		-> daca pozitia este egala cu numarul de intrari in LC, atucni cardul
		este inserat la sfarsit;
		-> daca pozitia este mai mare decat numarul de intrari in LC, atunci
		se adauga noi intrari nule in lista panacand se ajunge la pozitia
		necesara de inserare a cardului si acesta este adaugat la sfarsit;
		-> daca pozitia este mai mica decat umarul de intrari in LC, atunci
		este parcursa lista LC si cardul va fi adaugat la inceput pe pozitia
		corespunzatoare;
		2) Functia delete_card: Parcurge lista LC data ca parametru si cauta
	in functie de CARD_NUMBER cardul ce trebuie sters. Cand este gasit este 
	apelata functia de Eliminare.
		3) Functia cancel: Parcurge lista LC data ca parametru si cauta in 
	functie de CARD_NUMBER cardul in care se va adauga la inceput in history
	operatia cancel realizata cu succes.
		4) Functia insert_card: Parcurge lista LC data ca parametru si cauta in 
	functie de CARD_NUMBER cardul ce trebuie inserat. In primul se verifica 
	daca pinul este unul valid, in structura cardului am adaugat o variabila
	de tipul int nb_fails ce retine de cate ori a fost introdus pinul gresit,
	aceasta creste atunci cand pinul introdus este invalid. In cazul in care 
	valoarea aceste variabile este 3 cardul este blocat iar inserarea cardului
	esueaza. Daca niciuna din cele 2 situatii nu a fost indeplinita se verifica
	daca pinul este cel initial verificand variabila nb_change_pin, aflata si 
	ea in structura cardului: daca are valoare 0 se afiseaza mesajul "You must
	change your pin" iar daca valoarea este mai mare ca 0 se schimba starea 
	pinului in ACTIVE. In functie de cazurile prezentate anterior se realizeaza
	si inserarea in history cu FAIL sau SUCCESS.
		5) Functia pin_change: Parcurge lista LC data ca parametru si cauta in 
	functie de CARD_NUMBER cardul a carui pin trebuie schimbat. Se verifica 
	daca pinul este valid adica daca are 4 cifre. Pinul este schimbat si va 
	creste si valoare variabile nb_change_pin, adaugandu-se si operatia in 
	history fie daca a reusit sau a esuat.
		6) Functia unblock_card: Parcurge lista LC data ca parametru si cauta 
	in functie de CARD_NUMBER cardul ce trebuie deblocat. Aceasta functie are
	scop resetarea variabile nb_fails. Daca cardul are starea "LOCKED" sau 
	"NEW" este schimbata in "ACTIVE".
		7) Functia balance_inquiry: Parcurge lista LC data ca parametru si 
	cauta in functie de CARD_NUMBER cardul a carei suma trebuie afisata. 
	Daca cardul este gasit se afiseaza suma si in final operatia este adaugata
	in history ca SUCCESS daca a reusit si cu FAIL in caz contrar.
		8) Functia recharge: Parcurge lista LC data ca parametru si cauta in 
	functie de CARD_NUMBER cardul ce trebuie alimentat. Daca cardul a fost 
	gasit se verifica daca suma este un multiplu de 10, in caz afirmativ suma
	este adaugata pe card si va fi si afisata. Se actualizeaza si history-ul in
	functie de realizarea operatiei.
		9) Functia cash_withdrawal: Parcurge lista LC data ca parametru si 
	cauta in functie de CARD_NUMBER cardul de unde trebuie retrasa o suma data.
	Daca cardul a fost gasit se verifica daca suma ce vrea a fi retrasa este un 
	multiplu de 10 si daca exista suficiente fonduri. In caz afirmativ suma 
	retrasa se scade din soldul curent al cardului. Se actualizeaza history.
		10) Functia transfer_funds: Parcurge lista LC data ca parametru si 
	cauta in functie de CARD_NUMBER cardul sursa si se verifica daca exista
	fonduri suficiente pentru transfer si daca suma data este un multiplu de
	10. Este cautat apoi si cardul destinatie in care se adaug suma ce este 
	scazuta din soldul cardului sursa. Operatia est adaugata in history-ul
	ambelor carduri fie ea FAIL sau SUCCESS.
		11) Functia reverse_transaction: Parcurge lista LC data ca parametru si 
	cauta in functie de CARD_NUMBER cardul sursa si cardul destinatie. Daca pe 
	cardul destinatie nu exista fonduri suficiente tranzictia nu poate fi 
	anulata. Daca nu atunci suma este retrasa de pe cardul destinatie si 
	adaugata pe cardul sursa, totodat fiind eliminata din history-ul cardului
	destinatie operatia de transfer_funds, apeland funtia de eliminare din HS.
	Se actualizeaza si history-ul cardului sursa daca operatia de anulare a 
	transictiei a reusit sau nu.
		12) Functia show: daca nu are dat ca parametru un CARD_NUMBER va afisa 
	informatiile tuturor cardurilor iar in caz contrat doar a cardului dat.
	
		PUNCTAJ CHECKER LOCAL : 62/85, la a doua rulare a checker-ului imi
		trec mai multe teste pe valgrind
