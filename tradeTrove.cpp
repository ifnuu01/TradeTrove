#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct User
{
    string username;
    string password;
    int role;
    double saldo;
    int id;
    bool status;
};

struct Item
{
    int id;
    string name;
    double harga;
    string seller;
    int status;
};

struct review
{
    int id;
    int idAplikasi;
    string usernamePembeli;
    string review;
    int rating;
};

struct transaksi
{
    int id;
    int status; // 0 = pending, 1 = success, 2 = failed
    Item aplikasi;
    string usernamePembeli;
};

User Akun[100];
Item Aplikasi[100];
review daftarReview[100];
transaksi daftarTransaksi[100];

void displayMainMenu(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi);
void menuAdmin(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi);
void menuPenjual(int &jumlahAkun, int index, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi);
void lihatAplikasi(int &jumlahAplikasi, int index, string role, string status, int &jumlahReview);
void menuPembeli(int &jumlahAkun, int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi);
void sortAplikasi(string role, int &jumlahAplikasi, string sortBy, string mode, int &jumlahReview, int index, int &jumlahTransaksi, int &jumlahAkun);
void menuSortingAplikasi(int &jumlahAplikasi, int jumlahReview, int index, string role, int &jumlahTransaksi, int &jumlahAkun);
int binarySearchApp(Item data[], int jumlahData, string cari);
void menuSortingPengguna(int index, int &jumlahAkun);
void sortPengguna(string sortBy, string mode, string option, int &jumlahAkun);
int binarySearchUsername(int jumlahData, string cari);

void enter(bool ignore = 0)
{
    if (ignore)
    {
        cin.ignore();
    }
    cout << "Tekan Enter untuk melanjutkan...";
    cin.ignore(1000, '\n');
    system("cls || clear");
}

void getlineInput(string prompt, string *input)
{
    do
    {
        cout << prompt;
        getline(cin, *input);
        if (input->empty())
            cout << "Input tidak boleh kosong\n";
    } while (input->empty());
}

void getInputint(string prompt, int *input, string message)
{
    do
    {
        cout << prompt;
        cin >> *input;
        if (message == "1")
        {
            if (cin.fail() || *input <= 0)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus angka atau lebih dari 0\n";
            }
            else
            {
                break;
            }
        }
        else if (message == "2")
        {
            if (cin.fail() || *input < 0)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus angka dan tidak boleh mines\n";
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    } while (true);
}

int bacaAkun()
{
    int jumlahAkun = 0;
    ifstream fileAkun("database/akun.csv");
    if (!fileAkun.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }
    string line;
    while (getline(fileAkun, line))
    {
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
    }
    fileAkun.close();
    return jumlahAkun;
}

int bacaAplikasi()
{
    int jumlahAplikasi = 0;
    ifstream fileAplikasi("database/aplikasi.csv");
    if (!fileAplikasi.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }
    string line;
    while (getline(fileAplikasi, line))
    {
        stringstream ss(line);
        ss >> Aplikasi[jumlahAplikasi].id;
        ss.ignore();
        getline(ss, Aplikasi[jumlahAplikasi].name, ',');
        ss >> Aplikasi[jumlahAplikasi].harga;
        ss.ignore();
        getline(ss, Aplikasi[jumlahAplikasi].seller, ',');
        ss >> Aplikasi[jumlahAplikasi].status;
        ++jumlahAplikasi;
    }
    fileAplikasi.close();
    return jumlahAplikasi;
}

int bacaReview()
{
    int jumlahReview = 0;
    ifstream fileReview("database/review.csv");
    if (!fileReview.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }
    string line;
    while (getline(fileReview, line))
    {
        stringstream ss(line);
        ss >> daftarReview[jumlahReview].id;
        ss.ignore();
        ss >> daftarReview[jumlahReview].idAplikasi;
        ss.ignore();
        getline(ss, daftarReview[jumlahReview].usernamePembeli, ',');
        getline(ss, daftarReview[jumlahReview].review, ',');
        ss >> daftarReview[jumlahReview].rating;
        ++jumlahReview;
    }
    fileReview.close();
    return jumlahReview;
}

int bacaTransaksi()
{
    int jumlahTransaksi = 0;
    ifstream fileTransaksi("database/transaksi.csv");
    if (!fileTransaksi.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return 0;
    }
    string line;
    while (getline(fileTransaksi, line))
    {
        stringstream ss(line);
        ss >> daftarTransaksi[jumlahTransaksi].id;
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
    }
    fileTransaksi.close();
    return jumlahTransaksi;
}

void saveAkun(int &jumlahAkun)
{
    ofstream fileAkun("database/akun.csv");
    if (!fileAkun.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    for (int i = 0; i < jumlahAkun; i++)
    {
        fileAkun << Akun[i].username << "," << Akun[i].password << "," << Akun[i].role << "," << Akun[i].saldo << "," << Akun[i].id << "," << Akun[i].status << endl;
    }
    fileAkun.close();
}

void saveAplikasi(int &jumlahAplikasi)
{
    ofstream fileAplikasi("database/aplikasi.csv");
    if (!fileAplikasi.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        fileAplikasi << Aplikasi[i].id << "," << Aplikasi[i].name << "," << Aplikasi[i].harga << "," << Aplikasi[i].seller << "," << Aplikasi[i].status << endl;
    }
    fileAplikasi.close();
}

void saveReview(int &jumlahReview)
{
    ofstream fileReview("database/review.csv");
    if (!fileReview.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    for (int i = 0; i < jumlahReview; i++)
    {
        fileReview << daftarReview[i].id << "," << daftarReview[i].idAplikasi << "," << daftarReview[i].usernamePembeli << "," << daftarReview[i].review << "," << daftarReview[i].rating << endl;
    }
    fileReview.close();
}

void saveTransaksi(int &jumlahTransaksi)
{
    ofstream fileTransaksi("database/transaksi.csv");
    if (!fileTransaksi.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    for (int i = 0; i < jumlahTransaksi; i++)
    {
        fileTransaksi << daftarTransaksi[i].id << "," << daftarTransaksi[i].status << "," << daftarTransaksi[i].aplikasi.id << "," << daftarTransaksi[i].aplikasi.name << "," << daftarTransaksi[i].aplikasi.harga << "," << daftarTransaksi[i].aplikasi.seller << "," << daftarTransaksi[i].usernamePembeli << endl;
    }
    fileTransaksi.close();
}

void registrasi(int &jumlahAkun)
{
    string username, password;
    int role;
    cout << "Pilih Role : " << endl;
    cout << "1. Penjual" << endl;
    cout << "2. Pembeli" << endl;
    getInputint("Role : ", &role, "1");
    while (role < 1 || role > 2)
    {
        cout << "Role tidak valid" << endl;
        getInputint("Role : ", &role, "1");
    }
    Akun[jumlahAkun].role = role;
    cin.ignore();
    getlineInput("Username : ", &username);
    getlineInput("Password : ", &password);
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].username == username)
        {
            system("cls || clear");
            cout << "Username '" << username << "' sudah digunakan silahkan input ulang!" << endl;
            registrasi(jumlahAkun);
            return;
        }
    }
    Akun[jumlahAkun].username = username;
    Akun[jumlahAkun].password = password;
    Akun[jumlahAkun].saldo = 0.00;
    Akun[jumlahAkun].id = (jumlahAkun == 0) ? 1 : Akun[jumlahAkun - 1].id + 1;
    Akun[jumlahAkun].status = false;
    jumlahAkun++;
    saveAkun(jumlahAkun);
    cout << "Registrasi Berhasil! Menunggu Konfirmasi" << endl;
}

