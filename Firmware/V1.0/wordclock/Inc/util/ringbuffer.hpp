/* 
 * c_ringbuffer.h
 *
 * Created: 05.01.2016 11:13:17
 * Author: Nils
 */

#ifndef __C_RINGBUFFER_H__
#define __C_RINGBUFFER_H__

#include <cstdint>

template <typename T1, int S>
class Ringbuffer {
		/************************************************************************/
		/* Diese Klasse stellt einen Ringpuffer zur Verf�gung, welcher �ber verschiedene Methoden gef�llt, geleert und �berpr�ft werden kann. Die Klasse hat KEINE Abh�ngigkeiten. */
		/************************************************************************/
		//variables
	private:
		volatile int8_t iFindPos;    ///aktuelle Position des Finden Zeigers; letzte fundstelle
		volatile int8_t iReadPos;    ///aktuelle Position des Lesepointers; das n�chste zu lesende zeichen
		volatile int8_t iPos;    ///Position des ersten leeren Feldes im Ringbuffer
		volatile int8_t iSize;    ///Aktuelle Anzahl von Zeichen im Ringpuffer
		T1 bBuffer[S];    ///Der eigentiche Buffer: Ein Array

		int16_t getStart();    ///Gibt die Position des ersten Zeichens im array an

		//functions
	public:
		Ringbuffer();
		int addItem(T1 val, int p = -1);    ///Ein Zeichen hinzuf�gen; Default: Am Ende der Kette
		bool addItems(T1* val, int n, int p = -1);    ///Mehrere Zeichen hinzuf�gen; Default: Am Ende der Kette
		T1 RemoveItem(int p = 0);    ///Ein Zeichen lesen und aus dem Buffer entfernen; Default: Am Anfang der Kette
		int RemoveItems(T1* dest, int n, int p = 0);    ///Mehrere Zeichen lesenund aus dem Buffer entfernen; Default: Am Anfang der Kette
		bool replaceItem(T1 val, int p);    ///Ersetzt an Position p den Wert mit val.
		bool orItem(T1 val, int p);    ///Verodert ein Item
		T1* readItemPointer(int pos);    ///Ein Pointer auf ein Zeichen ausgeben
		T1 readItem(int pos);    ///Ein Zeichen lesen
		T1 readNext();    //Liest das n�chste Zeichen. Initiiert mit readItem()
		T1 readLast();    ///Gibt das letzte ELement der Liste aus
		int getSize();    ///Anzahl von Elementen ausgeben
		void flush();    ///Buffer leeren
		void fill(T1 val, int n);    ///F�llt den Buffer mit Elementen auf
		int findFirstOf(T1 val);    ///Liefert die erste Position von val im Buffer
		int findNextOf(T1 val);    ///Liefert die n�chste Position von val. Initiiert von FindfirstOf
		int findLastOf(T1 val);    ///Liefert die letzte Position von val im Buffer
		bool full();    ///Gibt an, ob der Buffer voll ist
		bool last();    ///Gibt an, ob das zuletzt gelesene Zeichen das letzet im Buffer ist
		T1 calcAverage();    ///Berechnet den Durchschnittswert aller Elemente im Ringbuffer
		T1 calcVarianz();
		T1 lastVal();    ///Gibt das letzte Element zur�ck
};
//c_ringbuffer

//  ====================================================================================================================
//  c_ringbuffer
//  ====================================================================================================================
/// \brief    Konstruktor
//  ====================================================================================================================
/// \abstract Ringbuffer wird initialisiert.
//  ====================================================================================================================
template <typename T1, int S>
Ringbuffer<T1, S>::Ringbuffer() {
	iPos = 0;    //Zeiger auf das erste leere Element des Ringpuffers
	iSize = 0;    //Aktuelle Anzahl von Zeichen im Ringpuffer
	iReadPos = 0;    //Das n�chste zu lesende Zeichen ist das erste Zeichen
	iFindPos = -1;    //Aktuelle keine Position bekannt (-1)
}

//  ====================================================================================================================
//  getStart
//  ====================================================================================================================
/// \brief    Ermittelt die Position des ersten Zeichens im Datenarray
//  ====================================================================================================================
/// \abstract Das Ergebnis bezieht sich auf die tats�chliche Position im Array und f�ngt bei 0 an.
//  ====================================================================================================================
template <typename T1, int S>
int16_t Ringbuffer<T1, S>::getStart() {
	int start = iPos - iSize;

	return start < 0 ? start + S : start;
}

