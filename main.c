#include <stdio.h>
#include <locale.h> //Türkçe karakterler için kütüphane
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h> // SLEEP
#include <ctype.h> // lowerCase

//global değişkenler
char globalUser[50];
int globalIsAdmin;

// definelar
#define KONSOLTEMIZLE printf("\e[1;1H\e[2J");

// fonksiyonlar
int girisMenu();
void gecersizSecim();
void girisEkrani();
void kayitEkrani();
void menu();
void clearInputBuffer();
int authenticateUser(char *username, char *password, int isAdmin);
void toLowerCase(char *str);
void rastgeleCizim();

//kitap fonksiyonları
void kitapAra();
void kitapEkle();
void kitapHepsi();
void kitapSil();
void kitapOduncAl();
void kitapOduncBirak();
void tumOduncKitaplar();

//structlar
struct User {
    char username[50];
    char password[50];
    int isAdmin; // Yönetici için 1, kullanıcı ise 0
};

struct Book {
    char title[256];
    char author[256];
    char genre[256];
    int pages;
};

int main() {
    globalIsAdmin = 3; // giriş yapmadan önce önemsiz bir değere atanır
    KONSOLTEMIZLE;
    //hoşgeldiniz (bir kere çalışır)
 
    setlocale(LC_ALL, "tr_TR.UTF-8"); //BU ÇALIŞIYOR!
    girisMenu();

}


// FONKSİYONLAR
void kitapEkle();

int girisMenu() {
    globalIsAdmin=5; //bug engelleyici
    KONSOLTEMIZLE;
    printf("═════════════════HOŞGELDİNİZ═════════════════\n");

    rastgeleCizim();

    printf("═════════════════════════════════════════════\n");
    int secim;
    printf("════════════════════════GİRİŞ MENÜSÜ════════════════════════\n");
    printf("1 - Kullanıcı Giriş\n2 - Kayıt Ol\n3 - Uygulamayı Kapat\n");
    printf("════════════════════════════════════════════════════════════\n");

    printf("Lütfen Seçiminizi Yapınız: ");
    scanf("%d", &secim);

    clearInputBuffer();

    switch (secim)
    {
    case 1:
    girisEkrani();
    break;
    case 2:
    kayitEkrani();
    break;
    case 3:
    exit(0);
    default:
    gecersizSecim();
        break;
    }


}

