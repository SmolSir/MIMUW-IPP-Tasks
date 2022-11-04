/** @file
  Interfejs klasy stosu wielomianów rzadkich wielu zmiennych.

  @authors Bartosz Smolarczyk <b.smolarczyk@student.uw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "memory.h"
#include "poly.h"

/**
 * To jest struktura przechowująca stos wielomianów.
 */
typedef struct Stack{
    /**
     * To jest rozmiar tablicy przechowującej wielomiany.
     */
    size_t size;
    /**
     * To jest liczba wielomianów na stosie pomniejszona o jeden. Jeżeli jest
     * równa -1, to stos jest pusty.
     */
    int top;
    /**
     * To jest tablica przechowująca wielomiany.
     */
    Poly *arr;
} Stack;

/**
 * To jest numer aktualnie przetwarzanego przez program wiersza.
 */
extern size_t line_number;

/**
 * To jest stos wielomianów wykorzystywany przez program.
 */
extern Stack stack;

/**
 * Inicjalizuje pusty stos wielomianów.
 */
void InitStack();

/**
 * Sprawdza, czy stos jest pusty.
 * @return Czy stos jest pusty?
 */
static inline bool IsEmpty() {
    return stack.top < 0;
}

/**
 * Wstawia wielomian na wierzchołek stosu.
 * @param[in] p : wielomian
 */
void Push(const Poly *p);

/**
 * Usuwa wielomian z wierzchołka stosu, jeśli stos nie jest pusty.
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void Pop();

/**
 * Wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru.
 */
void Zero();

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem - wypisuje
 * na standardowe wyjście 0 (nie) lub 1 (tak).
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void IsCoeff();

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru -
 * wypisuje na standardowe wyjście 0 (nie) lub 1 (tak).
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void IsZero();

/**
 * Wstawia na stos kopię wielomianu z wierzchołka.
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void Clone();

/**
 * Neguje wielomian na wierzchołku stosu.
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void Neg();

/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek
 * stosu ich sumę.
 * Jeżeli na stosie nie ma wystarczającej liczby wielomianów, wywołuje
 * StackUnderflow().
 */
void Add();

/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek
 * stosu ich iloczyn.
 * Jeżeli na stosie nie ma wystarczającej liczby wielomianów, wywołuje
 * StackUnderflow().
 */
void Mul();

/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je
 * i wstawia na wierzchołek stosu różnicę.
 * Jeżeli na stosie nie ma wystarczającej liczby wielomianów, wywołuje
 * StackUnderflow().
 */
void Sub();

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe - wypisuje na
 * standardowe wyjście 0 (nie) lub 1 (tak).
 * Jeżeli na stosie nie ma wystarczającej liczby wielomianów, wywołuje
 * StackUnderflow().
 */
void IsEq();

/**
 * Wypisuje na standardowe wyjście stopień wielomianu (-1 dla wielomianu
 * tożsamościowo równego zeru).
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 */
void Deg();

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną
 * o numerze idx.
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 * @param[in] idx : numer zmiennej
 */
void DegBy(const size_t idx);

/**
 * Wylicza wartość wielomianu na wierzchołku stosu w punkcie @f$x@f$, usuwa
 * wielomian z wierzchołka i wstawia na stos wynik operacji.
 * Jeżeli stos jest pusty, wywołuje StackUnderflow().
 * @param[in] x : wartość argumentu @f$x@f$
 */
void At(const poly_coeff_t x);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu.
 */
void Print();

#endif /* __STACK_H__ */