/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych.

  @authors Bartosz Smolarczyk <b.smolarczyk@student.uw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "memory.h"
#include "poly.h"

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p)) return;

    for (size_t i = 0; i < p->size; i++) {
        PolyDestroy(&p->arr[i].p);
    }
    free(p->arr);
    *p = PolyZero();
}


Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff);
    }

    Mono *mono_array = MemAlloc(p->size, sizeof(Mono));

    for (size_t i = 0; i < p->size; i++) {
        mono_array[i] = MonoClone(&p->arr[i]);
    }

    Poly new_poly = {.size = p->size, .arr = mono_array};
    return new_poly;
}

/**
 * Przegląda tablicę jednomianów i usuwa z nich te, które jako współczynnik
 * mają wielomian zerowy.
 * @param[in] monos : wskaźnik na tablicę jednomianów
 * @param[in] monos_size : rozmiar tablicy jednomianów
 * @return wielomian powstały z wejściowej tablicy niezawierający jednomianów
 * zerowych, \n
 * LUB \n
 * wielomian zerowy, jeśli wszystkie jednomiany z wejściowej tablicy są zerowe.
 */
static Poly PolyNoZeros(Mono *monos, size_t monos_size) {
    Mono *new_monos = MemAlloc(monos_size, sizeof(Mono));
    size_t new_monos_size = 0;

    for (size_t i = 0; i < monos_size; i++) {
        if (PolyIsZero(&monos[i].p)) continue;

        new_monos[new_monos_size] = MonoClone(&monos[i]);
        MonoDestroy(&monos[i]);
        new_monos_size++;
    }
    free(monos);
    
    if (!new_monos_size) {
        free(new_monos);
        return PolyZero();
    }
    if (new_monos_size == 1 && 
        PolyIsCoeff(&new_monos[0].p) &&
        new_monos[0].exp == 0) {

        Poly new_poly = (Poly) {.coeff = new_monos[0].p.coeff, .arr = NULL};
        free(new_monos);
        
        return new_poly;
    }

    new_monos = (Mono*) MemRealloc(new_monos, new_monos_size, sizeof(Mono));
    return (Poly) {.size = new_monos_size, .arr = new_monos};
}

/**
 * Dodaje stałą do wielomianu, który nie jest wielomianem stałym.
 * @param[in] c : stała @f$c@f$
 * @param[in] p : wielomian @f$p@f$
 * @return @f$c@f$ + @f$p@f$
 */
static Poly PolyAddCoeff(poly_coeff_t c, const Poly *p) {
    Mono *monos = MemAlloc(p->size + 1, sizeof(Mono));

    monos[0] = (Mono) {.p = PolyFromCoeff(c), .exp = 0};

    for (size_t i = 0; i < p->size; i++) {
        monos[i + 1] = MonoClone(&p->arr[i]);
    }

    Poly new_poly = PolyAddMonos(p->size + 1, monos);
    free(monos);
    return new_poly;
}


Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsZero(p)) return PolyClone(q);
    if (PolyIsZero(q)) return PolyClone(p);
    
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff + q->coeff);
    }

    if (PolyIsCoeff(p)) return PolyAddCoeff(p->coeff, q);
    if (PolyIsCoeff(q)) return PolyAddCoeff(q->coeff, p);

    Mono *monos_sum = MemAlloc(p->size + q->size, sizeof(Mono));
    size_t q_id = 0;

    for (size_t p_id = 0; p_id < p->size; p_id++) {
        while (q_id < q->size &&
               MonoGetExp(&p->arr[p_id]) >= MonoGetExp(&q->arr[q_id])) {
            monos_sum[p_id + q_id] = MonoClone(&q->arr[q_id]);
            q_id++;
        }
        monos_sum[p_id + q_id] = MonoClone(&p->arr[p_id]);
    }
    
    while (q_id < q->size) {
        monos_sum[p->size + q_id] = MonoClone(&q->arr[q_id]);
        q_id++;
    }

    Poly new_poly = PolyAddMonos(p->size + q->size, monos_sum);
    free(monos_sum);
    return new_poly;
}

/**
 * Porównuje dwa jednomiany ze względu na wykładniki ich zmiennych.
 * @param[in] a : jednomian @f$a@f$
 * @param[in] b : jednomian @f$b@f$
 * @return różnicę wykładników zmiennych jednomianów @f$a@f$ oraz @f$b@f$.
 */
