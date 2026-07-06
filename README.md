# STM32 Dual-Display Serial Terminal (OLED & UART)

This project features a real-time, dual-display terminal system. It captures serial input via UART and simultaneously displays the content on both a computer terminal (e.g., Tera Term) and a physical SSD1306 OLED display, including advanced text-wrapping and scrolling functionality.

## Project Overview
![Terminal & OLED Sync](assets/dual_display_view.png)

## Features
- **Dual Output:** Synchronized text rendering on both OLED display and serial monitor.
- **Word Wrap Logic:** Intelligent text handling to break lines correctly without splitting words.
- **Scrolling Interface:** Support for manual scrolling (`+` and `-` keys) and automatic follow-up scrolling.
- **Interactive Control:** Real-time character processing with Backspace support.

## Technical Architecture

- **Microcontroller:** STM32
- **Displays:** SSD1306 OLED (I2C)
- **Communication:** UART (115200 Baud)
- **Features:** Custom scrolling logic, character interrupt handling.

## How It Works
1. **Input:** UART interrupts capture incoming bytes.
2. **Buffer Management:** `terminal_history` array manages current screen lines.
3. **Synchronization:** `Refresh_All_Screens` updates both the I2C OLED and UART interface simultaneously.

## License
Copyright (c) 2026 STMicroelectronics. Provided as-is.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


# STM32 Çift Ekranlı Seri Terminal (OLED & UART)

Bu proje, gerçek zamanlı ve çift ekranlı bir terminal sistemidir. UART üzerinden alınan verileri eş zamanlı olarak hem bilgisayar terminaline hem de donanımsal SSD1306 OLED ekrana yansıtır; ayrıca gelişmiş metin kaydırma (word wrap) ve satır kaydırma (scroll) özelliklerine sahiptir.

## Proje Genel Bakış
Bu sistem, gömülü bir cihazın kendi ekranı ile uzak terminal arasında veri tutarlılığını sağlamak için tasarlanmıştır.

## Özellikler
- **Çift Çıktı:** OLED ekran ve seri monitör üzerinde senkronize metin işleme.
- **Kelime Kaydırma (Word Wrap):** Metinlerin kelimeleri bölmeden alt satıra geçmesini sağlayan akıllı algoritma.
- **Kaydırma Arayüzü:** `+` ve `-` tuşları ile manuel satır kaydırma ve otomatik takip.
- **Etkileşimli Kontrol:** Backspace desteği ile anlık karakter düzenleme.

## Teknik Mimari
- **Mikrokontrolcü:** STM32
- **Ekran:** SSD1306 OLED (I2C)
- **Haberleşme:** UART (115200 Baud)
- **Programlama Dili:** C (HAL Kütüphanesi)

## Çalışma Mantığı
1. **Giriş:** UART kesmeleri (interrupt) gelen veriyi anlık yakalar.
2. **Tampon Bellek:** `terminal_history` dizisi, ekrandaki satırları ve imleç konumunu yönetir.
3. **Senkronizasyon:** `Refresh_All_Screens` fonksiyonu, OLED ekranı ve seri terminali aynı anda günceller.

## Lisans
Telif Hakkı (c) 2026 STMicroelectronics. "Olduğu gibi" sağlanmıştır.
