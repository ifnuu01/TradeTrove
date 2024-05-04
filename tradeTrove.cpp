#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
    int role;
    double saldo;
    int id;
    bool status;
};

struct Item {
    int id;
    string name;
    double harga;
    string seller;
    bool status; 
};

User Akun[100];
Item Aplikasi[100];

void displayMainMenu(int &jumlahAkun);
void menuAdmin(int &jumlahAkun, int &jumlahAplikasi);
void menuPenjual(int &jumlahAkun, int index);


void enter() {
    cout << "Tekan Enter untuk melanjutkan...";
    cin.get();
    system("cls || clear");
}

void getlineInput(string prompt, string *input) {
    do{
        cout << prompt;
        getline(cin, *input);
        if(input->empty()) cout << "Input tidak boleh kosong\n";
    }while(input->empty());
}

void getInputint(string prompt, int *input, string message) {
    do{
        cout << prompt;
        cin >> *input;
        if (message == "1"){
            if(cin.fail()|| *input <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus angka atau lebih dari 0\n"; 
            }else{
                break;
            }
        }else if (message == "2"){
            if(cin.fail()|| *input < 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus angka dan tidak boleh mines\n"; 
            }else{
                break;
            }
        }else{
            break;
        }
    }while(true);
}


int bacaAkun() {
    int jumlahAkun = 0;
    ifstream fileAkun("akun.csv");
    if(!fileAkun.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }string line;
    while (getline(fileAkun, line)) {
        stringstream ss(line);
        getline(ss, Akun[jumlahAkun].username, ',');
        getline(ss, Akun[jumlahAkun].password, ',');
        ss >> Akun[jumlahAkun].role;
        ss.ignore();
        ss >> Akun[jumlahAkun].saldo;
        ss.ignore();
        ss >> Akun[jumlahAkun].id;
        ss.ignore();
        ss >> Akun[jumlahAkun].status;
        ++jumlahAkun;
    }fileAkun.close();
    return jumlahAkun;
}

int bacaAplikasi(){
    int jumlahAplikasi = 0;
    ifstream fileAplikasi("aplikasi.csv");
    if(!fileAplikasi.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }string line;
    while (getline(fileAplikasi, line)) {
        stringstream ss(line);
        ss >> Aplikasi[jumlahAplikasi].id;
        ss.ignore();
        getline(ss, Aplikasi[jumlahAplikasi].name, ',');
        ss >> Aplikasi[jumlahAplikasi].harga;
        ss.ignore();
        getline(ss, Aplikasi[jumlahAplikasi].seller, ',');
        ss >> Aplikasi[jumlahAplikasi].status;
        ++jumlahAplikasi;
    }fileAplikasi.close();
    return jumlahAplikasi;

}

void saveAkun(int &jumlahAkun) {
    ofstream fileAkun("akun.csv");
    if(!fileAkun.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahAkun; i++) {
        fileAkun << Akun[i].username << "," << Akun[i].password << "," << Akun[i].role << "," << Akun[i].saldo << "," << Akun[i].id << "," << Akun[i].status << endl;
    }fileAkun.close();
}

void saveAplikasi(int &jumlahAplikasi) {
    ofstream fileAplikasi("aplikasi.csv");
    if(!fileAplikasi.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahAplikasi; i++) {
        fileAplikasi << Aplikasi[i].id << "," << Aplikasi[i].name << "," << Aplikasi[i].harga << "," << Aplikasi[i].seller << "," << Aplikasi[i].status << endl;
    }fileAplikasi.close();
}