static int MonoCompare(const void *a, const void *b) {
    Mono *mono_a = (Mono*) a;
    Mono *mono_b = (Mono*) b;
    return (int) (mono_a->exp - mono_b->exp);
}


Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (!count) return PolyZero();

    Mono *new_monos = MemAlloc(count, sizeof(Mono));
    size_t new_monos_count = 0;

    for (size_t i = 1; i < count; i++) {
        if (MonoGetExp(&monos[i]) < MonoGetExp(&monos[i - 1])) {
            qsort((void*) monos, count, sizeof(Mono), MonoCompare);
            break;
        }
    }

    for (size_t i = 0; i < count; i++) {
        Mono new_mono = MonoClone(&monos[i]);

        while (i + 1 < count &&
            MonoGetExp(&new_mono) == MonoGetExp(&monos[i + 1])) {
                Poly tmp_poly = PolyClone(&new_mono.p);
                PolyDestroy(&new_mono.p);
                new_mono.p = PolyAdd(&tmp_poly, &monos[i + 1].p);
                
                MonoDestroy((Mono*) &monos[i]);
                PolyDestroy(&tmp_poly);
                i++;
        }

        new_monos[new_monos_count] = MonoClone(&new_mono);
        MonoDestroy(&new_mono);
        MonoDestroy((Mono*) &monos[i]);
        new_monos_count++;
    }

    return PolyNoZeros(new_monos, new_monos_count);
}

/**
 * Mnoży przez stałą @f$c@f$ wielomian @f$p@f$, który nie jest wielomianem
 * stałym.
 * @param[in] c : stała @f$c@f$
 * @param[in] p : wielomian @f$p@f$
 * @return @f$c@f$ * @f$p@f$
 */
static Poly PolyMulCoeff(poly_coeff_t c, const Poly *p) {
    Mono *monos = MemAlloc(p->size, sizeof(Mono));

    for (size_t i = 0; i < p->size; i++) {
        Poly c_poly = PolyFromCoeff(c);
        Poly mul_poly = PolyMul(&c_poly, &p->arr[i].p);
        
        monos[i] = (Mono) {.p = PolyClone(&mul_poly), .exp = p->arr[i].exp};
        PolyDestroy(&mul_poly);
    }
    
    Poly new_poly = PolyAddMonos(p->size, monos);
    free(monos);
    return new_poly;
}


Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) || PolyIsZero(q)) return PolyZero();
    
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff * q->coeff);
    }

    if (PolyIsCoeff(p)) return PolyMulCoeff(p->coeff, q);
    if (PolyIsCoeff(q)) return PolyMulCoeff(q->coeff, p);
    
    Mono *monos = MemAlloc(p->size * q->size, sizeof(Mono));
    size_t next_mono = 0;

    for (size_t p_id = 0; p_id < p->size; p_id++) {
        for (size_t q_id = 0; q_id < q->size; q_id++) {
            Poly mul_poly = PolyMul(&p->arr[p_id].p, &q->arr[q_id].p);
            poly_exp_t new_exp = p->arr[p_id].exp + q->arr[q_id].exp;
            
            monos[next_mono] = (Mono) {.p = mul_poly, .exp = new_exp};
            next_mono++;
        }
    }
    
    Poly new_poly = PolyAddMonos(p->size * q->size, monos);
    free(monos);
    return new_poly;
}

/**
 * Tworzy wielomian przeciwny do wielomianu @f$p@f$ poprzez zmianę znaku
 * wszystkich jednomianów składowych @f$b@f$.
 * @param[in] p : wielomian @f$p@f$
 * @return @f$-p@f$
 */
static Poly PolyNegCoeffs(Poly *p) {
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(-p->coeff);
    }

    for (size_t i = 0; i < p->size; i++) {
        p->arr[i].p = PolyNegCoeffs(&p->arr[i].p);
    }
    return *p;
}


Poly PolyNeg(const Poly *p) {
    Poly new_poly = PolyClone(p);
    return PolyNegCoeffs(&new_poly);
}


Poly PolySub(const Poly *p, const Poly *q) {
    Poly q_neg = PolyNeg(q);
    Poly new_poly = PolyAdd(p, &q_neg);
    
    PolyDestroy(&q_neg);
    return new_poly;
}


poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p)) return -1;
    if (PolyIsCoeff(p)) return 0;

    if (var_idx) {
        poly_exp_t max_deg = 0;
        
        for (size_t i = 0; i < p->size; i++) {
            poly_exp_t deg_next_var = PolyDegBy(&p->arr[i].p, var_idx - 1);
            max_deg = (deg_next_var > max_deg ? deg_next_var : max_deg);
        }
        return max_deg;
    }

    poly_exp_t max_deg_by = 0;
    
    for (size_t i = 0; i < p->size; i++) {
        max_deg_by = (p->arr[i].exp > max_deg_by ? p->arr[i].exp : max_deg_by);
    }
    return max_deg_by;
}

/**
 * Rekurencyjnie znajduje stopień wielomianu @f$p@f$.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] max_exp : dotychczas znaleziony stopień wielomianu
 * @return stopień wielomianu @f$p@f$.
 */
static poly_exp_t PolyDegHelper(const Poly *p, poly_exp_t max_exp) {
    if (PolyIsCoeff(p)) return max_exp;

    for (size_t i = 0; i < p->size; i++) {
        poly_exp_t mono_deg = PolyDegHelper(&p->arr[i].p, 0) + p->arr[i].exp;
        max_exp = (max_exp > mono_deg ? max_exp : mono_deg);
    }
    return max_exp;
}


poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) return -1;
    return PolyDegHelper(p, 0);
}


bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return p->coeff == q->coeff;
    }
    if (PolyIsCoeff(p) || PolyIsCoeff(q)) {
        return false;
    }
    if (p->size != q->size) {
        return false;
    }

    bool mono_arrays_eq = true;

    for (size_t i = 0; i < p->size && mono_arrays_eq; i++) {
        mono_arrays_eq &= (p->arr[i].exp == q->arr[i].exp);
    }
    for (size_t i = 0; i < p->size && mono_arrays_eq; i++) {
        mono_arrays_eq &= PolyIsEq(&p->arr[i].p, &q->arr[i].p);
    }
    return mono_arrays_eq;
}

/**
 * Wykonuje potęgowanie liczb całkowitych.
 * @param[in] x : podstawa potęgi
 * @param[in] y : wykładnik potęgi
 * @return @f$x^y@f$
 */
static poly_coeff_t FastPower(poly_coeff_t x, poly_exp_t y) {
    if (x == 1 || y == 0) return 1;
    if (y == 1) return x;

    if (y % 2) {
        return x * FastPower(x * x, (y - 1) / 2);
    }
    return FastPower(x * x, y / 2);
}

/**
 * Tworzy wielomian będący wynikiem wywołania funkcji PolyAt().
 * @param[in] p : @f$p(x_0,x_1,x_2,...,x_k)@f$
 * @param[in] x : wartość argumentu @f$x@f$
 * @param[in] monos_size : końcowy rozmiar tablicy jednomianów zwracanego
 * wielomianu
 * @return @f$p(x,x_0,x_1,...,x_{k-1})@f$
 */
static Poly PolyAtHelper(const Poly *p, poly_coeff_t x, size_t monos_size) {
    Mono *monos = MemAlloc(monos_size, sizeof(Mono));
    size_t monos_cnt = 0;

    for (size_t i = 0; i < p->size; i++) {
        Poly tmp_poly = PolyClone(&p->arr[i].p);
        Poly x_poly = PolyFromCoeff(FastPower(x, p->arr[i].exp));

        if (PolyIsCoeff(&tmp_poly)) {
            Poly mul_poly = PolyMul(&tmp_poly, &x_poly);
            monos[monos_cnt] = (Mono) {.p = mul_poly, .exp = 0};
            monos_cnt++;
            continue;
        }
        
        for (size_t tmp_id = 0; tmp_id < tmp_poly.size; tmp_id++) {
            Mono tmp_mono = tmp_poly.arr[tmp_id];
            Poly mul_poly = PolyMul(&tmp_mono.p, &x_poly);
            monos[monos_cnt] = (Mono) {.p = mul_poly, .exp = tmp_mono.exp};
            monos_cnt++;
        }

        PolyDestroy(&tmp_poly);
    }

    Poly new_poly = PolyAddMonos(monos_size, monos);
    free(monos);
    return new_poly;
}


Poly PolyAt(const Poly *p, poly_coeff_t x) {
    assert(x % 1 == 0);
    if (PolyIsCoeff(p)) return PolyClone(p);

    size_t monos_size = 0;
    for (size_t i = 0; i < p->size; i++) {
        Poly tmp_poly = p->arr[i].p;

        if (PolyIsCoeff(&tmp_poly)) {
            monos_size++;
        }
        else {
            monos_size += tmp_poly.size;
        }
    }

    return PolyAtHelper(p, x, monos_size);
}