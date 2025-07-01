
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

class Barang {
public:
  string namaPenggadai;
  string nama;
  string nomorTransaksi;
  string status;
  double harga;
  tm tanggalGadai;
  tm tanggalJatuhTempo;
  double bungaPerHari;

  Barang(string np, string n, string nt, string s, double h, tm tg, tm tj,
         double b)
      : namaPenggadai(np), nama(n), nomorTransaksi(nt), status(s), harga(h),
        tanggalGadai(tg), tanggalJatuhTempo(tj), bungaPerHari(b) {}

  double hitungBunga() {
    time_t tGadai = mktime(&tanggalGadai);
    time_t tJatuhTempo = mktime(&tanggalJatuhTempo);
    double durasiHari = difftime(tJatuhTempo, tGadai) / (60 * 60 * 24);
    return harga * bungaPerHari * durasiHari;
  }
};

class Gudang {
private:
  vector<Barang> barang;

public:
  // Linear Search - mencari barang berdasarkan nama
  int linearSearchByName(const string &namaBarang) {
    cout << "\n=== LINEAR SEARCH ===\n";
    cout << "Mencari barang dengan nama: " << namaBarang << "\n";

    for (int i = 0; i < (int)barang.size(); i++) {
      cout << "Langkah " << (i + 1) << ": Membandingkan '" << barang[i].nama
           << "' dengan '" << namaBarang << "'\n";

      if (barang[i].nama == namaBarang) {
        cout << "DITEMUKAN! Barang '" << namaBarang
             << "' ditemukan pada indeks " << i << "\n";
        return i; // Return index jika ditemukan
      }
    }

    cout << "TIDAK DITEMUKAN! Barang '" << namaBarang
         << "' tidak ada dalam database.\n";
    return -1; // Return -1 jika tidak ditemukan
  }

  // Binary Search - mencari barang berdasarkan nomor transaksi (harus diurutkan
  // dulu)
  int binarySearchByTransactionNumber(const string &nomorTransaksi) {
    cout << "\n=== BINARY SEARCH ===\n";
    cout << "Mencari barang dengan nomor transaksi: " << nomorTransaksi << "\n";

    // Pertama, urutkan array berdasarkan nomor transaksi
    vector<pair<string, int> > sortedTransactions;
    for (int i = 0; i < (int)barang.size(); i++) {
      sortedTransactions.push_back(make_pair(barang[i].nomorTransaksi, i));
    }
    sort(sortedTransactions.begin(), sortedTransactions.end());

    cout << "Data sudah diurutkan berdasarkan nomor transaksi.\n";

    int left = 0;
    int right = (int)sortedTransactions.size() - 1;
    int step = 1;

    while (left <= right) {
      int mid = left + (right - left) / 2;
      string midTransaction = sortedTransactions[mid].first;

      cout << "Langkah " << step << ": left=" << left << ", right=" << right
           << ", mid=" << mid << " ('" << midTransaction << "')\n";

      if (midTransaction == nomorTransaksi) {
        cout << "DITEMUKAN! Nomor transaksi '" << nomorTransaksi
             << "' ditemukan pada indeks asli "
             << sortedTransactions[mid].second << "\n";
        return sortedTransactions[mid].second; // Return indeks asli
      } else if (midTransaction < nomorTransaksi) {
        cout << "'" << midTransaction << "' < '" << nomorTransaksi
             << "' -> Cari di bagian kanan\n";
        left = mid + 1;
      } else {
        cout << "'" << midTransaction << "' > '" << nomorTransaksi
             << "' -> Cari di bagian kiri\n";
        right = mid - 1;
      }
      step++;
    }

    cout << "TIDAK DITEMUKAN! Nomor transaksi '" << nomorTransaksi
         << "' tidak ada dalam database.\n";
    return -1;
  }

