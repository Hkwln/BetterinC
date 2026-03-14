# Übung: Syntax-Directed Translator in C

## Infix-Ausdruck → Intermediate Code (mit Variablen & Symbol Table)

> Ziel: Du erweiterst deinen mini-Übersetzer schrittweise bis zu einem echten
> Compiler-Frontend: Lexer (2.6) → Parser → Symbol Table (2.7) → Intermediate Code (2.8).
> Kein Framework, keine Bibliothek – nur C, dein Editor und das Dragon Book (Kap. 2).

---

## Was du bisher gebaut hast ✓

Ein Programm, das arithmetische Ausdrücke von **Infix** in **Postfix** übersetzt:

```
Eingabe:  3 + 4 * 2
Ausgabe:  3 4 2 * +

Eingabe:  (1 + 2) * (3 - 4)
Ausgabe:  1 2 + 3 4 - *

Eingabe:  10 / 2 + 3
Ausgabe:  10 2 / 3 +
```

Das war Kap. 2.1–2.5. Jetzt geht es weiter.

---

## Schritt 1 – Lexer verbessern (Kap. 2.6) ✓ / erweiterbar

Du hast bereits einen funktionierenden Lexer. Kap. 2.6 führt zwei wichtige
Erweiterungen ein, die du jetzt brauchst:

**Bezeichner (Identifier):** Variablennamen wie `x`, `foo`, `total`.

Erweitere dein Token um:

- `TOKEN_IDENT` – ein Name, z.B. `x` oder `result`
- `TOKEN_ASSIGN` – `=`

Erweitere dein `Token`-Struct um ein `char name[64]` Feld für den Namen.

**Testfälle:**

```
"x = 3 + 4"   → IDENT(x), ASSIGN, NUMBER(3), PLUS, NUMBER(4), EOF
"y = x * 2"   → IDENT(y), ASSIGN, IDENT(x), STAR, NUMBER(2), EOF
```

---

## Schritt 2 – Symbol Table (Kap. 2.7)

Eine Symbol Table speichert alle Variablen, die das Programm kennt.
Für jetzt reicht: Name → Wert (integer).

Implementiere eine einfache Symbol Table mit zwei Funktionen:

```c
void   symtable_set(const char *name, int value);
int    symtable_get(const char *name);  /* exit(1) wenn nicht gefunden */
```

Intern: ein Array von Einträgen mit festem Limit (z.B. 64 Variablen) reicht.

```c
typedef struct { char name[64]; int value; } Symbol;
Symbol table[64];
int table_size = 0;
```

**Wann wird sie benutzt?**

- `symtable_set` → wenn eine Zuweisung geparst wird: `x = 3 + 4`
- `symtable_get` → wenn ein Bezeichner in einem Ausdruck vorkommt: `x * 2`

---

## Schritt 3 – Parser erweitern: Zuweisungen

Erweitere die Grammatik um Zuweisungen und Bezeichner als Werte:

```
program → stmt*
stmt    → IDENT '=' expr
expr    → term   ( ('+' | '-') term   )*
term    → factor ( ('*' | '/') factor )*
factor  → NUMBER | IDENT | '(' expr ')'
```

`factor()` muss jetzt bei `TOKEN_IDENT` in der Symbol Table nachschlagen.
`stmt()` parst eine Zuweisung und speichert das Ergebnis in der Symbol Table.

**Eingabe (mehrere Zeilen):**

```
x = 3 + 4
y = x * 2
```

**Ausgabe (Zwischenschritt — Postfix je Zeile):**

```
x = 3 4 +
y = x 2 *
```

---

## Schritt 4 – Intermediate Code Generation (Kap. 2.8)

Statt Postfix ausgeben: generiere **3-Adress-Code** (Three-Address Code).
Das ist die Sprache echter Compiler-Backends (LLVM IR, GCC RTL).

Jede Instruktion hat die Form: `temp = operand op operand`

```
Eingabe:  x = 3 + 4 * 2
Ausgabe:
  t1 = 4 * 2
  t2 = 3 + t1
  x  = t2
```

Dafür brauchst du einen **temporären Variablenzähler**:

```c
int tmp_count = 0;
char *new_temp(void);  /* gibt "t1", "t2", ... zurück */
```

Ändere `expr()`, `term()` und `factor()` so, dass sie einen Tempnamen
zurückgeben (`char *`) statt direkt zu drucken. Die Instruktion wird erst
gedruckt, wenn beide Operanden bekannt sind.

**Eingabe:**

```
x = 3 + 4 * 2
y = (x + 1) * 3
```

**Ausgabe:**

```
t1 = 4 * 2
t2 = 3 + t1
x  = t2
t3 = x + 1
t4 = t3 * 3
y  = t4
```

---

## Erwartete Dateistruktur

```
syntax_directed_translator/
├── exercise.md       ← diese Datei
├── src/
│   ├── lexer.h / lexer.c       ← Token-Typen, next_token()
│   ├── symtable.h / symtable.c ← Symbol Table (neu)
│   ├── parser.h / parser.c     ← expr/term/factor/stmt
│   └── main.c                  ← Eingabe lesen, Schleife über Zeilen
└── Makefile
```

---

## Hinweise

- **Temporäre Namen:** `new_temp()` kann einfach `sprintf` in einen statischen
  Puffer schreiben: `sprintf(buf, "t%d", ++tmp_count)`.
- **Mehrere Zeilen:** `main()` liest in einer Schleife mit `fgets` — eine Zeile
  pro `stmt()` Aufruf. Vergiss nicht `advance()` vor jedem `stmt()`.
- **Bezeichner in `factor()`:** Wenn du einen `TOKEN_IDENT` siehst, ruf
  `symtable_get()` auf und gib den Tempnamen zurück wie bei einer Zahl.

---

## Erfolgskriterien

```bash
printf "x = 3 + 4 * 2\ny = x + 1\n" | ./translator
```

Erwartete Ausgabe:

```
t1 = 4 * 2
t2 = 3 + t1
x  = t2
t3 = x + 1
y  = t3
```

---

## Verbindung zum Dragon Book

| Konzept hier              | Dragon Book                        |
| ------------------------- | ---------------------------------- |
| `Token` + `next_token`    | Lexical Analysis (2.6)             |
| Bezeichner, `=`           | Lexical Analysis (2.6)             |
| `symtable_get/set`        | Symbol Tables (2.7)                |
| `expr/term/factor`        | Recursive-Descent Parsing (2.4)    |
| `stmt` + Zuweisung        | Syntax-Directed Translation (2.3)  |
| 3-Adress-Code, `new_temp` | Intermediate Code Generation (2.8) |