int login(int &jumlahAkun)
{
    string username, password;
    cin.ignore();
    cout << "====================\n";
    cout << "   Halaman Login" << endl;
    cout << "====================\n";
    getlineInput("Username : ", &username);
    getlineInput("Password : ", &password);
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].username == username && Akun[i].password == password)
        {
            if (Akun[i].status == true)
            {
                cout << "Login Berhasil" << endl;
                return i;
            }
            else
            {
                cout << "Akun belum dikonfirmasi" << endl;
                return -1;
            }
        }
    }
    cout << "Username atau password salah !!" << endl;
    return -1;
}

void displayMainMenu(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi)
{
    int pilihan;
    int index;
    do
    {
        cout << "========================\n";
        cout << "       Trade Trove" << endl;
        cout << "========================\n";
        cout << "      1. Registrasi" << endl;
        cout << "      2. Login" << endl;
        cout << "      3. Exit" << endl;
        cout << "========================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            registrasi(jumlahAkun);
            enter();
            break;
        case 2:
            index = login(jumlahAkun);
            enter();
            if (index != -1)
            {
                switch (Akun[index].role)
                {
                case 1:
                    menuPenjual(jumlahAplikasi, index, jumlahReview, jumlahTransaksi, jumlahAkun);
                    break;
                case 2:
                    menuPembeli(jumlahAkun, jumlahAplikasi, index, jumlahReview, jumlahTransaksi);
                    break;
                case 3:
                    menuAdmin(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
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
    } while (pilihan != 3);
}

void lihatRequestAkun(int &jumlahAkun)
{
    cout << "===========================\n";
    cout << "       Request Akun" << endl;
    cout << "===========================\n";
    bool found = false;
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].status == false)
        {
            cout << "Username : " << Akun[i].username << endl;
            cout << "ID : " << Akun[i].id << endl;
            switch (Akun[i].role)
            {
            case 1:
                cout << "Role : Penjual" << endl;
                break;
            case 2:
                cout << "Role : Pembeli" << endl;
                break;
            default:
                cout << "Role tidak ditemukan" << endl;
                break;
            }
            cout << "Status : Belum dikonfirmasi" << endl;
            cout << "________________________________________\n\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "Tidak ada request akun..." << endl;
    }
}

void konfirmasiAkun(int &jumlahAkun)
{
    int idAkun;
    cin.ignore();
    getInputint("Masukan Id Akun yang ingin dikonfirmasi (Ketik 0 untuk keluar): ", &idAkun, "2");
    if (idAkun == 0)
    {
        system("cls || clear");
        cout << "Keluar dari menu konfirmasi akun." << endl;
        return;
    }
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].id == idAkun && Akun[i].status == false)
        {
            Akun[i].status = true;
            sortPengguna("id", "asc", "lihat", jumlahAkun);
            saveAkun(jumlahAkun);
            system("cls || clear");
            cout << "Akun berhasil dikonfirmasi." << endl;
            return;
        }
    }
    system("cls || clear");
    cout << "Akun tidak ditemukan atau sudah dikonfirmasi sebelumnya..." << endl;
}

void coutPengguna(int id, string name, bool status, int role)
{
    cout << "ID : " << id << endl;
    cout << "Username : " << name << endl;
    switch (role)
    {
    case 1:
        cout << "Role : Penjual" << endl;
        break;
    case 2:
        cout << "Role : Pembeli" << endl;
        break;
    default:
        cout << "Role tidak ditemukan" << endl;
        break;
    }
    if (status)
    {
        cout << "Status : Sudah dikonfirmasi" << endl;
    }
    else
    {
        cout << "Status : Belum dikonfirmasi" << endl;
    }
    cout << "________________________________________\n\n";
}

void lihatPengguna(int &jumlahAkun)
{
    cout << "========================================\n";
    cout << "            List Pengguna" << endl;
    cout << "========================================\n";
    if (jumlahAkun == 0)
    {
        cout << "\nBelum ada pengguna" << endl;
        return;
    }
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].username != "admin" && Akun[i].role != 3)
        {
            cout << i << ".\tUsername : " << Akun[i].username << endl;
            cout << "\tID : " << Akun[i].id << endl;
            switch (Akun[i].role)
            {
            case 1:
                cout << "\tRole : Penjual" << endl;
                break;
            case 2:
                cout << "\tRole : Pembeli" << endl;
                break;
            case 3:
                cout << "\tRole : Admin" << endl;
                break;
            default:
                cout << "\tRole tidak ditemukan" << endl;
                break;
            }
            if (Akun[i].status)
            {
                cout << "\tStatus : Sudah dikonfirmasi" << endl;
            }
            else
            {
                cout << "\tStatus : Belum dikonfirmasi" << endl;
            }
            cout << "________________________________________\n\n";
        }
    }
}

