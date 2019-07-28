Kendryte OpenOCD with Sysprogs Fixes
=======

This is a fork of the Kendryte version of OpenOCD supporting the Kendryte K210 chip.

The original Kendryte OpenOCD requires selecting one of the 2 chip cores prior to debugging it with OpenOCD. Furthermore, breakpoints triggered on the core that is not explicitly debugged would crash the debug session instead of having it stopped in OpenOCD.

Sysprogs has fixed it by modifying the polling logic to check the status of both cores (HARTs in RISC-V terms) and automatically switching to debugging the core (HART) that triggered last breakpoint.
However, this still leads to occasional crashes, as OpenOCD appears to mix the contexts (registers?) from different HARTs.

In order to fully resolve this and get reliable debugging behavior, the following design changes will need to be applied:

* The semantics of the "Current HART" field should change from "HART selected for current operation" to "HART that has its state (e.g. registers) reported to OpenOCD as the current target's state". It should only be changed when the target is changing from 'running' to 'stopped'.
* All register reading/writing and other state manipulation functions should EXPLICITLY take the HART ID as a parameter and should *NOT* change the 'current HART' value.
* Extra care needs to be taken when accessing the DMCONTROL register to *NOT* accidentally change the HARTID field there and to *NOT* accidentally clear HALTNOT flag or other flags.

Ideally, the riscv-011.c should be redesigned similar to riscv-013.c to support the "risc_v" RTOS that exposes each HART as a separate thread to GDB.


P.S. A very good overview of RISC-V debug logic can be found here: https://sifive.cdn.prismic.io/sifive%2F645ebcda-d46b-47e4-80fc-29fe673dcc68_riscv-debug-spec-0.11nov12.pdf