  // Fungsi untuk menampilkan detail barang berdasarkan index
  void tampilkanDetailBarang(int index) {
    if (index >= 0 && index < (int)barang.size()) {
      Barang &b = barang[index];
      cout << "\n=== DETAIL BARANG DITEMUKAN ===\n";
      cout << "Penggadai    : " << b.namaPenggadai << "\n";
      cout << "Nama Barang  : " << b.nama << "\n";
      cout << "No. Transaksi: " << b.nomorTransaksi << "\n";
      cout << "Status       : " << b.status << "\n";
      cout << "Harga        : Rp " << fixed << setprecision(2) << b.harga
           << "\n";

      char bufferGadai[20], bufferTempo[20];
      strftime(bufferGadai, sizeof(bufferGadai), "%d-%m-%Y", &b.tanggalGadai);
      strftime(bufferTempo, sizeof(bufferTempo), "%d-%m-%Y",
               &b.tanggalJatuhTempo);

      cout << "Tgl Gadai    : " << bufferGadai << "\n";
      cout << "Tgl J. Tempo : " << bufferTempo << "\n";
      cout << "Bunga        : Rp " << fixed << setprecision(2)
           << b.hitungBunga() << "\n";
      cout << "===============================\n";
    }
  }
  void tambahBarang(const Barang &b) { barang.push_back(b); }

  void tampilkanBarang() {
    cout << "\n================================================================"
            "==============\n";
    cout << "                                 PT. Ahmad Dahlan                 "
            "              \n";
    cout << "                          Daftar Barang yang Ada di Gudang        "
            "              \n";
    cout << "=================================================================="
            "==============\n\n";

    cout << left << setw(15) << "Penggadai" << setw(20) << "Nama Barang"
         << setw(20) << "No. Transaksi" << setw(12) << "Status" << setw(15)
         << "Harga" << setw(15) << "Tgl Gadai" << setw(20) << "Tgl Jatuh Tempo"
         << setw(12) << "Bunga" << endl;

    cout << string(129, '=') << endl;

    bool adaBarang = false;
    for (int i = 0; i < (int)barang.size(); i++) {
      Barang b = barang[i];
      if (b.status == "Di Gudang") {
        adaBarang = true;

        char bufferGadai[20], bufferTempo[20];
        strftime(bufferGadai, sizeof(bufferGadai), "%d-%m-%Y", &b.tanggalGadai);
        strftime(bufferTempo, sizeof(bufferTempo), "%d-%m-%Y",
                 &b.tanggalJatuhTempo);

        cout << left << setw(15) << b.namaPenggadai << setw(20) << b.nama
             << setw(20) << b.nomorTransaksi << setw(12) << b.status << setw(15)
             << fixed << setprecision(2) << b.harga << setw(15)
             << bufferGadai << setw(20) << bufferTempo << setw(12) << fixed
             << setprecision(2) << b.hitungBunga() << endl;
      }
    }

    if (!adaBarang) {
      cout << "\nTidak ada barang dengan status 'Di Gudang'.\n";
    }
  }

  void tampilkanBarangKeluar() {
    cout << "\n===============================================\n";
    cout << "              PT. Ahmad Dahlan                 \n";
    cout << "             Barang yang Sudah Keluar          \n";
    cout << "===============================================\n\n";

    cout << left << setw(25) << "Nama Barang" << setw(20) << "No. Transaksi"
         << setw(12) << "Status" << endl;
    cout << string(60, '=') << endl;

    bool adaBarang = false;
    for (int i = 0; i < (int)barang.size(); i++) {
      Barang b = barang[i];
      if (b.status == "Keluar") {
        adaBarang = true;
        cout << left << setw(25) << b.nama << setw(20) << b.nomorTransaksi
             << setw(12) << b.status << endl;
      }
    }

    if (!adaBarang) {
      cout << "\nTidak ada barang yang keluar.\n";
    } else {
      cout << "\nTerima kasih telah mempercayai layanan kami.\n";
    }
  }

  void tampilkanHargaBarang() {
    cout << "\n===============================================\n";
    cout << "              PT. Ahmad Dahlan                 \n";
    cout << "             Daftar Harga Barang              \n";
    cout << "===============================================\n\n";

    cout << left << setw(25) << "Nama Barang" << setw(15) << "Harga" << endl;
    cout << string(40, '=') << endl;

    for (int i = 0; i < (int)barang.size(); i++) {
      Barang b = barang[i];
      cout << left << setw(25) << b.nama << setw(15) << fixed << setprecision(2)
           << b.harga << endl;
    }
  }

