# Plymouth Boot Animation Integration - TODO (TTY Mode)

## Ziel
Conway's Game of Life als **text-basierte** Plymouth Boot Animation im TTY-Stil, die nahtlos zu `ly` Login übergeht.

**Design-Philosophie:**
- ✅ Nur Terminal/TTY (kein Framebuffer-Rendering)
- ✅ Schwarz-Weiß ASCII-Art (`#` und `.`)
- ✅ Minimalistisch wie `ly` login manager
- ✅ Nahtloser Übergang (gleiches Look & Feel)

---

## Phase 1: Plymouth Plugin Struktur

### 1.1 Plymouth "two-step" Plugin verstehen
**Wichtig:** Wir nutzen **ModuleName=two-step** statt `script`!

Das `two-step` Plugin ist für text-basierte Themes:
- [ ] Analysiere `/usr/share/plymouth/themes/text/` als Beispiel
- [ ] `two-step` Plugin nutzt direkt den Text-Modus
- [ ] Keine Grafik, nur ASCII-Zeichen im Terminal

### 1.2 Theme Verzeichnis erstellen
- [ ] Theme-Ordner anlegen: `/usr/share/plymouth/themes/conway-gol/`
- [ ] Benötigte Dateien:
  - [ ] `conway-gol.plymouth` - Theme Descriptor (minimal)
  - [ ] `conway-gol` - Ausführbares Binary (unser Programm)

### 1.3 Theme Descriptor schreiben
Datei: `conway-gol.plymouth`

```ini
[Plymouth Theme]
Name=Conway Game of Life
Description=Text-based Conway's Game of Life Boot Animation
ModuleName=two-step

[two-step]
ImageDir=/usr/share/plymouth/themes/conway-gol
DialogHorizontalAlignment=.5
DialogVerticalAlignment=.5
TitleHorizontalAlignment=.5
```

**Frage beantwortet:**
- ❌ **Kein `console-log-background-color` nötig** - nur für graphische Themes
- ❌ **Kein `script-env-var` nötig** - nur für Script-Plugin
- ✅ `two-step` ist perfekt für TTY-only Themes!

---

## Phase 2: Code Anpassungen für TTY-Modus

### 2.1 TTY Terminal Detection
**Aktuell:** Code nutzt bereits `printf()` - PERFEKT!  
**Benötigt:** Minimale Anpassungen für Plymouth TTY

- [ ] Terminal-Größe dynamisch erkennen (nicht hardcoded 40x40)
- [ ] Verwende `TIOCGWINSZ` ioctl für Terminal-Größe:
  ```c
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  uint32_t term_cols = w.ws_col;
  uint32_t term_rows = w.ws_row;
  ```
- [ ] Grid-Größe an Terminal anpassen
- [ ] Funktioniert ohne Änderung am Render-Code!

### 2.2 STDOUT statt Escape-Codes
**Plymouth two-step schreibt in STDOUT**

Aktuelle Änderungen nötig:
- [ ] Entferne `\033[H` (cursor home) - Plymouth steuert das
- [ ] Entferne `\033[?25l` und `\033[?25h` (cursor hide/show)
- [ ] Nutze einfach `printf()` für Grid-Output
- [ ] **Kein Framebuffer, kein /dev/fb0 - einfach nur Text!**

### 2.3 Minimalistisches Design (wie ly)
- [ ] Nur ASCII: `#` für lebendig, `.` für tot
- [ ] Schwarz-Weiß Terminal (kein ANSI-Color nötig)
- [ ] Kein Progressbar, kein fancy UI
- [ ] Cleaner Look: nur das Grid

**Optional:**
- [ ] Boot-Message unten: "Booting..." oder Hostname
- [ ] Minimale Statuszeile möglich

### 2.4 Performance-Optimierung
- [ ] FPS limitieren auf 10-20 (text-mode muss nicht schnell sein)
- [ ] CPU-Last minimal halten (wichtig während Boot!)
- [ ] Grid-Größe reduzieren falls Terminal zu klein (< 20x20)

---

## Phase 3: Plymouth Integration (two-step Mode)

### 3.1 Standalone Binary für two-step
**Einfacher Ansatz:** Kein Plymouth-Plugin-API nötig!

Das `two-step` Modul ruft einfach ein Binary auf:
- [ ] Kompiliere `live` Binary (bereits fertig!)
- [ ] Binary nach `/usr/share/plymouth/themes/conway-gol/` kopieren
- [ ] Plymouth ruft das Binary beim Boot auf
- [ ] Binary schreibt in STDOUT → Plymouth zeigt es an

**Keine Plugin-API nötig!** Viel einfacher als gedacht.

### 3.2 Code-Anpassungen für Plymouth-Mode
- [ ] Entferne interaktive Eingaben (falls vorhanden)
- [ ] Infinite Loop mit Signal-Handling:
  ```c
  volatile sig_atomic_t running = 1;
  void sigterm_handler(int sig) { running = 0; }
  
  int main() {
      signal(SIGTERM, sigterm_handler);
      signal(SIGINT, sigterm_handler);
      
      while(running) {
          loop(...);
          usleep(100000);
      }
  }
  ```