//  ====================================================================================================================
//  addItem
//  ====================================================================================================================
/// \brief    F�gt dem Ringbuffer ein Element hinzu
//  ====================================================================================================================
/// \abstract P gibt die Position im Ringbuffer an. Default -1 => am Ende des Buffers. Wenn der Buffer voll ist, wird\n
///           nichts unternommen. Am Anfang und Ende des Ringbuffers ben�tigt weniger Ressourcen, als mittendrin ein\n
///           Element hinzuzuf�gen - dann m�ssen alle anderen Elemente verschoben werden.
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::addItem(T1 val,    ///< [in] zu speicherndes Element
	int p    ///< [in] Position.
	) {
	int curr, prev;

	if (iSize >= (signed int) S)
		return -1;    //Puffer ist voll
	else if (p >= iSize) p = -1;    //Puffergrenze �berschritten

	if (p == -1) {
		bBuffer[iPos] = val;    //Ein Zeichen in den Puffer schreiben
		if (++iPos >= (signed int) S) iPos = 0;    //Positionszeiger erh�hen und am Ende Ringspuffers auf 0 setzen
	}
	else if (p == 0) {
		curr = getStart();    //Startposition ermitteln
		if (--curr < 0) curr += S;    //Startposition -1
		bBuffer[curr] = val;    //Vafiable setzen
	}
	else {
		curr = getStart();

		while (p-- > 0) {    //X zeichen verschieben
			prev = curr - 1;
			if (prev < 0) prev += S;

			bBuffer[prev] = bBuffer[curr];
			if (++curr >= S) curr -= S;
		}
		if (--curr < 0) curr += S;
		bBuffer[curr] = val;    //Vafiable setzen
	}

	iSize++;    //Anzahl + 1

	return iSize;
}

//  ====================================================================================================================
//  addItems
//  ====================================================================================================================
/// \brief    F�gt dem Ringbuffer mehrere Element hinzu
//  ====================================================================================================================
/// \abstract P gibt die Position im Ringbuffer an. Default -1 => am Ende des Buffers. Wenn der Buffer voll ist, wird\n
///           nichts mehr unternommen. Am Anfang und Ende des Ringbuffers ben�tigt weniger Ressourcen, als mittendrin ein\n
///           Element hinzuzuf�gen - dann m�ssen alle anderen Elemente verschoben werden.
//  ====================================================================================================================
template <typename T1, int S>
bool Ringbuffer<T1, S>::addItems(T1* val,    ///< [in] Zeiger auf die Elemente
	int n,    ///< [in] Anzahl der Elemente
	int p    ///< [in] Position im Ringbuffer
	) {
	bool result = true;

	while (n-- > 0) {
		result = addItem(*val++, p);    //Einen String in den Buffer schreiben
		if (result == false) break;
	}

	return result;
}

//  ====================================================================================================================
//  readItemPointer
//  ====================================================================================================================
/// \brief    Gibt den Pointer auf ein Element des Ringbuffers wieder.
//  ====================================================================================================================
/// \abstract Position f�ngt bei 0 an.
//  ====================================================================================================================
template <typename T1, int S>
T1* Ringbuffer<T1, S>::readItemPointer(int pos    ///< [in] Position des zu lesenden zeichens
	) {
	if (pos > iSize)
		return 0;
	else {
		int start = getStart();
		start += pos;

		if (start >= S) start -= S;

		return &bBuffer[start];
	}
}

//  ====================================================================================================================
//  readItem
//  ====================================================================================================================
/// \brief   Liest ein Element des Pointers.
//  ====================================================================================================================
/// \abstract Position f�ngt bei 0 an. Setzt die Variable iReadPos - readNext() wird durch diese Funktion initialisiert.
//  ====================================================================================================================
template <typename T1, int S>
T1 Ringbuffer<T1, S>::readItem(int pos    ///< [in] Position des zu lesenden zeichens
	) {
	//if(pos > iSize) return 0;
	//else{
	int start = getStart();
	start += pos;

	if (start >= S) start -= S;

	iReadPos = pos + 1;
	if (iReadPos >= iSize) iReadPos = 0;

	return bBuffer[start];
	//}
}

//  ====================================================================================================================
//  readNext
//  ====================================================================================================================
/// \brief   Liest das n�chste Element des Pointers.
//  ====================================================================================================================
/// \abstract Position wird durch readItem() initialisiert.
//  ====================================================================================================================
template <typename T1, int S>
T1 Ringbuffer<T1, S>::readNext() {
	return readItem(iReadPos);
}

template <typename T1, int S>
T1 Ringbuffer<T1, S>::readLast() {
	return readItem(getSize() - 1);
}

