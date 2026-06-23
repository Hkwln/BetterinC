# Übung: Syntax-Directed Translator in C

AIM for this project:

implement if / while with jump code ( 2.9)
//fix so that this behaviour is valid:
4+x = 10-2

It would be a DREAM if this project turns into a real C-compiler

### TODO: DOES it matter? -- needs a AST

What should be the case:
a+a*(b-c)+(b-c) * d;
t_1 = b+c;
t_2 = a* t_1;
t_3 = a+ t_2;
t_4 = t_1*d;
t_5 = t_3+t_4;

MY program:
a+a*(b-c)+(b-c) * d;
t_1 = b+c;
t_2 = a* t_1;
t_3 = a+ t_2;
t_4 = b+c; +++++
t_5 = t_4*d;
t_6 = t_3+t_4;

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

## Erwartete Dateistruktur

```
syntax_directed_translator/
├── exercise.md       ← diese Datei
├── src/
│   ├── lexer.h / lexer.c       ← Token-Typen, next_token()
│   ├── symtable.h / symtable.c ← Symbol Table
│   ├── parser.h / parser.c     ← expr/term/factor/stmt
│   └── main.c                  ← Eingabe lesen, Schleife über Zeilen
└── Makefile
```

## Verbindung zum Dragon Book

| Konzept hier              | Dragon Book                        |
| ------------------------- | ---------------------------------- |
| `Token` + `next_token`    | Lexical Analysis (2.6)             |
| Bezeichner, `=`           | Lexical Analysis (2.6)             |
| `symtable_get/set`        | Symbol Tables (2.7)                |
| `expr/term/factor`        | Recursive-Descent Parsing (2.4)    |
| `stmt` + Zuweisung        | Syntax-Directed Translation (2.3)  |
| 3-Adress-Code, `new_temp` | Intermediate Code Generation (2.8) |
