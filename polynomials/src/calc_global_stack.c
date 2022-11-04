/** @file
  Implementacja kalkulatora wielomianów rzadkich wielu zmiennych.

  @authors Bartosz Smolarczyk <b.smolarczyk@student.uw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

/**
 * To jest makro wymagane przez getline.
 */
#define _GNU_SOURCE

#include "stack.h"
#include <string.h>
#include <errno.h>
#include <ctype.h>

/**
 * To jest najmniejsza poprawna wartość wykładnika jednomianu.
 */
#define MIN_EXP 0

/**
 * To jest największa poprawna wartość wykładnika jednomianu.
 */
#define MAX_EXP 2147483647

/**
 * To jest numer aktualnie przetwarzanego przez program wiersza. Wartością
 * początkową jest zero.
 */
size_t line_number = 0;

/**
 * To jest stos wielomianów wykorzystywany przez program. Przed użyciem należy
 * go zainicjalizować za pomocą InitStack().
 */
Stack stack;

/**
 * Wypisuje na wyjście diagnostyczne komunikat, że program wykrył niepoprawną
 * nazwę polecenia. Komunikat zawiera informację o numerze wiersza, w którym
 * wystąpił błąd.
 */
static void WrongCommand() {
    fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);
}

/**
 * Wypisuje na wyjście diagnostyczne komunikat, że w poleceniu DEG_BY nie
 * podano parametru lub jest on niepoprawny. Komunikat zawiera informację
 * o numerze wiersza, w którym wystąpił błąd.
 */
static void DegByWrongVariable() {
    fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line_number);
}

/**
 * Wypisuje na wyjście diagnostyczne komunikat, że w poleceniu AT nie podano
 * parametru lub jest on niepoprawny. Komunikat zawiera informację o numerze
 * wiersza, w którym wystąpił błąd.
 */
static void AtWrongValue() {
    fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line_number);
}

/**
 * Wypisuje na wyjście diagnostyczne komunikat, że wykryto błąd podczas
 * parsowania wielomianu. Komunikat zawiera informację o numerze wiersza,
 * w którym wystąpił błąd.
 */
static void WrongPoly() {
    fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
}

/**
 * Funkcja pomocnicza dla DegByHelper() oraz AtHelper(). Sprawdza, czy
 * funkcja poprawnie sparsowała swój parametr i czy nie występują po nim żadne
 * znaki inne niż ewentualny znak nowej linii.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @param[in] end_ptr : wskaźnik na znak wiersza, na którym zakończyło się
 * parsowanie
 * @return Czy po sparsowaniu parametru wykryto błąd w wierszu?
 */
static bool InvalidParamRead(char *line, size_t line_size, char *end_ptr) {
    if (errno) {
        errno = 0;
        return true;
    }
    if (end_ptr - line < (long int) line_size - 1) {
        return true;
    }
    if (end_ptr - line == (long int) line_size - 1 && end_ptr[0] == '\0') {
        return true;
    }
    return false;
}

/**
 * Funkcja pomocnicza dla polecenia DEG_BY. Sprawdza poprawność wiersza
 * i parsuje parametr, następnie wywołuje na nim polecenie DegBy().\n
 * Jeżeli wykryje błąd, wywołuje DegByWrongVariable().
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 */
static void DegByHelper(char *line, size_t line_size) {
    size_t prefix_size = strlen("DEG_BY");
    
    if (line_size < prefix_size + 2) {
        DegByWrongVariable();
        return;
    }
    if (line[prefix_size] != ' ' || !isdigit(line[prefix_size + 1])) {
        DegByWrongVariable();
        return;
    }

    char *end_ptr;
    size_t val = strtoul(line + prefix_size, &end_ptr, 10);
    
    if (InvalidParamRead(line, line_size, end_ptr)) {
        DegByWrongVariable();
        return;
    }

    DegBy(val);
}