template <typename T1, int S>
void Ringbuffer<T1, S>::fill(T1 val, int n) {
	while (n-- > 0)
		addItem(val);
}

//  ====================================================================================================================
//  RemoveItem
//  ====================================================================================================================
/// \brief   Entfernt ein Zeichen aus dem Buffer und gibt dieses zur�ck.
//  ====================================================================================================================
/// \abstract P gibt die Position im Ringbuffer an. Default 0 => am Anfang des Buffers. Am Anfang und Ende des Ringbuffers\n
///           ben�tigt weniger Ressourcen, als mittendrin ein Element zu entfernen - dann m�ssen alle anderen Elemente verschoben werden.
//  ====================================================================================================================
template <typename T1, int S>
T1 Ringbuffer<T1, S>::RemoveItem(int p    ///< [in] Position des Elements
	) {
	T1 result;

	//if(iSize == 0) return 0;									//Kein Inhalt im Buffer
	//else if(p >= iSize) return 0;

	int start = getStart();

	if (p == 0) {
		result = bBuffer[start];    //Erstes Zeichen ausgeben
	}
	else if (p == iSize - 1) {
		if (--iPos < 0) iPos += S;
		result = bBuffer[iPos];
	}
	else {
		//result = readItem(p);									//Zeichen auslesen

		start += p;    //Zeiger (ab 0) auf das zu lesende Zeichen setzen
		if (start >= S) start -= S;

		result = bBuffer[start];

		while (p-- > 0) {    //Die �brigen zeichen durchgehen und jeweils einen nach vorne schieben
			int next = start - 1;    //N�chste Position berechnen
			if (next < 0) next += S;

			bBuffer[start] = bBuffer[next];
			start = next;
		}
	}
	iSize--;

	return result;
}

//  ====================================================================================================================
//  RemoveItems
//  ====================================================================================================================
/// \brief   Entfernt mehrere Zeichen aus dem Buffer und speichert diese in einem Zeigerarray.
//  ====================================================================================================================
/// \abstract P gibt die Position im Ringbuffer an. Default 0 => am Anfang des Buffers. Am Anfang und Ende des Ringbuffers\n
///           ben�tigt weniger Ressourcen, als mittendrin ein Element zu entfernen - dann m�ssen alle anderen Elemente verschoben werden.
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::RemoveItems(T1* dest,    ///< [out] Speicherort
	int n,    ///< [in] Anzahl der Elemente
	int p    ///< [in] Position
	) {
	int read = 0;

	if (n == -1) n = iSize;    //Gesamten Buffer lesen

	while (n-- > 0) {
		*dest++ = RemoveItem(p);    //n - Zeichen lesen
		read++;
		if (iSize == 0) break;    //Wenn buffer leer, dann abbrechen
	}

	return read;    //Ergebnis ausgeben
}

//  ====================================================================================================================
//  replaceItem
//  ====================================================================================================================
/// \brief   Ersetzt ein Element im Buffer.
//  ====================================================================================================================
/// \abstract -
//  ====================================================================================================================
template <typename T1, int S>
bool Ringbuffer<T1, S>::replaceItem(T1 val,    ///< [in] Neuer Wert
	int p    ///< [in] Position
	) {
	int start = getStart();

	if (p >= iSize)    //Element p gibt es nicht
		return false;

	start += p;    //Position berechnen
	if (start >= S) start -= S;

	bBuffer[start] = val;
	return true;
}

//  ====================================================================================================================
//  replaceItem
//  ====================================================================================================================
/// \brief   Verodert ein Element im Buffer.
//  ====================================================================================================================
/// \abstract -
//  ====================================================================================================================
template <typename T1, int S>
bool Ringbuffer<T1, S>::orItem(T1 val, int p) {
	int start = getStart();

	if (p >= iSize)    //Element p gibt es nicht
		return false;

	start += p;    //Position berechnen
	if (start >= S) start -= S;

	bBuffer[start] |= val;
	return true;
}

//  ====================================================================================================================
//  getSize
//  ====================================================================================================================
/// \brief   Gibt die Anzahl der Elemente im Buffer zur�ck.
//  ====================================================================================================================
/// \abstract -
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::getSize() {
	return iSize;    //Aktuelle Anzahl von Zeichen im Ringpuffer ausgeben
}

//  ====================================================================================================================
//  flush
//  ====================================================================================================================
/// \brief   Setzt den Buffer auf Startwerte zur�ck.
//  ====================================================================================================================
/// \abstract -
//  ====================================================================================================================
template <typename T1, int S>
void Ringbuffer<T1, S>::flush() {    //Leert den Ringbuffer
	iSize = 0;
	iPos = 0;
	iReadPos = 0;
	iFindPos = -1;
}