void toLowerCase(char *str) {
        while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

void gecersizSecim() 
{
    printf("═════════════════════════════════════════════\n");
    printf("GEÇERSİZ SEÇİM, iki saniye içinde ana menüye dönülüyor\n");
    printf("═════════════════════════════════════════════\n");
    sleep(2);
    KONSOLTEMIZLE;   //CLEAR SCREEN
    menu();
}

void islemTamamlandi()
 {
    printf("═════════════════════════════════════════════\n");
    printf("İşlem Tamamlandı\n");
    printf("Ana menüye dönmek için herhangi bir tuşa basınız.\n");
    printf("═════════════════════════════════════════════\n");
    clearInputBuffer();
    getchar(); // herhangi bir girdi al
    KONSOLTEMIZLE; // CLEAR SCREEN
    menu();
 }

void girisEkrani() {
    char username[50];
    char password[50];
    printf("Kullanıcı adını giriniz: ");
    scanf("%s", username);
    printf("Şifrenizi giriniz: ");
    scanf("%s", password);

    if (authenticateUser(username, password, 0) || (authenticateUser(username, password, 1))) {
        printf("Giriş Başarılı!\n");
        //globalUser değişkenini username olarak ata
        strcpy(globalUser, username);

        printf("DEBUG -- %s, %d\n", globalUser, globalIsAdmin);
        menu();


    } else {
        printf("Giriş başarısız, geçersiz bilgiler.\n");
        girisMenu();
    }

}

void kayitEkrani() {
    struct User newUser;
    int kayit1;
    int teyit;

    printf("Kullanıcı adını giriniz: ");
    scanf("%s", newUser.username);

    printf("Şifrenizi giriniz: ");
    scanf("%s", newUser.password);

    printf("1 - Kullanıcı Olarak Kaydet\n2 - Yönetici Olarak Kaydet\n");
    scanf("%d",&kayit1);
    
    switch (kayit1) {
    case 1:
    newUser.isAdmin = 0;
    printf("Kullanıcı Olarak Kaydedildi, ");
    goto kayit;
    case 2:
    clearInputBuffer();
    printf("Yönetici teyit şifresini giriniz: ");
    scanf("%d", &teyit);
    if (teyit==666) {
        newUser.isAdmin=1;
        printf("Yönetici olarak kaydedildi, ");
        goto kayit;
        }
    else {
        printf("Teyit şifresi yanlış, giriş ekranına dönülüyor.\n");
        girisMenu();
        } 
    default:
    printf("Geçersiz seçim, giriş ekranına dönülüyor.\n");
    girisMenu();
    }
kayit:
    // kullanıcı veri dosyasını ekleme modunda aç
    FILE *file = fopen("data/users/users.txt", "a");
    if (file == NULL) {
        printf("Dosya açılamadı!\n");
        exit(1);
    }

    // yeni kullanıcıyı kaydet
    fprintf(file, "%s %s %d\n", newUser.username, newUser.password, newUser.isAdmin);

    // dosyayı kapat
    fclose(file);

    printf("Kayıt tamamlandı, giriş ekranına dönülüyor.\n");

    girisMenu();

    

}

int authenticateUser(char *username, char *password, int isAdmin) {
    struct User user;
    FILE *file = fopen("data/users/users.txt", "r");

    // Dosyayı aç
    if (file == NULL) {
        printf("Dosya açılamadı!\n");
        exit(1);
    }

    // Her diziyi ara
    while (fscanf(file, "%s %s %d", user.username, user.password, &user.isAdmin) == 3) {
        // struct bilgileri doğru mu kontrol et
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0 && user.isAdmin == isAdmin) {
            globalIsAdmin = user.isAdmin;          


            fclose(file);

            return 1; // yetkilendirme tamamlandı
            
        }
    }
    fclose(file);
    return 0;//yetkilendirme başarısız
}

void menu() {
    KONSOLTEMIZLE;
    printf("═════════════════════════════════════════════\n");
    rastgeleCizim();

    printf("═════════════════════════════════════════════\n");
    printf("═════════════════════════════════════════════\n");
    printf("Hoşgeldiniz: %s\n",globalUser);
    int secenek;
    int odunc;
    if(globalIsAdmin == 1) {
        goto yonetici;
    }  if(globalIsAdmin == 0) { goto kullanici;}
    else {girisMenu();}


    yonetici:
        //ana menü tuşları
        printf("Ana Menü\n"); 
        printf("1. Kitap Ekle\n");
        printf("2. Kitap Ara\n");
        printf("3. Tüm Kitapları Göster\n");
        printf("4. Kitap Sil\n");
        printf("5. Çıkış Yap\n");
    printf("═════════════════════════════════════════════\n");    
        printf("Lütfen seçiminizi yapın: ");    
    
        scanf("%d", &secenek);
        clearInputBuffer();
        switch (secenek) {
            case 1:
            kitapEkle();
            break;
            case 2:
            kitapAra();
            break;
            case 3:
            kitapHepsi();
            break;
            case 4:
            kitapSil();
            break;
            case 5:
            girisMenu();
            break;
            default:
                gecersizSecim();
        }
    exit(0);
    kullanici:

        //ana menü tuşları
        printf("Ana Menü\n");
        printf("1. Tüm Kitapları Göster\n");
        printf("2. Kitap Ara\n");
        printf("3. Kitap Ödünç Al/Bırak\n");
        printf("4. Ödünç Aldığım Kitapları Görüntüle\n");
        printf("5. Çıkış Yap\n");
    printf("═════════════════════════════════════════════\n");
        scanf("%d", &secenek);

        switch (secenek) {
            case 1:
            kitapHepsi();
            break;
            case 2:
            kitapAra();
            break;
            case 3:
            int secenek2;
            printf("1- Kitap Ödünç Al\n");
            printf("2- Kitap Teslim Et\n");
            printf("3- Menüye Geri Dön\n");
            printf("Seçiminizi yapınız: ");
            scanf("%d",&secenek2);
            if (secenek2==1) { kitapOduncAl();}
            if (secenek2==2) {kitapOduncBirak();}
            else {menu();}
            break;
            case 4:
            tumOduncKitaplar();
            break;
            case 5:
            girisMenu();
            default:
                gecersizSecim();
        }
    exit(0);
}

