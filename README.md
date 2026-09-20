# wish — простий Unix-шелл (OSTEP `processes-shell`)

Реалізація завдання [processes-shell](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/processes-shell/README.md)
мовою C++17 з використанням POSIX API: `fork()`, `execv()`, `waitpid()`, `chdir()`, `access()`, `dup2()`.

Статус: **22/22 офіційних тестів проходять**.

---

## Важливо про Visual Studio 2022

Шелл принципово потребує `fork()`/`execv()` — цього **немає** у Windows і MSVC.
Тому проєкт зібрано як **CMake-проєкт VS 2022, що цілиться у WSL2 (Ubuntu + g++)**.
Код редагується і дебажиться у VS, а компілюється та запускається всередині Linux.

### Налаштування один раз

1. У PowerShell (від адміністратора): `wsl --install -d Ubuntu`, перезавантаження.
2. В Ubuntu: `sudo apt update && sudo apt install -y build-essential gdb cmake git rsync zip`
3. У Visual Studio Installer додати робоче навантаження **«Розробка на C++ для Linux»**
   (Linux and embedded development with C++) і компонент **C++ CMake tools**.

### Відкриття проєкту

`File → Open → Folder…` → обрати цю теку. VS сам побачить `CMakeLists.txt` і `CMakePresets.json`.
У випадному списку конфігурацій обрати **`wsl-debug`** → `Build → Build All`.

Це і є «проєкт VS 2022»: сучасна VS працює з CMake-текою без `.sln`/`.vcxproj`.

> Якщо VS здасться важкою для цього завдання — легша альтернатива: **VS Code + розширення WSL**
> (відкрити теку прямо в Ubuntu, `make`, дебаг через gdb). Функціонально те саме, менше налаштувань.

---

## Збірка й тести (термінал WSL)

```bash
make                # збирає ./wish
./test-wish.sh      # проганяє всі 22 тести
./test-wish.sh -c   # показує діф, коли тест падає
make clean
```

Тести (`tests/`, `tester/`, `test-wish.sh`) — офіційні, з репозиторію OSTEP.
Оновити: `./scripts/get-tests.sh`.

Ручна перевірка:

```bash
./wish                    # інтерактивний режим
./wish batch.txt          # пакетний режим (без запрошення)
```

---

## Структура

| Файл | Призначення |
|---|---|
| `src/main.cpp` | Вибір режиму (інтерактивний / пакетний), цикл читання через `getline()` |
| `src/parser.h/.cpp` | Лексер (`>` і `&` — окремі лексеми навіть без пробілів) і валідація |
| `src/shell.h/.cpp` | Шлях пошуку, вбудовані команди, `fork`/`execv`/`waitpid`, перенаправлення |
| `src/error.h/.cpp` | Єдине повідомлення `An error has occurred` у `stderr` |

## Ключові рішення (для співбесіди)

- **Один рядок → список команд.** `parse_line()` ріже рядок по `&`; порожні сегменти
  (`"cmd &"`, рядок із пробілів, сам `&`) відкидаються **без** помилки.
- **Паралельність.** Спочатку запускаються **всі** процеси рядка, і лише потім
  батько чекає на кожен `waitpid()` — інакше це була б послідовність, а не паралель.
- **Пошук програми — у батьку.** `access(dir + "/" + name, X_OK)` викликається до `fork()`,
  щоб повідомлення про невідому команду не потрапило у файл перенаправлення.
- **Перенаправлення.** `open(O_CREAT|O_WRONLY|O_TRUNC, 0644)` у нащадку, `dup2` і на stdout,
  і на stderr (особливість цього завдання). Синтаксичні помилки: `>` без команди, без файлу,
  два файли, два `>`.
- **Вбудовані команди у батьківському процесі.** `exit` (0 аргументів), `cd` (рівно 1),
  `path` (0+, завжди перезаписує; початково `/bin`).
- **Фатальні помилки** (>1 аргумент запуску, нечитабельний batch-файл) → `exit(1)`;
  решта помилок → повідомлення і робота триває.
- **Чому не `system()`:** заборонено умовою; і це був би не шелл, а обгортка над `/bin/sh`.

---

## Здача: історія в git

Викладач дивиться на **послідовність розроблення**, тож комітити варто етапами,
перевіряючи кожен крок (`make && ./test-wish.sh`):

```bash
git init
git add .gitignore README.md CMakeLists.txt CMakePresets.json Makefile && git commit -m "Каркас проєкту: CMake + Makefile"
# далі — по одному кроку, після того як ви прогнали тести:
#  2. цикл читання та запрошення wish>            (main.cpp, error.cpp)
#  3. лексер і розбір рядка                        (parser.*)
#  4. fork/execv/waitpid для однієї команди        (shell.*)
#  5. пошук у path через access()
#  6. вбудовані exit / cd / path
#  7. перенаправлення > (stdout + stderr)
#  8. паралельні команди через &
#  9. пакетний режим і коди виходу
# 10. офіційні тести OSTEP: 22/22
git remote add origin <URL>
git push -u origin main
```

Матеріали: [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/),
розділ [Process API](http://www.ostep.org/cpu-api.pdf).
