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

struct review {
    int id;
    int idAplikasi;
    string usernamePembeli;
    string review;
    int rating;
};

struct transaksi {
    int id;
    int idPembeli;
    int status; // 0 = pending, 1 = success, 2 = failed
    Item aplikasi;
    string usernamePembeli;
};

User Akun[100];
Item Aplikasi[100];
review daftarReview[100];
transaksi daftarTransaksi[100];

void displayMainMenu(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi);
void menuAdmin(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview);
void menuPenjual(int &jumlahAkun, int index, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi);
void lihatAplikasi(int &jumlahAplikasi, int index, string role, string status, int &jumlahReview);
void menuPembeli(int &jumlahAkun, int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi);


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
    ifstream fileAkun("database/akun.csv");
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
    ifstream fileAplikasi("database/aplikasi.csv");
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

int bacaReview(){
    int jumlahReview = 0;
    ifstream fileReview("database/review.csv");
    if(!fileReview.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }string line;
    while (getline(fileReview, line)) {
        stringstream ss(line);
        ss >> daftarReview[jumlahReview].id;
        ss.ignore();
        ss >> daftarReview[jumlahReview].idAplikasi;
        ss.ignore();
        getline(ss, daftarReview[jumlahReview].usernamePembeli, ',');
        getline(ss, daftarReview[jumlahReview].review, ',');
        ss >> daftarReview[jumlahReview].rating;
        ++jumlahReview;
    }fileReview.close();
    return jumlahReview;
}

int bacaTransaksi(){
    int jumlahTransaksi = 0;
    ifstream fileTransaksi("database/transaksi.csv");
    if(!fileTransaksi.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }string line;
    while (getline(fileTransaksi, line)) {
        stringstream ss(line);
        ss >> daftarTransaksi[jumlahTransaksi].id;
        ss.ignore();
        ss >> daftarTransaksi[jumlahTransaksi].idPembeli;
        ss.ignore();
        ss >> daftarTransaksi[jumlahTransaksi].status;
        ss.ignore();
        ss >> daftarTransaksi[jumlahTransaksi].aplikasi.id;
        ss.ignore();
        getline(ss, daftarTransaksi[jumlahTransaksi].aplikasi.name, ',');
        ss >> daftarTransaksi[jumlahTransaksi].aplikasi.harga;
        ss.ignore();
        getline(ss, daftarTransaksi[jumlahTransaksi].aplikasi.seller, ',');
        getline(ss, daftarTransaksi[jumlahTransaksi].usernamePembeli, ',');
        ++jumlahTransaksi;
    }fileTransaksi.close();
    return jumlahTransaksi;
}

void saveAkun(int &jumlahAkun) {
    ofstream fileAkun("database/akun.csv");
    if(!fileAkun.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahAkun; i++) {
        fileAkun << Akun[i].username << "," << Akun[i].password << "," << Akun[i].role << "," << Akun[i].saldo << "," << Akun[i].id << "," << Akun[i].status << endl;
    }fileAkun.close();
}

void saveAplikasi(int &jumlahAplikasi) {
    ofstream fileAplikasi("database/aplikasi.csv");
    if(!fileAplikasi.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahAplikasi; i++) {
        fileAplikasi << Aplikasi[i].id << "," << Aplikasi[i].name << "," << Aplikasi[i].harga << "," << Aplikasi[i].seller << "," << Aplikasi[i].status << endl;
    }fileAplikasi.close();
}

void saveReview(int &jumlahReview) {
    ofstream fileReview("database/review.csv");
    if(!fileReview.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahReview; i++) {
        fileReview << daftarReview[i].id << "," << daftarReview[i].idAplikasi << "," << daftarReview[i].usernamePembeli << "," << daftarReview[i].review << "," << daftarReview[i].rating << endl;
    }fileReview.close();
}