- [ ] Plymouth sendet SIGTERM zum Beenden
- [ ] Graceful Shutdown implementieren

### 3.3 Alternative: Script Wrapper (noch einfacher)
Falls Binary-Probleme:

Erstelle `conway-gol` Shell-Script:
```bash
#!/bin/bash
cd /usr/share/plymouth/themes/conway-gol
exec ./live
```

- [ ] Script macht Binary ausführbar
- [ ] Plymouth ruft Script auf
- [ ] Funktioniert identisch

---

## Phase 4: Boot-Übergang zu `ly`

### 4.1 Plymouth Exit Detection
- [ ] Plymouth sendet Signal beim Beenden
- [ ] Animation sanft ausblenden (Fade-out)
- [ ] Framebuffer aufräumen für `ly`

### 4.2 Nahtloser Übergang (TTY zu TTY)
**Perfekt:** Beide nutzen denselben TTY!

**Strategie:**
- [ ] Plymouth läuft auf TTY1 (Game of Life Animation)
- [ ] `ly` übernimmt TTY1 nach Plymouth-Exit
- [ ] Kein Framebuffer-Konflikt - beide nutzen Text-Modus
- [ ] Letzter Frame von Game of Life wird von `ly` überschrieben

**Empfehlung: Einfach beenden**
- [ ] SIGTERM empfangen → Animation stoppen
- [ ] Terminal clearen: `printf("\033[2J\033[H");`
- [ ] Programm beenden
- [ ] `ly` startet und übernimmt TTY

**Kein Fade-out nötig** - TTY-Übergang ist instant!

### 4.3 ly Konfiguration
- [ ] `ly` läuft automatisch auf TTY1 nach Plymouth
- [ ] **Kein Framebuffer-Konflikt** - beide nutzen nur TTY
- [ ] Perfekte Kompatibilität: Text-Modus zu Text-Modus
- [ ] Keine Anpassung in `/etc/ly/config.ini` nötig!

---

## Phase 5: Installation & Testing

### 5.1 Build-System (SUPER EINFACH!)
Kein Plymouth-API nötig, nur normales Binary:

```makefile
# Normale Kompilierung (wie bisher)
live: live.c bitmap.c data.c draw.c
	gcc -o live live.c bitmap.c data.c draw.c -lm

# Plymouth Installation
install-plymouth: live
	sudo mkdir -p /usr/share/plymouth/themes/conway-gol/
	sudo cp live /usr/share/plymouth/themes/conway-gol/
	sudo cp conway-gol.plymouth /usr/share/plymouth/themes/conway-gol/
	sudo chmod +x /usr/share/plymouth/themes/conway-gol/live
```

**Das war's!** Kein `.so` kompilieren, keine Plymouth-Bibliotheken.

### 5.2 Theme aktivieren
```bash
sudo plymouth-set-default-theme conway-gol
sudo update-initramfs -u
```

### 5.3 Testing ohne Reboot
```bash
# Plymouth im Testmodus
sudo plymouthd --debug --debug-file=/tmp/plymouth-debug.log
sudo plymouth show-splash
# Animation sollte starten
sudo plymouth quit
```

### 5.4 Echtes Boot-Testing
- [ ] Neu booten und Animation testen
- [ ] Übergang zu `ly` beobachten
- [ ] Logs prüfen: `/tmp/plymouth-debug.log`

---

## Phase 6: Feinschliff

### 6.1 Boot-Fortschritt Integration
- [ ] Boot-Prozent in Animation zeigen
- [ ] Z.B. Progressbar unten
- [ ] Oder Zell-Dichte ändert sich mit Fortschritt

### 6.2 Boot-Nachrichten
- [ ] Systemd Boot-Messages optional anzeigen
- [ ] Klein in Ecke oder als Overlay
- [ ] Oder komplett verstecken für cleanen Look

### 6.3 Fehlerbehandlung
- [ ] Graceful Fallback wenn Framebuffer fehlt
- [ ] Initramfs muss Framebuffer-Module laden
- [ ] Fehler-Logging für Debug

---

## Technische Herausforderungen

### ⚠️ Problem 1: Initramfs Größe
Plymouth läuft in initramfs (früh im Boot)
- [ ] Code muss klein sein (<500KB)
- [ ] Statisch linken oder minimale Deps
- [ ] Evtl. Grid-Größe reduzieren (20x20 statt 40x40)

### ✅ Kein Problem: Terminal zu Terminal!
- [ ] Code nutzt bereits `printf()` - PERFEKT!
- [ ] Minimale Anpassungen (Escape-Codes entfernen)
- [ ] Text-Modus funktioniert out-of-the-box!

### ⚠️ Problem 3: Timing
- [ ] Boot dauert 5-30 Sekunden
- [ ] Animation muss schön aussehen bei verschiedenen Längen
- [ ] Zu schnell = hektisch, zu langsam = langweilig