  void pilihJenisPembayaran() {
    int pilihan;
    int pilih;
    cout << "Pilih jenis pembayaran:\n1. Tunai\n2. Transfer\n3. "
            "Lainnya\nPilihan: ";
    cin >> pilihan;
    switch (pilihan) {
    case 1:
      cout << "Pembayaran Tunai dipilih.\n";
      break;
    case 2:
      cout << "Pembayaran Transfer dipilih.\n";
      break;
    case 3:
      cout << "Pembayaran Lainnya dipilih.\n";
      cout << "Pilih jenis pembayaran lainnya:\n1. Qris\n2. E-Wallet "
              "\nPilihan: ";
      cin >> pilih;
      if (pilih == 1) {
        cout << "Pembayaran Qris dipilih.\n";
      } else if (pilih == 2) {
        cout << "Pembayaran E-Wallet dipilih.\n";
      } else {
        cout << "Pilihan tidak valid.\n";
      }
      break;
    default:
      cout << "Pilihan tidak valid.\n";
    }
  }

  void inputBarangBaru() {
    string nama;
    string namaPenggadai;
    string nomorTransaksi;
    string status;
    float harga;
    float bunga;
    int tg;
    int tbulan;
    int ty;
    int tjg;
    int tjbulan;
    int tjy;

    cin.ignore(); // flush buffer
    cout << "Masukkan Nama Penggadai: ";
    getline(cin, namaPenggadai);
    cout << "Masukkan Nama Barang: ";
    getline(cin, nama);
    cout << "Masukkan Nomor Transaksi: ";
    getline(cin, nomorTransaksi);
    cout << "Masukkan Status (Di Gudang/Keluar): ";
    getline(cin, status);
    cout << "Masukkan Harga Barang: ";
    cin >> harga;
    cout << "Masukkan Bunga per Hari (misal 0.01): ";
    cin >> bunga;
    cout << "Masukkan Tanggal Gadai (dd mm yyyy): ";
    cin >> tg >> tbulan >> ty;
    cout << "Masukkan Tanggal Jatuh Tempo (dd mm yyyy): ";
    cin >> tjg >> tjbulan >> tjy;

    struct tm tGadai = {};
    tGadai.tm_mday = tg;
    tGadai.tm_mon = tbulan - 1;
    tGadai.tm_year = ty - 1900;

    struct tm tJTempo = {};
    tJTempo.tm_mday = tjg;
    tJTempo.tm_mon = tjbulan - 1;
    tJTempo.tm_year = tjy - 1900;

    Barang b(namaPenggadai, nama, nomorTransaksi, status, harga, tGadai,
             tJTempo, bunga);
    tambahBarang(b);
    cout << "\nBarang berhasil ditambahkan!\n\n";

    // Tampilkan tabel barang di gudang setelah tambah
    tampilkanBarang();
  }
};