//  ====================================================================================================================
//  findFirstOf
//  ====================================================================================================================
/// \brief   Findet die erste Position von VAL im Buffer und gibt diese zur�ck.
//  ====================================================================================================================
/// \abstract Die Position f�ngt bei 0 an. Diese Funktion initialisiert findNextOf
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::findFirstOf(T1 val    ///< [in] zu suchendes Zeichen
	) {
	int i, iSize_l;

	iSize_l = iSize;
	if (iSize_l == 0) return -1;    //Wenn Buffer leer, kein Ergebnis

	for (i = 0; i < iSize_l; i++) {    //Jedes Zeichen im Buffer vergleichen
		if (readItem(i) == val) {
			iFindPos = i;
			return i;    //Wenn gesuchtes Zeichen, die Position ausgeben
		}
	}

	iFindPos = -1;
	return -1;    //Nicht gefunden
}

//  ====================================================================================================================
//  findNextOf
//  ====================================================================================================================
/// \brief   Findet die n�chste Position von VAL im Buffer.
//  ====================================================================================================================
/// \abstract Die Position f�ngt bei 0 an. Diese Funktion wird initialisiert von findFirstOf
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::findNextOf(T1 val    ///< [in] zu suchendes Element
	) {
	int i, iSize_l;

	iSize_l = iSize;

	if (!(iFindPos == -1 || iSize_l == 0 || iFindPos + 1 >= iSize)) {
		for (i = iFindPos + 1; i < iSize_l; i++) {    //Jedes Zeichen im Buffer vergleichen
			if (readItem(i) == val) {
				iFindPos = i;
				return i;    //Wenn gesuchtes Zeichen, die Position ausgeben
			}
		}
	}

	iFindPos = -1;
	return -1;
}

//  ====================================================================================================================
//  findLastOf
//  ====================================================================================================================
/// \brief   Findet die letzte Position von VAL im Buffer.
//  ====================================================================================================================
/// \abstract Die Position f�ngt bei 0 an.
//  ====================================================================================================================
template <typename T1, int S>
int Ringbuffer<T1, S>::findLastOf(T1 val) {
	int start, i, iSize_l;

	iSize_l = iSize;
	if (iSize_l == 0) return -1;    //Wenn Buffer leer, kein Ergebnis

	while (iSize_l > 0) {
		if (readItem(iSize_l - 1) == val) return iSize_l - 1;

		iSize_l--;
	}

	iFindPos = -1;
	return -1;    //Nicht gefunden
}

//  ====================================================================================================================
//  full
//  ====================================================================================================================
/// \brief  �berpr�ft, bo der Buffer voll ist.
//  ====================================================================================================================
/// \abstract -
//  ====================================================================================================================
template <typename T1, int S>
bool Ringbuffer<T1, S>::full() {
	if ((unsigned int) iSize >= S)
		return true;
	else
		return false;
}

template <typename T1, int S>
bool Ringbuffer<T1, S>::last() {
	if (iReadPos >= iSize)
		return true;
	else
		return false;
}

//  ====================================================================================================================
//  calcAverage
//  ====================================================================================================================
/// \brief  Berechnet den Durchschnittswert von allen im Buffer gespeicherten Elementen.
//  ====================================================================================================================
/// \abstract Eignet sich nur bei reinen Zahlenwerten. Bei String, Structs etc. ist Funktion sinnlos.
//  ====================================================================================================================
template <typename T1, int S>
T1 Ringbuffer<T1, S>::calcAverage() {
	int i;
	long result = 0;

	for (i = 0; i < iSize; i++)    //Jeses Item aufsummieren
		result += readItem(i);    //

	result /= iSize;    //Durchschnitt berechnen

	return (T1) result;    //Ausgeben
}

template <typename T1, int S>
T1 Ringbuffer<T1, S>::calcVarianz() {
	long long sum = 0;
	T1 avr = calcAverage();
	int i;

	for (i = 0; i < iSize; i++)
		sum += ((long) readItem(i) - (long) avr) * ((long) readItem(i) - (long) avr);

	sum /= iSize;

	return (T1) sum;
}

template <typename T1, int S>
T1 Ringbuffer<T1, S>::lastVal() {
	int start = getStart();

	start += iSize - 1;    //Position berechnen
	if (start >= iSize) start -= S;

	return bBuffer[start];
}

#endif //__C_RINGBUFFER_H__
