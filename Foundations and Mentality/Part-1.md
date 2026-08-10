# Chapter 1.1 – What Are Bug Hunting and Vulnerability Hunting?

Think of these two concepts as intertwined yet distinct disciplines—each carrying its own emotion and purpose.

---

## Bug Hunting – "When the Code Breaks Its Promise"

A software project is essentially a **contract**:

- “This function takes this data and produces that result.”
- “This array holds at most 100 elements.”
- “I will free this memory block after using it.”

**Bug hunting** is the art of catching the moments when that contract is **broken**. Whenever the code violates its own rules, the resulting inconsistency is a **bug**.

**Examples:**

- A function returns `-1` to signal an error, but the caller doesn't check it and uses the returned value directly as an array index → **Bug** (logic error).
- A `for` loop uses `i <= length` while the array is of size `length` → **Bug** (off-by-one error).
- A `malloc` fails and returns `NULL`, but the next line does a `memcpy` into that pointer → **Bug** (null pointer dereference).

> **The goal of bug hunting:** “This code isn't living up to its claim. Where did it go wrong?”  
> At this stage, we are **not yet concerned with security**—we are only looking for **correctness** and **consistency**.

---

## Vulnerability Hunting – "Can This Bug Be Turned Against Us?"

You found a bug. But what if that bug can be **deliberately triggered by a malicious user**, and that trigger compromises the system's **confidentiality, integrity, or availability**? Then that bug becomes a **vulnerability**.

**Vulnerability hunting** is looking at every bug you find through this lens:

> “How could an attacker **exploit** this error to their advantage?  
> What could they gain?  
> How could it crash my system, let them sneak in, or elevate privileges?”

**Examples to illustrate the difference:**

| Scenario | Is it a Bug? | Is it a Vulnerability? | Why? |
|----------|-------------|------------------------|------|
| Use after `malloc` returns `NULL` | Yes | No (mostly) | There is no special attacker input to trigger this; the system crashes due to memory exhaustion, but that typically only causes DoS and often isn't remotely controllable. |
| A network server performs `memcpy` without checking the length of the incoming packet | Yes | **Yes** | An attacker can craft a special packet to overflow the stack, overwrite the return address, and execute arbitrary code (RCE). |
| In authentication, equality check accidentally uses assignment (`if (user = "admin")` instead of `if (user == "admin")`) | Yes | **Yes** (Authentication Bypass) | This makes every login succeed; an attacker can gain admin access without valid credentials. |

---

## Relationship and Distinction – "Every Vulnerability Is a Bug, but Not Every Bug Is a Vulnerability"

This sentence is the **backbone** of the entire discipline.

- A **bug** is a physical or logical error in the code.
- A **vulnerability** is when that bug **intersects with the attack surface**—i.e., it can be triggered by user‑supplied data and the result has a **negative impact** on the system. Only then does the bug become a vulnerability.

To decide, ask these three questions:

1. **Is it controllable?** – Can an attacker choose the data that triggers the bug? (e.g., a file name, network packet, environment variable)
2. **What's the impact?** – When successfully triggered, what does the system lose? (Confidentiality, integrity, or availability?)
3. **Is there an exploitation chain?** – If this bug alone doesn't do much, can it be combined with another bug to become a more serious flaw?

---

## Why Is This Distinction Especially Important in C/C++?

In C and C++, memory management is entirely in the programmer's hands. This means:

- A **buffer overflow** isn't just a crash—it can mean **taking over the control flow**.
- A **use‑after‑free** isn't just a runtime error—it can lead to **RCE or information leaks**.

Therefore, in C/C++, the majority of bugs carry **potential vulnerability**. As a hunter, you must look at every bug with the question: *"How could this be exploited?"* That is what elevates you from a ordinary tester to a **security researcher**.

---

## In Summary

- **Bug Hunting:** “Why isn't the code working?”  
- **Vulnerability Hunting:** “How can this malfunction become an attack vector?”

These two disciplines are like **two flights of stairs on the same path**.  
First, you find the bug; then you question its **security context**.  
Anyone who skips this questioning is merely a “bug reporter.”  
Anyone who embraces it becomes a “security researcher.”

---