### ⚠️ Problem 4: Architektur-Kompatibilität
- [ ] Funktioniert auf x86_64, ARM, etc.?
- [ ] Framebuffer-Format kann unterschiedlich sein (RGB vs BGR)
- [ ] Testen auf Zielarchitektur

---

## TTY-Modus Vorteile

### Warum two-step besser ist als Framebuffer:

✅ **Super einfach** - Kein Plymouth-Plugin-API  
✅ **Kein Framebuffer** - Nur normales Terminal  
✅ **Nahtlos zu ly** - Beide nutzen TTY  
✅ **Minimalistisch** - Schwarz-Weiß ASCII-Art  
✅ **Weniger Code** - Nutzt existierendes `printf()`  
✅ **Schneller** - Kein Pixel-Rendering overhead  

### Was bereits funktioniert:
- ✅ `print_bitmap()` Funktion
- ✅ Game of Life Logic
- ✅ Terminal-Output mit `printf()`

### Was geändert werden muss:
- ⚠️ Escape-Codes entfernen (`\033[H`, `\033[?25l`)
- ⚠️ Terminal-Größe dynamisch erkennen
- ⚠️ Signal-Handling für SIGTERM
- ⚠️ Infinite Loop statt 1000 Epochen

---

## Ressourcen

### Plymouth Dokumentation
- `/usr/share/plymouth/themes/` - Beispiel Themes
- `man plymouth` - Plymouth Manpage
- `/usr/include/plymouth-1/ply-boot-splash-plugin.h` - Plugin API

### Framebuffer
- `/dev/fb0` - Framebuffer Device
- `man fbdev` - Framebuffer Dokumentation
- `fbset` - Framebuffer-Info anzeigen

### Initramfs
- `lsinitramfs /boot/initrd.img-$(uname -r)` - Inhalt anzeigen
- `update-initramfs -u` - Initramfs neu bauen

---

## Zeitabschätzung (TTY-Modus)

| Phase | Aufwand | 
|-------|---------|
| Phase 1: Plymouth two-step verstehen | 1-2 Stunden |
| Phase 2: Code für TTY anpassen | 2-3 Stunden |
| Phase 3: Binary Integration | 1-2 Stunden |
| Phase 4: ly Übergang testen | 1-2 Stunden |
| Phase 5: Testing & Debug | 2-4 Stunden |
| Phase 6: Polish | 1-2 Stunden |
| **Gesamt** | **8-15 Stunden** |

**Viel weniger Aufwand** da kein Framebuffer/Plugin nötig!

---

## Nächste Schritte (Empfohlen)

### Schritt 1: Code für Plymouth vorbereiten (2-3 Stunden)
```c
// In live.c - Änderungen:
1. Terminal-Größe dynamisch: ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)
2. Signal-Handler: signal(SIGTERM, sigterm_handler)
3. Escape-Codes entfernen: Kein \033[H, \033[?25l
4. Infinite Loop: while(running) statt for(e=0; e<1000; e++)
5. Cleaner Output: Nur Grid, keine Progressbar
```

### Schritt 2: .plymouth Datei erstellen (15 Minuten)
```ini
[Plymouth Theme]
Name=Conway Game of Life
Description=Text-based Conway's Game of Life
ModuleName=two-step

[two-step]
ImageDir=/usr/share/plymouth/themes/conway-gol
```

### Schritt 3: Installation testen (30 Minuten)
```bash
make
sudo mkdir -p /usr/share/plymouth/themes/conway-gol/
sudo cp live /usr/share/plymouth/themes/conway-gol/
sudo cp conway-gol.plymouth /usr/share/plymouth/themes/conway-gol/
sudo plymouth-set-default-theme conway-gol
sudo update-initramfs -u
```

### Schritt 4: Testen ohne Reboot (1 Stunde)
```bash
sudo plymouthd --debug --tty=/dev/tty7
sudo plymouth show-splash
# Animation sollte laufen!
sudo plymouth quit
```

### Schritt 5: Echter Boot-Test (1 Stunde)
```bash
sudo reboot
# Beobachte Game of Life beim Booten
# Übergang zu ly testen
```

**So einfach ist es!** 🎮➡️🚀

---

## Beispiel: Minimale Code-Änderungen

```c
// Vor main():
volatile sig_atomic_t running = 1;
void sigterm_handler(int sig) { running = 0; }

int main() {
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);
    
    // Terminal-Größe erkennen
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    uint32_t width = w.ws_col;
    uint32_t height = w.ws_row - 2; // -2 für Rand
    
    // ... init code ...
    
    // OHNE Escape-Codes:
    printf("Generation 0:\n");
    print_bitmap(live);
    
    // Infinite Loop
    for (int e = 0; running; e++) {
        // Kein \033[H - Plymouth steuert das
        printf("Generation %d\n", e + 1);
        
        // ... game logic ...
        
        print_bitmap(current_bmp);
        fflush(stdout);
        usleep(100000);
    }
    
    // Cleanup bei SIGTERM
    printf("\033[2J\033[H"); // Clear screen
    return 0;
}
```

Viel Erfolg mit dem TTY-Theme! 🚀