/**
 * Funkcja pomocnicza dla polecenia AT. Sprawdza poprawność wiersza
 * i parsuje parametr, następnie wywołuje na nim polecenie At().\n
 * Jeżeli wykryje błąd, wywołuje AtWrongValue().
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 */
static void AtHelper(char *line, size_t line_size) {
    size_t prefix_size = strlen("AT");

    if (line_size <= prefix_size + 2) {
        AtWrongValue();
        return;
    }
    if (line[prefix_size] != ' ') {
        AtWrongValue();
        return;
    }
    if (!isdigit(line[prefix_size + 1]) && line[prefix_size + 1] != '-') {
        AtWrongValue();
        return;
    }

    char *end_ptr;
    poly_coeff_t val = strtol(line + prefix_size, &end_ptr, 10);
    
    if (InvalidParamRead(line, line_size, end_ptr)) {
        AtWrongValue();
        return;
    }
    
    At(val);
}

/**
 * Porównuje wiersz i dane polecenie.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @param[in] command : polecenie
 * @return Czy wiersz i dane polecenie są równe?
 */
static bool Compare(char *line, size_t line_size, char *command) {
    size_t command_length = strlen(command);
    
    for (size_t i = 0; i < command_length; i++) {
        if (line[i] != command[i]) {
            return false;
        }
    }

    if (line_size == command_length) {
        return true;
    }
    if (line_size == command_length + 1 && line[line_size - 1] == '\n') {
        return true;
    }
    return false;
}

/**
 * Parsuje wiersz zawierający polecenie.
 * Jeżeli wiersz nie zawiera żadnego z rozpoznawanych poleceń, wywołuje
 * WrongCommand().
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 */
static void ParseCommand(char *line, size_t line_size) {
    if (Compare(line, line_size, "ZERO")) {
        Zero();
    }
    else if (Compare(line, line_size, "IS_COEFF")) {
        IsCoeff();
    }
    else if (Compare(line, line_size, "IS_ZERO")) {
        IsZero();
    }
    else if (Compare(line, line_size, "CLONE")) {
        Clone();
    }
    else if (Compare(line, line_size, "ADD")) {
        Add();
    }
    else if (Compare(line, line_size, "MUL")) {
        Mul();
    }
    else if (Compare(line, line_size, "NEG")) {
        Neg();   
    }
    else if (Compare(line, line_size, "SUB")) {
        Sub();
    }
    else if (Compare(line, line_size, "IS_EQ")) {
        IsEq();
    }
    else if (Compare(line, line_size, "DEG")) {
        Deg();
    }
    else if (strncmp(line, "DEG_BY", strlen("DEG_BY")) == 0) {
        DegByHelper(line, line_size);
    }
    else if (strncmp(line, "AT", strlen("AT")) == 0) {
        AtHelper(line, line_size);
    }
    else if (Compare(line, line_size, "PRINT")) {
        Print();   
    }
    else if (Compare(line, line_size, "POP")) {
        Pop();
    }
    else {
        WrongCommand();
    }
}

/**
 * Sprawdza, czy dany znak jest dozwolony w zapisie wielomianu.
 * @param[in] c : znak
 * @return Czy dany znak jest dozwolony w zapisie wielomianu?
 */
static bool CheckPolyChar(char c) {
    bool res = 
        isdigit(c) ||
        c == '(' ||
        c == ')' ||
        c == ',' ||
        c == '+' ||
        c == '-';
    return res;
}

/**
 * Sprawdza, czy w wierszu występują znaki niedozwolone w zapisie
 * wielomianu.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @return Czy w wierszu występują znaki niedozwolone w zapisie wielomianu?
 */
static bool InvalidPolyCharacter(char *line, size_t line_size) {
    for (size_t i = 0; i < line_size - 1; i++) {
        if (!CheckPolyChar(line[i])) {
            return true;
        }
    }
    return false;
}

