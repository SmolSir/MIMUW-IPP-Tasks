/** @file
  Implementacja klasy stosu wielomianów rzadkich wielu zmiennych.

  @authors Bartosz Smolarczyk <b.smolarczyk@student.uw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"

void InitStack() {
    stack.size = INIT_ARRAY_SIZE;
    stack.top = -1;
    stack.arr = (Poly*) MemAlloc(INIT_ARRAY_SIZE, sizeof(Poly));
}

/**
 * Wypisuje na wyjście diagnostyczne komunikat, że na stosie jest za mało
 * wielomianów, aby wykonać polecenie. Komunikat zawiera informację o numerze
 * wiersza, w którym wystąpił błąd.
 */
static void StackUnderflow() {
    fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);
}

/**
 * Zwiększa dwukrotnie rozmiar tablicy przechowującej wielomiany poprzez
 * wywołanie MemRealloc().
 */
static void ResizeUp() {
    stack.size *= 2;
    stack.arr = (Poly*) MemRealloc(stack.arr, stack.size, sizeof(Poly));
}

/**
 * Jeżeli rozmiar stosu jest większy od #INIT_ARRAY_SIZE, dwukrotnie zmniejsza
 * rozmiar stosu.
 */
static void ResizeDown() {
    if (stack.size <= INIT_ARRAY_SIZE) {
        return;
    }

    stack.size /= 2;
    stack.arr = (Poly*) MemRealloc(stack.arr, stack.size, sizeof(Poly));
}

/**
 * Sprawdza, czy tablica przechowująca wielomiany wymaga realokacji pamięci.
 * Jeżeli tablica jest pełna, podwaja jej rozmiar wywołując ResizeUp().\n
 * Jeżeli tablica jest zapełniona w 1/4, zmniejsza dwukrotnie jej rozmiar
 * wywołując ResizeDown().
 */
static void TryResize() {
    if (stack.top + 1 == (int) stack.size) {
        ResizeUp();
    }
    if (stack.top + 1 == (int) stack.size / 4) {
        ResizeDown();
    }
}

void Push(const Poly *p) {
    TryResize();
    stack.top++;
    stack.arr[stack.top] = PolyClone(p);
}

/**
 * Zwraca kopię wielomianu na wierzchołku stosu. Niezmiennikiem tej funkcji
 * jest obecność wielomianu na stosie. Należy przed jej wywołaniem zapewnić
 * ten niezmiennik.
 * @return Kopia wielomianu na wierzchołku stosu.
 */
static Poly Top() {
    return PolyClone(&stack.arr[stack.top]);
}

void Pop() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    PolyDestroy(&stack.arr[stack.top]);
    stack.top--;
    TryResize();
}

void Zero() {
    TryResize();
    stack.top++;
    stack.arr[stack.top] = PolyZero();
}

void IsCoeff() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    if (PolyIsCoeff(&stack.arr[stack.top])) {
        fprintf(stdout, "1\n");
    }
    else {
        fprintf(stdout, "0\n");
    }
}

void IsZero() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }
    
    if (PolyIsZero(&stack.arr[stack.top])) {
        fprintf(stdout, "1\n");
    }
    else {
        fprintf(stdout, "0\n");
    }
}

void Clone() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    Push(&tmp);
    PolyDestroy(&tmp);
}

void Neg() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    Pop();
    Poly new_poly = PolyNeg(&tmp);
    Push(&new_poly);

    PolyDestroy(&tmp);
    PolyDestroy(&new_poly);
}

/**
 * Sprawdza, czy na stosie są conajmniej dwa wielomiany.
 * @return Czy na stosie są conajmniej dwa wielomiany?
 */
static bool HasTwoPolys() {
    return stack.top > 0;
}

/**
 * Wywołuje zadaną funkcję typu Poly na dwóch wielomianach z wierzchu stosu,
 * usuwa te dwa wielomiany, a następnie wstawia wynik wywołania funkcji na
 * wierzchołek stosu.
 * @param[in] f : wskaźnik na funkcję do wywołania
 */
static void PolyFromTwoArgsFunc(Poly (*f)(const Poly*, const Poly*)) {
    if (!HasTwoPolys()) {
        StackUnderflow();
        return;
    }

    Poly poly_one = Top();
    Pop();
    Poly poly_two = Top();
    Pop();
    Poly res = (*f)(&poly_one, &poly_two);
    Push(&res);

    PolyDestroy(&poly_one);
    PolyDestroy(&poly_two);
    PolyDestroy(&res);
}

void Add() {
    PolyFromTwoArgsFunc(PolyAdd);
}

void Mul() {
    PolyFromTwoArgsFunc(PolyMul);
}

void Sub() {
    PolyFromTwoArgsFunc(PolySub);
}

void IsEq() {
    if (!HasTwoPolys()) {
        StackUnderflow();
        return;
    }

    Poly poly_one = Top();
    Pop();
    Poly poly_two = Top();
    Push(&poly_one);

    bool res = PolyIsEq(&poly_one, &poly_two);
    res ? fprintf(stdout, "1\n") : fprintf(stdout, "0\n");

    PolyDestroy(&poly_one);
    PolyDestroy(&poly_two);
}

void Deg() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    fprintf(stdout, "%d\n", PolyDeg(&tmp));
    
    PolyDestroy(&tmp);
}

void DegBy(const size_t idx) {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    fprintf(stdout, "%d\n", PolyDegBy(&tmp, idx));
    
    PolyDestroy(&tmp);
}

void At(const poly_coeff_t x) {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    Poly res = PolyAt(&tmp, x);
    Pop();
    Push(&res);
    
    PolyDestroy(&tmp);
    PolyDestroy(&res);
}

/**
 * Funkcja pomocnicza dla Print(). Rekurencyjnie wypisuje na standardowe
 * wyjście zadany wielomian.
 * @param[in] p : wielomian do wypisania.
 */
static void RecursivePrint(const Poly *p) {
    if (PolyIsCoeff(p)) {
        fprintf(stdout, "%ld", p->coeff);
        return;
    }
   
    for (size_t i = 0; i < p->size; i++) {
        i ? fprintf(stdout, "+(") : fprintf(stdout, "(");
        RecursivePrint(&p->arr[i].p);
        fprintf(stdout, ",%d)", p->arr[i].exp);
    }
}

void Print() {
    if (IsEmpty()) {
        StackUnderflow();
        return;
    }

    Poly tmp = Top();
    RecursivePrint(&tmp);
    fprintf(stdout, "\n");
    PolyDestroy(&tmp);
}