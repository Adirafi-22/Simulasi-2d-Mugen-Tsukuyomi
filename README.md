# Simulasi 2D: Mugen Tsukuyomi 
**Tugas Besar UTS Komputer Grafik (Praktik)** - D4 Teknik Informatika Politeknik Negeri Bandung

# Informasi Saya
Nama    : Adi Rafi Chaerufarizki
NIM     : 241524001
Kelas   : 2A-D4
Semester: 4

---

## Deskripsi Proyek
Repositori ini berisi program simulasi grafika 2D yang merekonstruksi fenomena "Mugen Tsukuyomi" menggunakan bahasa C dan library Raylib. 
Fokus utama dari proyek ini bukan sekadar menampilkan gambar statis (seperti `.png` atau `.jpg`), melainkan memetakan bentuk-bentuk asimetris dan objek ke dalam sistem koordinat 2D. Hal ini dicapai murni menggunakan algoritma grafika primitif, matematika, dan matriks transformasi.

---

## Optimisasi Performa
Untuk memastikan simulasi berjalan mulus dan stabil di 60 FPS, proyek ini menerapkan beberapa optimisasi:

* **Caching (RenderTexture):** Objek statis dengan komputasi tinggi (seperti gunung dan batang pohon) di-render satu kali ke dalam memori *cache*. Dengan cara ini, CPU tidak perlu menggambarnya ulang dari nol setiap *frame*.
* **Pembagian Beban (CPU & GPU):** Objek utama 100% diproses menggunakan *Software Rendering* (CPU). Namun, untuk merender partikel kecil dalam jumlah masif (seperti ratusan bintang dan debu), proses rendering dipindahkan ke GPU (*Hardware Acceleration*) menggunakan fungsi bawaan Raylib. Hal ini memastikan CPU memiliki sumber daya yang cukup untuk memproses kalkulasi matematika dan matriks pada objek utama tanpa menyebabkan *lag*.

---

## Cara Menjalankan Program

1. Pastikan *compiler* **GCC** (MinGW) dan *library* **Raylib** sudah terkonfigurasi di perangkat Anda.
2. *Clone* atau unduh *repository* ini.
3. Buka terminal di direktori proyek.
4. Lakukan kompilasi program. Contoh perintah kompilasi menggunakan GCC:
   ```bash
   gcc main.c animasi.c algoritma.c pola.c menu.c about.c -o mugen_tsukuyomi.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