/**
 * Sprawdza, czy znaki plus (+) oraz minus (-) w wierszu występują
 * w niedozwolonych miejscach.
 * - Znak plus może wystąpić jedynie po nawiasie okrągłym zamykającym
 * i jednocześnie przed nawiasem okrągłym otwierającym;\n
 * - Znak minus może wystąpić jedynie przed cyfrą.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @return Czy znaki plus (+) oraz minus (-) w wierszu występują
 * w niedozwolonych miejscach?
 */
static bool InvalidPlusMinus(char *line, size_t line_size) {
    if (line[0] == '+' || line[line_size - 1] == '+') {
        return true;
    }
    if (line[0] == '-' && (line_size == 1 || !isdigit(line[1]))) {
        return true;
    }
    if (line[line_size - 1] == '-') {
        return true;
    }
    
    for (size_t i = 1; i < line_size - 1; i++) {
        if (line[i] == '-' && (line[i - 1] != '(' || !isdigit(line[i + 1]))) {
            return true;
        }
        if (line[i] == '+' && (line[i - 1] != ')' || line[i + 1] != '(')) {
            return true;
        }
    }
    return false;
}

/**
 * Sprawdza, czy liczby występujące w wierszu wykraczają poza dozwolone
 * zakresy.
 * - Gdy natrafi na cyfrę, którą poprzedza przecinek (,), to sprawdza czy mieści
 * się w dozwolonym zakresie dla wykładników;
 * - Gdy natrafi na cyfrę, której nie poprzedza przecinek (,), lub natrafi na
 * znak minus (-), to sprawdza czy mieści się w dozwolonym zakresie dla
 * współczynników.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @return Czy liczby występujące w wierszu wykraczają poza dozwolone zakresy?
 */
static bool InvalidNumbers(char *line, size_t line_size) {
    char *num_end = line;

    if (isdigit(line[0]) || line[0] == '-') {
        strtol(line, &num_end, 10);
        if (errno) {
            errno = 0;
            return true;
        }
    }
    for (size_t i = num_end - line; i < line_size - 1; i++) {
        if (isdigit(line[i]) && line[i - 1] == ',') {
            long tmp = strtol(line + i, &num_end, 10);
            i = num_end - line;
            if (errno || tmp < MIN_EXP || tmp > MAX_EXP) {
                errno = 0;
                return true;
            }
        }
        else if (isdigit(line[i]) || line[i] == '-') {
            strtol(line + i, &num_end, 10);
            i = num_end - line;
            if (errno) {
                errno = 0;
                return true;
            }
        }
    }
    return false;
}

/**
 * Sprawdza, czy w rozmieszczeniu nawiasów okrągłych w wierszu występują
 * podstawowe błędy.
 * Błąd zostaje stwierdzony, gdy nawiasy okrągłe tworzą niepoprawne wyrażenie
 * nawiasowe lub gdy dwa nawiasy okrągłe zamykające występują na sąsiednich
 * pozycjach w wierszu.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @return Czy w rozmieszczeniu nawiasów okrągłych w wierszu występują
 * podstawowe błędy?
 */
static bool InvalidBraces(char *line, size_t line_size) {
    int brace_depth = 0;
    
    for (size_t i = 0; i < line_size - 1; i++) {
        if (line[i] == '(') {
            brace_depth++;
        }
        if (line[i] == ')') {
            brace_depth--;
            if (line[i + 1] == ')') {
                return true;
            }
        }
        if (brace_depth < 0) {
            return true;
        }
    }
    return brace_depth > 0;
}

/**
 * Sprawdza, czy wiersz zawiera podstawowe błędy w opisie wielomianu.
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 * @return Czy wiersz zawiera podstawowe błędy w opisie wielomianu?
 */
static bool InitialInvalidPoly(char *line, size_t line_size) {
    bool res =
        InvalidPolyCharacter(line, line_size) ||
        InvalidPlusMinus(line, line_size) ||
        InvalidNumbers(line, line_size) ||
        InvalidBraces(line, line_size);
    return res;
}

