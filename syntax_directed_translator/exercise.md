# Übung: Syntax-Directed Translator in C

## Infix-Ausdruck → Postfix (Reverse Polish Notation)

> Ziel: Du baust from scratch einen mini-Übersetzer in C.
> Kein Framework, keine Bibliothek – nur C, dein Editor und das Dragon Book (Kap. 2).

---

## Was du baust

Ein Programm, das arithmetische Ausdrücke von **Infix** in **Postfix** übersetzt:

```
Eingabe:  3 + 4 * 2
Ausgabe:  3 4 2 * +

Eingabe:  (1 + 2) * (3 - 4)
Ausgabe:  1 2 + 3 4 - *

Eingabe:  10 / 2 + 3
Ausgabe:  10 2 / 3 +
```

Warum Postfix? Stack-Maschinen (JVM, WebAssembly, frühe Compiler-Backends) arbeiten
genau so. Wenn du das verstehst, verstehst du wie Code-Generierung funktioniert.

---

## Schritt 1 – Der Lexer (Tokenizer)

**Aufgabe:** Schreibe eine Funktion, die einen String zeichenweise liest und Tokens produziert.

Ein Token ist eine der folgenden Kategorien:

- `TOKEN_NUMBER` – eine ganze Zahl, z.B. `42`
- `TOKEN_PLUS` – `+`
- `TOKEN_MINUS` – `-`
- `TOKEN_STAR` – `*`
- `TOKEN_SLASH` – `/`
- `TOKEN_LPAREN` – `(`
- `TOKEN_RPAREN` – `)`
- `TOKEN_EOF` – Ende der Eingabe

```c
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_STAR, TOKEN_SLASH,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value; /* nur relevant wenn type == TOKEN_NUMBER */
} Token;
```

**Deine Aufgabe:**
Implementiere `Token next_token(const char **src)` — sie liest das nächste Token
aus `*src` und rückt den Zeiger vor. Leerzeichen überspringen.

**Testfälle für Schritt 1:**

```
"3 + 4"   → NUMBER(3), PLUS, NUMBER(4), EOF
"(10/2)"  → LPAREN, NUMBER(10), SLASH, NUMBER(2), RPAREN, EOF
```

---

## Schritt 2 – Der Parser (Rekursiver Abstieg)

**Konzept:** Du übersetzt die Grammatik direkt in C-Funktionen.
Jede Regel wird eine Funktion. Das ist _recursive descent parsing_.

Die Grammatik für Ausdrücke mit korrekter Präzedenz:

```
expr    → term  ( ('+' | '-') term  )*
term    → factor ( ('*' | '/') factor )*
factor  → NUMBER | '(' expr ')'
```

Warum diese Struktur? `*` und `/` binden stärker als `+` und `-`.
Die Grammatik kodiert das direkt — `term` wird _innerhalb_ von `expr` aufgerufen,
also werden Multiplikationen zuerst verarbeitet.

**Deine Aufgabe:**
Implementiere drei Funktionen:

```c
void expr(void);    /* verarbeitet + und - */
void term(void);    /* verarbeitet * und / */
void factor(void);  /* verarbeitet Zahlen und geklammerte Ausdrücke */
```

Jede Funktion soll beim Parsen sofort die Postfix-Ausgabe erzeugen (→ `printf`).
Das ist _syntax-directed translation_: die Ausgabe wird während des Parsens generiert.

**Hinweis zur Ausgabereihenfolge:**
Bei `expr`: erst linken `term` parsen (gibt schon Postfix aus), dann `term` rechts,
dann erst den Operator ausgeben. Bei `factor`: Zahl sofort ausgeben.

---

## Schritt 3 – Alles zusammenbauen

Schreibe `main()`, die:

1. Eine Zeile von `stdin` liest
2. Den Lexer initialisiert
3. `expr()` aufruft
4. Einen Zeilenumbruch ausgibt

Minimales Gerüst:

```c
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* --- Token-Typen --- */
/* --- Globaler "lookahead" Token --- */
Token lookahead;
const char *cursor;

/* next_token() liest das nächste Token */
/* advance()    ruft next_token() auf und speichert in lookahead */
/* match(type)  prüft ob lookahead == type, dann advance(), sonst Fehler */

void factor(void);
void term(void);
void expr(void);

int main(void) {
    char line[256];
    if (!fgets(line, sizeof(line), stdin)) return 1;
    cursor = line;
    advance();   /* erstes Token laden */
    expr();
    printf("\n");
    return 0;
}
```

---

## Schritt 4 – Bonus: Stack-VM Code-Generierung

Statt Postfix-Zahlen auszugeben, generiere Instruktionen für eine imaginäre Stack-VM:

```
Eingabe:  3 + 4 * 2
Ausgabe:
  PUSH 3
  PUSH 4
  PUSH 2
  MUL
  ADD
```

Das ist ein echter erster Schritt in Richtung Code-Generierung.
Ändere nur die Ausgabe in `expr()`, `term()` und `factor()` — der Parser bleibt gleich.

---

## Schritt 5 – Bonus: Evaluator

Statt Instruktionen ausgeben: Ausdrücke direkt berechnen.
Ändere die Funktionen so, dass sie `int` zurückgeben:

```c
int expr(void);
int term(void);
int factor(void);
```

Dann gib am Ende das Ergebnis aus. Vergleiche mit einem Taschenrechner.

---

## Erwartete Dateistruktur

```
syntax_directed_translator/
├── exercise.md          ← diese Datei
├── translator.c         ← deine Implementierung
└── Makefile             ← optional: gcc -Wall -o translator translator.c
```

---

## Hinweise & häufige Fehler

- **Lookahead:** Der Parser muss immer _ein Token voraus_ schauen. Halte immer
  eine globale `lookahead`-Variable aktuell.
- **match() nicht vergessen:** Nach dem Erkennen eines Tokens musst du es
  "konsumieren" (advance aufrufen), sonst läuft der Parser in einer Endlosschleife.
- **Klammern in `factor()`:** `'(' expr ')'` — du konsumierst `(`, rufst `expr()`
  auf (der gibt schon alles aus), dann konsumierst du `)`. Nichts extra ausgeben.
- **Fehlerbehandlung:** Erstmal nur `exit(1)` mit einer Fehlermeldung reicht.

---

## Erfolgskriterien

```bash
echo "3 + 4 * 2"      | ./translator   # → 3 4 2 * +
echo "(1 + 2) * 3"    | ./translator   # → 1 2 + 3 *
echo "10 / 2 + 3 * 4" | ./translator   # → 10 2 / 3 4 * +
echo "((1+2))"        | ./translator   # → 1 2 +
```

Wenn diese vier Fälle stimmen, hast du einen funktionierenden Syntax-Directed Translator.

---

## Verbindung zum Dragon Book

| Konzept hier           | Dragon Book Kap. 2                      |
| ---------------------- | --------------------------------------- |
| `Token` + `next_token` | Lexical Analysis (2.6)                  |
| Grammatikregeln        | Context-Free Grammars (2.2)             |
| `expr/term/factor`     | Recursive-Descent Parsing (2.4)         |
| Ausgabe beim Parsen    | Syntax-Directed Translation (2.3)       |
| Bonus: Stack-VM        | Intermediate Code Generation (Kap. 6/8) |

Nach dieser Übung wirst du Kap. 2 mit ganz anderen Augen lesen.
