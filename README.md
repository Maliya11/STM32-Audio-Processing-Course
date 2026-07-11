# STM32 Audio processing framework

## 📌 Overview
A low-level, high-performance digital audio processing framework built entirely from scratch for the **ARM Cortex-M7 (STM32F746VG MCU)** platform. 

> ⚠️ **No HAL layer is used.** All peripheral drivers are written via pure **bare-metal programming** using direct register manipulation.

---

## 🏗️ Incremental Development Pipeline

The repository is structured progressively, with each directory introducing a specific bare-metal layer or core audio optimization:

| Module | Core Implementation Focus |
| :--- | :--- |
| `0_test` | Minimal hardware bring-up and toolchain verification. |
| `1_system_drivers_fpu` | Enabling the Floating Point Unit (FPU) for accelerated audio math. |
| `2_system_drivers_uart` | Register-level asynchronous serial communication. |
| `3_system_drivers_timerbase` | Timer peripheral scheduling. |
| `4_system_drivers_gpio` | Bitmask pin control. |
| `5_system_drivers_adc` | Analog-to-Digital conversion. |
| `6_system_drivers_spi` | SPI driver implementation. |
| `7_system_drivers_i2c` | I2C driver implementation. |
| `8_clock_config` | Phase-Locked Loop (PLL) configuration. |
| `9_i2s_driver` | Inter-IC Sound (I2S) protocol transmission using polling techniques. |
| `10_i2s_driver-dma` | Refactoring the I2S with Direct Memory Access (DMA) circular buffering. |
| `11_cs43l22_dac_driver` | Register configuration for CS43L22 audio codec. |
| `12_pdm_audio_processing` | PDM-to-PCM filter processing. |

---

## 🛠️ Technical Specifications

*   **Architecture:** Bare-metal C code targeting the ARM Cortex-M7 core.
*   **Audio Core:** Circular DMA buffering, I2S data interface.
*   **IDE Support:** STM32CubeIDE / Keil MDK.

---

## 📚 Reference Documentation

The official datasheets and programming guides used to extract register bitmasks are included in the root folder:
*   `cortex_m7_user_guide.pdf` – Core pipeline and execution characteristics.
*   `stm32f746vg_datasheet.pdf` – Device pinout, memory spaces, and electrical specs.
*   `stm32f7_reference_manual.pdf` – Full register descriptions for all internal peripherals.
*   `stm32f7_programming_manual.pdf` – Cortex-M7 instruction sets.
*   `stm32_pdm_2_pcm_library_user_manual.pdf` – Documentation for the PDM signal decimation library.