void clearInputBuffer()
 {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// KİTAP FONKSİYONLARI
void kitapEkle()
{
    char kitap1[50]; // Kitap ismi
    char kitap2[50]; // Kitap yazarı
    char kitap3[30]; // Kitap türü
    int kitap4; // Sayfa sayısı
    printf("═════════════════════════════════════════════\n");
    printf("Seçilen: Kitap Ekle \n");



    printf("Kitap İsmini Giriniz: \n");
    fgets(kitap1, 50, stdin);

    printf("Kitap Yazarını Giriniz: \n");
    fgets(kitap2, 50, stdin);

    printf("Kitap Türünü Giriniz: \n");
    fgets(kitap3, 50, stdin);

    printf("Kitap Sayfa Sayısını Giriniz: \n");
    scanf("%d", &kitap4);


    clearInputBuffer();
    kitap1[strcspn(kitap1, "\n")] = 0; //bu kodlar alınan kullanıcı stringlerindeki \n satırını siler
    kitap2[strcspn(kitap2, "\n")] = 0;
    kitap3[strcspn(kitap3, "\n")] = 0;


    printf("Girdiğiniz Kitabın Bilgileri:\n");
    printf("1.İsim: %s\n", kitap1);
    printf("2.Yazar: %s\n", kitap2);
    printf("3.Tür: %s\n", kitap3);
    printf("4.Sayfa Sayısı: %d\n", kitap4);

    
 // bu kısım alınan kullanıcı değerlerini kitaplar.txt dosyasına her girişi yeni satırda kaydeder.
    FILE *file = fopen("data/kitaplar.txt", "a");

    if (file != NULL) {
        fprintf(file, "%s,%s,%s,%d\n",kitap1,kitap2,kitap3,kitap4);
        fclose(file);
        printf("Kitap bilgileri başarıyla kaydedildi.\n");
    } else {
        printf("DİKKAT, DOSYA AÇILAMADI!\n");
        gecersizSecim();

       
    }

    islemTamamlandi();
}

void kitapAra()
{
    char searchInput[50];
    int lineCount=1;

    char str1[50];
    char str2[50];
    char str3[30];
    int int1;

    //geçiçi değişkenler
    char tstr1[50];
    char tstr2[50];
    char tstr3[30];
    int tint1;

    int found=0;
    printf("\nKitap İsmi/Yazarı/Türü Yazınız: ");
    scanf(" %[^\n]", searchInput);  // istenen bilgiyi ara

    // büyük küçük harf uyumu farketmezsizin aramak için değiştir
    toLowerCase(searchInput);


    FILE *file;
    file = fopen("data/kitaplar.txt", "r");  // Okuma modunda aç
    if (file == NULL) {
        printf("DİKKAT!, DOSYA AÇILAMADI.\n");
        
    }


    while (fscanf(file, " %[^,],%[^,],%[^,],%d", str1, str2, str3, &int1) == 4) {
        strcpy(tstr1, str1);
        strcpy(tstr2, str2);
        strcpy(tstr3, str3);
        tint1 = int1;
        

        toLowerCase(str1);
        toLowerCase(str2);
        toLowerCase(str3);

        // ARAMAYLA UYUŞANI BUL
        if (strstr(str1, searchInput) || strstr(str2, searchInput) ||
            strstr(str3, searchInput)) {
                found=1;
            strcpy(str1, tstr1);
            strcpy(str2, tstr2);
            strcpy(str3, tstr3);
            int1 = tint1;

            printf("Arama Sonucu - Kitap Kodu : %d - Kitap İsmi: %s - Yazar: %s - Tür: %s - Sayfa Sayısı: %d\n", lineCount, str1, str2, str3, int1);
        }
        if(found==0){
            printf("Aradığınız girdiyle sonuç bulunamadı.\n");
            islemTamamlandi();
            menu();
        }
        lineCount++;
    }

    // Dosyayı kapat
    fclose(file);
    islemTamamlandi();
}

void kitapHepsi(){
    FILE *file;
    file = fopen("data/kitaplar.txt", "r");  // Yazma modunda aç

    if (file == NULL) {
        printf("DİKKAT, DOSYA AÇILAMAI!.\n");
    }

    // Değişkenler
    char str1[50];
    char str2[50];
    char str3[30];
    int int1;
    printf("═════════════════════════════════════════════\n");
    int lineCount = 0;
    printf("Kütüphanemizdeki tüm kitaplar aşağıdadır: \n");
    // Her sıradaki yazıları oku ve ayrı ayrı değişkenlere ata(virgül ile)
    while (fscanf(file, " %[^,],%[^,],%[^,],%d,%d,%s", str1, str2, str3, &int1) == 4) {
        // Okunan yazıları yazdır
        printf("Kitap Kodu : %d - Kitap İsmi: %s - Yazar: %s - Tür: %s - Sayfa Sayısı: %d\n", ++lineCount, str1,str2,str3,int1);
    }

    // Dosyayı kapat
    fclose(file);
    islemTamamlandi();
}

void kitapSil(){
    const char *filename = "data/kitaplar.txt";
    FILE *file, *temp;
    char buffer[1000];
    int lineToDelete, currentLine = 1;

    // Dosyayı okuma modunda aç
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya okuma modunda açılamadı.\n");
        exit(1);
    }

    // Yazdırmak için geçiçi bir dosya aç
    temp = fopen("data/temp.txt", "w");
    if (temp == NULL) {
        printf("HATA! Geçiçi dosya açılamadı.\n");
        fclose(file);
        exit(1);
    }

    // Silinecek kitap kodunu kullanıcıdan iste
    printf("KİTAPLARIN KODLARINI (KİTAP ARA) veya (TÜM KİTAPLARI GÖSTER) butonlarından öğrenebilirsiniz.\n");
    printf("Silinecek kitabın kodunu giriniz: ");
    scanf("%d", &lineToDelete);

    // Orijnal dosyadaki metinleri geçiçi dosyaya geçir
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (currentLine != lineToDelete) {
            fputs(buffer, temp);
        }
        currentLine++;
    }

    // İki dosyayı da kapat
    fclose(file);
    fclose(temp);

    // Orijinal dosyayı sil
    if (remove(filename) != 0) {
        printf("HATA! Orijinal dosya silinemedi.\n");
        exit(1);
    }

    // Geçiçi dosyanın ismini değiştir
    if (rename("data/temp.txt", filename) != 0) {
        printf("Geçiçi dosya ismi değiştirelemedi.\n");
        exit(1);
    }

    printf("%d : Kodlu kitap başarıyla silindi\n", lineToDelete);

    islemTamamlandi();
}