/**
 * Parsuje wiersz zawierający współczynnik i wstawia go na wierzchołek stosu.
 * Jeżeli po współczynniku w wierszu występują znaki inne niż znak końca linii,
 * wywołuje WrongPoly().
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 */
static void ParseCoeff(char *line, size_t line_size) {
    char *ptr;

    poly_coeff_t coeff = strtol(line, &ptr, 10);
    if (ptr - line == (long int) line_size) {
        Poly new_poly = PolyFromCoeff(coeff);
        Push(&new_poly);
    }
    else if (ptr - line == (long int) line_size - 1 && ptr[0] != '\0') {
        Poly new_poly = PolyFromCoeff(coeff);
        Push(&new_poly);
    }
    else {
        WrongPoly();
    }
}

static Poly RecursivePoly(char *line, size_t line_size, size_t *id) {
    if (isdigit(line[*id]) || line[*id] == '-') {
        char *ptr;
        poly_coeff_t coeff = strtol(line + *id, &ptr, 10);
        *id = ptr - line;
        return PolyFromCoeff(coeff);
    }
    
    Mono *monos = MemAlloc(INIT_ARRAY_SIZE, sizeof(Mono));
    size_t monos_size = INIT_ARRAY_SIZE;
    size_t monos_id = 0;
    bool left_brace = false;
    bool comma = false;

    while (*id < line_size - 1) {
        if (line[*id] == '(') {
            if (left_brace) {
                errno = 1;
                break;
            }
            (*id)++;
            left_brace = true;
            monos[monos_id].p = RecursivePoly(line, line_size, id);
        }
        else if (line[*id] == ',') {
            if (comma || !isdigit(line[*id + 1])) {
                errno = 1;
                break;
            }
            (*id)++;
            comma = true;

            char *ptr;
            poly_exp_t exp = strtol(line + *id, &ptr, 10);
            *id = ptr - line;
            monos[monos_id].exp = exp;
            monos_id++;
            
            if (monos_id == monos_size) {
                monos_size *= 2;
                monos = MemRealloc(monos, monos_size, sizeof(Mono));
            }
        }
        else if (line[*id] == ')') {
            if (!comma) {
                errno = 1;
                break;
            }
            (*id)++;
            left_brace = false;

            if (line[*id] == ',') {
                break;
            }
        }
        else {
            (*id)++;
            comma = false;
        }
    }
    
    Poly new_poly = PolyAddMonos(monos_id, monos);
    free(monos);
    return new_poly;
}

/**
 * Parsuje wiersz niezawierający polecenia.
 * Jeżeli parsowanie zakończyło się bez błędów, wstawia na wierzchołek stosu
 * sparsowany wielomian.
 * Jeżeli podczas parsowania wystąpił błąd, wywołuje WrongPoly().\n
 * @param[in] line : wiersz
 * @param[in] line_size : długość wiersza
 */
static void ParsePoly(char *line, size_t line_size) {
    if (InitialInvalidPoly(line, line_size)) {
        WrongPoly();
        return;
    }
    if (isdigit(line[0]) || line[0] == '-') {
        ParseCoeff(line, line_size);
        return;
    }
    if (line[0] != '(') {
        WrongPoly();
        return;
    }

    size_t id = 0;
    Poly new_poly = RecursivePoly(line, line_size, &id);
    
    if (errno) {
        errno = 0;
        WrongPoly();
        PolyDestroy(&new_poly);
        return;
    }

    Push(&new_poly);
    PolyDestroy(&new_poly);
}

/**
 * Funkcja main programu.
 */
int main() {

    char *line = NULL;
    size_t line_size = 0;
    InitStack();

    while ((line_size = getline(&line, &line_size, stdin)) != (size_t) -1) {
        line_number++;

        if (line[0] == '#' || (line_size == 1 && line[0] == '\n')) {
            continue;
        }

        if (isalpha(line[0])) {
            ParseCommand(line, line_size);
        }
        else {
            ParsePoly(line, line_size);
        }
    }

    while (!IsEmpty()) {
        Pop();
    }
    free(stack.arr);
    free(line);
}