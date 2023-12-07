#include <stdio.h>
#include <locale.h> //Türkçe karakterler için kütüphane
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h> // SLEEP

// BUTON PROTOTİPLERİ

//yapım aşamasındaki fonksiyonlar
void anaMenu();
void kullaniciMenu();
void yoneticiMenu();
void rastgeleCizim();
void rastgeleSoz();


//bütün kitap fonksiyonları
void kitapEkle(); // kitap ekleme butonu ana fonksiyonu
void kitapHepsi();
void kitapAra();
void kitapSil();



// ana fonksiyon prototipleri
void menu(); // en başa dönmek istiyorsanız menu fonksiyonunu çağırmanız yeterli.
void gecersizSecim(); // geçersiz seçim yapıldığında ana menüye atar
void islemTamamlandi(); //işlem tamamlandıktan sonra ana menüye dönmeden önce uyarır.
void clearInputBuffer(); // kullanıcı girdisi aldıktan sonra girdi temizleme


int main (){
    // karakter seçimini UTF-8 Türkçe yapar
    setlocale(LC_ALL, "tr_TR.UTF-8"); //BU ÇALIŞIYOR!
    //setlocale(LC_ALL, "Turkish");
    //setlocale(LC_ALL, "tr_TR");

    //hoşgeldiniz bölümü (bir kez çalışır)
    printf("═════════════════HOŞGELDİNİZ═════════════════\n");

    rastgeleCizim();

    printf("═════════════════════════════════════════════\n");

    menu(); // Ana Menü fonksiyonunu çağırır

    }


//     -----------------  fonksiyonlar  -----------------    

void menu(){

      //switch case için gerekli(AÇIKLAMASI DEĞİŞECEK!!!!)
        int secenek;

        //ana menü tuşları
        printf("Ana Menü\n"); 
        printf("1. Kitap Ekle\n");
        printf("2. Kitap Ara\n");
        printf("3. Tüm Kitapları Göster\n");
        printf("4. Kitap Sil\n");
        printf("5. Sistemi Kapat\n");
        printf("Lütfen seçiminizi yapın: ");
        scanf("%d", &secenek);


    // Kullanıcı girdisine göre fonksiyonlarına iletir
        switch (secenek) {
            case 1:
                clearInputBuffer(); 
                kitapEkle();
                break;
            case 2:
                clearInputBuffer();
                kitapAra();
                break;
            case 3:
                clearInputBuffer();
                kitapHepsi();
                break;
            case 4:
                clearInputBuffer();
                kitapSil();
                break;
            case 5:
                printf("Sistem kapatılıyor. İyi günler!\n");
                exit(1);
            default:
                gecersizSecim();

                
        }

}



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

    
 // bu kısım alınan kullanıcı değerlerini veri.txt dosyasına her girişi yeni satırda kaydeder.
    FILE *file = fopen("veri.txt", "a");

    if (file != NULL) {
        fprintf(file, "%s, %s, %s, %d\n",kitap1,kitap2,kitap3,kitap4);
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
    printf("═════════════════════════════════════════════\n");
    printf("Seçilen: Kitap Ara\n");
    const char *fileName = "veri.txt";

    FILE *file = fopen(fileName, "r");  //r , w , a

    if (file != NULL) {
        char bookName[100], authorName[100], bookType[50];
        int numPages;
        char search[100];

        printf("Aradığınız bilgiyi girin: ");
        fgets(search, sizeof(search), stdin);
        search[strcspn(search, "\n")] = '\0'; // \n karakterini siler.

        int lineCount = 1;
        int found = 0;

        while (fscanf(file, "%9z9[^,],%99[^,],%49[^,],%d\n", bookName, authorName, bookType, &numPages) == 4) {
            // Check if the search string is found in any field
            if (strstr(bookName, search) != NULL || strstr(authorName, search) != NULL ||
                strstr(bookType, search) != NULL || numPages == atoi(search)) {
                printf("Kitap Kodu: %d -- Kitap: %s, Yazar: %s, Türü: %s, Sayfa Sayısı: %d\n", lineCount, bookName, authorName, bookType, numPages);
                found = 1;
            }

            lineCount++;
        }

        if (!found) {
            printf("Böyle bir kayıt bulunamadı.\n");
        }

        fclose(file);
    } else {
        printf("HATA! Dosya açılamadı.\n");
    }

    islemTamamlandi();
}

void kitapHepsi()
{

    printf("═════════════════════════════════════════════\n");
    printf("Seçilen: Tüm Kitapları Göster\n");

     FILE *file = fopen("veri.txt", "r");

    if (file != NULL) {
        int line = 1; // sıra sayısı / kodu
        char bookName[100], authorName[100], bookType[50];
        int numPages;

        // dosyanın sonuna kadar okur
        while (fscanf(file, "%99[^,],%99[^,],%49[^,],%d\n", bookName, authorName, bookType, &numPages) == 4) {
            // okunan verileri değerlere ata
            printf("Kitap Kodu: %d -- Kitap: %s, Yazar:%s, Türü:%s, Sayfa Sayısı: %d\n", line, bookName, authorName, bookType, numPages);

            line++;
        }

        fclose(file);
    } else {
        printf("Error opening the file.\n");
    }

    islemTamamlandi();
}

void kitapSil()
{
    printf("═════════════════════════════════════════════\n");
    printf("Seçilen: Kitap Sil\n");
    const char *fileName = "veri.txt";
    int lineToDelete;
    printf("Silmek istediğinz kitabın kodunu girin: \n");
    scanf("%d",&lineToDelete);


   
    

    FILE *oldFile = fopen(fileName, "r");
    FILE *newFile = fopen("temp.txt", "w");

    if (oldFile == NULL || newFile == NULL) {
        printf("HATA! Dosya açılamadı.\n");
        exit(1);
    }

    char buffer[1000];
    int currentLine = 1;

    // Copy lines from old file to new file, excluding the line to delete
    while (fgets(buffer, sizeof(buffer), oldFile) != NULL) {
        if (currentLine != lineToDelete) {
            fputs(buffer, newFile);
        }
        currentLine++;
    }

    fclose(oldFile);
    fclose(newFile);

    // Replace the old file with the new one
    remove(fileName);
    rename("temp.txt", fileName);

    printf("Kodu %d olan kitap silindi.\n", lineToDelete);
    islemTamamlandi();
    
}

void gecersizSecim() 
{
    printf("═════════════════════════════════════════════\n");
    printf("GEÇERSİZ SEÇİM, iki saniye içinde ana menüye dönülüyor\n");
    printf("═════════════════════════════════════════════\n");
    sleep(2000);
    printf("\e[1;1H\e[2J"); //CLEAR SCREEN
    menu();
}

void clearInputBuffer()
 {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void islemTamamlandi()
 {
    printf("═════════════════════════════════════════════\n");
    printf("İşlem Tamamlandı\n");
    printf("Ana menüye dönmek için herhangi bir tuşa basınız.\n");
    printf("═════════════════════════════════════════════\n");
    getchar(); // herhangi bir girdi al
    printf("\e[1;1H\e[2J"); // CLEAR SCREEN
    menu();
 }

 void rastgeleCizim() 
 {
    srand(time(NULL));
    int num = rand() % 5 + 1;
    char snum[5];

    char filename[30];

    itoa(num, snum, 5); // rastgele int sayısı stringe çevrilir.

    printf("debug - random number is %d\n", num);
    //const char *filename = ("../art/%s.txt", snum);
    sprintf(filename, "art/%d.txt", num);
    
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

 void rastgeleSoz() {

 }