int main() {
  Gudang gudang;

  // Data awal (contoh)
  tm tg1 = {};
  tg1.tm_year = 2025 - 1900;
  tg1.tm_mon = 6 - 1;
  tg1.tm_mday = 1;
  tm tj1 = tg1;
  tj1.tm_mday = 4;
  Barang b1("SURYA PRIMA", "Air Cooler", "FPB-0001", "Di Gudang", 25000, tg1,
            tj1, 0.1);
  gudang.tambahBarang(b1);

  tm tg2 = {};
  tg2.tm_year = 2025 - 1900;
  tg2.tm_mon = 6 - 1;
  tg2.tm_mday = 1;
  tm tj2 = tg2;
  tj2.tm_mday = 4;
  Barang b2("TEDY BATTERY", "Accu 150 Ampere Incco", "FPB-0002", "Keluar",
            1400000, tg2, tj2, 0.1);
  gudang.tambahBarang(b2);

  tm tg3 = {};
  tg3.tm_year = 2025 - 1900;
  tg3.tm_mon = 6 - 1;
  tg3.tm_mday = 1;
  tm tj3 = tg3;
  tj3.tm_mday = 4;
  Barang b3("SURYA PRIMA", "Shock breaker", "FPB-0003", "Di Gudang", 500000,
            tg3, tj3, 0.1);
  gudang.tambahBarang(b3);

  tm tg4 = {};
  tg4.tm_year = 2025 - 1900;
  tg4.tm_mon = 6 - 1;
  tg4.tm_mday = 1;
  tm tj4 = tg4;
  tj4.tm_mday = 4;
  Barang b4("INTERTRUCK PARTS", "Booster Rem MERCY", "FPB-0004", "Di Gudang",
            1299450, tg4, tj4, 0.1);
  gudang.tambahBarang(b4);

  tm tg5 = {};
  tg5.tm_year = 2025 - 1900;
  tg5.tm_mon = 6 - 1;
  tg5.tm_mday = 1;
  tm tj5 = tg5;
  tj5.tm_mday = 4;
  Barang b5("JAYA MANDIRI DIESEL", "As Stang rem", "FPB-0005", "Di Gudang",
            125000, tg5, tj5, 0.1);
  gudang.tambahBarang(b5);

  tm tg6 = {};
  tg6.tm_year = 2025 - 1900;
  tg6.tm_mon = 6 - 1;
  tg6.tm_mday = 1;
  tm tj6 = tg6;
  tj6.tm_mday = 4;
  Barang b6("TEKNIKA COKRA", "Anting-Anting Rem", "FPB-0006", "Di Gudang",
            395000, tg6, tj6, 0.1);
  gudang.tambahBarang(b6);

  tm tg7 = {};
  tg7.tm_year = 2025 - 1900;
  tg7.tm_mon = 6 - 1;
  tg7.tm_mday = 1;
  tm tj7 = tg7;
  tj7.tm_mday = 4;
  Barang b7("MORO JAYA TEKNIK", "Adaptor 3/4 Male", "FPB-0007", "Di Gudang",
            30000, tg7, tj7, 0.1);
  gudang.tambahBarang(b7);

  tm tg8 = {};
  tg8.tm_year = 2025 - 1900;
  tg8.tm_mon = 6 - 1;
  tg8.tm_mday = 1;
  tm tj8 = tg8;
  tj8.tm_mday = 4;
  Barang b8("WANAJAYA BAN", "Ban Dalam 900-20 GT", "FPB-0008", "Di Gudang",
            285000, tg8, tj8, 0.1);
  gudang.tambahBarang(b8);

  tm tg9 = {};
  tg9.tm_year = 2025 - 1900;
  tg9.tm_mon = 6 - 1;
  tg9.tm_mday = 1;
  tm tj9 = tg9;
  tj9.tm_mday = 4;
  Barang b9("SURYA PRIMA", "Bohlam H4 BOSS", "FPB-0009", "Di Gudang", 30000,
            tg9, tj9, 0.1);
  gudang.tambahBarang(b9);

  tm tg10 = {};
  tg10.tm_year = 2025 - 1900;
  tg10.tm_mon = 6 - 1;
  tg10.tm_mday = 1;
  tm tj10 = tg10;
  tj10.tm_mday = 4;
  Barang b10("TEKNIKA COKRA", "Dinamo Amper", "FPB-00010", "Di Gudang", 510000,
             tg10, tj10, 0.1);
  gudang.tambahBarang(b10);

  tm tg11 = {};
  tg11.tm_year = 2025 - 1900;
  tg11.tm_mon = 6 - 1;
  tg11.tm_mday = 1;
  tm tj11 = tg11;
  tj11.tm_mday = 4;
  Barang b11("INTERTRUCK PARTS", "Filter Udara Besar Volvo", "FPB-00011",
             "Di Gudang", 350000, tg11, tj11, 0.1);
  gudang.tambahBarang(b11);

  tm tg12 = {};
  tg12.tm_year = 2025 - 1900;
  tg12.tm_mon = 6 - 1;
  tg12.tm_mday = 1;
  tm tj12 = tg12;
  tj12.tm_mday = 4;
  Barang b12("SURYA PRIMA", "Air cooler", "FPB-00012", "Di Gudang", 25000, tg12,
             tj12, 0.1);
  gudang.tambahBarang(b12);

  tm tg13 = {};
  tg13.tm_year = 2025 - 1900;
  tg13.tm_mon = 6 - 1;
  tg13.tm_mday = 1;
  tm tj13 = tg13;
  tj13.tm_mday = 4;
  Barang b13("SURYA PRIMA", "Bohlam H4 BOSS", "FPB-00013", "Di Gudang", 30000,
             tg13, tj13, 0.1);
  gudang.tambahBarang(b13);

  tm tg14 = {};
  tg14.tm_year = 2025 - 1900;
  tg14.tm_mon = 6 - 1;
  tg14.tm_mday = 1;
  tm tj14 = tg14;
  tj14.tm_mday = 4;
  Barang b14("INTERTRUCK PARTS", "King Pin MERCY", "FPB-00014", "Di Gudang",
             375000, tg14, tj14, 0.1);
  gudang.tambahBarang(b14);

  tm tg15 = {};
  tg15.tm_year = 2025 - 1900;
  tg15.tm_mon = 6 - 1;
  tg15.tm_mday = 1;
  tm tj15 = tg15;
  tj15.tm_mday = 4;
  Barang b15("INTERTRUCK PARTS", "Kaca Spion mercy", "FPB-00015", "Di Gudang",
             120000, tg15, tj15, 0.1);
  gudang.tambahBarang(b15);

  tm tg16 = {};
  tg16.tm_year = 2025 - 1900;
  tg16.tm_mon = 6 - 1;
  tg16.tm_mday = 1;
  tm tj16 = tg16;
  tj16.tm_mday = 4;
  Barang b16("TEKNIKA COKRA", "Anting-Anting Rem", "FPB-00016", "Di Gudang",
             395000, tg16, tj16, 0.1);
  gudang.tambahBarang(b16);

  tm tg17 = {};
  tg17.tm_year = 2025 - 1900;
  tg17.tm_mon = 6 - 1;
  tg17.tm_mday = 1;
  tm tj17 = tg17;
  tj17.tm_mday = 4;
  Barang b17("TEDY BATTERY", "Accu 150 Ampere Incco", "FPB-00017", "Di Gudang",
             1400000, tg17, tj17, 0.1);
  gudang.tambahBarang(b17);

  tm tg18 = {};
  tg18.tm_year = 2025 - 1900;
  tg18.tm_mon = 6 - 1;
  tg18.tm_mday = 1;
  tm tj18 = tg18;
  tj18.tm_mday = 4;
  Barang b18("WANAJAY BAN", "Ban Dalam 900-20GT", "FPB-00018", "Di Gudang",
             285000, tg18, tj18, 0.1);
  gudang.tambahBarang(b18);

  tm tg19 = {};
  tg19.tm_year = 2025 - 1900;
  tg19.tm_mon = 6 - 1;
  tg19.tm_mday = 1;
  tm tj19 = tg19;
  tj19.tm_mday = 4;
  Barang b19("INTERTRUCK PARTS", "Kaca Pintu LH VOLVO", "FPB-00019",
             "Di Gudang", 385000, tg19, tj19, 0.1);
  gudang.tambahBarang(b19);

  tm tg20 = {};
  tg20.tm_year = 2025 - 1900;
  tg20.tm_mon = 6 - 1;
  tg20.tm_mday = 1;
  tm tj20 = tg20;
  tj20.tm_mday = 4;
  Barang b20("INTERTRUCK PARTS", "Filter Solar (BOSCH)", "FPB-00020",
             "Di Gudang", 45000, tg20, tj20, 0.1);
  gudang.tambahBarang(b20);

  int pilihan;
  do {
    cout << "\nMenu:\n";
    cout << "1. Tambahkan Barang Baru\n";
    cout << "2. Tampilkan Barang di Gudang\n";
    cout << "3. Tampilkan Barang Keluar\n";
    cout << "4. Tampilkan Harga Barang\n";
    cout << "5. Pilih Jenis Pembayaran\n";
    cout << "6. Cari berdasarkan Nama Barang\n";
    cout << "7. Cari berdasarkan Nomor Transaksi\n";
    cout << "0. Keluar\n";
    cout << "Pilih: ";
    cin >> pilihan;

    switch (pilihan) {
    case 1:
      gudang.inputBarangBaru();
      break;
    case 2:
      gudang.tampilkanBarang();
      break;
    case 3:
      gudang.tampilkanBarangKeluar();
      break;
    case 4:
      gudang.tampilkanHargaBarang();
      break;
    case 5:
      gudang.pilihJenisPembayaran();
      break;
    case 6: {
      string namaBarang;
      cin.ignore(); // flush buffer
      cout << "Masukkan nama barang yang dicari: ";
      getline(cin, namaBarang);
      int result = gudang.linearSearchByName(namaBarang);
      if (result != -1) {
        gudang.tampilkanDetailBarang(result);
      }
      break;
    }
    case 7: {
      string nomorTransaksi;
      cin.ignore(); // flush buffer
      cout << "Masukkan nomor transaksi yang dicari: ";
      getline(cin, nomorTransaksi);
      int result = gudang.binarySearchByTransactionNumber(nomorTransaksi);
      if (result != -1) {
        gudang.tampilkanDetailBarang(result);
      }
      break;
    }
    case 0:
      cout << "Keluar dari program.\n";
      break;
    default:
      cout << "Pilihan tidak valid.\n";
    }
  } while (pilihan != 0);

  return 0;
}
