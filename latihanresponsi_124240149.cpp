#include <iostream>
#include <cstring>

using namespace std;

struct Buku {
    char judul[100];
    char pengarang[100];
    int tahun;
};

struct Node {
    Buku buku;
    Node* left;
    Node* right;
};

enum AksiType { ADD, DELETE };

struct Aksi {
    AksiType tipe;
    Buku buku;
};

const int MAX_STACK = 100;

struct Stack {
    Aksi data[MAX_STACK];
    int top;

    Stack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_STACK - 1;
    }

    void push(Aksi aksi) {
        if (!isFull()) {
            data[++top] = aksi;
        } else {
            cout << "Stack penuh, tidak bisa menyimpan log aksi.\n";
        }
    }

    Aksi pop() {
        if (!isEmpty()) {
            return data[top--];
        } else {
            cout << "Stack kosong, tidak ada aksi untuk di-undo.\n";
            Aksi kosong;
            kosong.tipe = ADD; 
            kosong.buku.judul[0] = '\0';
            return kosong;
        }
    }
};

Node* root = NULL;
Stack logStack;

// Fungsi untuk membuat node baru
Node* createNode(Buku buku) {
    Node* newNode = new Node;
    newNode->buku = buku;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Fungsi membandingkan judul buku (case-insensitive)
int compareJudul(const char* a, const char* b) {
    return strcasecmp(a, b);
}

// Menambahkan buku ke BST
Node* tambahBuku(Node* root, Buku buku, bool& added) {
    if (root == NULL) {
        added = true;
        return createNode(buku);
    }
    int cmp = compareJudul(buku.judul, root->buku.judul);
    if (cmp == 0) {
        cout << "Buku dengan judul \"" << buku.judul << "\" sudah ada. Tidak ditambahkan.\n";
        added = false;
        return root;
    } else if (cmp < 0) {
        root->left = tambahBuku(root->left, buku, added);
    } else {
        root->right = tambahBuku(root->right, buku, added);
    }
    return root;
}

// Menampilkan buku secara in-order
void tampilkanInOrder(Node* root) {
    if (root != NULL) {
        tampilkanInOrder(root->left);
        cout << "Judul      : " << root->buku.judul << endl;
        cout << "Pengarang  : " << root->buku.pengarang << endl;
        cout << "Tahun      : " << root->buku.tahun << endl;
        cout << "--------------------------\n";
        tampilkanInOrder(root->right);
    }
}

// Mencari node minimum di subtree
Node* findMin(Node* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Menghapus buku berdasarkan judul dari BST
Node* hapusBuku(Node* root, const char* judul, bool& deleted, Buku& bukuDihapus) {
    if (root == NULL) {
        deleted = false;
        return NULL;
    }
    int cmp = compareJudul(judul, root->buku.judul);
    if (cmp < 0) {
        root->left = hapusBuku(root->left, judul, deleted, bukuDihapus);
    } else if (cmp > 0) {
        root->right = hapusBuku(root->right, judul, deleted, bukuDihapus);
    } else {
        // Ditemukan node yang akan dihapus
        deleted = true;
        bukuDihapus = root->buku;

        if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = findMin(root->right);
        root->buku = temp->buku;
        root->right = hapusBuku(root->right, temp->buku.judul, deleted, bukuDihapus);
    }
    return root;
}

// Fungsi undo aksi terakhir
void undoAksi() {
    if (logStack.isEmpty()) {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return;
    }
    Aksi aksi = logStack.pop();
    if (aksi.tipe == ADD) {
        bool deleted;
        Buku dummy;
        root = hapusBuku(root, aksi.buku.judul, deleted, dummy);
        if (deleted) {
            cout << "Buku \"" << aksi.buku.judul << "\" telah dihapus.\n";
        } else {
            cout << "Undo gagal: Buku tidak ditemukan.\n";
        }
    } else if (aksi.tipe == DELETE) {
        bool added;
        root = tambahBuku(root, aksi.buku, added);
        if (added) {
            cout << "Buku \"" << aksi.buku.judul << "\" telah dikembalikan.\n";
        } else {
            cout << "Undo gagal: Buku sudah ada.\n";
        }
    }
}

// Fungsi input buku baru
Buku inputBuku() {
    Buku buku;
    cout << "Masukkan Judul Buku: ";
    cin.ignore();
    cin.getline(buku.judul, 100);
    cout << "Masukkan Pengarang: ";
    cin.getline(buku.pengarang, 100);
    cout << "Masukkan Tahun Terbit: ";
    cin >> buku.tahun;
    return buku;
}

int main() {
     int pilihan;
    do {
        cout << "\n=== Kenangan dalam Sebuah Buku ===\n";
        cout << "1. Tambah Buku Baru\n";
        cout << "2. Tampilkan Daftar Buku\n";
        cout << "3. Hapus Buku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "Pilih Menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                Buku buku = inputBuku();
                bool added;
                root = tambahBuku(root, buku, added);
                if (added) {
                    Aksi aksi;
                    aksi.tipe = ADD;
                    aksi.buku = buku;
                    logStack.push(aksi);
                    cout << "Buku berhasil ditambahkan.\n";
                }
                break;
            }
            case 2:
                if (root == NULL) {
                    cout << "Daftar buku kosong.\n";
                } else {
                    cout << "\nDaftar Buku :\n";
                    cout << "--------------------------\n";
                    tampilkanInOrder(root);
                }
                break;
            case 3: {
                cout << "Masukkan judul buku yang ingin dihapus: ";
                char judul[100];
                cin.ignore();
                cin.getline(judul, 100);
                bool deleted;
                Buku bukuDihapus;
                root = hapusBuku(root, judul, deleted, bukuDihapus);
                if (deleted) {
                    Aksi aksi;
                    aksi.tipe = DELETE;
                    aksi.buku = bukuDihapus;
                    logStack.push(aksi);
                    cout << "Buku \"" << judul << "\" berhasil dihapus.\n";
                } else {
                    cout << "Buku dengan judul \"" << judul << "\" tidak ditemukan.\n";
                }
                break;
            }
            case 4:
                undoAksi();
                break;
            case 5:
                cout << "Keluar Program.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 5);
    return 0;
}