void hapusAkun(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi)
{
    int id;
    getInputint("Masukkan ID akun yang ingin dihapus (Ketik 0 untuk keluar): ", &id, "2");
    if (id == 0)
    {
        system("cls || clear");
        cout << "Keluar dari menu hapus akun." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahAkun; i++)
    {
        if (Akun[i].id == id && id > 1)
        {
            string username = Akun[i].username;
            for (int j = i; j < jumlahAkun - 1; j++)
            {
                Akun[j] = Akun[j + 1];
            }
            jumlahAkun--;
            for (int j = i; j < jumlahAkun; j++)
            {
                Akun[j].id = j + 1;
            }
            saveAkun(jumlahAkun);
            for (int j = 0; j < jumlahAplikasi;)
            {
                if (Aplikasi[j].seller == username)
                {
                    int idAplikasi = Aplikasi[j].id;
                    for (int k = j; k < jumlahAplikasi - 1; k++)
                    {
                        Aplikasi[k] = Aplikasi[k + 1];
                    }
                    jumlahAplikasi--;
                    for (int k = 0; k < jumlahReview;)
                    {
                        if (daftarReview[k].idAplikasi == idAplikasi)
                        {
                            for (int l = k; l < jumlahReview - 1; l++)
                            {
                                daftarReview[l] = daftarReview[l + 1];
                            }
                            jumlahReview--;
                            for (int l = k; l < jumlahReview; l++)
                            {
                                daftarReview[l].id = l + 1;
                            }
                        }
                        else
                        {
                            k++;
                        }
                    }
                }
                else
                {
                    j++;
                }
            }
            saveAplikasi(jumlahAplikasi);
            saveReview(jumlahReview);
            for (int j = 0; j < jumlahReview;)
            {
                if (daftarReview[j].usernamePembeli == username)
                {
                    for (int k = j; k < jumlahReview - 1; k++)
                    {
                        daftarReview[k] = daftarReview[k + 1];
                    }
                    jumlahReview--;
                    for (int k = j; k < jumlahReview; k++)
                    {
                        daftarReview[k].id = k + 1;
                    }
                }
                else
                {
                    j++;
                }
            }
            saveReview(jumlahReview);
            for (int k = 0; k < jumlahTransaksi;)
            {
                if (daftarTransaksi[k].usernamePembeli == username && !daftarTransaksi[k].status)
                {
                    for (int l = k; l < jumlahTransaksi - 1; l++)
                    {
                        daftarTransaksi[l] = daftarTransaksi[l + 1];
                    }
                    jumlahTransaksi--;
                    for (int l = k; l < jumlahTransaksi; l++)
                    {
                        daftarTransaksi[l].id = l + 1;
                    }
                }
                else
                {
                    k++;
                }
            }
            saveTransaksi(jumlahTransaksi);
            system("cls || clear");
            cout << "Akun berhasil dihapus." << endl;
            found = true;
            return;
        }
    }
    if (!found)
    {
        system("cls || clear");
        cout << "Akun tidak ditemukan..." << endl;
    }
}

void konfirmasiAplikasi(int &jumlahAplikasi)
{
    int appId;
    getInputint("Masukkan ID aplikasi yang ingin dikonfirmasi (Ketik 0 untuk keluar): ", &appId, "2");
    if (appId == 0)
    {
        system("cls || clear");
        cout << "Keluar dari menu konfirmasi aplikasi." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        if (Aplikasi[i].id == appId && Aplikasi[i].status == 0)
        {
            cout << "Success == 1 || Failed == 2" << endl;
            int pilihan;
            getInputint("Konfirmasi Aplikasi : ", &pilihan, "1");
            if (pilihan == 1)
            {
            Aplikasi[i].status = 1;
            }else if (pilihan == 2)
            {
                Aplikasi[i].status = 2;
            }else
            {
                cout << "Pilihan tidak ditemukan" << endl;
                return;
            }
            saveAplikasi(jumlahAplikasi);
            system("cls || clear");
            (pilihan == 1 || pilihan == 2) ? cout << "Aplikasi berhasil dikonfirmasi.\n" : cout << "Aplikasi di tolak\n";
            found = true;
            return;
        }
    }
    if (!found)
    {
        system("cls || clear");
        cout << "Aplikasi tidak ditemukan atau sudah dikonfirmasi sebelumnya..." << endl;
    }
}