void kitapOduncAl(){
    char buffer[256];

    FILE *file = fopen("data/kitaplar.txt", "r");
    if (file == NULL) {
        perror("DOSYA AÇILAMADI");
        
    }

    int desiredLine;
    printf("Ödünç alacağınız kitabın kodunu giriniz: ");
    scanf("%d", &desiredLine);
    printf("═════════════════════════════════════════════\n");
    struct Book book;
    int currentLine = 0;

    // Dosyayı sıra sıra oku
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        currentLine++;

        // İstenilen sıra kontrolu
        if (currentLine == desiredLine) {
            // Sırayı parçalara ayır ve değişkenlere ata
            sscanf(buffer, "%[^,],%[^,],%[^,],%d",
                   book.title, book.author, book.genre, &book.pages);

            // Parçalara ayrılan değişkenleri yazdır.
            printf("Ödünç aldığınız kitabın bilgileri: \nKitap Adı: %s\nYazar: %s\nTür: %s\nSayfa Sayısı: %d\n",
                   book.title, book.author, book.genre, book.pages);

            // Tek bir sıra kopyalanmak isteniyorsa break kullanılabilir
            // break;
        }
    }

    fclose(file);
    //YAZDIRMA KISMI
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "data/users/%s.txt", globalUser);
    FILE *fileUser = fopen(filePath, "a");

    if (fileUser != NULL) {
        fprintf(fileUser, "%s,%s,%s,%d\n",book.title,book.author,book.genre,book.pages);
        fclose(fileUser);
        printf("Kitap bilgileri başarıyla kaydedildi.\n");
    } else {
        printf("DİKKAT, DOSYA AÇILAMADI!\n");
        gecersizSecim();
    }

    islemTamamlandi();
}

