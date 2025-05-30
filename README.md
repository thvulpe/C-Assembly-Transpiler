# 🛠️ C-to-Assembly Transpiler

A minimal C-like to x86 assembly transpiler. This project demonstrates how basic high-level constructs map to low-level instructions, providing a practical introduction to compiler design and assembly language.

## 🎯 Objective

Convert simple C-style statements into x86 assembly instructions. Focus areas include:
- Arithmetic & logical operations
- Data movement
- Control flow (conditionals & loops)
- Register usage conventions

> ⚠️ This is technically a transpiler, not a full compiler.

---

## 💡 Features

- ✅ Supports `MOV`, `ADD`, `SUB`, `MUL`, `DIV`
- ✅ Logical ops: `AND`, `OR`, `XOR`
- ✅ Bitwise shifts: `SHL`, `SHR`
- ✅ Conditionals with `CMP`, `JE`, `JNE`, `JG`, `JL`, etc.
- ✅ Loops: `for`, `while`
- ✅ Register mapping: `A → eax`, `B → ebx`, `C → ecx`, `D → edx`
- ✅ Output is valid x86 syntax (NASM-style)
- ✅ Clean memory management (Valgrind-safe)

---


## ⚙️ Usage

### 🧱 Build

```bash
make build

▶️ Run

```bash
./transpiler input_file output_file

## 👨‍💻 Author

Theodor Vulpe
Student at University Politehnica of Bucharest – Faculty of Automatic Control and Computers
