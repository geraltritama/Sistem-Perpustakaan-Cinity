#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <iomanip>
using namespace std;

struct Buku {
    string id;
    string judul;
    string pengarang;
    int rak;
    Buku* next;
    bool dipinjam = false;
};

struct Anggota {
    string id;
    string nama;
};

struct TreeNode {
    Buku* data;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* insertBST(TreeNode* root, Buku* b) {
    if (root == nullptr)
        return new TreeNode{b, nullptr, nullptr};
    if (b->judul < root->data->judul)
        root->left = insertBST(root->left, b);
    else
        root->right = insertBST(root->right, b);
    return root;
}

Buku* searchBST(TreeNode* root, string judul) {
    if (!root) return nullptr;
    if (judul == root->data->judul) return root->data;
    return judul < root->data->judul ? searchBST(root->left, judul) : searchBST(root->right, judul);
}

void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->data->judul << " oleh " << root->data->pengarang << ", Rak " << root->data->rak << endl;
    inorder(root->right);
}

//buat peminjaman
queue<Buku*> antrianPeminjaman;

//buat pengembalian
stack<Buku*> tumpukanPengembalian;

const int MAX = 100;
Buku* daftarBuku[MAX];
Anggota daftarAnggota[MAX];
int jumlahBuku = 0, jumlahAnggota = 0;
const int MAX_RAK = 10;
int grafRak[MAX_RAK][MAX_RAK];

