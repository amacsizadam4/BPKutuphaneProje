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


//kitap fonksiyonları
void kitapAra();
void kitapEkle();
void kitapHepsi();
void kitapSil();
void kitapOduncAl();
void kitapOduncBirak();

//structlar
struct User {
    char username[50];
    char password[50];
    int isAdmin; // Yönetici için 1, kullanıcı ise 0
};



int main() {
    KONSOLTEMIZLE;
    //hoşgeldiniz eklenecek
    setlocale(LC_ALL, "tr_TR.UTF-8"); //BU ÇALIŞIYOR!
    girisMenu();

}


// FONKSİYONLAR
void kitapEkle();

int girisMenu() {
    int secim;
    printf("════════════════════════GİRİŞ MENÜSÜ════════════════════════\n");
    printf("1 - Kullanıcı Giriş\n2 - Kayıt Ol\n3 - Uygulamayı Kapat\n");
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


    // Copy the contents of username to globalUser
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
    FILE *file = fopen("data/users.txt", "a");
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
    FILE *file = fopen("data/users.txt", "r");

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
    printf("═════════════════════════════════════════════\n");
    int secenek;
    int odunc;
    if(globalIsAdmin == 1) {
        goto yonetici;
    } else { goto kullanici;}


    yonetici:
        //ana menü tuşları
        printf("Ana Menü\n"); 
        printf("1. Kitap Ekle\n");
        printf("2. Kitap Ara\n");
        printf("3. Tüm Kitapları Göster\n");
        printf("4. Kitap Sil\n");
        printf("5. Ödünç Alınan Kitapları Görüntüle\n");
        printf("6. Çıkış Yap\n");
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
            break;
            case 6:
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
            if (secenek2==1) {
                kitapOduncAl();
                printf("Ödünç almak istediğiniz kitabın kodunu yazınız: ");
                scanf("%d", odunc);
                

                }
            if (secenek2==2) {kitapOduncBirak();}
            break;
            case 4:
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
    int musaitlik=1;
    char kisi[10] = "Müsait";
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
        fprintf(file, "%s,%s,%s,%d,%d,%s\n",kitap1,kitap2,kitap3,kitap4,musaitlik,kisi);
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
    int int1, int2;
    char str4[50];

    //geçiçi değişkenler
    char tstr1[50];
    char tstr2[50];
    char tstr3[30];
    int tint1, tint2;
    char tstr4[50];

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


    while (fscanf(file, " %[^,],%[^,],%[^,],%d,%d,%s", str1, str2, str3, &int1, &int2, str4) == 6) {
        strcpy(tstr1, str1);
        strcpy(tstr2, str2);
        strcpy(tstr3, str3);
        tint1 = int1;
        tint2 = int2;
        strcpy(tstr4, str4);
        

        toLowerCase(str1);
        toLowerCase(str2);
        toLowerCase(str3);
        toLowerCase(str4);

        // ARAMAYLA UYUŞANI BUL
        if (strstr(str1, searchInput) || strstr(str2, searchInput) ||
            strstr(str3, searchInput) || strstr(str4, searchInput)) {
                found=1;
            strcpy(str1, tstr1);
            strcpy(str2, tstr2);
            strcpy(str3, tstr3);
            int1 = tint1;
            int2 = tint2;
            strcpy(str4, tstr4);                
            printf("Search result - Line %d: %s, %s, %s, %d, %d, %s\n", lineCount, str1, str2, str3, int1, int2, str4);
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
    int int1, int2;
    char str4[50];

    int lineCount = 0;

    // Her sıradaki yazıları oku ve ayrı ayrı değişkenlere ata(virgül ile)
    while (fscanf(file, " %[^,],%[^,],%[^,],%d,%d,%s", str1, str2, str3, &int1, &int2, str4) == 6) {
        // Okunan yazıları yazdır
        //printf("Read values: %c, %c, %c, %d, %d, %c\n", char1, char2, char3, int1, int2, char4);
        printf("Kitap Kodu : %d - Kitap İsmi: %s - Yazar: %s - Tür: %s - Sayfa Sayısı: %d - Sahibi: %s\n", ++lineCount, str1,str2,str3,int1,str4);
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

    // Open a temporary file for writing
    temp = fopen("data/temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        exit(1);
    }

    // Get the line number to delete from the user
    printf("Enter the line number to delete: ");
    scanf("%d", &lineToDelete);

    // Read lines from the original file and write to the temporary file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (currentLine != lineToDelete) {
            fputs(buffer, temp);
        }
        currentLine++;
    }

    // Close both files
    fclose(file);
    fclose(temp);

    // Remove the original file
    if (remove(filename) != 0) {
        printf("Error removing the original file.\n");
        exit(1);
    }

    // Rename the temporary file to the original file
    if (rename("data/temp.txt", filename) != 0) {
        printf("Error renaming the temporary file.\n");
        exit(1);
    }

    printf("Line %d deleted successfully.\n", lineToDelete);

    islemTamamlandi();
}

void kitapOduncAl(){
}
void kitapOduncBirak(){

}