void menuAdmin(int &jumlahAkun, int &jumlahAplikasi, int &jumlahReview, int &jumlahTransaksi)
{
    int pilihan, index, pilihanOption;
    string searchUsername;
    int idx;
    do
    {
        cout << "===========================================\n";
        cout << "               Menu Admin" << endl;
        cout << "===========================================\n";
        cout << "       1. Konfirmasi Akun Pengguna" << endl;
        cout << "       2. Konfirmasi Aplikasi Penjual" << endl;
        cout << "       3. Lihat Pengguna" << endl;
        cout << "       4. Lihat Aplikasi Penjual" << endl;
        cout << "       5. Hapus Akun Pengguna" << endl;
        cout << "       6. Searching Akun Pengguna" << endl;
        cout << "       7. Sorting Akun Pengguna" << endl;
        cout << "       8. Logout" << endl;
        cout << "===========================================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            lihatRequestAkun(jumlahAkun);
            konfirmasiAkun(jumlahAkun);
            break;
        case 2:
            cout << "=====================================\n";
            cout << "     Konfirmasi Aplikasi Penjual" << endl;
            cout << "=====================================\n";
            lihatAplikasi(jumlahAplikasi, 2, "admin", "unconfirm", jumlahReview);
            konfirmasiAplikasi(jumlahAplikasi);
            break;
        case 3:
            lihatPengguna(jumlahAkun);
            enter(1);
            break;
        case 4:
            lihatAplikasi(jumlahAplikasi, 2, "admin", "all", jumlahReview);
            enter(1);
            break;
        case 5:
            lihatPengguna(jumlahAkun);
            hapusAkun(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
            break;
        case 6:
            cout << "========================================\n";
            cout << "          Searching Pengguna" << endl;
            cout << "========================================\n";
            sortPengguna("name", "asc", "lihat", jumlahAkun);
            cin.ignore();
            getlineInput("Masukkan nama Pengguna yang ingin dicari (Ketik 0 untuk keluar): ", &searchUsername);
            idx = binarySearchUsername(jumlahAkun, searchUsername);
            if (idx == 0)
            {
                system("cls || clear");
                cout << "Keluar dari menu searching akun." << endl;
                break;
            }
            else if (idx != -1)
            {
                system("cls || clear");
                cout << "Hasil Pencarian: \n\n";
                coutPengguna(Akun[idx].id, Akun[idx].username, Akun[idx].status, Akun[idx].role);
                if (!Akun[idx].status)
                {
                    cout << "Konfirmasi Akun == 1 || Hapus Akun == 2 || Keluar == 3" << endl;
                    getInputint("Pilihan: ", &pilihanOption, "1");
                    switch (pilihanOption)
                    {
                    case 1:
                        konfirmasiAkun(jumlahAkun);
                        break;
                    case 2:
                        sortPengguna("id", "asc", "hapus", jumlahAkun);
                        hapusAkun(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
                        system("cls || clear");
                        cin.ignore();
                        break;
                    case 3:
                        system("cls || clear");
                        cout << "Keluar dari menu searching akun Pengguna" << endl;
                        break;
                    default:
                        cout << "Pilihan tidak ditemukan" << endl;
                        break;
                    }
                }
                else
                {
                    cout << "Hapus Akun == 1 || Keluar == 2" << endl;
                    getInputint("Pilihan: ", &pilihanOption, "1");
                    switch (pilihanOption)
                    {
                    case 1:
                        sortPengguna("id", "asc", "hapus", jumlahAkun);
                        hapusAkun(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
                        cin.ignore();
                        break;
                    case 2:
                        system("cls || clear");
                        cout << "Keluar dari menu searching akun Pengguna" << endl;
                        break;
                    default:
                        cout << "Pilihan tidak ditemukan" << endl;
                        break;
                    }
                }
            }
            else
            {
                system("cls || clear");
                cout << "Username tidak ditemukan!" << endl;
            }
            break;
        case 7:
            menuSortingPengguna(index, jumlahAkun);
            break;
        case 8:
            cout << "Logout" << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan" << endl;
            break;
        }
    } while (pilihan != 8);
}

void tambahAplikasi(int &jumlahAplikasi, int index)
{
    cout << "=====================================\n";
    cout << "       Tambah Aplikasi" << endl;
    cout << "=====================================\n";
    string nama;
    double harga;
    cin.ignore();
    getlineInput("Nama Aplikasi : ", &nama);
    cout << "Harga Aplikasi : ";
    cin >> harga;
    if (cin.fail() || harga <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        system("cls || clear");
        cout << "Input tidak valid!" << endl;
        return;
    }
    getInputint("Input ID Unik Untuk Aplikasi : ", &Aplikasi[jumlahAplikasi].id, "1");
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        if (Aplikasi[i].id == Aplikasi[jumlahAplikasi].id)
        {
            system("cls || clear");
            cout << "ID sudah digunakan, silahkan input ulang" << endl;
            tambahAplikasi(jumlahAplikasi, index);
            return;
        }
    }
    Aplikasi[jumlahAplikasi].name = nama;
    Aplikasi[jumlahAplikasi].harga = harga;
    Aplikasi[jumlahAplikasi].seller = Akun[index].username;
    Aplikasi[jumlahAplikasi].status = 0;
    jumlahAplikasi++;
    saveAplikasi(jumlahAplikasi);
    system("cls || clear");
    cout << "Aplikasi berhasil ditambahkan! Menunggu Konfirmasi" << endl;
}

void coutAplikasi(int id, string name, double harga, string seller, int status, string role)
{
    cout << "ID : " << id << endl;
    cout << "Nama Aplikasi : " << name << endl;
    cout << "Harga : " << harga << endl; 
    cout << "Seller : " << seller << endl;
    if (role == "penjual")
    {
        if(status == 0){
            cout << "Status : Pending " << endl;
        }
        else if (status == 1)
        {
            cout << "Status : Success" << endl;
        }
        else
        {
            cout << "Status : Failed" << endl;
        }
    }cout << "________________________________________\n\n";
}

void lihatAplikasi(int &jumlahAplikasi, int index, string role, string status, int &jumlahReview)
{
    cout << "=====================================\n";
    cout << "            List Aplikasi" << endl;
    cout << "=====================================\n";
    bool found = false;
    if (role == "admin" && status == "unconfirm")
    {
        for (int i = 0; i < jumlahAplikasi; i++)
        {
            if (Aplikasi[i].status == 0)
            {
                coutAplikasi(Aplikasi[i].id, Aplikasi[i].name, Aplikasi[i].harga, Aplikasi[i].seller, Aplikasi[i].status, "penjual");
                found = true;
            }
        }
    }
    else if (role == "admin" && status == "all")
    {
        for (int i = 0; i < jumlahAplikasi; i++)
        {
            coutAplikasi(Aplikasi[i].id, Aplikasi[i].name, Aplikasi[i].harga, Aplikasi[i].seller, Aplikasi[i].status, "penjual");
            found = true;
        }
    }
    else if (role == "pembeli" && status == "confirm")
    {
        for (int i = 0; i < jumlahAplikasi; i++)
        {
            if (Aplikasi[i].status == 1)
            {
                coutAplikasi(Aplikasi[i].id, Aplikasi[i].name, Aplikasi[i].harga, Aplikasi[i].seller, Aplikasi[i].status, "pembeli");
                found = true;
                bool foundReview = false;
                cout << "========================\n";
                cout << "        Review" << endl;
                cout << "========================\n";
                for (int j = 0; j < jumlahReview; j++)
                {
                    if (daftarReview[j].idAplikasi == Aplikasi[i].id)
                    {
                        cout << "Pembeli : " << daftarReview[j].usernamePembeli << endl;
                        cout << "Rating : " << daftarReview[j].rating << endl;
                        cout << "Review : " << daftarReview[j].review << endl;
                        cout << "========================\n\n";
                        foundReview = true;
                    }
                }
                if (!foundReview)
                {
                    cout << "Belum ada review untuk aplikasi ini... " << endl;
                }
                cout << "________________________________________\n\n";
            }
        }
    }
    else
    {
        for (int i = 0; i < jumlahAplikasi; i++)
        {
            if (Aplikasi[i].seller == Akun[index].username)
            {
                coutAplikasi(Aplikasi[i].id, Aplikasi[i].name, Aplikasi[i].harga, Aplikasi[i].seller, Aplikasi[i].status, "penjual");
                found = true;
            }
        }
    }
    if (!found)
    {
        cout << "Belum ada aplikasi..." << endl;
    }
}

void editAplikasi(int &jumlahAplikasi, int index)
{
    int appId;
    getInputint("Masukkan ID aplikasi untuk diedit (Ketik 0 untuk keluar): ", &appId, "2");
    if (appId == 0)
    {
        system("cls || clear");
        cout << "Keluar dari menu edit aplikasi" << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        if (Aplikasi[i].id == appId && Aplikasi[i].seller == Akun[index].username)
        {
            string newName;
            double newPrice;
            cin.ignore();
            getlineInput("Masukkan nama baru untuk aplikasi: ", &newName);
            cout << "Masukkan harga baru untuk aplikasi: ";
            cin >> newPrice;
            if (cin.fail() || newPrice < 0)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                system("cls || clear");
                cout << "Input tidak valid!" << endl;
                return;
            }
            Aplikasi[i].name = newName;
            Aplikasi[i].harga = newPrice;
            saveAplikasi(jumlahAplikasi);
            cout << "Aplikasi berhasil diedit!" << endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Aplikasi tidak ditemukan atau Anda tidak memiliki hak akses untuk mengedit aplikasi tersebut!" << endl;
    }
}

void hapusAplikasi(int &jumlahAplikasi, int index)
{
    int appId;
    getInputint("Masukkan ID aplikasi untuk dihapus (Ketik 0 untuk keluar): ", &appId, "2");
    if (appId == 0)
    {
        cout << "Keluar dari menu hapus aplikasi" << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        if (Aplikasi[i].id == appId && Aplikasi[i].seller == Akun[index].username)
        {
            for (int j = i; j < jumlahAplikasi - 1; j++)
            {
                Aplikasi[j] = Aplikasi[j + 1];
            }
            jumlahAplikasi--;
            saveAplikasi(jumlahAplikasi);
            cout << "Aplikasi berhasil dihapus!" << endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Aplikasi tidak ditemukan atau Anda tidak memiliki hak akses untuk menghapus aplikasi tersebut!" << endl;
    }
}

void konfirmasiPenjualan(int &jumlahAplikasi, int index, int &jumlahTransaksi, int &jumlahAkun)
{
    int idApp;
    getInputint("Masukkan ID transaksi yang ingin dikonfirmasi (Ketik 0 untuk keluar): ", &idApp, "2");
    if (idApp == 0)
    {
        cout << "Keluar dari menu konfirmasi penjualan" << endl;
        return;
    }
    bool found = false;
    int status;
    for (int i = 0; i < jumlahTransaksi; i++)
    {
        if (daftarTransaksi[i].id == idApp && daftarTransaksi[i].aplikasi.seller == Akun[index].username && daftarTransaksi[i].status == 0)
        {
            found = true;
            getInputint("Masukkan status transaksi (1 = success, 2 = failed): ", &status, "1");
            while (status < 1 || status > 2)
            {
                cout << "Status transaksi tidak valid!" << endl;
                getInputint("Masukkan status transaksi (1 = success, 2 = failed): ", &status, "1");
            }
            if (status == 1)
            {
                Akun[index].saldo += daftarTransaksi[i].aplikasi.harga;
                daftarTransaksi[i].status = 1;
                cout << "Transaksi berhasil!" << endl;
            }
            else
            {
                daftarTransaksi[i].status = 2;
                for (int j = 0; j < jumlahAkun; j++)
                {
                    if (daftarTransaksi[i].usernamePembeli == Akun[j].username)
                    {
                        Akun[j].saldo += daftarTransaksi[i].aplikasi.harga;
                        break;
                    }
                }
                cout << "Transaksi gagal!" << endl;
            }
        }
    }
    saveTransaksi(jumlahTransaksi);
    saveAkun(jumlahAkun);
    if (!found)
    {
        cout << "ID transaksi tidak ditemukan atau sudah dikonfirmasi sebelumnya!" << endl;
    }
}

void daftarTransaksiAplikasi(int &jumlahTransaksi, int index, string role)
{
    cout << "=====================================\n";
    cout << "      Daftar Transaksi Aplikasi" << endl;
    cout << "=====================================\n";
    bool found = false;
    for (int i = 0; i < jumlahTransaksi; i++)
    {
        if (daftarTransaksi[i].aplikasi.seller == Akun[index].username && daftarTransaksi[i].status == 0 && role == "penjual")
        {
            cout << "ID Transaksi : " << daftarTransaksi[i].id << endl;
            cout << "Nama Pembeli : " << daftarTransaksi[i].usernamePembeli << endl;
            cout << "Nama Aplikasi : " << daftarTransaksi[i].aplikasi.name << endl;
            cout << "Status : " << "Pending" << endl;
            cout << "________________________________________\n\n";
            found = true;
        }
        else if (daftarTransaksi[i].usernamePembeli == Akun[index].username && role == "pembeli")
        {
            cout << "ID Transaksi : " << daftarTransaksi[i].id << endl;
            cout << "Nama Penjual : " << daftarTransaksi[i].aplikasi.seller << endl;
            cout << "Nama Aplikasi : " << daftarTransaksi[i].aplikasi.name << endl;
            cout << "Status : ";
            if (daftarTransaksi[i].status == 0)
            {
                cout << "Pending" << endl;
            }
            else if (daftarTransaksi[i].status == 1)
            {
                cout << "Success" << endl;
            }
            else
            {
                cout << "Failed" << endl;
            }
            cout << "________________________________________\n\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "Belum ada transaksi..." << endl;
    }
}

void menuPenjual(int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi, int &jumlahAkun)
{
    int pilihan, idx, pilihanOption;
    string searchApp;
    bool found = false;

    do
    {
        cout << "=======================================\n";
        cout << "           Menu Penjual" << endl;
        cout << "=======================================\n";
        cout << "       1. Lihat Aplikasi" << endl;
        cout << "       2. Tambah Aplikasi" << endl;
        cout << "       3. Edit Aplikasi" << endl;
        cout << "       4. Hapus Aplikasi" << endl;
        cout << "       5. Konfirmasi Penjualan" << endl;
        cout << "       6. Sorting Aplikasi" << endl;
        cout << "       7. Searching Aplikasi" << endl;
        cout << "       8. Logout" << endl;
        cout << "=======================================\n";
        cout << "Total Saldo : $" << Akun[index].saldo << endl;
        cout << "=======================================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
            enter(1);
            break;
        case 2:
            tambahAplikasi(jumlahAplikasi, index);
            break;
        case 3:
            cout << "=====================================\n";
            cout << "         Edit Aplikasi" << endl;
            cout << "=====================================\n";
            lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
            editAplikasi(jumlahAplikasi, index);
            break;
        case 4:
            cout << "=====================================\n";
            cout << "         Hapus Aplikasi" << endl;
            cout << "=====================================\n";
            lihatAplikasi(jumlahAplikasi, index, "penjual", "all", jumlahReview);
            hapusAplikasi(jumlahAplikasi, index);
            enter(1);
            break;
        case 5:
            cout << "=====================================\n";
            cout << "   Konfirmasi Penjualan Aplikasi" << endl;
            cout << "=====================================\n";
            daftarTransaksiAplikasi(jumlahTransaksi, index, "penjual");
            konfirmasiPenjualan(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
            enter(1);
            break;
        case 6:
            menuSortingAplikasi(jumlahAplikasi, jumlahReview, index, "penjual", jumlahTransaksi, jumlahAkun);
            break;
        case 7:
            cout << "=======================================\n";
            cout << "       Searching Aplikasi" << endl;
            cout << "=======================================\n";
            sortAplikasi("all", jumlahAplikasi, "name", "asc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            cin.ignore();
            getlineInput("Masukkan nama aplikasi yang ingin dicari (Ketik 0 untuk keluar): ", &searchApp);
            idx = binarySearchApp(Aplikasi, jumlahAplikasi, searchApp);
            if (idx == 0)
            {
                system("cls || clear");
                cout << "Keluar dari menu searching aplikasi." << endl;
                break;
            }
            else if (idx != -1)
            {
                if (Aplikasi[idx].seller == Akun[index].username)
                {
                    coutAplikasi(Aplikasi[idx].id, Aplikasi[idx].name, Aplikasi[idx].harga, Aplikasi[idx].seller, Aplikasi[idx].status, "penjual");
                    cout << "Edit == 1 || Hapus == 2 || Keluar == 3" << endl;
                    getInputint("Pilihan: ", &pilihanOption, "1");
                    switch (pilihanOption)
                    {
                    case 1:
                        editAplikasi(jumlahAplikasi, index);
                        break;
                    case 2:
                        hapusAplikasi(jumlahAplikasi, index);
                        break;
                    case 3:
                        cout << "Keluar dari menu searching aplikasi" << endl;
                        break;
                    default:
                        cout << "Pilihan tidak ditemukan" << endl;
                        break;
                    }
                }
                else
                {
                    cout << "Aplikasi tidak ditemukan!" << endl;
                }
            }
            else
            {
                cout << "Aplikasi tidak ditemukan!" << endl;
            }
            enter(1);
            break;
        case 8:
            cout << "Logout" << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan" << endl;
            break;
        }
    } while (pilihan != 8);
}

void isiSaldo(int &jumlahAkun, int index)
{
    cout << "===================================\n";
    cout << "       Halaman Isi Saldo" << endl;
    cout << "===================================\n";
    double saldo;
    cout << "Masukkan jumlah saldo yang ingin diisi (Ketik 0 untuk keluar): $";
    cin >> saldo;
    if (cin.fail() || saldo < 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input tidak valid!" << endl;
        return;
    }
    if (saldo == 0)
    {
        cout << "Keluar" << endl;
        return;
    }
    Akun[index].saldo += saldo;
    saveAkun(jumlahAkun);
    cout << "Saldo berhasil diisi!" << endl;
}

void beliAplikasi(int &jumlahAplikasi, int index, int &jumlahTransaksi, int &jumlahAkun)
{
    int appId;
    if (jumlahAplikasi == 0)
    {
        return;
    }
    getInputint("Masukkan ID aplikasi yang ingin dibeli (Ketik 0 untuk keluar): ", &appId, "2");
    if (appId == 0)
    {
        system("cls || clear");
        cout << "Keluar dari menu beli aplikasi." << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahAplikasi; i++)
    {
        if (Aplikasi[i].id == appId && Aplikasi[i].status == 1)
        {
            if (Akun[index].saldo >= Aplikasi[i].harga)
            {
                Akun[index].saldo -= Aplikasi[i].harga;
                daftarTransaksi[jumlahTransaksi].id = (jumlahTransaksi == 0) ? 1 : daftarTransaksi[jumlahTransaksi - 1].id + 1;
                daftarTransaksi[jumlahTransaksi].aplikasi.id = Aplikasi[i].id;
                daftarTransaksi[jumlahTransaksi].aplikasi.name = Aplikasi[i].name;
                daftarTransaksi[jumlahTransaksi].aplikasi.harga = Aplikasi[i].harga;
                daftarTransaksi[jumlahTransaksi].aplikasi.seller = Aplikasi[i].seller;
                daftarTransaksi[jumlahTransaksi].usernamePembeli = Akun[index].username;
                daftarTransaksi[jumlahTransaksi].status = 0;
                jumlahTransaksi++;
                saveTransaksi(jumlahTransaksi);
                saveAkun(jumlahAkun);
                system("cls || clear");
                cout << "Transaksi berhasil!" << endl;
            }
            else
            {
                system("cls || clear");
                cout << "Saldo tidak mencukupi!" << endl;
            }
            found = true;
            break;
        }
    }
    if (!found)
    {
        system("cls || clear");
        cout << "Aplikasi tidak ditemukan atau belum dikonfirmasi!" << endl;
    }
}

void tambahReview(int &jumlahReview, int &jumlahTransaksi, int index, int &jumlahAplikasi)
{
    int idApp;
    getInputint("Masukkan ID aplikasi yang ingin direview (Ketik 0 untuk keluar): ", &idApp, "2");
    if (idApp == 0)
    {
        cout << "Keluar dari menu review aplikasi" << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < jumlahTransaksi; i++)
    {
        if (daftarTransaksi[i].id == idApp && daftarTransaksi[i].status == 1 && daftarTransaksi[i].usernamePembeli == Akun[index].username)
        {
            for (int j = 0; j < jumlahAplikasi; j++)
            {
                if (Aplikasi[j].id == daftarTransaksi[i].aplikasi.id && Aplikasi[j].name == daftarTransaksi[i].aplikasi.name)
                {
                    string review;
                    int rating;
                    cin.ignore();
                    getlineInput("Masukkan review: ", &review);
                    getInputint("Masukkan rating (1-5): ", &rating, "1");
                    while (rating < 1 || rating > 5)
                    {
                        cout << "Rating tidak valid!" << endl;
                        getInputint("Masukkan rating (1-5): ", &rating, "1");
                        system("cls || clear");
                    }
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
            }
        }
    }
    if (!found)
    {
        cout << "Gagal! Aplikasi tidak ditemukan atau status belum disetujui" << endl;
    }
}

void sortAplikasi(string role, int &jumlahAplikasi, string sortBy, string mode, int &jumlahReview, int index, int &jumlahTransaksi, int &jumlahAkun)
{
    if (jumlahAplikasi == 0)
    {
        return;
    }
    if (sortBy == "name" && mode == "asc")
    {
        for (int i = 0; i < jumlahAplikasi - 1; i++)
        {
            for (int j = 0; j < jumlahAplikasi - i - 1; j++)
            {
                if (Aplikasi[j].name > Aplikasi[j + 1].name)
                {
                    swap(Aplikasi[j], Aplikasi[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "name" && mode == "desc")
    {
        for (int i = 0; i < jumlahAplikasi - 1; i++)
        {
            for (int j = 0; j < jumlahAplikasi - i - 1; j++)
            {
                if (Aplikasi[j].name < Aplikasi[j + 1].name)
                {
                    swap(Aplikasi[j], Aplikasi[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "price" && mode == "asc")
    {
        for (int i = 0; i < jumlahAplikasi - 1; i++)
        {
            for (int j = 0; j < jumlahAplikasi - i - 1; j++)
            {
                if (Aplikasi[j].harga > Aplikasi[j + 1].harga)
                {
                    swap(Aplikasi[j], Aplikasi[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "price" && mode == "desc")
    {
        for (int i = 0; i < jumlahAplikasi - 1; i++)
        {
            for (int j = 0; j < jumlahAplikasi - i - 1; j++)
            {
                if (Aplikasi[j].harga < Aplikasi[j + 1].harga)
                {
                    swap(Aplikasi[j], Aplikasi[j + 1]);
                }
            }
        }
    }

    if (role == "pembeli")
    {
        lihatAplikasi(jumlahAplikasi, index, role, "confirm", jumlahReview);
        beliAplikasi(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
    }
    else if (role == "penjual")
    {
        lihatAplikasi(jumlahAplikasi, index, role, "all", jumlahReview);
        int pilihan;
        cout << "Edit == 1 || Hapus == 2 || Keluar == 3" << endl;
        getInputint("Pilihan : ", &pilihan, "1");
        switch (pilihan)
        {
        case 1:
            editAplikasi(jumlahAplikasi, index);
            break;
        case 2:
            hapusAplikasi(jumlahAplikasi, index);
            break;
        case 3:
            system("cls || clear");
            cout << "Kembali ke menu sorting." << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan." << endl;
            enter(1);
            break;
        }
    }
}

int binarySearchApp(Item data[], int jumlahData, string cari)
{
    if (cari == "0")
    {
        return 0;
    }
    int kiri = 0, kanan = jumlahData - 1;
    while (kiri <= kanan)
    {
        int tengah = (kiri + kanan) / 2;
        if (data[tengah].name == cari)
        {
            return tengah;
            break;
        }
        else if (data[tengah].name < cari)
        {
            kiri = tengah + 1;
        }
        else
        {
            kanan = tengah - 1;
        }
    }
    return -1;
}

void sortPengguna(string sortBy, string mode, string option, int &jumlahAkun)
{
    if (jumlahAkun == 0)
    {
        return;
    }
    if (sortBy == "name" && mode == "asc")
    {
        for (int i = 0; i < jumlahAkun - 1; i++)
        {
            for (int j = 0; j < jumlahAkun - i - 1; j++)
            {
                if (Akun[j].username > Akun[j + 1].username)
                {
                    swap(Akun[j], Akun[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "name" && mode == "dsc")
    {
        for (int i = 0; i < jumlahAkun - 1; i++)
        {
            for (int j = 0; j < jumlahAkun - i - 1; j++)
            {
                if (Akun[j].username < Akun[j + 1].username)
                {
                    swap(Akun[j], Akun[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "id" && mode == "asc")
    {
        for (int i = 0; i < jumlahAkun - 1; i++)
        {
            for (int j = 0; j < jumlahAkun - i - 1; j++)
            {
                if (Akun[j].id > Akun[j + 1].id)
                {
                    swap(Akun[j], Akun[j + 1]);
                }
            }
        }
    }
    else if (sortBy == "id" && mode == "dsc")
    {
        for (int i = 0; i < jumlahAkun - 1; i++)
        {
            for (int j = 0; j < jumlahAkun - i - 1; j++)
            {
                if (Akun[j].id < Akun[j + 1].id)
                {
                    swap(Akun[j], Akun[j + 1]);
                }
            }
        }
    }
    if (option != "hapus")
    {
        lihatPengguna(jumlahAkun);
    }
}

int binarySearchUsername(int jumlahData, string cari)
{
    if (cari == "0")
    {
        return 0;
    }
    int kiri = 0, kanan = jumlahData - 1;
    while (kiri <= kanan)
    {
        int tengah = (kiri + kanan) / 2;
        if (Akun[tengah].username == cari)
        {
            return tengah;
            break;
        }
        else if (Akun[tengah].username < cari)
        {
            kiri = tengah + 1;
        }
        else
        {
            kanan = tengah - 1;
        }
    }
    return -1;
}

void menuSortingAplikasi(int &jumlahAplikasi, int jumlahReview, int index, string role, int &jumlahTransaksi, int &jumlahAkun)
{
    int pilihan;
    do
    {
        cout << "=====================================\n";
        cout << "         Sorting Aplikasi" << endl;
        cout << "=====================================\n";
        cout << "       1. Sort by Name (A-Z)" << endl;
        cout << "       2. Sort by Price (Murah - Mahal)" << endl;
        cout << "       3. Sort by Name (Z-A)" << endl;
        cout << "       4. Sort by Price (Mahal - Murah)" << endl;
        cout << "       5. Back" << endl;
        cout << "===================================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            cout << "=====================================\n";
            cout << "          Sort by Name (A-Z)" << endl;
            cout << "=====================================\n";
            sortAplikasi(role, jumlahAplikasi, "name", "asc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            break;
        case 2:
            cout << "=====================================\n";
            cout << "    Sort by Price (Murah - Mahal)" << endl;
            cout << "=====================================\n";
            sortAplikasi(role, jumlahAplikasi, "price", "asc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            break;
        case 3:
            cout << "=====================================\n";
            cout << "           Sort by Name (Z-A)" << endl;
            cout << "=====================================\n";
            sortAplikasi(role, jumlahAplikasi, "name", "desc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            break;
        case 4:
            cout << "=====================================\n";
            cout << "    Sort by Price (Mahal - Murah)" << endl;
            cout << "=====================================\n";
            sortAplikasi(role, jumlahAplikasi, "price", "desc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            break;
        case 5:
            cout << "Keluar dari menu sorting aplikasi." << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan." << endl;
            break;
        }
    } while (pilihan != 5);
}

void menuSortingPengguna(int index, int &jumlahAkun)
{
    int pilihan;
    do
    {
        cout << "===================================\n";
        cout << "         Sorting Aplikasi" << endl;
        cout << "===================================\n";
        cout << "       1. Sort by Name (A-Z)" << endl;
        cout << "       2. Sort by ID (Ascending)" << endl;
        cout << "       3. Sort by Name (Z-A)" << endl;
        cout << "       4. Sort by ID (Descending)" << endl;
        cout << "       5. Back" << endl;
        cout << "===================================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            cout << "=======================================\n";
            cout << "          Sort by Name (A-Z)" << endl;
            cout << "=======================================\n";
            sortPengguna("name", "asc", "lihat", jumlahAkun);
            enter(1);
            break;
        case 2:
            cout << "=======================================\n";
            cout << "        Sort by ID (Ascending)" << endl;
            cout << "=======================================\n";
            sortPengguna("id", "asc", "lihat", jumlahAkun);
            enter(1);
            break;
        case 3:
            cout << "=======================================\n";
            cout << "          Sort by Name (Z-A)" << endl;
            cout << "=======================================\n";
            sortPengguna("name", "dsc", "lihat", jumlahAkun);
            enter(1);
            break;
        case 4:
            cout << "=======================================\n";
            cout << "       Sort by ID (Descending)" << endl;
            cout << "=======================================\n";
            sortPengguna("id", "dsc", "lihat", jumlahAkun);
            enter(1);
            break;
        case 5:
            system("cls || clear");
            cout << "Keluar dari menu sorting akun." << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan" << endl;
            break;
        }
    } while (pilihan != 5);
}

void menuPembeli(int &jumlahAkun, int &jumlahAplikasi, int index, int &jumlahReview, int &jumlahTransaksi)
{
    int pilihan, idx, pilihanOption;
    string searchApp;

    do
    {
        cout << "===================================\n";
        cout << "           Menu Pembeli" << endl;
        cout << "===================================\n";
        cout << "       1. Lihat Aplikasi" << endl;
        cout << "       2. Isi Saldo" << endl;
        cout << "       3. Beli Aplikasi" << endl;
        cout << "       4. Riwayat Transaksi" << endl;
        cout << "       5. Review Aplikasi " << endl;
        cout << "       6. Sorting Aplikasi " << endl;
        cout << "       7. Searching Aplikasi " << endl;
        cout << "       8. Logout" << endl;
        cout << "===================================\n";
        cout << "Total Saldo : $" << Akun[index].saldo << endl;
        cout << "===================================\n";
        getInputint("Pilihan : ", &pilihan, "1");
        system("cls || clear");
        switch (pilihan)
        {
        case 1:
            lihatAplikasi(jumlahAplikasi, index, "pembeli", "confirm", jumlahReview);
            enter(1);
            break;
        case 2:
            isiSaldo(jumlahAkun, index);
            enter();
            break;
        case 3:
            cout << "=====================================\n";
            cout << "         Beli Aplikasi" << endl;
            cout << "=====================================\n";
            lihatAplikasi(jumlahAplikasi, index, "pembeli", "confirm", jumlahReview);
            beliAplikasi(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
            enter(1);
            break;
        case 4:
            daftarTransaksiAplikasi(jumlahTransaksi, index, "pembeli");
            enter(1);
            break;
        case 5:
            cout << "=====================================\n";
            cout << "         Review Aplikasi" << endl;
            cout << "=====================================\n";
            daftarTransaksiAplikasi(jumlahTransaksi, index, "pembeli");
            tambahReview(jumlahReview, jumlahTransaksi, index, jumlahAplikasi);
            enter(1);
            break;
        case 6:
            menuSortingAplikasi(jumlahAplikasi, jumlahReview, index, "pembeli", jumlahTransaksi, jumlahAkun);
            break;
        case 7:
            cout << "=======================================\n";
            cout << "       Searching Aplikasi" << endl;
            cout << "=======================================\n";
            sortAplikasi("all", jumlahAplikasi, "name", "asc", jumlahReview, index, jumlahTransaksi, jumlahAkun);
            cin.ignore();
            getlineInput("Masukkan nama aplikasi yang ingin dicari : ", &searchApp);
            idx = binarySearchApp(Aplikasi, jumlahAplikasi, searchApp);
            if (idx != -1)
            {
                coutAplikasi(Aplikasi[idx].id, Aplikasi[idx].name, Aplikasi[idx].harga, Aplikasi[idx].seller, Aplikasi[idx].status, "penjual");
                cout << "Beli == 1 || Keluar == 2" << endl;
                getInputint("Pilihan: ", &pilihanOption, "1");
                switch (pilihanOption)
                {
                case 1:
                    beliAplikasi(jumlahAplikasi, index, jumlahTransaksi, jumlahAkun);
                    break;
                case 2:
                    cout << "Keluar dari menu searching aplikasi" << endl;
                    break;
                default:
                    cout << "Pilihan tidak ditemukan" << endl;
                    break;
                }
            }
            else
            {
                cout << "Aplikasi tidak ditemukan!" << endl;
            }
            enter(1);
            break;
        case 8:
            cout << "Logout" << endl;
            break;
        default:
            cout << "Pilihan tidak ditemukan" << endl;
            break;
        }
    } while (pilihan != 8);
}

int main()
{
    int jumlahAkun = bacaAkun();
    int jumlahAplikasi = bacaAplikasi();
    int jumlahReview = bacaReview();
    int jumlahTransaksi = bacaTransaksi();
    displayMainMenu(jumlahAkun, jumlahAplikasi, jumlahReview, jumlahTransaksi);
    return 0;
}
