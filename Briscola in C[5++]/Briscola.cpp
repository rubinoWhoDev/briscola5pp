#include "Briscola.h"
#include <iostream>
#include <random>

int Random(int min, int max) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(min, max);

	return distr(gen);
}

void Giocatore::setName(int n) {
	std::cout << "Giocatore " << n << " come ti chiami: ";
	std::cin.getline(nome, 30);
}

Giocatore::Giocatore(int n) : mano() {
	this->isChiamante = true;
	this->isCompagno = false;
	punteggio = 0;
	setName(n);
}

void Giocatore::Pesca(Carte& mazzo, int num_carte) {
	srand(time(NULL));

	for (int i = 0; i < num_carte; i++) {
		int random = Random(0, mazzo.getSize() - 1);
		if (random > mazzo.getSize() - 1) random = mazzo.getSize() - 1;
		Carta* c = new Carta(mazzo.PrendiCarta(random));
		c->setProprietario(this->nome);
		this->mano.AggiungiInCoda(*c);
	}
}

Carte Giocatore::getMano() {
	return mano;
}

void Giocatore::stampaNome() {
	std::cout << nome << std::endl;
}

void Giocatore::stampaMano() {
	mano.Stampa();
}

Giocatore::~Giocatore() {
	mano.~Carte();
	delete[] nome;
}

void Scambia(nodoCarte** primo, nodoCarte** secondo) {
	nodoCarte* aux = *primo, * aux2 = *secondo;
	Carta temp = aux->carta;
	aux->carta = aux2->carta;
	aux2->carta = temp;
}

void ordinaLista(nodoCarte*& lis){
	for(nodoCarte* aux = lis; aux != nullptr; aux = aux->next)
		for (nodoCarte* aux2 = aux->next; aux2 != nullptr; aux2 = aux2->next) {
			if (aux->carta.getPunti() == 0)
				if (aux->carta.getValore() < aux2->carta.getValore()) {
					Scambia(&aux, &aux2);
					continue;
				}
			if (aux->carta.getPunti() < aux2->carta.getPunti())
				Scambia(&aux, &aux2);
		}
}

void Giocatore::OrdinaCarte() {
	Carte carteSegno[4];
	Carte* finale = new Carte;

	for (int i = 0; i < 4; i++) {
		for (nodoCarte* aux = this->mano.getPrimo(); aux != nullptr; aux = aux->next)
			if (aux->carta.getSegno() == (Segno)i)
				carteSegno[i].AggiungiInCoda(aux->carta);
	}

	for (int i = 0; i < 4; i++) {
		ordinaLista(carteSegno[i].getPrimo());
		while (carteSegno[i].getPrimo() != nullptr) {
			finale->AggiungiInCoda(carteSegno[i].PrendiCarta(0));
		}
	}
	nodoCarte* nodo = finale->getPrimo();
	mano.setCoda(nodo);
}

void inizializzaGiocatori(Giocatore**& giocatori) {
	giocatori = new Giocatore * [5];
	for (int i = 0; i < 5; i++)
		giocatori[i] = new Giocatore(i + 1);
}