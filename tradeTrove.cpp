#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;


// Struk masih dalam perkembangan
struct User {
    int role;
    int id;
    string username;
    string password;
    string nama;
};

struct produk{
    int idProduk;
    string namaProduk;
    int hargaProduk;
};

struct penjual{
    produk produkPenjual;
    User akunPenjual;
};

struct pembeli{
    User akunPembeli;
};

penjual userPenjual[100];
pembeli userPembeli[100];

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

// fungsi baca file csv khusus untuk akun masih dalam perkembangan
int bacaAkunCsv(penjual akunPenjual[], pembeli akunPembeli[], const string& nama_file) {
    ifstream file(nama_file);
    if (!file.is_open()) {
        cout << "Gagal membuka file " << nama_file << endl;
        return 0;
    }
    int jumlahAkun = 0;
    string line;
    while (getline(file, line) && jumlahAkun < 100) {
        stringstream ss(line);
        string field;
        getline(ss, field, ',');
        if (field == "1"){
            akunPenjual[jumlahAkun].akunPenjual.role = stoi(field);
            getline(ss, field, ',');
            akunPenjual[jumlahAkun].akunPenjual.id = stoi(field);
            getline(ss, akunPenjual[jumlahAkun].akunPenjual.nama, ',');
            getline(ss, akunPenjual[jumlahAkun].akunPenjual.username, ',');
            getline(ss, akunPenjual[jumlahAkun].akunPenjual.password, ',');
        } else if (field == "2"){
            akunPembeli[jumlahAkun].akunPembeli.role = stoi(field);
            getline(ss, field, ',');
            akunPembeli[jumlahAkun].akunPembeli.id = stoi(field);
            getline(ss, akunPembeli[jumlahAkun].akunPembeli.nama, ',');
            getline(ss, akunPembeli[jumlahAkun].akunPembeli.username, ',');
            getline(ss, akunPembeli[jumlahAkun].akunPembeli.password, ',');
        }
        ++jumlahAkun;
    }
    file.close();
    return jumlahAkun;
}

// Fungsi untuk menyimpan data khusus akun, masih dalam perkembangan
void simpanAkunCsv(const penjual akunPenjual[], const pembeli akunPembeli[], int jumlahAkun, const string& nama_file) {
    ofstream file(nama_file);
    if (!file.is_open()) {
        cout << "Gagal membuka file " << nama_file << endl;
        return;
    }
    for (int i = 0; i < jumlahAkun; ++i) {
        if (akunPenjual[i].akunPenjual.role == 1){
            file << akunPenjual[i].akunPenjual.role << ',' << akunPenjual[i].akunPenjual.id << ','<< akunPenjual[i].akunPenjual.nama << ","<< akunPenjual[i].akunPenjual.username << ',' << akunPenjual[i].akunPenjual.password << '\n';
        } else if (akunPembeli[i].akunPembeli.role == 2){
            file << akunPembeli[i].akunPembeli.role << ',' << akunPembeli[i].akunPembeli.id << ',' << akunPenjual[i].akunPenjual.nama << ","<< akunPembeli[i].akunPembeli.username << ',' << akunPembeli[i].akunPembeli.password << '\n';
        }
    }
    file.close();
}

// fungsi registrasi akun
void registrasi(penjual akunPenjual[], pembeli akunPembeli[], int& jumlahAkun){
    if(jumlahAkun >= 100){
        cout << "Database sudah penuh" << endl;
        return;
    }
    int role;
    getInputint("Masukkan role (1. Penjual, 2. Pembeli): ", &role, "1");
    if (role == 1){
        akunPenjual[jumlahAkun].akunPenjual.role = role;
        cin.ignore();
        getlineInput("Masukan nama: ", &akunPenjual[jumlahAkun].akunPenjual.nama);
        akunPenjual[jumlahAkun].akunPenjual.id = rand(); // generate random id
        cout << "id yang di generate: " << akunPenjual[jumlahAkun].akunPenjual.id << "\n";
        getlineInput("Masukkan username: ", &akunPenjual[jumlahAkun].akunPenjual.username);
        getlineInput("Masukkan password: ", &akunPenjual[jumlahAkun].akunPenjual.password);
        jumlahAkun++;
    } else if (role == 2){
        akunPembeli[jumlahAkun].akunPembeli.role = role;
        cin.ignore();
        getlineInput("Masukan nama: ", &akunPembeli[jumlahAkun].akunPembeli.nama);
        akunPembeli[jumlahAkun].akunPembeli.id = rand(); // generate random id
        cout << "id yang di generate: " << akunPembeli[jumlahAkun].akunPembeli.id << "\n";
        getlineInput("Masukkan username: ", &akunPembeli[jumlahAkun].akunPembeli.username);
        getlineInput("Masukkan password: ", &akunPembeli[jumlahAkun].akunPembeli.password);
        jumlahAkun++;
    } else {
        cout << "Role tidak valid" << endl;
    }
}

// Fungsi untuk login
void login(penjual akunPenjual[], pembeli akunPembeli[], int jumlahAkun){
    string username, password;
    cin.ignore();
    getlineInput("Masukkan username: ", &username);
    getlineInput("Masukkan password: ", &password);    
    for (int i = 0; i < jumlahAkun; i++){
        if (akunPenjual[i].akunPenjual.username == username && akunPenjual[i].akunPenjual.password == password && akunPenjual[i].akunPenjual.role == 1){
            cout << "menu penjual"<< endl;
        } else if (akunPembeli[i].akunPembeli.username == username && akunPembeli[i].akunPembeli.password == password && akunPembeli[i].akunPembeli.role == 2){
            cout << "menu user"<< endl;
        }   
    }cout << "Username atau password salah" << endl;
}

int main(){
    int jumlahAkun = bacaAkunCsv(userPenjual, userPembeli, "akun.csv");
    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. Registrasi\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        getInputint("Pilihan Anda: ", &pilihan, "1");
        switch (pilihan) {
            case 1:
                registrasi(userPenjual, userPembeli, jumlahAkun);
                simpanAkunCsv(userPenjual, userPembeli, jumlahAkun, "akun.csv");
                break;
            case 2:
                login(userPenjual, userPembeli, jumlahAkun);
                break;
            case 3:
                cout << "Program selesai, sampai jumpa!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 3);

    return 0;
}