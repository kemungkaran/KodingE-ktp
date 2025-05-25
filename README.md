# RFID Door Access System using Arduino

## 📝 Deskripsi
Sistem ini merupakan kontrol akses pintu menggunakan RFID berbasis Arduino, dengan kemampuan menyimpan dan menghapus UID RFID melalui EEPROM. Mendukung mode pemrograman dengan kartu master, serta tombol penghapus untuk reset data.

## 🚀 Fitur
- Menyimpan hingga banyak UID di EEPROM
- Mode pemrograman: tambah atau hapus kartu
- Akses menggunakan kartu RFID
- Umpan balik LED dan buzzer
- Tombol untuk menghapus semua data

## 🧰 Komponen
- Arduino Uno
- RFID Reader MFRC522
- Relay
- LED Merah dan Hijau
- Buzzer
- Tombol Tekan (hapus data)
- Kabel jumper
- Breadboard

## 📌 Wiring (Pin Arduino)

| Fungsi        | Pin        |
|---------------|------------|
| RFID SDA      | 10         |
| RFID RST      | 9          |
| RFID MOSI     | 11         |
| RFID MISO     | 12         |
| RFID SCK      | 13         |
| LED Hijau     | 6          |
| LED Merah     | 7          |
| Relay         | 8          |
| Buzzer        | 2          |
| Tombol DELETE | 3          |

## 🛠️ Cara Kerja
1. Upload kode ke Arduino.
2. Jika EEPROM kosong, tempelkan kartu RFID untuk dijadikan master.
3. Tempelkan kartu master → Masuk program mode:
   - Tempel kartu baru → Ditambahkan.
   - Tempel kartu terdaftar → Dihapus.
4. Keluar program mode dengan tempelkan kembali kartu master.
5. Di mode normal:
   - Kartu terdaftar → akses diberikan.
   - Kartu tidak terdaftar → ditolak.

## 🧪 Reset EEPROM
Tekan dan tahan tombol DELETE selama 5 detik saat startup → EEPROM dihapus.

## 🔐 Keamanan
- EEPROM digunakan untuk menyimpan UID.
- Master card diperlukan untuk perubahan data.

## 📜 Lisensi
Proyek ini bersifat open-source, silakan gunakan dan modifikasi sesuai kebutuhan Anda.