void registrasi(int &jumlahAkun) {
    string username, password;
    int role;
    cout << "Pilih Role : " << endl;
    cout << "1. Penjual" << endl;
    cout << "2. Pembeli" << endl;
    getInputint("Role : ", &role, "1");
    Akun[jumlahAkun].role = role;
    cin.ignore();
    getlineInput("Username : ", &username);
    Akun[jumlahAkun].username = username;
    for (int i = 0; i < jumlahAkun; i++) {
        if (Akun[i].username == username) {
            cout << "Username sudah digunakan silahkan input ulang!" << endl;
            registrasi(jumlahAkun);
        }
    }
    getlineInput("Password : ", &password);
    Akun[jumlahAkun].password = password;
    Akun[jumlahAkun].saldo = 0.00;
    Akun[jumlahAkun].id = 0;
    Akun[jumlahAkun].status = false;
    jumlahAkun++;
    saveAkun(jumlahAkun);
    cout << "Registrasi Berhasil! Menunggu Konfirmasi" << endl;
}

int login(int &jumlahAkun) {
    string username, password;
    cin.ignore();
    getlineInput("Username : ", &username);
    getlineInput("Password : ", &password);
    for (int i = 0; i < jumlahAkun; i++) {
        if (Akun[i].username == username && Akun[i].password == password) {
            if (Akun[i].status == true){
                cout << "Login Berhasil" << endl;
                return i;
            } else {
                cout << "Username atau Password salah" << endl;
                return -1;
            }
        }
    }cout << "Akun tidak ditemukan" << endl;
    return -1;
}

void displayMainMenu(int &jumlahAkun, int &jumlahAplikasi) {
    int pilihan;
    int index;
    do{
        cout << "1. Registrasi" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan)
        {
        case 1:
            registrasi(jumlahAkun);
            break;
        case 2:
            index = login(jumlahAkun);
            if (index != -1) {
                switch (Akun[index].role)
                {
                case 1:
                    menuPenjual(jumlahAplikasi,index);
                    break;
                case 2:
                    cout << "Buyer" << endl;
                    break;
                case 3:
                    menuAdmin(jumlahAkun,jumlahAplikasi);
                    break;
                default:
                    cout << "Role tidak ditemukan" << endl;
                    break;
                };
            }
            break;
        case 3:
            cout << "Exit" << endl;
            break;
        default:
            break;
        }
    }while (pilihan != 3);    
}

void lihatRequestAkun(int &jumlahAkun){
    cout << "Request Akun" << endl;
    for (int i = 0; i < jumlahAkun; i++) {
        if (Akun[i].status == false) {
            cout << "Username : " << Akun[i].username;
            switch (Akun[i].role)
            {
            case 1:
                cout << " Role : Penjual";
                break;
            case 2:
                cout << " Role : Pembeli";
                break;
            default:
                cout << " Role tidak ditemukan";
                break;
            }
            cout << " (Belum dikonfirmasi)" << endl;
        }
    }
}

void konfirmasiAkun(int &jumlahAkun){
    string username;
    cin.ignore();
    getlineInput("Masukan username untuk mengkonfirmasi : ", &username);
    for (int i = 0; i < jumlahAkun; i++) {
        if (Akun[i].username == username) {
            Akun[i].status = true;
            getInputint("Masukan ID : ", &Akun[i].id, "2");
            cout << "Akun berhasil dikonfirmasi" << endl;
            saveAkun(jumlahAkun);
            return;
        }
    }cout << "Akun tidak ditemukan" << endl;
}

void lihatPengguna(int &jumlahAkun){
    cout << "Pengguna" << endl;
    for (int i = 1 ; i < jumlahAkun; i++) {
        cout << i << " Username : " << Akun[i].username << " ID : " << Akun[i].id ;
        switch (Akun[i].role)
        {
        case 1:
            cout << " Role : Penjual";
            break;
        case 2:
            cout << " Role : Pembeli";
            break;
        case 3:
            cout << " Role : Admin";
            break;
        default:
            cout << " Role tidak ditemukan";
            break;
        }
        if (Akun[i].status == true) {
            cout << " (Sudah dikonfirmasi)" << endl;
        } else {
            cout << " (Belum dikonfirmasi)" << endl;
        }
    }
}

void hapusAkun(int &jumlahAkun){
    int id;
    cin.ignore();
    getInputint("Masukan ID yang ingin dihapus: ", &id, "2");
    for (int i = 0 ; i < jumlahAkun; i++) {
        if (Akun[i].id == id) {
            for (int j = i; j < jumlahAkun; j++) {
                Akun[j] = Akun[j+1];
            }
            jumlahAkun--;
            cout << "Akun berhasil dihapus" << endl;
            saveAkun(jumlahAkun);  
            }
        }cout << "Akun tidak ditemukan" << endl;
}