void tambahBuku(TreeNode*& root) {
    char ulangi;
    cout << "====================================\n";
    cout << "|           Tambah Buku            |\n";
    cout << "====================================\n";
    do {
        string judulBaru, idBaru;
        cout << "Judul Buku: ";
        cin.ignore();
        getline(cin, judulBaru);

        cout << "ID Buku: ";
        cin >> idBaru;

        bool duplikatJudul = false, duplikatID = false;
        for (int i = 0; i < jumlahBuku; ++i) {
            if (daftarBuku[i]->judul == judulBaru) duplikatJudul = true;
            if (daftarBuku[i]->id == idBaru) duplikatID = true;
        }

        if (duplikatJudul) {
            cout << "Judul buku \"" << judulBaru << "\" sudah ada!\n";
        } else if (duplikatID) {
            cout << "ID buku \"" << idBaru << "\" sudah digunakan. Harap gunakan ID berbeda.\n";
        } else {
            Buku* b = new Buku;
            b->judul = judulBaru;
            b->id = idBaru;
            cout << "Pengarang: "; cin.ignore(); getline(cin, b->pengarang);
            cout << "Nomor Rak (0-9): "; cin >> b->rak;
            b->dipinjam = false;

            daftarBuku[jumlahBuku++] = b;
            root = insertBST(root, b);
            cout << "Buku berhasil ditambahkan!\n";
        }

        cout << "Ingin menambahkan buku lain? (y/n): ";
        cin >> ulangi;
    } while (ulangi == 'y' || ulangi == 'Y');

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void tambahAnggota() {
    char ulangi;
    cout << "====================================\n";
    cout << "|           Tambah Anggota         |\n";
    cout << "====================================\n";
    do {
        string namaBaru, idBaru;
        cout << "Nama: ";
        cin.ignore();
        getline(cin, namaBaru);

        cout << "ID Anggota: ";
        cin >> idBaru;

        bool duplikatNama = false;
        bool duplikatID = false;
        for (int i = 0; i < jumlahAnggota; ++i) {
            if (daftarAnggota[i].nama == namaBaru) {
                duplikatNama = true;
            }
            if (daftarAnggota[i].id == idBaru) {
                duplikatID = true;
            }
        }

        if (duplikatNama) {
            cout << "Nama \"" << namaBaru << "\" sudah terdaftar sebagai anggota!\n";
        } else if (duplikatID) {
            cout << "ID \"" << idBaru << "\" sudah digunakan. Harap gunakan ID yang berbeda.\n";
        } else {
            daftarAnggota[jumlahAnggota].nama = namaBaru;
            daftarAnggota[jumlahAnggota].id = idBaru;
            jumlahAnggota++;
            cout << "Anggota berhasil ditambahkan!\n";
        }

        cout << "Ingin menambahkan anggota lain? (y/n): ";
        cin >> ulangi;
    } while (ulangi == 'y' || ulangi == 'Y');

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void pinjamBuku(TreeNode* root) {
    cout << "====================================\n";
    cout << "|     Daftar Buku yang Ada         |\n";
    cout << "====================================\n";
    bool adaBukuTersedia = false;
    for (int i = 0; i < jumlahBuku; ++i) {
        if (!daftarBuku[i]->dipinjam) {
            cout << i + 1 << ". ID: " << daftarBuku[i]->id
                 << " | " << daftarBuku[i]->judul
                 << " oleh " << daftarBuku[i]->pengarang
                 << " (Rak " << daftarBuku[i]->rak << ")\n";
            adaBukuTersedia = true;
        }
    }

    if (!adaBukuTersedia) {
        cout << "Semua buku sedang dipinjam.\n";
    } else {
        string idInput;
        cout << "\nMasukkan ID buku yang ingin dipinjam: ";
        cin >> idInput;

        Buku* b = nullptr;
        for (int i = 0; i < jumlahBuku; ++i) {
            if (daftarBuku[i]->id == idInput) {
                b = daftarBuku[i];
                break;
            }
        }

        if (b) {
            if (!b->dipinjam) {
                b->dipinjam = true;
                antrianPeminjaman.push(b);
                cout << "Buku \"" << b->judul << "\" berhasil dipinjam.\n";
            } else {
                cout << "Maaf, buku tersebut sedang dipinjam dan belum dikembalikan.\n";
            }
        } else {
            cout << "Buku dengan ID tersebut tidak ditemukan!\n";
        }
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void kembalikanBuku() {
    vector<Buku*> sedangDipinjam;
    for (int i = 0; i < jumlahBuku; ++i) {
        if (daftarBuku[i]->dipinjam) {
            sedangDipinjam.push_back(daftarBuku[i]);
        }
    }

    if (sedangDipinjam.empty()) {
        cout << "Tidak ada buku yang sedang dipinjam.\n";
    } else {
    cout << "====================================\n";
    cout << "|    Daftar buku yang di pinjam    |\n";
    cout << "====================================\n";
        for (int i = 0; i < sedangDipinjam.size(); ++i) {
            cout << i + 1 << ". " << sedangDipinjam[i]->judul
                 << " oleh " << sedangDipinjam[i]->pengarang << endl;
        }

        int pilihan;
        cout << "Pilih nomor buku yang ingin dikembalikan: ";
        cin >> pilihan;

        if (pilihan >= 1 && pilihan <= sedangDipinjam.size()) {
            Buku* dikembalikan = sedangDipinjam[pilihan - 1];
            dikembalikan->dipinjam = false;
            tumpukanPengembalian.push(dikembalikan);
            cout << "Buku \"" << dikembalikan->judul << "\" berhasil dikembalikan.\n";
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void tampilkanBukuPerRak() {
    int rak;
    cout << "Masukkan nomor rak (0 - 9): ";
    cin >> rak;

    if (rak < 0 || rak >= MAX_RAK) {
        cout << "Nomor rak tidak valid.\n";
        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.ignore();
        cin.get();
        return;
    }

    bool adaBuku = false;
    for (int i = 0; i < jumlahBuku; ++i) {
        if (daftarBuku[i]->rak == rak) {
            adaBuku = true;
            break;
        }
    }

    if (!adaBuku) {
        cout << "Tidak ada buku di rak " << rak << ".\n";
    } else {
        cout << "\n========== RAK " << rak << " ==========\n";
        cout << "----------------------------------------------------\n";
        cout << "ID     Title               Author        Status\n";
        cout << "----------------------------------------------------\n";

        for (int i = 0; i < jumlahBuku; ++i) {
            Buku* b = daftarBuku[i];
            if (b->rak == rak) {
                string status = b->dipinjam ? "Dipinjam" : "Ada";
                cout << left
                     << setw(7) << b->id
                     << setw(20) << b->judul
                     << setw(15) << b->pengarang
                     << status << endl;
            }
        }
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void tampilkanBuku(TreeNode* root) {
    cout << "====================================\n";
    cout << "|           Daftar Buku            |\n";
    cout << "====================================\n";
    if (root == nullptr) {
        cout << "Belum ada buku yang tersedia.\n";
    } else {
        inorder(root);
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void tampilkanAnggota() {
    cout << "====================================\n";
    cout << "|           Daftar Anggota         |\n";
    cout << "====================================\n";
    if (jumlahAnggota == 0) {
        cout << "Belum ada anggota yang terdaftar.\n";
    } else {
        for (int i = 0; i < jumlahAnggota; ++i) {
            cout << i + 1 << ". Nama: " << daftarAnggota[i].nama
                 << ", ID: " << daftarAnggota[i].id << endl;
        }
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

void aturJarakAntarRak() {
    int dari, ke, jarak;
    cout << "Masukkan nomor rak asal (0-9): ";
    cin >> dari;
    cout << "Masukkan nomor rak tujuan (0-9): ";
    cin >> ke;
    cout << "Masukkan jarak antara rak " << dari << " dan rak " << ke << " (meter): ";
    cin >> jarak;

    if (dari >= 0 && dari < MAX_RAK && ke >= 0 && ke < MAX_RAK) {
        grafRak[dari][ke] = jarak;
        grafRak[ke][dari] = jarak;
        cout << "Jarak berhasil diatur, " << jarak << " meter!\n";
    } else {
        cout << "Input rak tidak valid!\n";
    }

    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore(); cin.get();
}

void tampilkanGrafRak() {
    cout << "Jarak antar Rak:\n   ";
    for (int i = 0; i < MAX_RAK; ++i) cout << setw(4) << i;
    cout << "\n";
    for (int i = 0; i < MAX_RAK; ++i) {
        cout << setw(2) << i << " ";
        for (int j = 0; j < MAX_RAK; ++j) {
            if (grafRak[i][j] == -1)
                cout << setw(4) << "-";
            else
                cout << setw(4) << grafRak[i][j];
        }
        cout << "\n";
    }
    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore(); cin.get();
}

int main() {
    TreeNode* root = nullptr;
    int pilihan;

    for (int i = 0; i < MAX_RAK; ++i) {
    for (int j = 0; j < MAX_RAK; ++j) {
        grafRak[i][j] = (i == j) ? 0 : -1;
    }
}

    do {
        system("cls");
        cout << "===================================\n";
        cout << "|Selamat Datang di Perpus Cihuyy:D|\n";
        cout << "===================================\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tambah Anggota\n";
        cout << "3. Pinjam Buku\n";
        cout << "4. Kembalikan Buku\n";
        cout << "5. Tampilkan Buku\n";
        cout << "6. Lihat Daftar Anggota\n";
        cout << "7. Tampilkan Buku Berdasarkan Rak\n";
        cout << "8. Atur Jarak Antar Rak \n";
        cout << "9. Tampilkan Jarak Graf Rak \n";
        cout << "0. Keluar\n";
        cout << "input nomor: ";
        cin >> pilihan;

        system("cls");

        switch (pilihan) {
            case 1: tambahBuku(root); break;
            case 2: tambahAnggota(); break;
            case 3: pinjamBuku(root); break;
            case 4: kembalikanBuku(); break;
            case 5: tampilkanBuku(root); break;
            case 6: tampilkanAnggota(); break;
            case 7: tampilkanBukuPerRak(); break;
            case 8: aturJarakAntarRak(); break;
            case 9: tampilkanGrafRak(); break;
            case 0: cout << "Terima kasih dan Sampai Jumpa :D\n"; break;
            default: cout << "Pilihan tidak valid.\n"; break;
        }

    } while (pilihan != 0);

    return 0;
}