void kitapOduncBirak() {
    char filename[256];
    snprintf(filename, sizeof(filename), "data/users/%s.txt", globalUser);
    FILE *file, *temp;
    char buffer[1000];
    int lineToDelete, currentLine = 1;

    // Dosyayı okuma modunda aç
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya okuma modunda açılamadı.\n");
        exit(1);
    }

    // Yazdırmak için geçiçi bir dosya aç
    temp = fopen("data/users/temp.txt", "w");
    if (temp == NULL) {
        printf("HATA! Geçiçi dosya açılamadı.\n");
        fclose(file);
        exit(1);
    }

    // Silinecek kitap kodunu kullanıcıdan iste
    printf("Geri teslim edeceğiniz kitapların kodunu ÖDÜNÇ ALDIĞIM KİTAPLARI GÖSTER kısmından öğrenin!\n");
    printf("Teslim edilecek kitabın kodunu giriniz: ");
    scanf("%d", &lineToDelete);

    // Orijnal dosyadaki metinleri geçiçi dosyaya geçir
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (currentLine != lineToDelete) {
            fputs(buffer, temp);
        }
        currentLine++;
    }

    // İki dosyayı da kapat
    fclose(file);
    fclose(temp);

    // Orijinal dosyayı sil
    if (remove(filename) != 0) {
        printf("HATA! Orijinal dosya silinemedi.\n");
        exit(1);
    }

    // Geçiçi dosyanın ismini değiştir
    if (rename("data/users/temp.txt", filename) != 0) {
        printf("Geçiçi dosya ismi değiştirelemedi.\n");
        exit(1);
    }

    printf("%d : Kodlu kitap başarıyla teslim edilddi\n", lineToDelete);

    islemTamamlandi();
}

void tumOduncKitaplar() {

    char filename[256];
    snprintf(filename, sizeof(filename), "data/users/%s.txt", globalUser);
    FILE *file;
    file = fopen(filename, "r");  // Yazma modunda aç

    if (file == NULL) {
        printf("DİKKAT, DOSYA AÇILAMAI!.\n");
    }

    // Değişkenler
    char str1[50];
    char str2[50];
    char str3[30];
    int int1;

    int lineCount = 0;
    printf("%s kullanıcısının ödünç aldığı kitaplar aşağıdadır: \n", globalUser);
    // Her sıradaki yazıları oku ve ayrı ayrı değişkenlere ata(virgül ile)
    while (fscanf(file, " %[^,],%[^,],%[^,],%d,%d,%s", str1, str2, str3, &int1) == 4) {
        // Okunan yazıları yazdır
        //printf("Read values: %c, %c, %c, %d, %d, %c\n", char1, char2, char3, int1, int2, char4);
        printf("Kitap Kodu : %d - Kitap İsmi: %s - Yazar: %s - Tür: %s - Sayfa Sayısı: %d\n", ++lineCount, str1,str2,str3,int1);
    }

    // Dosyayı kapat
    fclose(file);
    islemTamamlandi();

}

 void rastgeleCizim() 
 {
    srand(time(NULL));
    int num = rand() % 5 + 1;
    char snum[5];

    char filename[30];

    itoa(num, snum, 5); // rastgele int sayısı stringe çevrilir.

    //printf("debug - random number is %d\n", num);
    //const char *filename = ("../art/%s.txt", snum);
    sprintf(filename, "data/art/%d.txt", num);
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
    printf("Dosya açılamadı, ana menüye dönülüyor: %s\n", filename);
    menu();
}
    char line[1024]; // buffer
    while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line); // okunan sırayı yazdır
}
    printf("\n");
    fclose(fp);
 }