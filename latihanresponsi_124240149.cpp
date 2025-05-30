#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio> // Untuk FILE handling
using namespace std;

// Struktur Node untuk Linked List
struct Node
{
    char judul[100];
    int tahun;
    char genre[20];
    float rating;
    Node *next;
};

// Pointer ke awal daftar film
Node *head = nullptr;

// Array tipe genre yang valid
char genreValid[4][20] = {"horor", "komedi", "action", "romance"};
bool kembaliKeMenu()
{
    char pilihan;
    do
    {
        cout << "\nBalik ke menu? (y/n): ";
        cin >> pilihan;
        if (pilihan == 'y' || pilihan == 'Y')
            return true;
        if (pilihan == 'n' || pilihan == 'N')
            return false;
        cout << "Masukan tidak valid, silakan pilih 'y' atau 'n'.\n";
    } while (true);
}
// Mengecek apakah genre valid
bool cekGenreValid(const char *type)
{
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(genreValid[i], type) == 0)
        {
            return true;
        }
    }
    return false;
}

// Mengecek apakah judul sudah ada dalam linked list
bool cekDuplikasiJudul(const char *judul)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (strcmp(temp->judul, judul) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Menyimpan *hanya data film yang baru ditambahkan* ke file
void simpanDataFilm(Node *newFilm)
{
    FILE *file = fopen("data_film.txt", "a+"); // Mode append agar hanya menambah data baru
    if (file == nullptr)
    {
        cout << "Gagal membuka file." << endl;
        return;
    }

    fprintf(file, "%s,%d,%s,%.2f\n",
            newFilm->judul,
            newFilm->tahun,
            newFilm->genre,
            newFilm->rating);

    fclose(file);
}

// *Memuat data film dari file saat program dijalankan*
void muatDataFilm()
{
    FILE *file = fopen("data_film.txt", "r");
    if (file == nullptr)
    {
        return;
    }

    while (!feof(file))
    {
        Node *newFilm = new Node;
        fscanf(file, "%[^,],%d,%[^,],%f\n",
               newFilm->judul,
               &newFilm->tahun,
               newFilm->genre,
               &newFilm->rating);

        newFilm->next = head;
        head = newFilm;
    }

    fclose(file);
}

// *Menambahkan film baru ke linked list*
void tambahFilm()
{
    Node *newFilm = new Node;
    cout << "\nJudul Film: ";
    cin.ignore();
    cin.getline(newFilm->judul, 100);

    if (cekDuplikasiJudul(newFilm->judul))
    {
        cout << "Error: Judul film sudah ada!" << endl;
        delete newFilm;
        return;
    }

    cout << "Tahun Film: ";
    cin >> newFilm->tahun;

    cin.ignore();
    cout << "Genre: ";
    cin.getline(newFilm->genre, 20);

    if (!cekGenreValid(newFilm->genre))
    {
        cout << "Error: Genre tidak valid! Harus salah satu dari: horor, komedi, action, romance.\n";
        delete newFilm;
        return;
    }

    cout << "Rating Film: ";
    cin >> newFilm->rating;

    newFilm->next = head;
    head = newFilm;

    simpanDataFilm(newFilm); // *Hanya simpan data baru*
    cout << "Film berhasil ditambahkan!" << endl;
}

// *Menampilkan film berdasarkan rating rendah ke tinggi atau tinggi ke rendah*
void tampilkanFilm(bool ascending)
{
    if (head == nullptr)
    {
        cout << "Belum ada data film." << endl;
        return;
    }

    bool swapped;
    do
    {
        swapped = false;
        Node *temp = head;
        while (temp->next != nullptr)
        {
            if ((ascending && temp->rating > temp->next->rating) ||
                (!ascending && temp->rating < temp->next->rating))
            {
                swap(temp->judul, temp->next->judul);
                swap(temp->tahun, temp->next->tahun);
                swap(temp->genre, temp->next->genre);
                swap(temp->rating, temp->next->rating);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);

    cout << setw(20) << left << "Judul"
         << setw(10) << "Tahun"
         << setw(15) << "Genre"
         << setw(10) << "Rating" << endl;
    cout << string(55, '=') << endl;

    Node *temp = head;
    while (temp != nullptr)
    {
        cout << left
             << setw(20) << temp->judul
             << setw(10) << temp->tahun
             << setw(15) << temp->genre
             << setw(10) << fixed << setprecision(2) << temp->rating
             << endl;
        temp = temp->next;
    }
    cout << string(55, '=') << endl;
    if (!kembaliKeMenu()) // *Tambahkan pilihan balik ke menu*
    {
        exit(0);
    }
}

// *Menu pencarian film*
void cariFilm()
{
    int opsi;
    cout << "\nPilih metode pencarian:\n"
         << "1. Berdasarkan Judul\n"
         << "2. Berdasarkan Genre\n"
         << "3. Berdasarkan Tahun\n"
         << "Pilihan: ";
    cin >> opsi;
    cin.ignore();

    Node *temp = head;
    bool ditemukan = false;

    if (opsi == 1) // Cari berdasarkan judul
    {
        char judulCari[100];
        cout << "Masukkan Judul Film yang dicari: ";
        cin.getline(judulCari, 100);

        while (temp != nullptr)
        {
            if (strcmp(temp->judul, judulCari) == 0)
            {
                ditemukan = true;
                cout << "\nFilm ditemukan:\n"
                     << "Judul: " << temp->judul << "\n"
                     << "Tahun: " << temp->tahun << "\n"
                     << "Genre: " << temp->genre << "\n"
                     << "Rating: " << fixed << setprecision(2) << temp->rating << endl;
                break; // Menghentikan pencarian setelah menemukan satu hasil
            }
            temp = temp->next;
        }
    }
    else if (opsi == 2) // Cari berdasarkan genre
    {
        char genreCari[20];
        cout << "Masukkan Genre yang dicari: ";
        cin.getline(genreCari, 20);

        while (temp != nullptr)
        {
            if (strcmp(temp->genre, genreCari) == 0)
            {
                ditemukan = true;
                cout << "\nFilm ditemukan:\n"
                     << "Judul: " << temp->judul << "\n"
                     << "Tahun: " << temp->tahun << "\n"
                     << "Genre: " << temp->genre << "\n"
                     << "Rating: " << fixed << setprecision(2) << temp->rating << endl;
            }
            temp = temp->next;
        }
    }
    else if (opsi == 3) // Cari berdasarkan tahun
    {
        int tahunCari;
        cout << "Masukkan Tahun yang dicari: ";
        cin >> tahunCari;

        while (temp != nullptr)
        {
            if (temp->tahun == tahunCari)
            {
                ditemukan = true;
                cout << "\nFilm ditemukan:\n"
                     << "Judul: " << temp->judul << "\n"
                     << "Tahun: " << temp->tahun << "\n"
                     << "Genre: " << temp->genre << "\n"
                     << "Rating: " << fixed << setprecision(2) << temp->rating << endl;
            }
            temp = temp->next;
        }
    }
    else
    {
        cout << "Pilihan tidak valid, silakan pilih 1, 2, atau 3.\n";
        return;
    }

    if (!ditemukan)
    {
        cout << "Film tidak ditemukan." << endl;
    }
    if (!kembaliKeMenu()) // *Tambahkan pilihan balik ke menu*
    {
        exit(0);
    }
}

// *Menghapus film dari linked list dan memperbarui file*
void hapusFilm()
{
    if (head == nullptr)
    {
        cout << "Belum ada data film untuk dihapus." << endl;
        return;
    }

    char judulHapus[100];
    cout << "\nMasukkan Judul Film yang akan dihapus: ";
    cin.ignore();
    cin.getline(judulHapus, 100);

    Node *temp = head, *prev = nullptr;
    bool ditemukan = false;

    while (temp != nullptr)
    {
        if (strcmp(temp->judul, judulHapus) == 0)
        {
            if (prev == nullptr)
                head = temp->next; // Hapus node pertama
            else
                prev->next = temp->next; // Hapus node di tengah atau akhir

            delete temp;
            ditemukan = true;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!ditemukan)
    {
        cout << "Film tidak ditemukan." << endl;
        return;
    }

    // *Memperbarui file setelah penghapusan*
    FILE *file = fopen("data_film.txt", "w"); // Mode "w" untuk menimpa file dengan data baru
    if (file == nullptr)
    {
        cout << "Gagal memperbarui file." << endl;
        return;
    }

    Node *temp2 = head;
    while (temp2 != nullptr)
    {
        fprintf(file, "%s,%d,%s,%.2f\n",
                temp2->judul,
                temp2->tahun,
                temp2->genre,
                temp2->rating);
        temp2 = temp2->next;
    }

    fclose(file);
    cout << "Film berhasil dihapus!" << endl;
    if (!kembaliKeMenu()) // *Tambahkan pilihan balik ke menu*
    {
        exit(0);
    }
}
// *Fungsi untuk kembali ke menu utama atau keluar*

// *Menu utama*
void tampilkanMenu()
{
    cout << "\n=== MENU BIOSKOP ===\n"
         << "1. Tambah Film\n"
         << "2. Tampilkan Rating Film Dari Yang Terendah\n"
         << "3. Tampilkan Rating Film Dari Yang Tertinggi\n"
         << "4. Cari Data Film\n"
         << "5. Hapus Data Film\n"
         << "6. Keluar\n"
         << "Pilih menu: ";
}
// login
struct User
{
    string username;
    string password;
};

void login()
{
    cout << "LOGIN KE BIOSKOP" << endl;

    User users[2] = {
        {"dimasfebri", "145"},
        {"nisrina", "149"}};

    string inputUser, inputPass;
    int check = 0;
    const int MAX_ATTEMPTS = 3;
    bool loginBerhasil = false;

    do
    {
        cout << "Masukkan Username: ";
        cin >> inputUser;
        cout << "Masukkan Password: ";
        cin >> inputPass;

        // Cek semua user yang ada
        for (int i = 0; i < 2; i++)
        {
            if (inputUser == users[i].username && inputPass == users[i].password)
            {
                cout << "Halo, " << inputUser << "! SELAMAT DATANG DI BIOSKOP" << endl;
                loginBerhasil = true;
                break;
            }
        }

        if (!loginBerhasil)
        {
            check++;
            cout << "Username atau Password salah." << endl;
            cout << "Percobaan tersisa: " << MAX_ATTEMPTS - check << endl
                 << endl;
        }
    } while (check < MAX_ATTEMPTS && !loginBerhasil);

    if (!loginBerhasil)
    {
        cout << "Terlalu banyak percobaan! Program keluar." << endl;
        exit(0); // Keluar dari program jika login gagal
    }
}

int main()
{
    login();        // Memanggil fungsi login sebelum menjalankan program utama
    muatDataFilm(); // Memuat data dari file saat program dimulai
    int pilihan;
    do
    {
        tampilkanMenu();
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahFilm();
            break;
        case 2:
            tampilkanFilm(true);
            break;
        case 3:
            tampilkanFilm(false);
            break;
        case 4:
            cariFilm();
            break;
        case 5:
            hapusFilm();
            break;
        case 6:
            cout << "Keluar dari program." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 6);

    return 0;
}