void saveTransaksi(int &jumlahTransaksi) {
    ofstream fileTransaksi("database/transaksi.csv");
    if(!fileTransaksi.is_open()) {
        cout << "File tidak ditemukan" << endl;
        return;
    }for (int i = 0; i < jumlahTransaksi; i++) {
        fileTransaksi << daftarTransaksi[i].id << "," << daftarTransaksi[i].idPembeli << "," << daftarTransaksi[i].status << "," << daftarTransaksi[i].aplikasi.id << "," << daftarTransaksi[i].aplikasi.name << "," << daftarTransaksi[i].aplikasi.harga << "," << daftarTransaksi[i].aplikasi.seller << "," << daftarTransaksi[i].usernamePembeli << endl;
    }fileTransaksi.close();
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

void displayMainMenu(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi) {
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
                    menuPenjual(jumlahAplikasi,index, jumlahReview, jumlahTransaksi, jumlahAkun);
                    break;
                case 2:
                    menuPembeli(jumlahAkun,jumlahAplikasi,index, jumlahReview, jumlahTransaksi);
                    break;
                case 3:
                    menuAdmin(jumlahAkun,jumlahAplikasi, jumlahReview);
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

void hapusAkun(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview){
    int id;
    getInputint("Masukkan ID akun yang ingin dihapus : ", &id, "1");
    bool found = false;
    for (int i = 0; i < jumlahAkun; i++) {
        if (Akun[i].id == id) {
            for (int j = i; j < jumlahAkun - 1; j++) {
                Akun[j] = Akun[j + 1];
            }
            jumlahAkun--;
            saveAkun(jumlahAkun);
            for (int j = 0; j < jumlahAplikasi; j++) {
                if (Aplikasi[j].seller == Akun[i].username) {
                    for (int k = j; k < jumlahAplikasi - 1; k++) {
                        Aplikasi[k] = Aplikasi[k + 1];
                    }
                    jumlahAplikasi--;
                    for (int k = j; k < jumlahAplikasi; k++) {
                        Aplikasi[k].id = k + 1;
                    }
                    for (int k = 0; k < jumlahReview; k++) {
                        if (daftarReview[k].idAplikasi == Aplikasi[j].id) {
                            for (int l = k; l < jumlahReview - 1; l++) {
                                daftarReview[l] = daftarReview[l + 1];
                            }
                            jumlahReview--;
                            for (int l = k; l < jumlahReview; l++) {
                                daftarReview[l].id = l + 1;
                            }
                        }
                    }
                }
            }
            saveAplikasi(jumlahAplikasi);
            saveReview(jumlahReview);
            for (int j = 0; j < jumlahReview; j++) {
                if (daftarReview[j].usernamePembeli == Akun[i].username) {
                    for (int k = j; k < jumlahReview - 1; k++) {
                        daftarReview[k] = daftarReview[k + 1];
                    }
                    jumlahReview--;
                    for (int k = j; k < jumlahReview; k++) {
                        daftarReview[k].id = k + 1;
                    }
                }
            }
            saveReview(jumlahReview);
            cout << "Akun berhasil dihapus" << endl;
            found = true;
            return;
        }
    }if(!found){
            cout << "Akun tidak ditemukan" << endl;
    }
}

void konfirmasiAplikasi(int &jumlahAplikasi) {
    int appId;
    cout << "Masukkan ID aplikasi untuk dikonfirmasi: ";
    cin >> appId;
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++) {
        if (Aplikasi[i].id == appId && !Aplikasi[i].status) {
            Aplikasi[i].status = true;
            saveAplikasi(jumlahAplikasi);
            cout << "Aplikasi berhasil dikonfirmasi!" << endl;
            found = true;
            return;
        }
    }
    if (!found) {
        cout << "Aplikasi tidak ditemukan atau sudah dikonfirmasi sebelumnya!" << endl;
    }
}

void menuAdmin(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview){
    int pilihan;
    do{
        cout << "Menu Admin" << endl;
        cout << "1. Konfirmasi Akun Pengguna" << endl;
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
                lihatAplikasi(jumlahAplikasi, 2, "admin", "unconfirm", jumlahReview);
                konfirmasiAplikasi(jumlahAplikasi);
                break;
            case 3:
                lihatPengguna(jumlahAkun);
                break;
            case 4:
                lihatAplikasi(jumlahAplikasi, 2, "admin", "all", jumlahReview);
                break;
            case 5:
                lihatPengguna(jumlahAkun);
                hapusAkun(jumlahAkun, jumlahAplikasi, jumlahReview);
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
    getlineInput("Nama Aplikasi : ", &nama);
    cout << "Harga Aplikasi : ";
    cin >> harga;
    Aplikasi[jumlahAplikasi].id = (jumlahAplikasi == 0) ? 1 : Aplikasi[jumlahAplikasi - 1].id + 1;
    Aplikasi[jumlahAplikasi].name = nama;
    Aplikasi[jumlahAplikasi].harga = harga;
    Aplikasi[jumlahAplikasi].seller = Akun[index].username;
    Aplikasi[jumlahAplikasi].status = false;
    jumlahAplikasi++;
    saveAplikasi(jumlahAplikasi);
    cout << "Aplikasi berhasil ditambahkan! Menunggu Konfirmasi" << endl;
}

void lihatAplikasi(int &jumlahAplikasi, int index, string role, string status, int &jumlahReview){
    cout << "Aplikasi Penjual" << endl;
    if (role == "admin" && status == "unconfirm"){
        for (int i = 0; i < jumlahAplikasi; i++) {
            if (Aplikasi[i].status == false) {
                cout <<"ID : " << Aplikasi[i].id << " Nama Aplikasi : " << Aplikasi[i].name << " Harga : " << Aplikasi[i].harga << " Seller : " << Aplikasi[i].seller;
                cout << " (Belom dikonfirmasi)" << endl;
            }
        }
    }else if (role == "admin" && status == "all"){
        for (int i = 0; i < jumlahAplikasi; i++) {
            cout <<"ID : " << Aplikasi[i].id << " Nama Barang : " << Aplikasi[i].name << " Harga : " << Aplikasi[i].harga << " Seller : " << Aplikasi[i].seller;
            if (Aplikasi[i].status == true) {
                cout << " (Sudah dikonfirmasi)" << endl;
            } else {
                cout << " (Belum dikonfirmasi)" << endl;
            }
        }
    }else if(role == "pembeli" && status == "confirm"){
        for (int i = 0; i < jumlahAplikasi; i++) {
            if (Aplikasi[i].status == true) {
                cout <<"ID : " << Aplikasi[i].id << " Nama Barang : " << Aplikasi[i].name << " Harga : " << Aplikasi[i].harga << " Seller : " << Aplikasi[i].seller << endl;
                cout << "Review : " << endl;
                bool foundReview = false;
                for (int j = 0; j < jumlahReview; j++) {
                    if (daftarReview[j].idAplikasi == Aplikasi[i].id) {
                        cout << "Rating : " << daftarReview[j].rating << endl;
                        cout << "Pembeli : " << daftarReview[j].usernamePembeli << endl;
                        cout << "Review : " << daftarReview[j].review << endl;
                        foundReview = true;
                    }
                }if (!foundReview){
                    cout << "Belum ada review untuk aplikasi ini. " << endl;
                }
            }
        }
    }else{
        for (int i = 0; i < jumlahAplikasi; i++) {
            if (Aplikasi[i].seller == Akun[index].username) {
                cout <<"ID : " << Aplikasi[i].id << " Nama Barang : " << Aplikasi[i].name << " Harga : " << Aplikasi[i].harga << " Seller : " << Aplikasi[i].seller;
                if (Aplikasi[i].status == true) {
                    cout << " (Sudah dikonfirmasi)" << endl;
                } else {
                    cout << " (Belum dikonfirmasi)" << endl;
                }
            }
        }
    }
}

void editAplikasi(int &jumlahAplikasi, int index) {
    int appId;
    getInputint("Masukkan ID aplikasi untuk diedit: ", &appId, "1");
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++) {
        if (Aplikasi[i].id == appId && Aplikasi[i].seller == Akun[index].username) {
            string newName;
            double newPrice;
            cin.ignore();
            getlineInput("Masukkan nama baru untuk aplikasi: ", &newName);
            cout << "Masukkan harga baru untuk aplikasi: ";
            cin >> newPrice;
            Aplikasi[i].name = newName;
            Aplikasi[i].harga = newPrice;
            saveAplikasi(jumlahAplikasi);
            cout << "Aplikasi berhasil diedit!" << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Aplikasi tidak ditemukan atau Anda tidak memiliki hak akses untuk mengedit aplikasi tersebut!" << endl;
    }
}

void hapusAplikasi(int &jumlahAplikasi, int index) {
    int appId;
    getInputint("Masukkan ID aplikasi untuk dihapus: ", &appId, "1");
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++) {
        if (Aplikasi[i].id == appId && Aplikasi[i].seller == Akun[index].username) {
            for (int j = i; j < jumlahAplikasi - 1; j++) {
                Aplikasi[j] = Aplikasi[j + 1];
            }
            jumlahAplikasi--;
            for (int j = i; j < jumlahAplikasi; j++) {
                Aplikasi[j].id = j + 1;
            }
            saveAplikasi(jumlahAplikasi);
            cout << "Aplikasi berhasil dihapus!" << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Aplikasi tidak ditemukan atau Anda tidak memiliki hak akses untuk menghapus aplikasi tersebut!" << endl;
    }
}

void konfirmasiPenjualan(int &jumlahAplikasi, int index, int &jumlahTransaksi, int &jumlahAkun){
    int idApp;
    getInputint("Masukkan ID aplikasi yang ingin dikonfirmasi: ", &idApp, "1");
    bool found = false;
    int status;
    getInputint("Masukkan status transaksi (1 = success, 2 = failed): ", &status, "2");
    if (status < 1 || status > 2) {
        cout << "Status transaksi tidak valid!" << endl;
        return;
    }else {
        for (int i = 0 ; i < jumlahTransaksi; i++) {
            if (daftarTransaksi[i].id == idApp && daftarTransaksi[i].aplikasi.seller == Akun[index].username && daftarTransaksi[i].status == 0) {
                if (status == 1) {
                    Akun[index].saldo += daftarTransaksi[i].aplikasi.harga;
                    daftarTransaksi[i].status = 1;
                    cout << "Transaksi berhasil!" << endl;
                } else {
                    daftarTransaksi[i].status = 2;
                    for (int j = 0; j < jumlahAkun; j++) {
                        if (daftarTransaksi[i].idPembeli == Akun[j].id) {
                            Akun[j].saldo += daftarTransaksi[i].aplikasi.harga;
                            break;
                        }
                    }
                    cout << "Transaksi gagal!" << endl;
                }
            }
        }saveTransaksi(jumlahTransaksi);
    }

}

void daftarTransaksiAplikasi(int &jumlahTransaksi, int index, string role){
    cout << "Daftar Transaksi Aplikasi" << endl;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (daftarTransaksi[i].aplikasi.seller == Akun[index].username && daftarTransaksi[i].status == 0 && role == "penjual") {
            cout << "ID Transaksi : " << daftarTransaksi[i].id << " Nama Pembeli : " << daftarTransaksi[i].usernamePembeli << " Nama Aplikasi : " << daftarTransaksi[i].aplikasi.name << " Status : " << "Pending" << endl;
        }else if(daftarTransaksi[i].idPembeli == Akun[index].id && role == "pembeli"){
            cout << "ID Aplikasi : " << daftarTransaksi[i].id << " Nama Penjual : " << daftarTransaksi[i].aplikasi.seller << " Nama Aplikasi : " << daftarTransaksi[i].aplikasi.name << " Status : ";
            if (daftarTransaksi[i].status == 0) {
                cout << "Pending" << endl;
            } else if (daftarTransaksi[i].status == 1) {
                cout << "Success" << endl;
            } else {
                cout << "Failed" << endl;
            }
        }
    }
}

void menuPenjual(int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi, int &jumlahAkun){
    int pilihan;
    do{
        cout << "Menu Penjual" << endl;
        cout << "1. Lihat Aplikasi" << endl;
        cout << "2. Tambah Aplikasi" << endl;
        cout << "3. Edit Aplikasi" << endl;
        cout << "4. Hapus Aplikasi" << endl;
        cout << "5. Konfirmasi Penjualan" << endl;
        cout << "6. Logout" << endl;
        cout << "Total Saldo : $" << Akun[index].saldo << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan){
            case 1:
                lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
                break;
            case 2:
                tambahAplikasi(jumlahAplikasi, index);
                break;
            case 3:
                lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
                editAplikasi(jumlahAplikasi, index);
                break;
            case 4:
                lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
                hapusAplikasi(jumlahAplikasi, index);
                break;
            case 5:
                daftarTransaksiAplikasi(jumlahTransaksi, index, "penjual");
                konfirmasiPenjualan(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
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

void isiSaldo(int &jumlahAkun, int index){
    double saldo;
    cout << "Masukkan jumlah saldo yang ingin diisi : $";
    cin >> saldo;
    Akun[index].saldo += saldo;
    saveAkun(jumlahAkun);
    cout << "Saldo berhasil diisi!" << endl;
}

void beliAplikasi(int &jumlahAplikasi, int index, int &jumlahTransaksi, int &jumlahAkun){
    int appId;
    cout << "Masukkan ID aplikasi yang ingin dibeli: ";
    cin >> appId;
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++) {
        if (Aplikasi[i].id == appId && Aplikasi[i].status) {
            if (Akun[index].saldo >= Aplikasi[i].harga) {
                Akun[index].saldo -= Aplikasi[i].harga;
                daftarTransaksi[jumlahTransaksi].id = (jumlahTransaksi == 0) ? 1 : daftarTransaksi[jumlahTransaksi - 1].id + 1;
                daftarTransaksi[jumlahTransaksi].idPembeli = Akun[index].id;
                daftarTransaksi[jumlahTransaksi].aplikasi.id = Aplikasi[i].id;
                daftarTransaksi[jumlahTransaksi].aplikasi.name = Aplikasi[i].name;
                daftarTransaksi[jumlahTransaksi].aplikasi.harga = Aplikasi[i].harga;
                daftarTransaksi[jumlahTransaksi].aplikasi.seller = Aplikasi[i].seller;
                daftarTransaksi[jumlahTransaksi].usernamePembeli = Akun[index].username;
                daftarTransaksi[jumlahTransaksi].status = 0;
                jumlahTransaksi++;
                saveTransaksi(jumlahTransaksi);
                saveAkun(jumlahAkun);
                cout << "Transaksi berhasil!" << endl;
            } else {
                cout << "Saldo tidak mencukupi!" << endl;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Aplikasi tidak ditemukan atau belum dikonfirmasi!" << endl;
    }
}

void tambahReview(int &jumlahReview, int &jumlahTransaksi, int index){
    int idApp;
    getInputint("Masukkan ID aplikasi yang ingin direview: ", &idApp, "1");
    bool found = false;
    for (int i = 0; i < jumlahTransaksi ; i++){
        if (daftarTransaksi[i].id == idApp && daftarTransaksi[i].status == 1 && daftarTransaksi[i].idPembeli == Akun[index].id){
            string review;
            int rating;
            cin.ignore();
            getlineInput("Masukkan review: ", &review);
            getInputint("Masukkan rating (1-5): ", &rating, "1");
            daftarReview[jumlahReview].id = (jumlahReview == 0) ? 1 : daftarReview[jumlahReview - 1].id + 1;
            daftarReview[jumlahReview].idAplikasi = daftarTransaksi[i].aplikasi.id;
            daftarReview[jumlahReview].usernamePembeli = Akun[index].username;
            daftarReview[jumlahReview].review = review;
            daftarReview[jumlahReview].rating = rating;
            jumlahReview++;
            saveReview(jumlahReview);
            cout << "Review berhasil ditambahkan!" << endl;
            found = true;
            break;
        }
    }if(!found){
        cout << "Aplikasi tidak ditemukan atau belum dikonfirmasi!" << endl;
    }
}

void menuPembeli(int &jumlahAkun,int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi){
    int pilihan;
    do{
        cout << "Menu Pembeli" << endl;
        cout << "1. Lihat Aplikasi" << endl;
        cout << "2. Isi Saldo" << endl;
        cout << "3. Beli Aplikasi" << endl;
        cout << "4. Riwayat Transaksi" << endl;
        cout << "5. Review Aplikasi " << endl;
        cout << "6. Logout" << endl;
        cout << "Total Saldo : $" << Akun[index].saldo << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan){
            case 1:
                lihatAplikasi(jumlahAplikasi, index, "pembeli", "confirm", jumlahReview);
                break;
            case 2:
                isiSaldo(jumlahAkun, index);
                break;
            case 3:
                lihatAplikasi(jumlahAplikasi, index, "pembeli", "confirm", jumlahReview);
                beliAplikasi(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
                break;
            case 4:
                daftarTransaksiAplikasi(jumlahTransaksi, index, "pembeli"); 
                break;
            case 5:
                daftarTransaksiAplikasi(jumlahTransaksi, index, "pembeli"); 
                tambahReview(jumlahReview, jumlahTransaksi, index);
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
    int jumlahReview = bacaReview();
    int jumlahTransaksi = bacaTransaksi();
    // cout << "Jumlah Akun : " << jumlahAkun << endl;
    // cout << "Jumlah Aplikasi : " << jumlahAplikasi << endl;
    // cout << "Jumlah Review : " << jumlahReview << endl;
    // cout << "Jumlah Transaksi : " << jumlahTransaksi << endl;
    displayMainMenu(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
    return 0;
}