void menuAdmin(int &jumlahAkun, int &jumlahAplikasi){
    int pilihan;
    do{
        cout << "Menu Admin" << endl;
        cout << "1. Konfirmasi 1Akun Pengguna" << endl;
        cout << "2. Konfirmasi Aplikasi Penjual" << endl;
        cout << "3. Lihat Pengguna" << endl;
        cout << "4. Lihat Aplikasi Penjual" << endl;
        cout << "5. Hapus Akun Pengguna" << endl;
        cout << "6. Logout" << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan){
            case 1:
                lihatRequestAkun(jumlahAkun);
                konfirmasiAkun(jumlahAkun);
                break;
            case 2:
                cout << "Konfirmasi Aplikasi Penjual" << endl;
                break;
            case 3:
                lihatPengguna(jumlahAkun);
                break;
            case 4:
                cout << "Lihat Aplikasi Penjual" << endl;
                break;
            case 5:
                lihatPengguna(jumlahAkun);
                hapusAkun(jumlahAkun);
                break;
            case 6:
                cout << "Logout" << endl;
                break;
            default:
                cout << "Pilihan tidak ditemukan" << endl;
                break;
        }
    }while (pilihan != 6);
}

void tambahAplikasi(int &jumlahAplikasi, int index){
    string nama;
    double harga;
    cin.ignore();
    getlineInput("Nama Barang : ", &nama);
    cout << "Harga Barang : ";
    cin >> harga;
    Aplikasi[jumlahAplikasi].id = 0;
    Aplikasi[jumlahAplikasi].name = nama;
    Aplikasi[jumlahAplikasi].harga = harga;
    Aplikasi[jumlahAplikasi].seller = Akun[index].username;
    Aplikasi[jumlahAplikasi].status = false;
    jumlahAplikasi++;
    saveAplikasi(jumlahAplikasi);
    cout << "Aplikasi berhasil ditambahkan! Menunggu Konfirmasi" << endl;
}

void lihatAplikasi(int &jumlahAplikasi, int index){
    cout << "Aplikasi Penjual" << endl;
    for (int i = 0; i < jumlahAplikasi; i++) {
        if (Aplikasi[i].id == Akun[index].id) {
            cout << "Nama Barang : " << Aplikasi[i].name << " Harga : " << Aplikasi[i].harga << " Seller : " << Aplikasi[i].seller;
            if (Aplikasi[i].status == true) {
                cout << " (Sudah dikonfirmasi)" << endl;
            } else {
                cout << " (Belum dikonfirmasi)" << endl;
            }
        }
    }
}

void menuPenjual(int &jumlahAplikasi, int index){
    int pilihan;
    do{
        cout << "Menu Penjual" << endl;
        cout << "1. Lihat Barang" << endl;
        cout << "2. Tambah Barang" << endl;
        cout << "3. Edit Barang" << endl;
        cout << "4. Hapus Barang" << endl;
        cout << "5. Konfirmasi Penjualan" << endl;
        cout << "6. Logout" << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan){
            case 1:
                lihatAplikasi(jumlahAplikasi, index);
                break;
            case 2:
                lihatAplikasi(jumlahAplikasi, index);
                break;
            case 3:
                cout << "Edit Barang" << endl;
                break;
            case 4:
                cout << "Hapus Barang" << endl;
                break;
            case 5:
                cout << "Logout" << endl;
                break;
            case 6:
                cout << "Logout" << endl;
                break;
            default:
                cout << "Pilihan tidak ditemukan" << endl;
                break;
        }
    }while (pilihan != 6);
}

int main() {
    int jumlahAkun = bacaAkun();
    int jumlahAplikasi = bacaAplikasi();
    displayMainMenu(jumlahAkun, jumlahAplikasi);
    return 0;
}
