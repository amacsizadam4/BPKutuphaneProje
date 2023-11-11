#include <stdio.h>
#include <locale.h> //Türkçe karakterler için kütüphane
#include <string.h>
#include <stdlib.h>

// BUTON PROTOTİPLERİ
//kitap ekleme fonksiyonları
void kitapEkle(); // kitap ekleme butonu ana fonksiyonu
void clearInputBuffer(); // kullanıcı girdisi aldıktan sonra girdi temizleme


//henüz tamamlanmadı
void kitapAra();
void kitapHepsi();
void kitapSil();




// ana fonksiyon prototipleri
void menu(); // en başa dönmek istiyorsanız menu fonksiyonunu çağırmanız yeterli
void gecersizSecim(); // geçersiz seçim yapıldığında ana menüye atar


int main (void){
    // karakter seçimini UTF-8 Türkçe yapar
    setlocale(LC_ALL, "tr_TR.UTF-8");
    //setlocale(LC_ALL, "Turkish");
    //hoşgeldiniz bölümü (bir kez çalışır)


    printf("test test test test test test test test test31\n");
    printf("-----------------HOŞGELDİNİZ-----------------\n");

    


    menu(); // Ana Menü fonksiyonunu çağırır

    }


//fonksiyonlar    

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
                kitapAra();
                break;
            case 3:
                kitapHepsi();
                break;
            case 4:
                kitapSil();
                break;
            case 5:
                printf("Sistem kapatılıyor. İyi günler!\n");
                return 0;
            default:
                gecersizSecim();

                
        }

}



void kitapEkle(){
    char kitap1[50]; // Kitap ismi
    char kitap2[50]; // Kitap yazarı
    char kitap3[30]; // Kitap türü
    int kitap4; // Sayfa sayısı
    printf("--------------------------");
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

    menu();
}

void kitapAra(){

}

void kitapHepsi(){
    
}

void kitapSil(){
    
}

void gecersizSecim() 
{
    printf("----------------------------\n");
    printf("GEÇERSİZ SEÇİM, ana menüye dönülüyor\n");
    printf("----------------------------\n");

    menu();
}


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}