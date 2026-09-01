# Emo Whacker

A whack-a-mole style game about managing your emotions, written in C++ with the WinBGIm graphics library.

Whack the negative emotions. Grab the positive ones. Score as high as you can in 30 seconds.

**[Watch the demo video](https://youtu.be/zmHXXFmtuQs)**

---

## About

This project combines two goals: build something genuinely fun to play, and practise object-oriented programming in C++ along the way.

The game takes the classic *Whack-A-Mole* formula and swaps the moles for emoticons. Whacking a sad or angry face stands in for confronting a negative emotion; grabbing a happy one stands in for reaching for the good stuff. It's a simple idea, but it gave us a real reason to work through inheritance, polymorphism, and class design rather than practising them as an exercise for their own sake.

---

## Gameplay

You get **30 seconds**. Emoticons pop up one at a time from a 3×3 grid of holes, each staying visible for about a second.

You have two tools, and **the spacebar switches between them**:

- **Hammer** — whack it
- **Claw** — grab it

**Left click** to use the current tool on whichever emoticon is on screen. Which tool you're holding when you click decides whether you gain or lose marks:

| Emoticon | Whack (Hammer) | Grab (Claw) |
|---|---|---|
| 😊 Happy (yellow) | **−10** | **+30** |
| 😢 Sad (blue) | **+15** | **−15** |
| 😠 Angry (red) | **+20** | **−20** |

Missing an emoticon costs you nothing, so there's no penalty for hesitating — but the clock keeps running.

**Two ways the game ends:**
- The 30 seconds run out
- Your score drops **below zero**, which ends the run immediately

Either way you land on the scoreboard, showing your final mark alongside the top 3 scores.

Your current score and the countdown stay visible at the top of the screen while you play.

---

## Running It

The game uses `graphics.h` (WinBGIm), which is **Windows-only**. You'll need a C++ compiler and the WinBGIm library.

### 1. Get a compiler

Either works:

- **[MSYS2](https://www.msys2.org/)** with the MinGW-w64 toolchain — then run `pacman -S mingw-w64-x86_64-gcc`
- **[Code::Blocks](https://www.codeblocks.org/downloads/binaries/)** — download the installer with **`mingw`** in the filename, e.g. `codeblocks-25.03mingw-setup.exe`. The plain one has no compiler bundled.

Confirm it works by opening a terminal and running `g++ --version`.

### 2. Get WinBGIm

Download **`graphics.h`** and **`libbgi.a`** from the [WinBGIm-64 releases page](https://github.com/ahmedshakill/WinBGIm-64/releases) (v1.0.1 or later).

> **Use this build, not the original from winbgim.codecutter.org.** That one was last compiled around 2006 and is 32-bit only, so a modern 64-bit toolchain refuses to link against it.

Copy the two files into your compiler's folders:

| File | Goes to |
|---|---|
| `graphics.h` | `<MinGW>\include\` |
| `libbgi.a` | `<MinGW>\lib\` |

Where `<MinGW>` is typically `C:\msys64\mingw64` or `C:\Program Files\CodeBlocks\MinGW`. If you're not sure, run `where.exe g++` in a terminal — it prints the path to `g++.exe`, and the MinGW folder is two levels up from there.

### 3. Build

```bash
g++ molnir.cpp -o molnir.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

The linker flags matter, and so does their order — `-lbgi` has to come first.

### 4. Play

```bash
./molnir.exe
```

The menu opens. Pick **Instructions** to read the rules in-game, or **Start Game**, type your name, and hit Enter.
